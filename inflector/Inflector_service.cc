// Implementation of Inflector

#include "artg4geantn02/detectors/Inflector_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Inflector::Inflector(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "inflector"),
                   p.get<std::string>("category", "inflector"),
                   p.get<std::string>("mother_category", "CHANGE_ME:defaultMother"))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Inflector::doBuildLVs() {

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Inflector::doPlaceToPVs( std::vector<G4LogicalVolume*>) {

}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
void gm2ringsim::Inflector::doCallArtProduces(art::EDProducer * producer) {

}

// Actually add the data to the event
void gm2ringsim::Inflector::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
}

using gm2ringsim::Inflector;
DEFINE_ART_SERVICE(Inflector)
