// Implementation of Collimator

#include "gm2ringsim/collimator/Collimator_service.hh"
#include "gm2ringsim/common/g2PreciseValues.cc"

#include "artg4/material/Materials.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/util/util.hh" 

#include "Geant4/G4Tubs.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4PVPlacement.hh"

using namespace gm2ringsim;

// Constructor for the service 
gm2ringsim::Collimator::Collimator(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "collimator"),
	       p.get<std::string>("category", "collimator"),
	       p.get<std::string>("mother_category", "vac")),
  geom_(myName())
{
  geom_.print();
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Collimator::doBuildLVs() {
  std::vector<G4LogicalVolume *> collLVs;
  
  // Loop over all 9 collimators
  for(G4int collNumber = 0; collNumber < geom_.nCollimators; collNumber++){
    if(geom_.collimatorType[collNumber] == OFF){
      collLVs.push_back(0);
      continue;
    }

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
std::vector<G4VPhysicalVolume *> gm2ringsim::Collimator::doPlaceToPVs( std::vector<G4LogicalVolume*> vacWalls) {
  G4double const theta1  = 0., phi = pi/2., theta2 = 0.;
  std::vector<G4VPhysicalVolume *> collPVs;

  char objectName[30];
  
  for(G4int collNumber = 0; collNumber < geom_.nCollimators; collNumber++){
    if(geom_.collimatorType[collNumber] == OFF) continue;

    if (vacWalls[geom_.cVacWallArray[collNumber]]->GetNoDaughters() != 1 ) {
      throw cet::exception("Collimator_service") << "What?? My wall has no vacuum! Abort! \n";
    }
    
    // If collimator is a half collimator on the outside of the storage
    // ring, then use this angle for Euler angle theta2
    G4double theta = theta2;
    if(geom_.collimatorType[collNumber] == HALF_LRO)
      theta = pi;
 
    // Get the vacuum chamber logical volume from the wall
    G4LogicalVolume* vacLV = vacWalls[geom_.cVacWallArray[collNumber]]->GetDaughter(0)->GetLogicalVolume();
   
    std::sprintf(objectName, "%s-%02d", e_collimator_names[geom_.collimatorType[collNumber]].c_str(),  collNumber);

    /** @bug tCB_Dphi from vacChamberParameters.hh should be used
	instead of 0.01*degree, but we have to fix the multiple
	inclusion problem. */
    collPVs.push_back(new G4PVPlacement(new G4RotationMatrix(theta1+0.01*degree, phi, theta),
					G4ThreeVector(R_magic(),
						      geom_.coll_z+R_magic()*sin(0.01*degree),
						      0.*m),
					lvs()[collNumber],
					objectName,
					vacLV,
					false,
					collNumber));
  }
  
  return collPVs;
}

using gm2ringsim::Collimator;
DEFINE_ART_SERVICE(Collimator)
