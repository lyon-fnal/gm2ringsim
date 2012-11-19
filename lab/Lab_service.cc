// Implementation of Lab

#include "gm2ringsim/lab/Lab_service.hh"
#include "gm2ringsim/lab/LabGeometry.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/services/Material_service.hh"

// Constructor for the service 
gm2ringsim::Lab::Lab(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "world"),
                   p.get<std::string>("category", "world"),
                   p.get<std::string>("mother_category", ""))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Lab::doBuildLVs() {
  
  // Need geometry
  LabGeometry g(myName());
  
  // Make the big box
  G4Box* lab_solid = new G4Box(myName(), g.lab_x, g.lab_y, g.lab_z );
  
  // Need materials
  art::ServiceHandle<artg4::MaterialService> m;
  
  // Make the logical volume
  G4LogicalVolume* lab_lv = new G4LogicalVolume(lab_solid, m->newAir(), myName() );
  
  // Set visualization attributes
  G4VisAttributes* labVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  lab_lv->SetVisAttributes(labVisAtt);
 
  // Make a vector with first element and return it
  return std::vector<G4LogicalVolume*> {lab_lv};
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Lab::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  return std::vector<G4VPhysicalVolume*> {
    new G4PVPlacement( 0,                 // no rotation
                       G4ThreeVector(),   // at (0,0,0)
                       lvs()[0],          // The logical volume
                       myName(),          // its name
                       0,                 // NO mother volume
                       false,             // No boolean operations
                       0 )                // copy number
       };
}

using gm2ringsim::Lab;
DEFINE_ART_SERVICE(Lab)
