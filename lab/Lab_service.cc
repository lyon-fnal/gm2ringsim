// Implementation of Lab

#include "artg4geantn02/detectors/Lab_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Lab::Lab(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "lab"),
                   p.get<std::string>("category", "lab"),
                   p.get<std::string>("mother_category", ""))
{}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Lab::doBuildLVs() {

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Lab::doPlaceToPVs( std::vector<G4LogicalVolume*>) {

}

using gm2ringsim::Lab;
DEFINE_ART_SERVICE(Lab)
