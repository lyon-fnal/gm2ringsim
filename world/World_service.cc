// Implementation of World

#include "gm2ringsim/world/World_service.hh"
#include "gm2ringsim/world/WorldGeometry.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "artg4/material/Materials.hh"

#include "Geant4/G4Box.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4Colour.hh"
#include "Geant4/G4PVPlacement.hh"


// Constructor for the service 
gm2ringsim::World::World(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "world"),
                   p.get<std::string>("category", "world"),
                   p.get<std::string>("mother_category", ""))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::World::doBuildLVs() {
  
  // Need geometry
  WorldGeometry g(myName());
  
  // Let's print out the geometry
  g.print();
  
  // Make the big box
  G4Box* world_solid = new G4Box(myName(), g.world_x, g.world_y, g.world_z );
  
  // Make the logical volume
  G4LogicalVolume* world_lv = new G4LogicalVolume(world_solid, artg4Materials::Air(), myName() );
  
  // Set visualization attributes
  G4VisAttributes* worldVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0));
  world_lv->SetVisAttributes(worldVisAtt);
 
  // Make a vector with first element and return it
  return std::vector<G4LogicalVolume*> {world_lv};
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::World::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
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

using gm2ringsim::World;
DEFINE_ART_SERVICE(World)
