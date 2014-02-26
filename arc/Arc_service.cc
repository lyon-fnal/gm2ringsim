// Implementation of Arc

#include "gm2ringsim/arc/Arc_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include "gm2geom/arc/ArcGeometry.hh"

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
//#include "Geant4/G4ClassicalRK4.hh"

#include "Geant4/G4NystromRK4.hh"	
#include "Geant4/G4CashKarpRKF45.hh"	
#include "Geant4/G4ClassicalRK4.hh"	
#include "Geant4/G4ConstRK4.hh"	
#include "Geant4/G4ExplicitEuler.hh"	
#include "Geant4/G4ImplicitEuler.hh"	
#include "Geant4/G4SimpleHeum.hh"	
#include "Geant4/G4SimpleRunge.hh"	
#include "Geant4/G4HelixExplicitEuler.hh"	
#include "Geant4/G4HelixHeum.hh"	
#include "Geant4/G4HelixImplicitEuler.hh"	
#include "Geant4/G4HelixSimpleRunge.hh"

#include "gm2ringsim/arc/StorageRingField.hh"

#include "gm2ringsim/fields/g2FieldEqRhs.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithSpin.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithEDM.hh"



// Constructor for the service 
gm2ringsim::Arc::Arc(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "arc"),
	       p.get<std::string>("category", "arc"),
	       p.get<std::string>("mother_category", "world")),
  sts_("SpinTracking"), //This is a shortcut to get the SpinTracking par list from the fhicl
  nospin_tracking_(true),
  spin_tracking_(sts_.spinTrackingEnabled),
  edm_tracking_(sts_.edmTrackingEnabled),
  withoutSpin_(0),    //will set in the constructor function
  withSpin_(0),        //will set in the constructor function
  withEDM_(0)
{
  G4cout << "=========== Arc (Storage Ring Field) ===========" << G4endl;
  G4cout << "| Beam Charge        = " << sts_.GetCharge() << G4endl;
  G4cout << "| Spin Tracking      = " << spin_tracking_ << G4endl;
  G4cout << "| EDM Tracking       = " << edm_tracking_ << G4endl;
  G4cout << "| Storage Field      = " << sts_.GetStorageFieldType() << G4endl;
  G4cout << "================================================" << G4endl;


  bool myspin = false;
  bool myedm = false;

  storageRingField *storageField = new storageRingField(sts_.GetCharge(), sts_.GetStorageFieldType());
  storageRingEMField *storageEMField = new storageRingEMField(sts_.GetCharge(), sts_.GetStorageFieldType());
  
  
  //----------------------------------------
  // build the spin ignoring field equations
  //----------------------------------------
  G4Mag_EqRhs *equation = new G4Mag_UsualEqRhs(storageField);
  G4MagIntegratorStepper *mag_stepper = NULL;
  if ( sts_.GetStepperType() == kNystromRK4 ) {
    mag_stepper = new G4NystromRK4(equation);
  }
  else if ( sts_.GetStepperType() == kCashKarpRKF45 ) {
    mag_stepper = new G4CashKarpRKF45(equation);
  }
  else if ( sts_.GetStepperType() == kClassicalRK4) {
    mag_stepper = new G4ClassicalRK4(equation);
  }
  else if ( sts_.GetStepperType() == kConstRK4) {   
    mag_stepper = new G4ConstRK4(equation);
  }
  else if ( sts_.GetStepperType() == kExplicitEuler) {
    mag_stepper = new G4ExplicitEuler(equation);
  }
  else if ( sts_.GetStepperType() == kImplicitEuler) {
    mag_stepper = new G4ImplicitEuler(equation);
  }
  else if ( sts_.GetStepperType() == kSimpleHeum) {
    mag_stepper = new G4SimpleHeum(equation);
  }
  else if ( sts_.GetStepperType() == kSimpleRunge) {
    mag_stepper = new G4SimpleRunge(equation);
  }
  else if ( sts_.GetStepperType() == kHelixExplicitEuler) {
    mag_stepper = new G4HelixExplicitEuler(equation);
  }
  else if ( sts_.GetStepperType() == kHelixHeum) {
    mag_stepper = new G4HelixHeum(equation);
  }
  else if ( sts_.GetStepperType() == kHelixImplicitEuler) {
    mag_stepper = new G4HelixImplicitEuler(equation);
  }
  else if ( sts_.GetStepperType() == kHelixSimpleRunge ) {
    mag_stepper = new G4HelixSimpleRunge(equation);
  }
  else {
    G4cout << "Could not set up Mag Stepper: " << sts_.GetStepperName() << "." << G4endl; exit(1);
    mag_stepper = new G4ClassicalRK4(equation);
  }
  if ( mag_stepper ) { G4cout << "Set up Mag Stepper: " << sts_.GetStepperName() << "." << G4endl; }
  //G4double stepsize = 0.01*mm;
  G4double stepsize = sts_.GetStepSize();
  G4cout << "Using StepSize: " << stepsize/mm << " mm." << G4endl;
  
  G4MagIntegratorStepper *stepper = mag_stepper;
  //G4ClassicalRK4 *stepper = new G4ClassicalRK4(equation);
  G4ChordFinder *iChordFinder = new G4ChordFinder(storageField, stepsize, stepper);
  withoutSpin_ = new G4FieldManager(storageField, iChordFinder);



  //----------------------------------------
  // build the spin evolving field equations
  //----------------------------------------  
  // build the spin evolving field equations
  if ( edm_tracking_ ) {
    if ( myedm ) {
      g2EqEMFieldWithEDM *equation2 = new g2EqEMFieldWithEDM(storageEMField);
      equation2->SetEta(sts_.GetEta());
      if ( sts_.GetGm2() >= 0 ) { equation2->SetAnomaly(sts_.GetGm2()); }
      stepper = new G4ClassicalRK4(equation2,12);
    }
    else {
      G4EqEMFieldWithEDM *equation2 = new G4EqEMFieldWithEDM(storageEMField);
      equation2->SetEta(sts_.GetEta());
      if ( sts_.GetGm2() >= 0 ) { equation2->SetAnomaly(sts_.GetGm2()); }
      
      stepper = new G4ClassicalRK4(equation2,12);
    }
    G4MagInt_Driver *driver = new G4MagInt_Driver(0.01*mm, stepper, stepper->GetNumberOfVariables());
    iChordFinder = new G4ChordFinder(driver);
    withEDM_ = new G4FieldManager(storageEMField, iChordFinder);
  }
  if ( spin_tracking_ ) {
    //G4double stepsize = 0.01*mm;
    G4double stepsize = sts_.GetStepSize();
    G4cout << "Using StepSize: " << stepsize/mm << " mm." << G4endl;
    if ( myspin ) {
      g2EqEMFieldWithSpin *equation2 = new g2EqEMFieldWithSpin(storageEMField);   
      if ( sts_.GetStepperType() == kCashKarpRKF45 ) {
	stepper = new G4CashKarpRKF45(equation2, 12);
      }
      else if ( sts_.GetStepperType() == kClassicalRK4) {
	stepper = new G4ClassicalRK4(equation2, 12);
      }
      else if ( sts_.GetStepperType() == kImplicitEuler) {
	stepper = new G4ImplicitEuler(equation2, 12);
      }
      else if ( sts_.GetStepperType() == kSimpleHeum) {
	stepper = new G4SimpleHeum(equation2, 12);
      }
      else if ( sts_.GetStepperType() == kSimpleRunge) {
	stepper = new G4SimpleRunge(equation2, 12);
      }
      else {
	G4cout << "Could not set up Mag Stepper: " << sts_.GetStepperName() << " w/ spin." << G4endl; exit(1);
	stepper = new G4ClassicalRK4(equation2, 12);
      }
      if ( stepper ) { G4cout << "Set up Mag Stepper: " << sts_.GetStepperName() << " w/ spin." << G4endl; }
      
      //G4MagIntegratorStepper *stepper = mag_stepper;     
      //stepper = new G4ClassicalRK4(equation2,12);
    }
    else {
      equation = new G4Mag_SpinEqRhs(storageField);    
      if ( sts_.GetStepperType() == kNystromRK4 ) {
	stepper = new G4NystromRK4(equation, 12);
      }
      else if ( sts_.GetStepperType() == kCashKarpRKF45 ) {
	stepper = new G4CashKarpRKF45(equation, 12);
      }
      else if ( sts_.GetStepperType() == kClassicalRK4) {
	stepper = new G4ClassicalRK4(equation, 12);
      }
      else if ( sts_.GetStepperType() == kConstRK4) {   
	stepper = new G4ConstRK4(equation, 12);
      }
      else if ( sts_.GetStepperType() == kImplicitEuler) {
	stepper = new G4ImplicitEuler(equation, 12);
      }
      else if ( sts_.GetStepperType() == kSimpleHeum) {
	stepper = new G4SimpleHeum(equation, 12);
      }
      else if ( sts_.GetStepperType() == kSimpleRunge) {
	stepper = new G4SimpleRunge(equation, 12);
      }
      else {
	G4cout << "Could not set up Mag Stepper: " << sts_.GetStepperName() << " w/ spin." << G4endl; exit(1);
	stepper = new G4ClassicalRK4(equation, 12);
      }
      if ( stepper ) { G4cout << "Set up Mag Stepper: " << sts_.GetStepperName() << " w/ spin." << G4endl; }
      //stepper = new G4ClassicalRK4(equation,12);
    }
    iChordFinder = new G4ChordFinder(storageField, stepsize, stepper);
    withSpin_ = new G4FieldManager(storageField, iChordFinder);
  }
}

gm2ringsim::Arc::~Arc() {
  if (withoutSpin_)
    delete withoutSpin_;

  if (withSpin_)
    delete withSpin_;

  if (withEDM_)
    delete withEDM_;
}
  
void gm2ringsim::Arc::initialize() {



}

G4LogicalVolume* gm2ringsim::Arc::makeAnArcLV(gm2geom::ArcGeometry const & g, unsigned int arcNum)
{
  

  mf::LogInfo("Arc") <<"In the Arc::makeAnArcLV, with spin_tracking_ set to "<<spin_tracking_;
  mf::LogInfo("Arc") <<"In the Arc::makeAnArcLV, with edm_tracking_ set to "<<edm_tracking_;

  G4FieldManager *tmpFieldManager = NULL;
  if ( nospin_tracking_ ) { 
    tmpFieldManager = withoutSpin_;
  }
  if (spin_tracking_) {
    tmpFieldManager = withSpin_;
  }
  if (edm_tracking_) {
    tmpFieldManager = withEDM_;
  }
  
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
  gm2geom::ArcGeometry g(myName());
  
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
