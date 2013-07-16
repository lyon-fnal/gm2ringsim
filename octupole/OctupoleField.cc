/** @file octupoleField.cc

    Provides the implementation of the pulsed octupole magnetic
    field.

    @sa quadField.cc
    @sa octupoleConstruction.cc

    Kevin Lynch, 2009
*/

#include "gm2ringsim/octupole/OctupoleField.hh"

#include "Geant4/globals.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/arc/StorageRingField.hh"

#include <cmath>

gm2ringsim::OctupoleField::OctupoleField(int Charge) : 
  octGeom_("octupole"),
  period_(octGeom_.period),
  offset_(octGeom_.offset),
  gradient_(octGeom_.gradient),
  Charge_(Charge)
{}

void gm2ringsim::OctupoleField::GetFieldValue( double const Point[4], 
				   double Bfield[3] ) const {

  // see comments in quadField.cc::GetFieldValue for coordinate system
  // conversions

  storageFieldController::getInstance().GetFieldValue(Point, Bfield, Charge_);

  G4double const& time = Point[3];

  /** @bug The 100 ns global offset should be set and queried from
      some global place, right? */
  if( time -100.*ns - offset_ > period_/2. ) 
    return;
  
  G4double const& x = Point[0];
  G4double const& y = Point[1];
  G4double const& z = Point[2];
  G4double const r = hypot(x,z);
  
  double const x_q = r - R_magic();
  double const& y_q = y;
  double const x_q2 = x_q*x_q;
  double const y_q2 = y_q*y_q;
  
  double const scale = gradient_*std::sin(time - 100*ns);
  
  double Bx_q = scale*(y_q2*y_q - 3.*x_q2*y_q);
  double By_q = scale*(x_q2*x_q - 3.*y_q2*x_q);

  //--------------------------
  // Assumes negative beam
  //--------------------------
  if ( Charge_ == 1 ) { 
    Bx_q *= -1;
    By_q *= -1;
  }
  
  Bfield[0] += Bx_q*x/r;
  Bfield[1] += By_q;
  Bfield[2] += Bx_q*z/r;    


  
  bool debug = false;
  if ( debug ) {
    double r = sqrt(Point[0]*Point[0] + Point[2]*Point[2]) - R_magic();
    double y = Point[1];
    std::cout.precision(3);
    double dB = Bfield[1] - 0.00145;
    dB = 0.0;
    if ( Bfield[0] > 0.01 || Bfield[2] > 0.01 || Bfield[0] < -0.01 || Bfield[2] < -0.01 || dB > 0.01 || dB < -0.01 || 1 ) {
      std::cout << "OctupoleField::GetFieldValue(" << r << " , " << y << ") = ["
		<< Bfield[0] << " , "
		<< Bfield[1] << " , "
		<< Bfield[2] << "]" << std::endl;
    }
  }
			     
}
