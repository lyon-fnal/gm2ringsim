// Implementation of Collimator

#include "gm2ringsim/collimator/Collimator_service.hh"

#include "artg4/material/Materials.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/util/util.hh" 

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PVPlacement.hh"

// There are three types of collimators
namespace gm2ringsim {
  enum e_collimator_type {FULL, HALF_LRO, HALF_SRO, OFF};
  std::vector<const char*> e_collimator_names = { "Collimator(full)", "Collimator(half-lro)", 
						  "Collimator(half-sro)", ""};
}

// Constructor for the service 
gm2ringsim::Collimator::Collimator(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "collimator"),
	       p.get<std::string>("category", "collimator"),
	       p.get<std::string>("mother_category", "vac")),
  geom_(myName())
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Collimator::doBuildLVs() {
  std::vector<G4LogicalVolume *> collLVs;
  
  // Loop over all 9 collimators
  for(G4int collNumber = 0; collNumber < geom_.nCollimators; collNumber++){
    if(geom_.collimatorType[collNumber] == OFF) continue;
    
    G4Tubs *collimator_S = new G4Tubs("coll", 
				      geom_.coll_rMin, 
				      geom_.coll_rMax, 
				      geom_.coll_z, 
				      geom_.coll_Sphi[geom_.collimatorType[collNumber]], 
				      geom_.coll_Dphi[geom_.collimatorType[collNumber]]);
    
    G4LogicalVolume *collimator_L = new G4LogicalVolume(collimator_S,
							artg4Materials::Cu(), "coll");
    
    collLVs.push_back(collimator_L); 

    // Set the visualization attributes
    artg4::setVisAtts(collimator_L, geom_.display, geom_.collimatorColor);
  }
  
  
  return collLVs;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Collimator::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  std::vector<G4VPhysicalVolume *> collPVs;

  //char objectName[30];
  
  return collPVs;
}

using gm2ringsim::Collimator;
DEFINE_ART_SERVICE(Collimator)
