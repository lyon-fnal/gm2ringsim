// Implementation of EventAndRunActionService

#include "gm2ringsim/actions/EventAndRunAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "gm2ringsim/actions/EventRecord.hh"

#include "Geant4/globals.hh"

#include "Geant4/G4Event.hh"
#include "Geant4/G4EventManager.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackingManager.hh"
#include "Geant4/G4TransportationManager.hh"
#include "Geant4/G4RunManagerKernel.hh"
#include "Geant4/G4UnitsTable.hh"


#include "Geant4/G4TrajectoryContainer.hh"
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4RunManager.hh"

#include <vector>


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
gm2ringsim::EventAndRunAction::EventAndRunAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  muonStorageStatus_(muonTrackingStatus::trackingMuon),
  hbFreq_(p.get<int>("heartbeatFreq",100)),
  hbLength_(p.get<int>("heartbeatLength",0)),
  //  ,eam_(new eventActionMessenger(this))
  muonStorageCounter_(0),
  logInfo_("EventAndRunAction")
{}
  
//beginOfEventAndRunAction
void gm2ringsim::EventAndRunAction::beginOfEventAction (const G4Event* pevent){
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

void gm2ringsim::EventAndRunAction::endOfEventAction(const G4Event* ){//evt) {
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
}//endOfEventAction


void gm2ringsim::EventAndRunAction::beginOfRunAction(const G4Run *currentRun) {
  // Initialization per run
  muonStorageCounter_ = 0;
  //FIXME:TEMP to use currentRun
  std::cout<<"currentRunID is "<<currentRun->GetRunID()<<std::endl;
  
  // FIXME: ARTIFY
  // Cannot use this block yet because RootStorageManager not implemented
  /* if( !rsm.BeginOfRun(currentRun->GetRunID()) ){
     G4cout << "The Root Storage Manager had a problem in BeginOfRunAction()."
     << "  Aborting Run....\n";
     G4RunManager::GetRunManager()->AbortRun();
     }*/

}   


   
void gm2ringsim::EventAndRunAction::endOfRunAction(const G4Run *currentRun) {
  
  G4double totalEvents = currentRun -> GetNumberOfEvent();
  G4double captureEfficiency = (muonStorageCounter_*100.) / totalEvents;
  G4double dcE = std::sqrt(muonStorageCounter_)*100/totalEvents;
  
  G4cout << "\n  Muons injected : "
	 << totalEvents << "\n"
	 << "  Muons stored : "
	 << muonStorageCounter_ << "\n"
	 <<"  Capture Efficiency : (" 
	 << captureEfficiency << " +/- "
	 << dcE << ")%\n\n";
  
  //FIXME: ARTIFY
  // No unique object manager or rootstoragemanager yet
  /*G4cout << "Unique Objects in Manager: "
	 << rsm.getUOM().count() << '\n';
  
  rsm.EndOfRun();
  */
  
} //RunAction::endOfRunAction(const G4Run*)
  

  void gm2ringsim::EventAndRunAction::incrementMuonStorageCounter()
  {++muonStorageCounter_;} 
    

void gm2ringsim::EventAndRunAction::callArtProduces(art::EDProducer *producer){
  //FIXME: Need to figure out the right name with multiple base classes
  producer->produces<EventRecordCollection>("EventAndRunAction");
}

void gm2ringsim::EventAndRunAction::fillEventWithArtStuff(art::Event &e) {
  std::unique_ptr< EventRecordCollection > myArtHits(new EventRecordCollection);
  myArtHits->emplace_back( true,10,1);
  e.put(std::move(myArtHits),"EventAndRunAction");
}


/** Setter for successful storage. */
void gm2ringsim::EventAndRunAction::successfulStorage() {
  muonStorageStatus_ = muonTrackingStatus::storedMuon;
  incrementMuonStorageCounter();
  setEventStatus(kStored);
} 

/** Setter for failed storage */
void gm2ringsim::EventAndRunAction::unsuccessfulStorage(){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
}

void gm2ringsim::EventAndRunAction::unsuccessfulStorage(G4String reason){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
  if ( reason == "Lab" ) { setEventStatus(kHitLab); }
  if ( reason == "Inflector" ) { setEventStatus(kHitInflector); }
  if ( reason == "StorageRegion" ) { setEventStatus(kBeyondStorageRegion);
  }
}

using gm2ringsim::EventAndRunAction;
DEFINE_ART_SERVICE(EventAndRunAction)
