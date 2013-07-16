// Implementation of MuonStorageStatusAction Service

#include "gm2ringsim/actions/muonStorageStatus/MuonStorageStatusAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "gm2ringsim/actions/muonStorageStatus/EventRecord.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

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
  TrackPositron_(p.get<bool>("TrackPositrons",false)),
  logInfo_("MuonStorageStatusAction"),
  myArtTrackHits_(new TrackingActionArtRecordCollection)
{
  
  G4cout << "================ MuonStorageStatusAction ================" << G4endl;
  G4cout << "| Turns For Storage:  " << turnsForStorage_ << G4endl;
  G4cout << "| Rmin,Rmax:          " << stored_rmin_ << " , " << stored_rmax_ << " mm." << G4endl;
  G4cout << "| |Y|max:             " << stored_y_ << " mm." << G4endl;
  G4cout << "| Track Positron:     " << TrackPositron_ << G4endl;
  G4cout << "=========================================================" << G4endl;

}
  
//beginOfMuonStorageStatusAction
void gm2ringsim::MuonStorageStatusAction::beginOfEventAction (const G4Event* pevent){
  //G4cout << "gm2ringsim::MuonStorageStatusAction::beginOfEventAction()" << G4endl;

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

  //G4cout << "gm2ringsim::MuonStorageStatusAction::beginOfEventAction() - Done" << G4endl;
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
  for ( int i = 0; i < 4; i++ ) { muonKillCounter_[i] = 0; }

  //FIXME:TEMP to use currentRun
  mf::LogDebug("MuonStorageAction_service")<<"currentRunID is "<<currentRun->GetRunID()<<std::endl;
  
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
  
  G4cout << G4endl;
  G4cout << "============== MuonStorageStatusAction::RunEnd ==============" << G4endl;
  G4cout << "   Muons injected     : " << totalEvents << G4endl;
  G4cout << "   Muons killed       : " << muonKillCounter_[3] << G4endl;
  G4cout << "      --> Beyond SR   : " << muonKillCounter_[2] << G4endl;
  G4cout << "      --> In The Lab  : " << muonKillCounter_[0] << G4endl;
  G4cout << "      --> Inflector   : " << muonKillCounter_[1] << G4endl;
  G4cout << "   Muons stored       : " << muonStorageCounter_ << G4endl;
  G4cout.precision(3);
  G4cout << "   Capture Efficiency : (" 
	 << captureEfficiency << " +/- "
	 << dcE << ")%" << G4endl;
  G4cout << "=============================================================" << G4endl;
  
} //RunAction::endOfRunAction(const G4Run*)


/** Operates on each step.    
    - Kills tracks if they wander into theLaboratory or the Inflector
    Mandrel.         
    - Kills a track and ends the event if the muon survives for a
    sufficient number of turns.
    - Kills a track if it wanders too far outside the storage volume.
*/  

void gm2ringsim::MuonStorageStatusAction::userSteppingAction(const G4Step *currentStep) {

  //G4cout << "gm2ringsim::MuonStorageStatusAction::userSteppingAction()" << G4endl;

 // currentStep, currentTrack, and currentEvent are the three
  // sources of relevant information, and therefore, access is
  // needed to their respective pointers.  While the G4Step pointer
  // is passed into this function automatically, the remaining two
  // pointers must be obtained manually
  
  G4Track *currentTrack = currentStep -> GetTrack();


  //--------------------------
  //
  // An aside for positrons leaving the storage region
  //
  //--------------------------
  if ( TrackPositron_ ) {
    int parentid = currentTrack->GetParentID();
    if ( parentid == 1 ) {
      G4ParticleDefinition *def = currentTrack->GetDefinition();
      int id = def->GetPDGEncoding();
      //G4cout << "Muon decay particle -> " << def->GetParticleName() << G4endl;
      if ( id == -11 ) { // positron from mu+ decay
	if ( ComputeRhat(currentTrack) > 45*mm || ComputeRhat(currentTrack) < -45*mm ) {
	  //G4cout << "  Outside of storage region: Tag it!" << G4endl;
	  //FillTrackingActionArtRecord(currentTrack, gm2ringsim::kDecay);
	  currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
	  return;
	}
      }
    }
  }

  //-------------------------
  //
  // Only run this for muons
  //
  //-------------------------
  if ( currentTrack->GetParticleDefinition()->GetPDGEncoding() != 13 &&
       currentTrack->GetParticleDefinition()->GetPDGEncoding() != -13 ) { return; }
  

  // MUON STORAGE  
  G4String const& currentVolumeName =
    currentTrack -> GetVolume() -> GetName();

  //  if( currentTrack->GetTrackID() == 1 )                                                                          
  //G4cout << currentVolumeName << '\n';                                                                         
  
  if( currentVolumeName == "InflectorMandrel" ){
    if( currentTrack -> GetTrackID() == 1 ) {
      unsuccessfulStorage("Inflector"); //Part of EventAction
      //FillTrackingActionArtRecord(currentTrack, gm2ringsim::kLost);
      currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
      //G4cout << "Muon is in the inflector." << G4endl;
      return;
    }
  }
  else if( currentVolumeName == "theLaboratory" ) {
    if( currentTrack -> GetTrackID() == 1 ) {
      unsuccessfulStorage("Lab"); //Part of EventAction      
      //FillTrackingActionArtRecord(currentTrack, gm2ringsim::kLost);
      currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
      //G4cout << "Muon is in the lab." << G4endl;
      return;
    }
  }

  G4int turn = TurnCounter::getInstance().turns();
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if( turn >= turnsForStorage_ ) {
      successfulStorage(); //Part of EventAction
      //FillTrackingActionArtRecord(currentTrack, gm2ringsim::kStore);
      currentTrack -> SetTrackStatus(fStopButAlive);
      //G4cout << "Stored Muon!" << G4endl;
      return;
    }
  }
  
  G4ThreeVector const currentPos =
    currentStep -> GetPostStepPoint() -> GetPosition();
  G4double const posX = currentPos.getX();
  G4double const posY = currentPos.getY();
  G4double const posZ = currentPos.getZ();
  G4double const posR = sqrt(posX*posX + posZ*posZ);
  
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if(posR < stored_rmin_ || posR > stored_rmax_ || std::abs(posY) > stored_y_) {
      //FillTrackingActionArtRecord(currentTrack, gm2ringsim::kLost);
      unsuccessfulStorage("StorageRegion"); //part of EventAction
      //G4cout << "Muon is beyond storage region." << G4endl;
      currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
      return;
    }
  }

} //MuonStoragestatusAction::userSteppAction


void gm2ringsim::MuonStorageStatusAction::incrementMuonStorageCounter()
{++muonStorageCounter_;} 


void gm2ringsim::MuonStorageStatusAction::incrementMuonKillCounter(int reason)
{++muonKillCounter_[reason];}


void gm2ringsim::MuonStorageStatusAction::callArtProduces(art::EDProducer *producer){
  //FIXME: Need to figure out the right name with multiple base classes
  producer->produces<EventRecordCollection>("MuonStorageStatusAction");
  producer->produces<TrackingActionArtRecordCollection>("FinishedTracks");
}

void gm2ringsim::MuonStorageStatusAction::fillEventWithArtStuff(art::Event &e) {
  std::unique_ptr< EventRecordCollection > myArtHits(new EventRecordCollection);
  //FIXME: ????
  // get the geant hit
  // conver the geant to ART record
  // Move this to endOfEventAction(G4Event*)
  myArtHits->emplace_back( true,10,1);
  e.put(std::move(myArtHits),"MuonStorageStatusAction");



  // Make a unique pointer for the tracking object
  e.put(std::move(myArtTrackHits_), "FinishedTracks");

  // myArtHits should now be invalid and set to nullptr. But in fact
  // due to https://cdcvs.fnal.gov/redmine/issues/3601 this does not happen.
  // So need to do a release to avoid a segfault
  myArtTrackHits_.release();
  
  // Point to a new valid collection
  myArtTrackHits_.reset( new TrackingActionArtRecordCollection() );
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
  //G4cout << "gm2ringsim::MuonStorageStatusAction::unsuccessfulStorage(" << reason << ")" << G4endl;
  muonStorageStatus_ = muonTrackingStatus::lostMuon;
  incrementMuonKillCounter(3);
  if ( reason == "Lab" ) { setEventStatus(kHitLab); incrementMuonKillCounter(0); }
  if ( reason == "Inflector" ) { setEventStatus(kHitInflector); incrementMuonKillCounter(1); }
  if ( reason == "StorageRegion" ) { setEventStatus(kBeyondStorageRegion); incrementMuonKillCounter(2); }
}

void gm2ringsim::MuonStorageStatusAction::setStoredMuonTurns(G4int numberOfTurns ) {
  turnsForStorage_ = numberOfTurns;  
}


// Fill ArtRecord
void gm2ringsim::MuonStorageStatusAction::FillTrackingActionArtRecord(G4Track * currentTrack, int status)
{
  //if ( currentTrack->GetTrackID() != 1 ) { return; }
  
  bool debug = false;
  bool keep_track = true;


  //-------------------------------------
  // Don't store unwanted truth particles
  //-------------------------------------
  G4cout.precision(3);
  if ( debug && !keep_track ) { G4cout << "Not storing track [" << currentTrack->GetDefinition()->GetParticleName() << "] for some reason." << G4endl; }
  if ( keep_track == false ) { return; }
  

  // Create a hit
  TrackingActionArtRecord tr;
  
  // Fill it based on the track.
  tr.trackType = std::string(currentTrack->GetDefinition()->GetParticleName());
  tr.trackID = currentTrack->GetTrackID();
  tr.parentTrackID = currentTrack->GetParentID();

  //TODO
  // Omit turns for now
  //  tr.turn = turnCounter::getInstance().turns();

  // Get the volume ID
  art::ServiceHandle<artg4::PhysicalVolumeStoreService> pvs;
  tr.volumeUID = pvs->idGivenPhysicalVolume( currentTrack->GetVolume() );

  // The track status, 0 for pre and 1 for post
  tr.status = status;
  
  G4ThreeVector pos = currentTrack->GetPosition();
  G4ThreeVector mom = currentTrack->GetMomentum();
  G4ThreeVector pol = currentTrack->GetPolarization();

  //-------------------
  // Position variables
  //-------------------
  G4double rhat = ComputeRhat(&pos);
  G4double vhat = ComputeVhat(&pos);
  G4double theta = ComputeTheta(&pos);
  tr.rhat = rhat;
  tr.vhat = vhat;
  tr.theta = theta;
  tr.time = currentTrack->GetGlobalTime();


  //-------------------
  // Momentum variables
  //-------------------

  G4double prhat = ComputePrhat(&pos, &mom);
  G4double pvhat = ComputePvhat(&pos, &mom);
  tr.p = mom.mag();
  tr.prhat = prhat;
  tr.pvhat = pvhat;
  tr.e = currentTrack->GetTotalEnergy();


  if ( debug ) { G4cout << "Found track[" << currentTrack->GetParticleDefinition()->GetParticleName() << "] [t=" << currentTrack->GetGlobalTime() << "] [" << currentTrack->GetParentID() << "] : " << status << "\t" << keep_track << "\t" << prhat << "\t" << pvhat << G4endl; }


  //-----------------------
  // Polarization variables
  //-----------------------
  tr.polx = pol.x();
  tr.poly = pol.y();
  tr.polz = pol.z();
  //G4cout << tr.trackType << "\tPol[x,z] = " << pol.x() << "\t" << pol.z() << G4endl;
  
  // Add the hit to our collection
  myArtTrackHits_->push_back(tr);
}

using gm2ringsim::MuonStorageStatusAction;
DEFINE_ART_SERVICE(MuonStorageStatusAction)
