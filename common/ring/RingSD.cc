/** @file ringSD.cc

    Provides implementations for the ring energy loss sensistive
    detector and associated messenger.

    - Kevin Lynch 2009
*/

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4LorentzVector.hh"

#include "gm2ringsim/common/ring/RingHit.hh"
#include "gm2ringsim/common/ring/RingSD.hh"

RingSD::RingSD(G4String name) : 
  G4VSensitiveDetector( name ),
  printLevel_(0), drawLevel_(0)
  //  rsdm_(new RingSDMessenger(this)) 

{
  collectionName.insert( name );

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
RingSD::~RingSD(){
}

void RingSD::Initialize(G4HCofThisEvent* HCoTE){

  thisHC_ = new RingHitsCollection
    ( SensitiveDetectorName, collectionName[0]);

  G4int const thisHCID = 
    G4SDManager::GetSDMpointer()->GetCollectionID(thisHC_);

  //  G4cout << "In trackerSD::Initialize():\n";
  //  G4cout << (int)thisHC_ << ' ' << thisHCID << '\n';

  // Add to HCoTE
  HCoTE->AddHitsCollection( thisHCID, thisHC_ );
}

G4bool RingSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){ 
  thisHC_->insert(new RingHit(thisStep));
  return true;
}



void RingSD::EndOfEvent(G4HCofThisEvent*) {

  G4int n = thisHC_->entries();

  if(! (printLevel_ > 0 || drawLevel_ > 0) )
    return;

  if(printLevel_ > 0 && n>0){
    G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
    for(G4int i=0; i<n; ++i){
      G4cout << '\t';
      (*thisHC_)[i]->Print();
    }
  }
  
  if(drawLevel_ > 0 && n>0)
    for(G4int i=0; i<n; ++i)
      (*thisHC_)[i]->Draw();
  
}



G4int RingSD::printLevel(G4int newLevel){
  G4int temp = printLevel_;
  printLevel_ = newLevel;
  return temp;
}

G4int RingSD::drawLevel(G4int newLevel){
  G4int temp = drawLevel_;
  drawLevel_ = newLevel;
  return temp;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

// RingSDMessenger::RingSDMessenger(RingSD *tsd) :
//   rsd_(tsd) {

//   topdir_ = new G4UIdirectory("/g2MIGTRACE/hits/");
//   topdir_->SetGuidance("g2MIGTRACE specific control of Sensitive Detectors");

//   dir_ = new G4UIdirectory("/g2MIGTRACE/hits/RingSD/");
//   dir_->SetGuidance("g2MIGTRACE specific control of tracking detectors");

//   printLevelCmd_ = 
//     new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/RingSD/printLevel", this);
//   printLevelCmd_->SetGuidance("Modify the print verbosity; 0 gives no output, greater than 0 gives printed output");
//   printLevelCmd_->SetParameterName("Verbosity", true);
//   printLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

//   drawLevelCmd_ = 
//     new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/RingSD/drawLevel", this);
//   drawLevelCmd_->SetGuidance("Modify the drawing verbosity; 0 gives no output, greater than 0 gives hit output");
//   drawLevelCmd_->SetParameterName("Verbosity", true);
//   drawLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

// }


// RingSDMessenger::~RingSDMessenger(){
//   delete drawLevelCmd_;
//   delete printLevelCmd_;
//   delete dir_;
// }

// void RingSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newval){ 
  
//   if( cmd == printLevelCmd_ ){
//     if( 0 == newval.size() )
//       G4cout << "Current print verbosity level: " 
// 	     << rsd_->PrintLevel() << '\n';
//     else
//       rsd_->PrintLevel(
// 		       printLevelCmd_->GetNewIntValue(newval)
// 		       );

//     return;
//   }

//   if( cmd == drawLevelCmd_ ){
//     if( 0 == newval.size() )
//       G4cout << "Current drawing verbosity level: " 
// 	     << rsd_->DrawLevel() << '\n';
//     else
//       rsd_->DrawLevel(
// 		      drawLevelCmd_->GetNewIntValue(newval)
// 		      );

//     return;
//   }

// }

