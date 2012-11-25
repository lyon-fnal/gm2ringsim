/** @file inflectorGeometry.cc
    
    Implements the non-inlinable members of the inflectorGeometry
    class.

    @author Kevin Lynch
    @date 2009
    @author Brendan Kiburg 
    @date artified 2012
*/

#include "gm2ringsim/inflector/inflectorGeometry.hh"

#include "Geant4/G4UnitsTable.hh"

namespace {

  // Length of mandrel (total - windows - conductor - endflanges)
  const G4double inflectorMandrelLength = 1696.*mm;
  
  const G4double windowWidth = 1.5*mm;
  const G4double conductorWidth = 2.4*mm;
  const G4double cryostatWindowThickness = 1.0*mm;

}

inflectorGeometry& inflectorGeometry::getInstance() {
  static inflectorGeometry ia;
  return ia;
}


inflectorGeometry::inflectorGeometry() :
  delta_(5.587*degree),
  gamma_(2.4*milliradian), 
  //  gamma_(45.*degree),
  //  gamma_(0.),
  zeta_(0.),
  //  zeta_(90.*degree),
  apertureDist_(77*mm),
  ipLaunch_(0.),
  oopLaunch_(0.),
  // constants
  windowThickness_(windowWidth),
  conductorThickness_(conductorWidth),
  cryostatThickness_(cryostatWindowThickness),
  mL_(inflectorMandrelLength/2.),
  iL_( inflectorMandrelLength/2.+
       windowThickness_+conductorThickness_+cryostatThickness_ )
{}
