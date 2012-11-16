// Implementation of Lab

#include "artg4geantn02/detectors/Lab_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Lab::Lab(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "CHANGE_ME:defaultName"),
                   p.get<std::string>("category", "CHANGE_ME:defaultCategory"),
                   p.get<std::string>("mother_category", "CHANGE_ME:defaultMother"))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Lab::doBuildLVs() {

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Lab::doPlaceToPVs( std::vector<G4LogicalVolume*>) {

}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
void gm2ringsim::Lab::doCallArtProduces(art::EDProducer * producer) {

}

// Actually add the data to the event
void gm2ringsim::Lab::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
}

using gm2ringsim::Lab;
DEFINE_ART_SERVICE(Lab)
