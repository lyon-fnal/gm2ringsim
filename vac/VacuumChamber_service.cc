// Implementation of VacuumChamber

#include "gm2ringsim/vac/VacuumChamber_service.hh"

#include "gm2ringsim/vac/VacGeometry.hh"

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
  
  VacGeometry g(myName());
  g.print();
  
  return std::vector<G4LogicalVolume*> { 0 };

}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::VacuumChamber::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  
  return std::vector<G4VPhysicalVolume*> { 0 };
}

using gm2ringsim::VacuumChamber;
DEFINE_ART_SERVICE(VacuumChamber)
