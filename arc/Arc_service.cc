// Implementation of Arc

#include "gm2ringsim/arc/Arc_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "gm2ringsim/arc/ArcGeometry.hh"

#include "boost/format.hpp"

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"

#include "Geant4/G4FieldManager.hh"
#include "Geant4/G4Mag_EqRhs.hh"
#include "Geant4/G4Mag_UsualEqRhs.hh"
#include "Geant4/G4Mag_SpinEqRhs.hh"
#include "Geant4/G4EqEMFieldWithEDM.hh"
#include "Geant4/G4EqEMFieldWithSpin.hh"
#include "Geant4/G4ChordFinder.hh"
#include "Geant4/G4MagIntegratorStepper.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "gm2ringsim/arc/StorageRingField.hh"

#include "gm2ringsim/fields/g2FieldEqRhs.hh"



// Constructor for the service 
gm2ringsim::Arc::Arc(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
		 p.get<std::string>("name", "arc"),
		 p.get<std::string>("category", "arc"),
		 p.get<std::string>("mother_category", "world")),
  sts_("SpinTracking"), //This is a shortcut to get the SpinTracking par list from the fhicl
  spin_tracking_(sts_.spinTrackingEnabled),
  withoutSpin_(0),    //will set in the constructor function
  withSpin_(0)        //will set in the constructor function
{
  bool do_EDM = false;
  
  storageRingField *storageField = new storageRingField();
  //storageRingEMField *storageEMField = new storageRingEMField();

  //----------------------------------------
  // build the spin ignoring field equations
  //----------------------------------------
  G4Mag_EqRhs *equation = new G4Mag_UsualEqRhs(storageField);
  G4ClassicalRK4 *stepper = new G4ClassicalRK4(equation);
  G4ChordFinder *iChordFinder = new G4ChordFinder(storageField, 0.01*mm, stepper);
  withoutSpin_ = new G4FieldManager(storageField, iChordFinder);



  //----------------------------------------
  // build the spin evolving field equations
  //----------------------------------------
  
  // build the spin evolving field equations
  if ( do_EDM == true ) {;
    //G4EqEMFieldWithEDM *equation2 = new G4EqEMFieldWithEDM(storageField);
    //G4EqEMFieldWithSpin *equation2 = new G4EqEMFieldWithSpin(storageEMField);
    //equation2->SetEta(1e-19);
    //stepper = new G4ClassicalRK4(equation2,12);
  }
  if ( do_EDM == false ) {
    equation = new G4Mag_SpinEqRhs(storageField);
    stepper = new G4ClassicalRK4(equation,12);
  }
  iChordFinder = new G4ChordFinder(storageField, 0.01*mm, stepper);
  withSpin_ = new G4FieldManager(storageField, iChordFinder);
//   if ( do_EDM == true ) {
//     withSpin_->SetFieldChangesEnergy(false);
//   }
  

}

gm2ringsim::Arc::~Arc() {
  if (withoutSpin_)
    delete withoutSpin_;

  if (withSpin_)
    delete withSpin_;
}
  
void gm2ringsim::Arc::initialize() {



}

G4LogicalVolume* gm2ringsim::Arc::makeAnArcLV(gm2ringsim::ArcGeometry const & g, unsigned int arcNum) {
  

mf::LogInfo("Arc") <<"In the Arc::makeAnArcLV, with spin_tracking_ set to "<<spin_tracking_;

  G4FieldManager *tmpFieldManager=withoutSpin_;
  if (spin_tracking_)
    tmpFieldManager = withSpin_;
  
  double extension = 0.0;
  if ( arcNum == 11 ) extension = g.arc11_rExtension;
  
  // Make the tub (note use of auto)
  G4Tubs* arc_S = new G4Tubs("wall",
                             g.arc_rMin,
                             g.arc_rMax + extension,
                             g.arc_z, g.arc_Sphi, g.arc_Dphi );
  
  // Give it a name
  std::string arcName = artg4::addNumberToName("ArcSection", arcNum);
  
  // Make the logical volume
  G4LogicalVolume* arc_L = new G4LogicalVolume(arc_S,
					       artg4Materials::Vacuum(),
					       arcName.c_str(),
					       tmpFieldManager //This is the spin-dependent field manager
                                               );

  // Set visualization
  artg4::setVisAtts( arc_L, g.display, g.arcColor );
    
  return arc_L;
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Arc::doBuildLVs() {
  
  // Get the geometry
  ArcGeometry g(myName());
  
  g.print();
  
  // Create the vector of logical volumes
  std::vector<G4LogicalVolume*> arcLVs;

  // Build the logical volumes
  for ( unsigned int arcNumber = 0; arcNumber != 12; ++arcNumber ) {
    
    // Push this into the vector
    arcLVs.push_back( makeAnArcLV(g, arcNumber) );
  }

  return arcLVs;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Arc::doPlaceToPVs( std::vector<G4LogicalVolume*> motherLV) {
  
  std::vector<G4VPhysicalVolume*> arcPVs;
  arcPVs.resize(lvs().size());
  
  // We need to loop over the logical volumes
  unsigned int arcNum = 0;
  for ( auto anArcLV : lvs() ) {
    
    // We to name the arc including its arc number
    // g2migtrace used sprintf. Let's use boost::format instead
    // (see http://www.boost.org/doc/libs/1_52_0/libs/format/doc/format.html )
    std::string arcLabel(
      boost::str( boost::format("ArcSection[%02d]") % arcNum )
    );
    
    // Make the physical volume
    arcPVs.push_back(
      new G4PVPlacement(
          new G4RotationMatrix(0.0, pi/2, (arcNum*pi)/6),
          G4ThreeVector(),
          anArcLV,
          arcLabel,
          motherLV[0],
          false,
          0
      )
    );
    
    ++arcNum;
  }
  
  return arcPVs;
}

using gm2ringsim::Arc;
DEFINE_ART_SERVICE(Arc)
