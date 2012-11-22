// Implementation of VacuumChamber

#include "gm2ringsim/vac/VacuumChamber_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

// Constructor for the service 
gm2ringsim::VacuumChamber::VacuumChamber(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "vac"),
                   p.get<std::string>("category", "vac"),
                   p.get<std::string>("mother_category", "arc"))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::VacuumChamber::doBuildLVs() {

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::VacuumChamber::doPlaceToPVs( std::vector<G4LogicalVolume*>) {

}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
void gm2ringsim::VacuumChamber::doCallArtProduces(art::EDProducer * producer) {

}

// Actually add the data to the event
void gm2ringsim::VacuumChamber::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    
}

using gm2ringsim::VacuumChamber;
DEFINE_ART_SERVICE(VacuumChamber)
