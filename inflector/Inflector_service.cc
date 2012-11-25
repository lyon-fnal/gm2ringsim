// Implementation of Inflector

#include "gm2ringsim/inflector/Inflector_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Inflector::Inflector(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "inflector"),
	       p.get<std::string>("category", "inflector"),
	       //	       p.get<std::string>("mother_category", "vac")),
	       p.get<std::string>("mother_category", "world")),
  infGeom_(myName())
{
  printf("In the Inflector service contsructor\n");
 }

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Inflector::doBuildLVs() {
  
  infGeom_.print();
  
  std::vector<G4LogicalVolume *> l_inflector;
  return l_inflector;

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Inflector::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  
  std::vector<G4VPhysicalVolume *> p_inflector;
  return p_inflector;

}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Inflector::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Inflector::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Inflector;
DEFINE_ART_SERVICE(Inflector)
