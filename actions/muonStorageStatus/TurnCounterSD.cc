/** @file turnCounterSD.cc
    
    Provides the implementation for this sensitive detector. 

    - Kevin Lynch 2009
*/

#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Step.hh"

#include "gm2ringsim/actions/muonStorageStatus/TurnCounterSD.hh"
#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

turnCounterSD::turnCounterSD(G4String name) :
  G4VSensitiveDetector( name ){

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
turnCounterSD::~turnCounterSD(){
}

void turnCounterSD::Initialize(G4HCofThisEvent* /*HCoTE*/){
}

G4bool turnCounterSD::ProcessHits(G4Step* thisStep, 
				  G4TouchableHistory*){  
  //  if(thisStep->IsFirstStepInVolume()) // why doesn't this work?
  if( thisStep->GetPreStepPoint()->GetPhysicalVolume() !=
      thisStep->GetPostStepPoint()->GetPhysicalVolume()  &&
      thisStep->GetTrack()->GetTrackID() == 1 // make sure we only
					      // track the primary!
      )
    turnCounter::getInstance().increment();

  //  G4cout << "Turns: " << turnCounter::getInstance().turns() << '\n';

  return true; 
}

void turnCounterSD::EndOfEvent(G4HCofThisEvent*) {
}
