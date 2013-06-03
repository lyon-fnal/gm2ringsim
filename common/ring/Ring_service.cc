// Implementation of Ring
#include "gm2ringsim/common/ring/Ring_service.hh"
#include "gm2ringsim/common/ring/RingHit.hh"
#include "gm2ringsim/common/ring/arthits/RingArtRecord.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh" 

#include "Geant4/G4RunManager.hh"
#include "Geant4/G4SDManager.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include <iostream>

// Constructor for the service 
gm2ringsim::Ring::Ring(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "Ring"),
	       p.get<std::string>("category", "Ring"),
	       p.get<std::string>("mother_category", "vac")),
  ringSDname_("RingSD"),
  ringSD_(artg4::getSensitiveDetector<RingSD>(ringSDname_))
{

}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Ring::doBuildLVs() {
  std::vector<G4LogicalVolume *> dummy;
  return dummy;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Ring::doPlaceToPVs(std::vector<G4LogicalVolume*> ){
  std::vector<G4VPhysicalVolume *> dummy;
  return dummy;
}

// Declare to Art what we are producing
void gm2ringsim::Ring::doCallArtProduces(art::EDProducer *producer) {
  mf::LogInfo("Ring_service") << "About to doCallArtProduces";
  producer->produces<RingArtRecordCollection>(category());
  mf::LogInfo("Ring_service") << "done with doCallArtProduces";
}


// Actually add the data to the event

void gm2ringsim::Ring::doFillEventWithArtHits(G4HCofThisEvent * hc) {
  mf::LogInfo("Ring") << "About to doFillEventWithArtHits";
  std::unique_ptr<RingArtRecordCollection> myArtHits(new RingArtRecordCollection);
  
  // Find the collection ID for the hits
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  // The string here is unfortunately a magic constant. It's the string used
  // by the sensitive detector to identify the collection of hits.
  G4int collectionID = fSDM->GetCollectionID(ringSDname_);
  RingHitsCollection* myCollection = 
    static_cast<RingHitsCollection*>(hc->GetHC(collectionID));

  // Check whether the collection exists
  if (NULL != myCollection) {
    std::vector<RingHit*> geantHits = *(myCollection->GetVector());
    mf::LogInfo("Ring") << "The size of the RingHit vector is "<<geantHits.size();
    for ( auto e : geantHits ) {
      myArtHits->push_back(convert(e));
    }
  } 
  else {
    throw cet::exception("Ring") << "Null collection of Geant Ring hits"
				      << ", aborting!" << std::endl;
  }

  // Now that we have our collection of artized hits, add them to the event.
  // Get the event from the detector holder service
  art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
  art::Event & e = detectorHolder -> getCurrArtEvent();

  // Put the hits into the event
  e.put(std::move(myArtHits), category());

}

using gm2ringsim::Ring;

DEFINE_ART_SERVICE(Ring)
