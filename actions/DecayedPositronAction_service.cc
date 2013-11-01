// Implementation of DecayedPositronAction Service

#include "gm2ringsim/actions/DecayedPositronAction_service.hh"

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

#include <string>
#include <sstream>
#include <iomanip>

#include <boost/algorithm/string.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

// Constructor
gm2ringsim::DecayedPositronAction::DecayedPositronAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  artg4::SteppingActionBase(p.get<std::string>("name")),
  stored_threshold_(p.get<double>("stored_threshold", -50.0) * mm),
  Nexposed_(0),
  Nkilled_(0),
  logInfo_("DecayedPositronAction")
{
  G4cout << "================ DecayedPositronAction ================" << G4endl;
  G4cout << "| Stored Threshold:   " << stored_threshold_ << G4endl;
  G4cout << "=========================================================" << G4endl;

}
  
//beginOfDecayedPositronAction
void gm2ringsim::DecayedPositronAction::beginOfEventAction (const G4Event* ) {
  //G4cout << "gm2ringsim::DecayedPositronAction::beginOfEventAction()" << G4endl;

  Nexposed_++;
}  //beginOfEventAction

void gm2ringsim::DecayedPositronAction::endOfEventAction(const G4Event* ){//evt) {
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


void gm2ringsim::DecayedPositronAction::beginOfRunAction(const G4Run *) {
  // Initialization per run
}   


   
void gm2ringsim::DecayedPositronAction::endOfRunAction(const G4Run *currentRun) {
  
  G4int totalEvents = currentRun -> GetNumberOfEvent();
  G4cout << G4endl;
  G4cout << "============== DecayedPositronAction::RunEnd ==============" << G4endl;
  G4cout << "   Total Events           : " << totalEvents << G4endl; 
  G4cout << "   Positrons seen         : " << Nexposed_ << G4endl;
  G4cout << "   Positrons killed       : " << Nkilled_ << G4endl;
  G4cout << "=============================================================" << G4endl;
  
} //RunAction::endOfRunAction(const G4Run*)


/** Operates on each step.    
    - Kills tracks if they wander into theLaboratory or the Inflector
    Mandrel.         
    - Kills a track and ends the event if the muon survives for a
    sufficient number of turns.
    - Kills a track if it wanders too far outside the storage volume.
*/  

void gm2ringsim::DecayedPositronAction::userSteppingAction(const G4Step *currentStep) {

  //G4cout << "gm2ringsim::DecayedPositronAction::userSteppingAction()" << G4endl;

 // currentStep, currentTrack, and currentEvent are the three
  // sources of relevant information, and therefore, access is
  // needed to their respective pointers.  While the G4Step pointer
  // is passed into this function automatically, the remaining two
  // pointers must be obtained manually
  
  G4Track *currentTrack = currentStep -> GetTrack();


  //-------------------------
  //
  // Only run this for positrons
  //
  //-------------------------
  if ( currentTrack->GetParticleDefinition()->GetPDGEncoding() != -11 ) { return; }
  int parentid = currentTrack->GetParentID();
  if ( parentid != 1 ) { return; }

  G4ThreeVector const currentPos =
    currentStep -> GetPostStepPoint() -> GetPosition();
  G4double const rhat = ComputeRhat(&currentPos);
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if( rhat < stored_threshold_ ) {
      currentTrack -> SetWeight(0.5);
      currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
      currentTrack -> SetGoodForTrackingFlag(true);
      currentTrack -> SetWeight(0.5);
      Nkilled_++;
      return;
    }
  }

} //MuonStoragestatusAction::userSteppAction


using gm2ringsim::DecayedPositronAction;
DEFINE_ART_SERVICE(DecayedPositronAction)
