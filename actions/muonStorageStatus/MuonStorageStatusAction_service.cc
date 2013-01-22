// Implementation of MuonStorageStatusAction Service

#include "gm2ringsim/actions/muonStorageStatus/MuonStorageStatusAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "gm2ringsim/actions/muonStorageStatus/EventRecord.hh"

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
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4UnitsTable.hh"
#include "Geant4/G4RunManagerKernel.hh"

#include <vector>

//FIXME: include
#include "TurnCounter.hh"
//#include "rootStorageManager.hh"

#include <string>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

// Constructor
gm2ringsim::MuonStorageStatusAction::MuonStorageStatusAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  artg4::SteppingActionBase(p.get<std::string>("name")),
  muonStorageStatus_(muonTrackingStatus::trackingMuon),
  hbFreq_(p.get<int>("heartbeatFreq",100)),
  hbLength_(p.get<int>("heartbeatLength",0)),
  muonStorageCounter_(0),
  turnsForStorage_(p.get<float>("turnsForStorage",2160)), //NSF: 5 muon decay lifetimes (in the lab frame)
  stored_rmin_(p.get<double>("stored_rmin", 7.0) * m), //g2migtrace defaults
  stored_rmax_(p.get<double>("stored_rmax",8.0) * m),
  stored_y_(p.get<double>("stored_y",0.074) * m),
  logInfo_("MuonStorageStatusAction")
{}
  
//beginOfMuonStorageStatusAction
void gm2ringsim::MuonStorageStatusAction::beginOfEventAction (const G4Event* pevent){
  // Initialization per event
  muonStorageStatus_ = muonTrackingStatus::trackingMuon;

  // reset the turn counter
  TurnCounter::getInstance().reset();
  
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

void gm2ringsim::MuonStorageStatusAction::endOfEventAction(const G4Event* ){//evt) {
  //FIXME: NOTHING other than the G4cout was commented out.
  // All needs to be reimplemented in ART


  //FIXME: If we need anything from the Geant event, we need to do it here.

  // store junk in Root file
  //FIXME: Need to bypass rootStorageManager and 
  // directly shove this into the ART eventRecord
  //rootStorageManager & rsm = rootStorageManager::getInstance();

  // populate the final event record
  //eventRecord& er = rsm.getEventRecord();
  //er.muWasStored = 
  //(muonStorageStatus_ == muonTrackingStatus::storedMuon) ? true : false;
  //er.lastTurn =
  //TurnCounter::getInstance().turns();

  // Fill the event status flag
  // er.EventStatus = (Int_t)getEventStatus();

  //G4cout << "EventStatus = " << er.EventStatus << "\n";
  
  // fill all the trees
  
  //rsm.fill_trees(currentEvent);
}//endOfEventAction


void gm2ringsim::MuonStorageStatusAction::beginOfRunAction(const G4Run *currentRun) {
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


   
void gm2ringsim::MuonStorageStatusAction::endOfRunAction(const G4Run *currentRun) {
  
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
  // If we need to do anything in Geant
  // No unique object manager or rootstoragemanager yet

  /*G4cout << "Unique Objects in Manager: "
	 << rsm.getUOM().count() << '\n';
  
  rsm.EndOfRun();
  */
  
} //RunAction::endOfRunAction(const G4Run*)


/** Operates on each step.    
    - Kills tracks if they wander into theLaboratory or the Inflector
    Mandrel.         
    - Kills a track and ends the event if the muon survives for a
    sufficient number of turns.
    - Kills a track if it wanders too far outside the storage volume.
*/  

void gm2ringsim::MuonStorageStatusAction::userSteppingAction(const G4Step *currentStep) {
 // currentStep, currentTrack, and currentEvent are the three
  // sources of relevant information, and therefore, access is
  // needed to their respective pointers.  While the G4Step pointer
  // is passed into this function automatically, the remaining two
  // pointers must be obtained manually
  
  G4Track *currentTrack = currentStep -> GetTrack();
  
  // MUON STORAGE  
  G4String const& currentVolumeName =
    currentTrack -> GetVolume() -> GetName();

  //  if( currentTrack->GetTrackID() == 1 )                                                                          
  //    G4cout << currentVolumeName << '\n';                                                                         
  
  if( currentVolumeName == "InflectorMandrel" ){
    if( currentTrack -> GetTrackID() == 1 ) {
      unsuccessfulStorage("Inflector"); //Part of EventAction
    }
    currentTrack -> SetTrackStatus(fStopAndKill);
  } else if( currentVolumeName == "theLaboratory" ){
    if( currentTrack -> GetTrackID() == 1 ) {
      unsuccessfulStorage("Lab"); //Part of EventAction
    }
    currentTrack -> SetTrackStatus(fStopAndKill);
  }

  G4int turn = TurnCounter::getInstance().turns();
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if( turn >= turnsForStorage_ )
      //  if( trackLength > minStoredTrackLength)                                                                    
      {
        successfulStorage(); //Part of EventAction
	currentTrack -> SetTrackStatus(fStopAndKill);
      }
  }
  
  G4ThreeVector const currentPos =
    currentStep -> GetPostStepPoint() -> GetPosition();
  G4double const posX = currentPos.getX();
  G4double const posY = currentPos.getY();
  G4double const posZ = currentPos.getZ();
  G4double const posR = sqrt(posX*posX + posZ*posZ);
  
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if(posR < stored_rmin_ || posR > stored_rmax_ || std::abs(posY) > stored_y_)
      {
        unsuccessfulStorage("StorageRegion"); //part of EventAction
      }
  }

} //MuonStoragestatusAction::userSteppAction


void gm2ringsim::MuonStorageStatusAction::incrementMuonStorageCounter()
{++muonStorageCounter_;} 


void gm2ringsim::MuonStorageStatusAction::callArtProduces(art::EDProducer *producer){
  //FIXME: Need to figure out the right name with multiple base classes
  producer->produces<EventRecordCollection>("MuonStorageStatusAction");
}

void gm2ringsim::MuonStorageStatusAction::fillEventWithArtStuff(art::Event &e) {
  std::unique_ptr< EventRecordCollection > myArtHits(new EventRecordCollection);
  //FIXME: ????
  // get the geant hit
  // conver the geant to ART record
  // Move this to endOfEventAction(G4Event*)
  myArtHits->emplace_back( true,10,1);
  e.put(std::move(myArtHits),"MuonStorageStatusAction");
}


/** Setter for successful storage. */
void gm2ringsim::MuonStorageStatusAction::successfulStorage() {
  muonStorageStatus_ = muonTrackingStatus::storedMuon;
  incrementMuonStorageCounter();
  setEventStatus(kStored);
} 

/** Setter for failed storage */
void gm2ringsim::MuonStorageStatusAction::unsuccessfulStorage(){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
}

void gm2ringsim::MuonStorageStatusAction::unsuccessfulStorage(G4String reason){
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
  if ( reason == "Lab" ) { setEventStatus(kHitLab); }
  if ( reason == "Inflector" ) { setEventStatus(kHitInflector); }
  if ( reason == "StorageRegion" ) { setEventStatus(kBeyondStorageRegion);
  }
}

void gm2ringsim::MuonStorageStatusAction::setStoredMuonTurns(G4int numberOfTurns ) {
  turnsForStorage_ = numberOfTurns;
  
}

using gm2ringsim::MuonStorageStatusAction;
DEFINE_ART_SERVICE(MuonStorageStatusAction)
