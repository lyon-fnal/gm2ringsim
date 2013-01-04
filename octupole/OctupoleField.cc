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

gm2ringsim::OctupoleField::OctupoleField() : 
  period_(20*microsecond), gradient_(0.4*gauss/cm3) {}

void gm2ringsim::OctupoleField::GetFieldValue( double const Point[4], 
				   double Bfield[3] ) const {

  // see comments in quadField.cc::GetFieldValue for coordinate system
  // conversions

  storageFieldController::getInstance().GetFieldValue(Point, Bfield);

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
  
  double const Bx_q = scale*(y_q2*y_q - 3.*x_q2*y_q);
  double const By_q = scale*(x_q2*x_q - 3.*y_q2*x_q);
  
  Bfield[0] += Bx_q*x/r;
  Bfield[1] += By_q;
  Bfield[2] += Bx_q*z/r;    
			     
}
