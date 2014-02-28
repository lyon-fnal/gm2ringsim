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

#include "TMath.h"

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <cstdlib>

#include "gm2ringsim/kicker/KickerField.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/arc/StorageRingField.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

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
  storageFieldController::getInstance().GetFieldValue(Point, Bfield, Charge_, StorageFieldType_);
  double Kfield[3];
  this->KickFieldValue(Point,Kfield);
  mod_->ModifyKickField(Point,Kfield);
  for(int i=0; i!=3; ++i) {
    Bfield[i] += Kfield[i];
  }
  
  bool debug = false;
  if ( debug ) {
    double r = sqrt(Point[0]*Point[0] + Point[2]*Point[2]) - R_magic();
    double y = Point[1];
    std::cout.precision(3);
    double dB = Bfield[1] - 0.00145;
    dB = 0.0;
    if ( Bfield[0] > 0.01 || Bfield[2] > 0.01 || Bfield[0] < -0.01 || Bfield[2] < -0.01 || dB > 0.01 || dB < -0.01 ) {
      std::cout << "KickField::GetFieldValue(" << r << " , " << y << ") = ["
		<< Bfield[0] << " , "
		<< Bfield[1] << " , "
		<< Bfield[2] << "]" << std::endl;
    }
  }
}

gm2ringsim::LCRKickField::LCRKickField(G4double kickerHV,
				       G4double kickerOffsetTime, G4double circuitC,
				       G4double circuitL, G4double circuitR,
				       KickModifier* mod,
				       int Charge,
				       int StorageFieldType) : 
  KickField(mod, Charge, StorageFieldType),
  HV(kickerHV), offsetTime(kickerOffsetTime), 
  C(circuitC), L(circuitL), R(circuitR),
  omega(sqrt( 1/(L*C) - (R*R/( 4.0*L*L )) )),
  X( R/( 2.0*L ) ),
  Charge_(Charge),
  StorageFieldType_(StorageFieldType)
{
  
  //-------------------
  // Correct for polarity if we run with positive muons.
  //-------------------
  if ( Charge_ == 1 ) { kickerHV *= -1; }
  G4double tmp_kickerHV = kickerHV;

  kickerHV = 95*kilovolt;
  if ( Charge_ == 1 ) { kickerHV *= -1; }
  G4cout << "============ LCRKickField ===============" << G4endl;
  G4cout << "| LCR HV: " << kickerHV/kilovolt << " kV" << G4endl;
  G4cout << "| Offset: " << offsetTime << G4endl;
  G4cout << "| Beam Charge:  " << Charge_ << G4endl;
  G4ThreeVector pnt(0, 0, gm2ringsim::R_magic());
  G4ThreeVector v(-1, 0, 0);
  double Bfield[3];
  double Point[4];
  Point[0] = pnt.x();
  Point[1] = pnt.y();
  Point[2] = pnt.z();
  for ( double t = 0.0; t < 1000.0; t += 5 ) {
    Point[3] = t;
    KickFieldValue(Point, Bfield);
    G4ThreeVector B(Bfield[0], Bfield[1], Bfield[2]);
    //G4cout << "INIT" << "\t" << t << "\t" << Bfield[1]/gauss << G4endl;
  }
  //if ( Charge_ == 1 ) {
  //for ( int i = 0; i < 3; i++ ) { Bfield[i] *= -1; }
  //}
  Point[3] = 0.0;
  G4ThreeVector B(Bfield[0], Bfield[1], Bfield[2]);
  if ( B.mag() > 0 ) {
    G4cout << "| v(0,0,R)      = " << v << G4endl;
    G4cout << "| B(0,0,R)      = " << B/gauss << "  Gauss." << G4endl;
    G4ThreeVector F = v.cross(B)/(v.mag()*B.mag());
    G4cout << "| q(v x B)      = " << Charge * F << G4endl;
    if ( Charge_ * F.x() < 0 ) { G4cout << "| Kick is toward to the center of the ring." << G4endl; }
    else { G4cout << "| Kick is outward from the center of the ring." << G4endl; }
  }
  G4cout << "===========================================" << G4endl;
  kickerHV = tmp_kickerHV;
  i0 = kickerHV / ( omega*L );
}

void gm2ringsim::LCRKickField::KickFieldValue(double const Point[4],
				  double Kfield[3]) const {
  bool debug = false;

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

  if ( debug ) {
    G4double x = Point[0];
    G4double y = Point[1];
    G4double z = Point[2];
    G4double r = sqrt( x*x + z*z );  
    G4double theta = atan2(x,z);
    if ( Kfield[1]/gauss > 0 && time == time ) {
      G4cout << "LCR\tT" << TurnCounter::getInstance().turns() << "\t" << time << "\t" << theta << "\t" << Kfield[1]/gauss << G4endl;
    }
    if ( 0 ) {
      G4cout << "(r,theta,y,t) = (" 
	     << r << ','
	     << theta << ',' 
	     << y << ',' 
	     << time << ")\t"
	     << "ByK = " << Kfield[1]/gauss << G4endl;
    }
  }
}


gm2ringsim::SquareKickField::SquareKickField(G4double kickSquareField,
					     KickModifier* mod,
					     int Charge,
					     int StorageFieldType) : 
  KickField(mod, Charge, StorageFieldType), squareField(kickSquareField), Charge_(Charge), StorageFieldType_(StorageFieldType)
{
  //-------------------
  // Correct for polarity if we run with positive muons.
  //-------------------
  if ( Charge_ == 1 ) { squareField *= -1; }
  G4double tmp_squareField = squareField;

  squareField = 220*gauss;
  if ( Charge_ == 1 ) { squareField *= -1; }
  G4cout << "============ SquareKickField ===============" << G4endl;
  G4cout << "| Square field: " << squareField << G4endl;
  G4cout << "| Beam Charge:  " << Charge_ << G4endl;
  G4ThreeVector pnt(0, 0, gm2ringsim::R_magic());
  G4ThreeVector v(-1, 0, 0);
  double Bfield[3];
  double Point[4];
  Point[0] = pnt.x();
  Point[1] = pnt.y();
  Point[2] = pnt.z();
  Point[3] = 0.0;
  KickFieldValue(Point, Bfield);
  //if ( Charge_ == 1 ) {
  //for ( int i = 0; i < 3; i++ ) { Bfield[i] *= -1; }
  //}
  G4ThreeVector B(Bfield[0], Bfield[1], Bfield[2]);
  if ( B.mag() > 0 ) {
    G4cout << "| v(0,0,R)      = " << v << G4endl;
    G4cout << "| B(0,0,R)      = " << B/gauss << "  Gauss." << G4endl;
    G4ThreeVector F = v.cross(B)/(v.mag()*B.mag());
    G4cout << "| q(v x B)      = " << Charge * F << G4endl;
    if ( Charge_ * F.x() < 0 ) { G4cout << "| Kick is toward to the center of the ring." << G4endl; }
    else { G4cout << "| Kick is outward from the center of the ring." << G4endl; }
  }
  G4cout << "===========================================" << G4endl;
  squareField = tmp_squareField;
}

namespace{
  // this time is really 150ns, but don't forget the 100ns global t0
  // offset 
  G4double const timeForOneFullCircuit = 200*ns;
}


void gm2ringsim::SquareKickField::KickFieldValue(G4double const Point[4],
						 G4double Kfield[3]) const {
  bool debug = false;

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



  if ( debug ) {
    G4double const time = Point[3];
    G4double x = Point[0];
    G4double y = Point[1];
    G4double z = Point[2];
    G4double r = sqrt( x*x + z*z );  
    G4double theta = atan2(x,z);
    G4cout.precision(4);
    if ( Kfield[1]/gauss > 0 && time == time ) {
      G4cout << "SQUARE\tT" << TurnCounter::getInstance().turns() << "\t" << time << "\t" << theta << "\t" << Kfield[1]/gauss << G4endl;
    }
    if ( Kfield[1] > 0 ) {
      ;//G4cout << time << "\t" << theta << "\t" << Kfield[1] << G4endl;
    }
    if ( 0 ) {
      G4cout << "(r,theta,y,t) = (" 
	     << r << ','
	     << theta << ',' 
	     << y << ',' 
	     << time << ")\t"
	     << "ByK = " << Kfield[1]/gauss << " Gauss." << G4endl;
    }
  }

}



gm2ringsim::NoKickField::NoKickField(KickModifier* mod, int Charge, int StorageFieldType) : KickField(mod, Charge, StorageFieldType), Charge_(Charge), StorageFieldType_(StorageFieldType) {}

void gm2ringsim::NoKickField::KickFieldValue(G4double const /*Point*/[4],
				 G4double Kfield[3]) const {
  Kfield[0] = Kfield[1] = Kfield[2] = 0.;
}
