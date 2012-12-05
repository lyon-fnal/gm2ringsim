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


#ifndef __MACH__
namespace {
  //  clockid_t const clkid = CLOCK_REALTIME;
  //  clock_t const clkid = CLOCK_REALTIME;
  
  // make sure the RSM is initialized before we exit PreInit state, or
  // the Messenger directories won't be populated correctly....
  //FIXME: Address in ART rootStorageManager& rsm = rootStorageManager::getInstance();
}
#endif


// Constructor
gm2ringsim::EventAndRunAction::EventAndRunAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  muonStorageStatus_(muonTrackingStatus::trackingMuon),
  //RunAction(currentRunAction), 
  hbFreq_(p.get<int>("heartbeatFreq",100)),
  hbLength_(p.get<int>("heartbeatLength",0)),
  //  ,eam_(new eventActionMessenger(this))
  start_(), // These have different types on MAC and LINUX
  end_(),   // but they are defined in the same relative order
#ifdef __MACH__
  cclock_(),
  mts_(),
#else
  clockID_(CLOCK_REALTIME),
#endif
  muonStorageCounter_(0),
  logInfo_("RunAction")
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
#ifdef __MACH__
  mach_clock_get_start_time();
#else 
  clock_gettime(clockID_, &start_);
#endif

}   


   
void gm2ringsim::EventAndRunAction::endOfRunAction(const G4Run *currentRun) {
#ifdef __MACH__
  mach_clock_get_end_time();
#else 
  clock_gettime(clockID_, &end_);
#endif
  
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
  
  G4cout << "Elapsed time this run: "
	 << clockDiff() << "sec\n";
  
  //FIXME: ARTIFY
  // No unique object manager or rootstoragemanager yet
  /*G4cout << "Unique Objects in Manager: "
	 << rsm.getUOM().count() << '\n';
  
  rsm.EndOfRun();
  */
  
} //RunAction::endOfRunAction(const G4Run*)
  

  void gm2ringsim::EventAndRunAction::incrementMuonStorageCounter()
  {++muonStorageCounter_;} 
    

#ifdef __MACH__

void gm2ringsim::EventAndRunAction::mach_clock_get_start_time(){
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock_);
  clock_get_time(cclock_, &start_);
  mach_port_deallocate(mach_task_self(), cclock_); 
}
void gm2ringsim::EventAndRunAction::mach_clock_get_end_time(){
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock_);
  clock_get_time(cclock_, &end_);
  mach_port_deallocate(mach_task_self(), cclock_); 
}
#endif


double gm2ringsim::EventAndRunAction::clockDiff()
{
  // mach_timespec_t and timespec both have the internal vars:
  // tv_nsec and tv_sec, so just let the compiler figure out the
  // type of temp and do the same calculation. In the end, we just
  // return a double

#ifdef __MACH__
  mach_timespec_t temp;
#else
  timespec temp;
#endif
  
  if ((end_.tv_nsec-start_.tv_nsec)<0) {
    temp.tv_sec = end_.tv_sec-start_.tv_sec-1;
    temp.tv_nsec = 1000000000+end_.tv_nsec-start_.tv_nsec;
  } else {
    temp.tv_sec = end_.tv_sec-start_.tv_sec;
    temp.tv_nsec = end_.tv_nsec-start_.tv_nsec;
  }
  return temp.tv_sec + temp.tv_nsec/1e9;
}//EventAndRunAction::clockDiff()




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
