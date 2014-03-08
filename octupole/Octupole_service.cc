// Implementation of Octupole
/** @file Octupole_service.cc

    Provides implementation of the octupoleConstruction member
    functions, and the geometry parameters of the physical octupole
    device.

    Reference values are derived from Koop et.al.  See reference
    documentation in the NIM directory (even though it is not actually
    a NIM).

    @author Kevin Lynch 
    @date 2011
    ARTized by
    @author Brendan Kiburg
    @date 2013

 */


#include "gm2ringsim/octupole/Octupole_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"


#include "Geant4/G4Torus.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4RunManager.hh"
#include "Geant4/G4UImanager.hh"
#include "Geant4/G4UnitsTable.hh"

#include "Geant4/G4Mag_EqRhs.hh"
#include "Geant4/G4ClassicalRK4.hh"
#include "Geant4/G4MagIntegratorStepper.hh"
#include "Geant4/G4ChordFinder.hh"

#include "gm2ringsim/fields/g2FieldEqRhs.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithSpin.hh"
#include "gm2ringsim/fields/g2EqEMFieldWithEDM.hh"
#include "artg4/util/array_macros.hh"
#include "artg4/material/Materials.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"

#include "gm2ringsim/common/ring/RingSD.hh"
#include "artg4/util/util.hh" 

#include <sstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <tr1/functional>

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Octupole::Octupole(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "octupole"),
	       p.get<std::string>("category", "octupole"),
	       p.get<std::string>("mother_category", "vac")) ,
  //FIXME: check that vac is the mother
  sts_("SpinTracking"),
  nospin_tracking_(true),
  spin_tracking_(sts_.spinTrackingEnabled),
  edm_tracking_(sts_.edmTrackingEnabled),
  og_(myName()), //OctupoleGeometry 
  withSpin_(0),
  withoutSpin_(0),
  withEDM_(0),
  fieldRegion_L_(0),
  fieldRegion_P_(0)
{
  if ( spin_tracking_ || edm_tracking_ ) { nospin_tracking_ = false; }

  G4cout << "=========== Octupole ===========" << G4endl;
  G4cout << "| Beam Charge      = " << sts_.GetCharge() << G4endl;
  G4cout << "| Spin Tracking    = " << spin_tracking_ << G4endl;
  G4cout << "| EDM Tracking     = " << edm_tracking_ << G4endl;
  G4cout << "=================================" << G4endl;

  for( std::size_t i=0; i!=tubes_L_.size(); ++i){
    tubes_L_[i] = water_L_[i] = 0;
    tubes_P_[i] = water_P_[i] = 0;
  }
  //FIXME: move to some common units file
  new G4UnitDefinition("gauss/cm3", "G/cm^3", "Octupole Gradient", gauss/cm3);
  new G4UnitDefinition("kiloauss/cm3", "kG/cm^3", "Octupole Gradient", kilogauss/cm3);
  new G4UnitDefinition("tesla/cm3", "T/cm^3", "Octupole Gradient", tesla/cm3);
  
  og_.print();
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Octupole::doBuildLVs() {
  
  buildLogicals();
  //FIXME: Decide if I want to save logical volumes
  std::vector<G4LogicalVolume *> l_octs;
  return l_octs;
  
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Octupole::doPlaceToPVs( std::vector<G4LogicalVolume*> motherLVs) {
  
  buildPhysicals(motherLVs);
  //FIXME: Decide if I want to save physicalVolumes
  std::vector<G4VPhysicalVolume *> p_octs;
  return p_octs;
  
}

void gm2ringsim::Octupole::buildLogicals(){
  // Build first the field region
  G4Torus *fieldRegion_S = new G4Torus("fieldRegion_S",
				       0., og_.fieldRad,
				       R_magic(), 
				       og_.fieldStartPhi, 
				       og_.fieldDeltaPhi);
  
  fieldRegion_L_ = new G4LogicalVolume(fieldRegion_S,
				       artg4Materials::Vacuum(),
				       "octupoleFieldRegion_L");	
  
  artg4::setVisAtts( fieldRegion_L_,
                     og_.display, og_.color,
                     [] (G4VisAttributes* att) {
                       att->SetForceWireframe(1);
		       //att->SetVisibility(0);
                     } );

  // Let's next build the tubes ... gonna loop like its 1999!

  G4Torus *tube_S;
  std::ostringstream oss;

  RingSD* ringSD = artg4::getSensitiveDetector<RingSD>("RingSD");
  for( std::size_t i=0; i!=dimensionof(og_.thetaPipe); ++i){
    oss.str("");
    oss << "tube_" << std::setw(2) << std::setfill('0') << i << "_S";
    tube_S = new G4Torus(oss.str(), 
			 0, og_.outerPipeRad,
			 R_magic()-og_.pipePlaceRad*std::cos(og_.thetaPipe[i]),
			 og_.fieldStartPhi, og_.fieldDeltaPhi);
    oss.str("");
    oss << "tube_" << std::setw(2) << std::setfill('0') << i << "_L";
    tubes_L_[i] = new G4LogicalVolume(tube_S,
				     artg4Materials::Cu(),
				     oss.str());
    
    // this is a semitransparent copper color, but most of the viewers
    // don't use the alpha channel.
    artg4::setVisAtts( tubes_L_[i],
		       og_.displayTubes, og_.tubeColor,
		       [] (G4VisAttributes* att) {
			 att->SetForceWireframe(1);
			 att->SetVisibility(1);
		       //att->SetVisibility(0);
                     } );
    tubes_L_[i]->SetSensitiveDetector( ringSD );


    oss.str("");
    oss << "water_" << std::setw(2) << std::setfill('0') << i << "_S";
    tube_S = new G4Torus(oss.str(), 
			 0, og_.innerPipeRad,
			 R_magic()-og_.pipePlaceRad*std::cos(og_.thetaPipe[i]),
			 og_.fieldStartPhi, og_.fieldDeltaPhi);
    oss.str("");
    oss << "water_" << std::setw(2) << std::setfill('0') << i << "_L";
    water_L_[i] = new G4LogicalVolume(tube_S,
				     artg4Materials::H2O(),
				     oss.str());
    
    artg4::setVisAtts( water_L_[i],
		       og_.displayWater, og_.waterColor,
		       [] (G4VisAttributes* att) {
			 att->SetForceSolid(1);
			 att->SetVisibility(1);
                     } );

    water_L_[i]->SetSensitiveDetector( ringSD );

  } // loop over tubes/water
  buildFieldManagers();
}

void gm2ringsim::Octupole::buildFieldManagers(){
  OctupoleField *field = new OctupoleField(sts_.GetCharge(), sts_.GetStorageFieldType());
  G4Mag_EqRhs *equation;
  G4MagIntegratorStepper *stepper;
  G4ChordFinder *chord;

  bool myspin = false;
  bool myedm = false;
  
  // spin free
  if ( nospin_tracking_ ) {
    equation = new g2TimeDepMagField_EqRhs(field);
    stepper = new G4ClassicalRK4(equation, 8);
    chord = new G4ChordFinder(field, .01*mm, stepper);
    withoutSpin_ = new G4FieldManager(field, chord);
  }

  // with spin
  if ( spin_tracking_ ) {
    if ( myspin ) {
      g2EqEMFieldWithSpin *equation2 = new g2EqEMFieldWithSpin(field);
      stepper = new G4ClassicalRK4(equation2, 12);
    }
    else {
      equation = new g2TimeDepMagField_SpinEqRhs(field);
      stepper = new G4ClassicalRK4(equation, 12);
    }
    chord = new G4ChordFinder(field, .01*mm, stepper);
    withSpin_ = new G4FieldManager(field, chord);
  }

  // with edm
  if ( edm_tracking_ ) {
    if ( myedm ) {
      g2EqEMFieldWithEDM *equation2 = new g2EqEMFieldWithEDM(field);
      equation2->SetEta(sts_.GetEta());
      if ( sts_.GetGm2() >= 0 ) { equation2->SetAnomaly(sts_.GetGm2()); }
      stepper = new G4ClassicalRK4(equation2, 12);
    }
    else {
      equation = new g2TimeDepMagField_SpinEqRhs(field);
      stepper = new G4ClassicalRK4(equation, 12);
    }
    chord = new G4ChordFinder(field, .01*mm, stepper);
    withEDM_ = new G4FieldManager(field, chord);
  }
}

void gm2ringsim::Octupole::do_enable_spintracking(){
  if( nospin_tracking_ ) {
    fieldRegion_L_->SetFieldManager(withSpin_, true);
  }
  else if ( spin_tracking_ ) {
    fieldRegion_L_->SetFieldManager(withoutSpin_, true);
  }
  else if ( edm_tracking_ ) {
    fieldRegion_L_->SetFieldManager(withEDM_, true);
  }
}

void gm2ringsim::Octupole::buildPhysicals(std::vector<G4LogicalVolume*> motherLVs){
  G4LogicalVolume* vacSection = motherLVs[og_.whichSector];
  
  std::ostringstream oss;

  fieldRegion_P_ = new G4PVPlacement(0, G4ThreeVector(), fieldRegion_L_, 
				    "octupoleFieldRegion",
				    vacSection,
				    false, 0);

  for( std::size_t i=0; i!=tubes_P_.size(); ++i){
    oss.str("");
    oss << "octupole_tube_" << std::setw(2) << std::setfill('0') << i;;
    G4ThreeVector tv(0, 
		     0,
		     -og_.pipePlaceRad*std::sin(og_.thetaPipe[i]));
    tubes_P_[i] = new G4PVPlacement(0, tv, tubes_L_[i], oss.str(),
				   fieldRegion_L_, false, 0);
    oss.str("");
    oss << "octupole_water_" << std::setw(2) << std::setfill('0') << i;;
    water_P_[i] = new G4PVPlacement(0, G4ThreeVector(), water_L_[i], oss.str(),
				   tubes_L_[i], false, 0);
  }

  do_enable_spintracking();
}



#include "Geant4/G4Field.hh"
#include "gm2ringsim/octupole/OctupoleField.hh"

// This is a nasty little function ... don't mess with it!
// Manipulating the field like this is safe in this case, but not in
// general.
gm2ringsim::OctupoleField *gm2ringsim::Octupole::GetField() const {
  G4Field const* f = fieldRegion_L_->GetFieldManager()->GetDetectorField();
  return 
    static_cast<OctupoleField*>(const_cast<G4Field*>(f));
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Octupole::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Octupole::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Octupole;
DEFINE_ART_SERVICE(Octupole)
