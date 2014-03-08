// Implementation of LostMuonAction Service

#include "gm2ringsim/actions/LostMuonAction_service.hh"

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
gm2ringsim::LostMuonAction::LostMuonAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::EventActionBase(p.get<std::string>("name")),
  artg4::RunActionBase(p.get<std::string>("name")),
  artg4::SteppingActionBase(p.get<std::string>("name")),
  stored_rmin_(p.get<double>("stored_rmin", -50.0) * mm),
  stored_rmax_(p.get<double>("stored_rmax",50.0) * mm),
  stored_y_(p.get<double>("stored_y",50.0) * mm),
  dist_from_ring_(p.get<double>("dist_from_ring",-1.0) * m),
  Nexposed_(0),
  Nkilled_(0),
  logInfo_("LostMuonAction")
{
  
  G4cout << "================ LostMuonAction ================" << G4endl;
  G4cout << "| Rmin,Rmax:          " << stored_rmin_ << " , " << stored_rmax_ << " mm." << G4endl;
  G4cout << "| |Y|max:             " << stored_y_ << " mm." << G4endl;
  G4cout << "=========================================================" << G4endl;

}
  
//beginOfLostMuonAction
void gm2ringsim::LostMuonAction::beginOfEventAction (const G4Event* ) {
  //G4cout << "gm2ringsim::LostMuonAction::beginOfEventAction()" << G4endl;

  Nexposed_++;
}  //beginOfEventAction

void gm2ringsim::LostMuonAction::endOfEventAction(const G4Event* ){//evt) {
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


void gm2ringsim::LostMuonAction::beginOfRunAction(const G4Run *) {
  // Initialization per run
}   


   
void gm2ringsim::LostMuonAction::endOfRunAction(const G4Run *currentRun) {
  
  G4int totalEvents = currentRun -> GetNumberOfEvent();
  G4cout << G4endl;
  G4cout << "============== LostMuonAction::RunEnd ==============" << G4endl;
  G4cout << "   Muons injected     : " << totalEvents << G4endl;
  G4cout << "   Muons seen         : " << Nexposed_ << G4endl;
  G4cout << "   Muons killed       : " << Nkilled_ << G4endl;
  G4cout << "=============================================================" << G4endl;
  
} //RunAction::endOfRunAction(const G4Run*)


/** Operates on each step.    
    - Kills tracks if they wander into theLaboratory or the Inflector
    Mandrel.         
    - Kills a track and ends the event if the muon survives for a
    sufficient number of turns.
    - Kills a track if it wanders too far outside the storage volume.
*/  

void gm2ringsim::LostMuonAction::userSteppingAction(const G4Step *currentStep) {

  //G4cout << "gm2ringsim::LostMuonAction::userSteppingAction()" << G4endl;

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
//   if ( currentTrack->GetParticleDefinition()->GetPDGEncoding() != 13 &&
//        currentTrack->GetParticleDefinition()->GetPDGEncoding() != -13 ) { return; }
  

  G4ThreeVector const currentPos =
    currentStep -> GetPostStepPoint() -> GetPosition();
  G4double const rhat = ComputeRhat(&currentPos);
  G4double const vhat = ComputeVhat(&currentPos);
  
  if( currentTrack -> GetTrackID() == 1 ) {
    if ( dist_from_ring_ > 0 ) {
      double dist = TMath::Sqrt(rhat*rhat + vhat*vhat);
      if ( dist > dist_from_ring_ ) {
	currentTrack -> SetWeight(1);
	currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
	currentTrack -> SetGoodForTrackingFlag(true);
	Nkilled_++;
	return;
      }
    }
    else {
      if( rhat < stored_rmin_ || rhat > stored_rmax_ || std::abs(vhat) > stored_y_) {
	//G4cout << rhat << " < " << stored_rmin_ << " , " << rhat << " > " << stored_rmax_ << " , " << std::abs(vhat) << " > " << stored_y_ << G4endl;
	currentTrack -> SetWeight(1);
	currentTrack -> SetTrackStatus(fKillTrackAndSecondaries);
	currentTrack -> SetGoodForTrackingFlag(true);
	Nkilled_++;
	return;
      }
    }
  }

} //MuonStoragestatusAction::userSteppAction


using gm2ringsim::LostMuonAction;
DEFINE_ART_SERVICE(LostMuonAction)
