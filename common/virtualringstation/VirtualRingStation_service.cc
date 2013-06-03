// Implementation of VirtualRingStation
#include "gm2ringsim/common/virtualringstation/VirtualRingStation_service.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationHit.hh"
#include "gm2ringsim/common/virtualringstation/arthits/VirtualRingStationArtRecord.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh" 

#include "Geant4/G4RunManager.hh"
#include "Geant4/G4SDManager.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

#include <iostream>

// Constructor for the service 
gm2ringsim::VirtualRingStation::VirtualRingStation(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
  DetectorBase(p,
	       p.get<std::string>("name", "VirtualRingStation"),
	       p.get<std::string>("category", "VirtualRingStation"),
	       p.get<std::string>("mother_category", "vac")),
  virtualringstationSDname_("VirtualRingStationSD"),
  virtualringstationSD_(artg4::getSensitiveDetector<VirtualRingStationSD>(virtualringstationSDname_))
{

}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::VirtualRingStation::doBuildLVs() {
  std::vector<G4LogicalVolume *> dummy;
  return dummy;
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::VirtualRingStation::doPlaceToPVs(std::vector<G4LogicalVolume*> ){
  std::vector<G4VPhysicalVolume *> dummy;
  return dummy;
}

// Declare to Art what we are producing
void gm2ringsim::VirtualRingStation::doCallArtProduces(art::EDProducer *producer) {
  mf::LogInfo("VirtualRingStation_service") << "About to doCallArtProduces";
  producer->produces<VirtualRingStationArtRecordCollection>(category());
  mf::LogInfo("VirtualRingStation_service") << "done with doCallArtProduces";
}


// Actually add the data to the event

void gm2ringsim::VirtualRingStation::doFillEventWithArtHits(G4HCofThisEvent * hc) {
  mf::LogInfo("VirtualRingStation") << "About to doFillEventWithArtHits";
  std::unique_ptr<VirtualRingStationArtRecordCollection> myArtHits(new VirtualRingStationArtRecordCollection);
  
  // Find the collection ID for the hits
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  // The string here is unfortunately a magic constant. It's the string used
  // by the sensitive detector to identify the collection of hits.
  G4int collectionID = fSDM->GetCollectionID(virtualringstationSDname_);
  mf::LogInfo("VirtualRingStation") << "Identified the virtualringstation collectionID as "<<collectionID;
  VirtualRingStationHitsCollection* myCollection = 
    static_cast<VirtualRingStationHitsCollection*>(hc->GetHC(collectionID));

  // Check whether the collection exists
  if (NULL != myCollection) {
    std::vector<VirtualRingStationHit*> geantHits = *(myCollection->GetVector());
    mf::LogInfo("VirtualRingStation") << "The size of the VirtualRingStationHit vector is "<<geantHits.size();
    for ( auto e : geantHits ) {
      myArtHits->push_back(convert(e));
    }
  } 
  else {
    throw cet::exception("VirtualRingStation") << "Null collection of Geant VirtualRingStation hits"
				      << ", aborting!" << std::endl;
  }

  // Now that we have our collection of artized hits, add them to the event.
  // Get the event from the detector holder service
  art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
  art::Event & e = detectorHolder -> getCurrArtEvent();

  // Put the hits into the event
  e.put(std::move(myArtHits), category());

}

using gm2ringsim::VirtualRingStation;

DEFINE_ART_SERVICE(VirtualRingStation)
