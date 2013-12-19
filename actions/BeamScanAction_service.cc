// Implementation of MuonStorageStatusAction Service

#include "gm2ringsim/actions/BeamScanAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "gm2ringsim/actions/muonStorageStatus/EventRecord.hh"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

#include "Geant4/globals.hh"
#include "Geant4/Randomize.hh"

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
#include <string>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

// Constructor
gm2ringsim::BeamScanAction::BeamScanAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  artg4::SteppingActionBase(p.get<std::string>("name")),
  dTime_(10000*ns),
  pTime_(0),
  freq_(p.get<double>("frequency")),
  logInfo_("BeamScanAction"),
  myArtTrackHits_(new TrackingActionArtRecordCollection)
{  
  G4cout << "================ BeamScanAction ================" << G4endl;
  G4cout << "| DeltaT:             " << pTime_ << G4endl;
  G4cout << "| Frequency:          " << freq_ << G4endl;
  G4cout << "=========================================================" << G4endl;

}
  
//beginOfBeamScanAction
void gm2ringsim::BeamScanAction::beginOfEventAction (const G4Event*){

  pTime_ = 0.0;

  bool debug = false;
  if ( debug ) { 
    G4cout << "gm2ringsim::BeamScanAction::beginOfEventAction()" << G4endl;
  }
  
  //G4cout << "gm2ringsim::BeamScanAction::beginOfEventAction() - Done" << G4endl;
}  //beginOfEventAction

void gm2ringsim::BeamScanAction::endOfEventAction(const G4Event* ){//evt) {
  bool debug = false;
  if ( debug ) {
    G4cout << "gm2ringsim::BeamScanAction::endOfEventAction()" << G4endl;
  }
}//endOfEventAction


void gm2ringsim::BeamScanAction::beginOfRunAction(const G4Run *) {
  bool debug = false;
  //G4double totalEvents = currentRun -> GetNumberOfEvent();
  if ( debug ) {
    G4cout << "gm2ringsim::BeamScanAction::beginOfRunAction()" << G4endl;
  }

}


   
void gm2ringsim::BeamScanAction::endOfRunAction(const G4Run *) {
  bool debug = false;
  
  //G4double totalEvents = currentRun -> GetNumberOfEvent();
  if ( debug ) {
    G4cout << "gm2ringsim::BeamScanAction::beginOfRunAction()" << G4endl;
  }
} //RunAction::endOfRunAction(const G4Run*)


/** Operates on each step.    
    - Kills tracks if they wander into theLaboratory or the Inflector
    Mandrel.         
    - Kills a track and ends the event if the muon survives for a
    sufficient number of turns.
    - Kills a track if it wanders too far outside the storage volume.
*/  

void gm2ringsim::BeamScanAction::userSteppingAction(const G4Step *currentStep) {


  G4double gTime = currentStep->GetPreStepPoint()->GetGlobalTime();

  bool record = false;
  if ( gTime - pTime_ > dTime_ ) {
    pTime_ = gTime;
    record = true;
  }
  record = false;
  G4double rand = G4UniformRand();
  // 0.99 ~= 200-400 ns
  if ( rand < 1/(double)freq_ ) {
    record = true;
    //G4cout << "Saving @ T = " << gTime << G4endl;
  }
  
  if ( record == false ) { return; }


  //G4cout << "gm2ringsim::BeamScanAction::userSteppingAction()" << G4endl;

 // currentStep, currentTrack, and currentEvent are the three
  // sources of relevant information, and therefore, access is
  // needed to their respective pointers.  While the G4Step pointer
  // is passed into this function automatically, the remaining two
  // pointers must be obtained manually
  
  G4Track *currentTrack = currentStep -> GetTrack();

  //-------------------------
  //
  // Only run this for muons
  //
  //-------------------------
  if ( currentTrack->GetParticleDefinition()->GetPDGEncoding() != 13 &&
       currentTrack->GetParticleDefinition()->GetPDGEncoding() != -13 ) { return; }
  
  
  FillTrackingActionArtRecord(currentTrack, gm2ringsim::kScan);

} //MuonStoragestatusAction::userSteppAction


void gm2ringsim::BeamScanAction::callArtProduces(art::EDProducer *producer){
  //FIXME: Need to figure out the right name with multiple base classes
  producer->produces<EventRecordCollection>("BeamScanAction");
  producer->produces<TrackingActionArtRecordCollection>("BeamScanTracks");
}

void gm2ringsim::BeamScanAction::fillEventWithArtStuff(art::Event &e) {
  std::unique_ptr< EventRecordCollection > myArtHits(new EventRecordCollection);
  //FIXME: ????
  // get the geant hit
  // conver the geant to ART record
  // Move this to endOfEventAction(G4Event*)
  myArtHits->emplace_back( true,10,1);
  e.put(std::move(myArtHits),"BeamScanAction");



  // Make a unique pointer for the tracking object
  e.put(std::move(myArtTrackHits_), "BeamScanTracks");

  // myArtHits should now be invalid and set to nullptr. But in fact
  // due to https://cdcvs.fnal.gov/redmine/issues/3601 this does not happen.
  // So need to do a release to avoid a segfault
  myArtTrackHits_.release();
  
  // Point to a new valid collection
  myArtTrackHits_.reset( new TrackingActionArtRecordCollection() );
}



// Fill ArtRecord
void gm2ringsim::BeamScanAction::FillTrackingActionArtRecord(G4Track * currentTrack, int status)
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

using gm2ringsim::BeamScanAction;
DEFINE_ART_SERVICE(BeamScanAction)
