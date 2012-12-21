// Implementation of Kicker

#include "gm2ringsim/kicker/Kicker_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

//#include CHANGE_ME: Add include for header for Art hit class

// Constructor for the service 
gm2ringsim::Kicker::Kicker(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
                   p.get<std::string>("name", "kicker"),
                   p.get<std::string>("category", "kicker"),
		 p.get<std::string>("mother_category", "vac")),
    kg_("Kicker")
{
  kg_.print();
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Kicker::doBuildLVs() {
  //FIXME: Decide which logical volumes we want to store in ART
  std::vector<G4LogicalVolume *> l_kicker;
  return l_kicker;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Kicker::doPlaceToPVs( std::vector<G4LogicalVolume*>) {
  //FIXME: Decide which physical volumes we want to store in ART
  std::vector<G4VPhysicalVolume *> p_kicker;
  return p_kicker;
}

// CHANGE_ME: You can delete the below if this detector creates no data

// Declare to Art what we are producing
//void gm2ringsim::Kicker::doCallArtProduces(art::EDProducer * producer) {
//
//}

// Actually add the data to the event
//void gm2ringsim::Kicker::doFillEventWithArtHits(G4HCofThisEvent * hc) {
//    
//}

using gm2ringsim::Kicker;
DEFINE_ART_SERVICE(Kicker)
