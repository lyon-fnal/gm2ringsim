// Implementation of EventActionService

#include "gm2ringsim/actions/EventAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "gm2ringsim/actions/RunAction_service.hh"
#include "gm2ringsim/actions/EventRecord.hh"

#include "Geant4/globals.hh"

#include "Geant4/G4Event.hh"
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackingManager.hh"
#include "Geant4/G4TransportationManager.hh"
#include "Geant4/G4RunManagerKernel.hh"
#include "Geant4/G4UnitsTable.hh"

//FIXME: include
//#include "turnCounter.hh"

//#include "runAction.hh"

//#include "rootStorageManager.hh"

#include <string>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

// Constructor
gm2ringsim::EventAction::EventAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  muonStorageStatus_(muonTrackingStatus::trackingMuon),
  //RunAction(currentRunAction), 
  hbFreq_(p.get<int>("heartbeatFreq",100)),
  hbLength_(p.get<int>("heartbeatLength",0))
  //  ,eam_(new eventActionMessenger(this))
{}
  
//beginOfEventAction
void gm2ringsim::EventAction::beginOfEventAction (const G4Event* pevent){
  // Initialization per event
  muonStorageStatus_ = muonTrackingStatus::trackingMuon;

  // reset the turn counter
  // FIXME: ARTIFY
  //  turnCounter::getInstance().reset();
  
  // Initialize event status
  setEventStatus(-1);

 // Heartbeat handler
  if( hbFreq_ > 0 && pevent->GetEventID()!=0 && pevent->GetEventID()%hbFreq_ == 0 ){
    //FIXME: Replace with messageFacility
    std::ostringstream oss;
    oss << "Processing Event Number " << pevent->GetEventID();
    std::string s = oss.str();
    std::string bs(hbLength_, '\b');
    G4cout << bs << s << std::flush;
    hbLength_ = s.size();
  }

}  //beginOfEventAction
// EndOfEventAction
void gm2ringsim::EventAction::endOfEventAction(const G4Event* ){//evt) {
  //FIXME: NOTHING other than the G4cout was commented out.
  // All needs to be reimplemented in ART

  // store junk in Root file
  //FIXME: Need to bypass rootStorageManager and 
  // directly shove this into the ART eventRecord
  //rootStorageManager & rsm = rootStorageManager::getInstance();

  // populate the final event record
  //eventRecord& er = rsm.getEventRecord();
  //er.muWasStored = 
  //(muonStorageStatus_ == muonTrackingStatus::storedMuon) ? true : false;
  //er.lastTurn =
  //turnCounter::getInstance().turns();

  // Fill the event status flag
  // er.EventStatus = (Int_t)getEventStatus();

  //G4cout << "EventStatus = " << er.EventStatus << "\n";
  
  // fill all the trees
  
  //rsm.fill_trees(currentEvent);
}   

/*
void gm2ringsim::EventAction::callArtProduces(art::EDProducer *producer){
  producer->produces<EventRecordCollection*>(myName());
}

void gm2ringsim::EventAction::fillEventWithArtStuff(art::Event &e) {
  std::unique_ptr< EventRecordCollection > myArtHits(new EventRecordCollection);
  myArtHits->emplace_back( true,10,1);
  e.put(std::move(myArtHits),myName());
}
*/

/** Setter for successful storage. */
void gm2ringsim::EventAction::successfulStorage() {
  muonStorageStatus_ = muonTrackingStatus::storedMuon;
  //FIXME: Deal with RunAction in ART
  //RunAction -> incrementMuonStorageCounter();
  
  //Get an ActionService Handle
  /*  art::ServiceHandle<gm2ringsim::RunAction> runAction;
  runAction->incrementMuonStorageCounter();
  */

  setEventStatus(kStored);
} 

/** Setter for failed storage */
void gm2ringsim::EventAction::unsuccessfulStorage(){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
}

void gm2ringsim::EventAction::unsuccessfulStorage(G4String reason){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
  if ( reason == "Lab" ) { setEventStatus(kHitLab); }
  if ( reason == "Inflector" ) { setEventStatus(kHitInflector); }
  if ( reason == "StorageRegion" ) { setEventStatus(kBeyondStorageRegion);
  }
}

using gm2ringsim::EventAction;
DEFINE_ART_SERVICE(EventAction)
