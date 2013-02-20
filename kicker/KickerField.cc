/** @file kickerField.cc

    Implements the kicker field and field modifier classes
    KickModifier, KickField, NoModifier, MorseModifier, NoKickField,
    LCRKickField, and SquareKickField. 

    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
*/

#include "Geant4/G4Track.hh"
#include "Geant4/G4RunManagerKernel.hh"
#include "Geant4/G4UnitsTable.hh"

#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <cstdlib>

#include "gm2ringsim/kicker/KickerField.hh"
#include "gm2ringsim/arc/StorageRingField.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"


namespace kickParams {

  G4double const kickTime1 = 120*ns;
  G4double const kickTime2 = 197.5*ns;
  G4double const kickPercent = 100.0;
  
  G4double const kPlate1HV = 95.*kilovolt;
  G4double const kPlate2HV = 95.*kilovolt;
  G4double const kPlate3HV = 95.*kilovolt;
  
  // Values obtained from g2track '/dat/kicker.dat'
  G4double const kicker1L = (1.164e-6)*henry;
  G4double const kicker1R = 11.35*ohm;
  G4double const kicker1C = 11.239996*nanofarad;
  
  G4double const kicker2L = (1.114e-6)*henry;
  G4double const kicker2R = 11.43*ohm;
  G4double const kicker2C = 11.299994*nanofarad;
  
  G4double const kicker3L = (1.187e-6)*henry;
  G4double const kicker3R = 11.33*ohm;
  G4double const kicker3C = 11.049994*nanofarad;
  
  // Myserious conversion factor from g2track
  G4double const conversionFactor = (196*gauss)/(5600*ampere);
}


gm2ringsim::MorseModifier::MorseModifier(G4String filename) : 
  fname_(filename), interp(0) {
  parse_file();
}

gm2ringsim::MorseModifier::~MorseModifier(){
  //FIXME: error: deleting object of polymorphic class type 'uniform_spline_interpolator<double>' which has non-virtual destructor might cause undefined behaviour [-Werror=delete-non-virtual-dtor]
  //if(interp)
  //  delete interp;
}

void gm2ringsim::MorseModifier::parse_file(){
  // assume in order and increasing
  std::ifstream is(fname_);

  std::string str;

  std::vector<double> ys,scales;

  while( is.good() ){
    std::getline(is, str);

    // clean leading and trailing whitespace
    boost::trim(str);
    // skip comment lines and blank lines
    if( str[0] == '#' || str.size() == 0){
      continue;
    }
    // tokenize
    boost::char_separator<char> sep(" \t");
    typedef boost::tokenizer<boost::char_separator<char> > tokenize_on_blanks;
    tokenize_on_blanks tokens(str,sep);
    tokenize_on_blanks::const_iterator b = tokens.begin(),
      e = tokens.end();
    int toks = std::distance(b,e);
    if( toks != 2 )
      G4cout << "Bad line!  Saw " << toks << " tokens!\n";

    ys.push_back(std::strtod( (*b++).c_str(), 0)*centimeter);
    scales.push_back(std::strtod( (*b++).c_str(), 0));

    // leave at end!
    if( is.eof() )
      break;
  }

  uniform_spline_interpolator<double> 
    *newinterp = new uniform_spline_interpolator<double>(ys.begin(), ys.end(),
							 scales.begin(), scales.end());

  std::swap(newinterp, interp);
  //FIXME:
  //delete newinterp;
}

void gm2ringsim::MorseModifier::ModifyKickField(G4double const Point[4],
				    G4double Bfield[3]) const {
  double point = Point[1]>0 ? Point[1] : -Point[1];

  double scale = (*interp)(point);
  //  G4cout << Point[1] << ' ' << scale << '\n';
  Bfield[0]*=scale;
  Bfield[1]*=scale;
  Bfield[2]*=scale;
}

void gm2ringsim::KickField::GetFieldValue(double const Point[4],
			      double Bfield[3]) const {
  storageFieldController::getInstance().GetFieldValue(Point, Bfield);
  double Kfield[3];
  this->KickFieldValue(Point,Kfield);
  mod_->ModifyKickField(Point,Kfield);
  for(int i=0; i!=3; ++i)
    Bfield[i] += Kfield[i];
  //  std::cout << Kfield[1] << ' ' << Bfield[1] << '\n';
}

gm2ringsim::LCRKickField::LCRKickField(G4double kickerHV,
			   G4double kickerOffsetTime, G4double circuitC,
			   G4double circuitL, G4double circuitR,
			   KickModifier* mod) : 
  KickField(mod),
  HV(kickerHV), offsetTime(kickerOffsetTime), 
  C(circuitC), L(circuitL), R(circuitR),
  omega(sqrt( 1/(L*C) - (R*R/( 4.0*L*L )) )),
  X( R/( 2.0*L ) ), i0( HV / ( omega*L ) )
{}

void gm2ringsim::LCRKickField::KickFieldValue(double const Point[4],
				  double Kfield[3]) const {


  // The concept of offset time ensures that the kicker has sufficient
  // time to ramp up its current such that the bunch of muons to be
  // kicked feels as close to a maximum kick as possible.  In order to
  // accomplish this, the kicker must start its kick *before* the bunch
  // events enters the ring.  Since everything starts at global time zero,
  // the code below adds the offset time to the global time, such that the 
  // kicker is where it should be when the muons arrive 

  G4double const time = Point[3] + offsetTime;
  G4double current;

  if( time > 10./X )
    current = 0.;
  else
    current = i0 * (exp (-X*time) * sin(omega*time) );
  
  // 'conversionFactor' (196gauss/5600ampere) is what I believe to 
  // be the conversion factor between the current on the plates
  // and the kicker magnetic field that it produces.  The value
  // above existed in g2track but no one was terribly sure where
  // it came from nor if it is correct.  See 'g2track/kicker.c', 
  // lines 170-176 for the original code.

  // KRL Update 2008-01-09
  // There is an identical value in the Kicker NIM paper, page 16, we
  // find:
  // "For a typical peak current of 4000 A; one calculates a field of
  // about 140 G at the center of the muon storage region[.]"

  Kfield[1] = current*kickParams::conversionFactor; 

  Kfield[0] = Kfield[2] = 0.;

  /*
  G4double x = Point[0];
  G4double y = Point[1];
  G4double z = Point[2];
  G4double r = sqrt( x*x + z*z );  
  G4double theta = atan2(x,z);
  G4cout << "(r,theta,y,t) = (" 
	 << r << ','
	 << theta << ',' 
	 << y << ',' 
	 << time << ")\t"
	 << "ByK = " << Bfield[1] << '\n';
*/

}


gm2ringsim::SquareKickField::SquareKickField(G4double kickSquareField,
				 KickModifier* mod) : 
  KickField(mod), squareField(kickSquareField) 
{
  //  std::cout << "Square field: " << squareField << '\n';
}

namespace{
  // this time is really 150ns, but don't forget the 100ns global t0
  // offset 
  G4double const timeForOneFullCircuit = 200*ns;
}

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"
  
void gm2ringsim::SquareKickField::KickFieldValue(G4double const /*Point*/[4],
				     G4double Kfield[3]) const {

  Kfield[0] = Kfield[1] = Kfield[2] = 0.;
  //  G4double const& time = Point[3];
  //  if(time < timeForOneFullCircuit){
  if( TurnCounter::getInstance().turns() == 0 ){
    Kfield[1] =  squareField;
    //    std::cout << squareField << '\n';
    //    Kfield[1] = 175.*gauss;
    //  } else if( turnCounter::getInstance().turns() == 1 ){
    //    Kfield[1] = 85.*gauss;
  }  

}



gm2ringsim::NoKickField::NoKickField(KickModifier* mod) : KickField(mod) {}

void gm2ringsim::NoKickField::KickFieldValue(G4double const /*Point*/[4],
				 G4double Kfield[3]) const {
  Kfield[0] = Kfield[1] = Kfield[2] = 0.;
}
