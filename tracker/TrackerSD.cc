/** @file trackerSD.cc

    Implements the sensitive detector and messengers for the in-ring
    beam trackers. 

    - Kevin Lynch 2009
*/


#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4LorentzVector.hh"

#include "trackerHit.hh"
#include "trackerSD.hh"
#include "eventAction.hh"

trackerSD::trackerSD(G4String name) : 
  G4VSensitiveDetector( name ),
  printLevel(0), drawLevel(0),
  tsdm_(new trackerSDMessenger(this)) {
  //  G4cout << "In trackerSD constructor: " << name << '\n';

  collectionName.push_back( name );
  collectionName.push_back( name+"_spin" );

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
trackerSD::~trackerSD(){
}

void trackerSD::Initialize(G4HCofThisEvent* HCoTE){
  trackerHC = new trackerHitsCollection
    ( SensitiveDetectorName, collectionName[0]);
  spinHC = new spinHitsCollection
    ( SensitiveDetectorName, collectionName[1]);

  G4int const trackerHCID = 
    G4SDManager::GetSDMpointer()->GetCollectionID(trackerHC);
  
  G4int const spinHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(spinHC);
  
  //    G4cout << "In trackerSD::Initialize():\n";
  //    G4cout << trackerHCID << '\n';
  //    G4cout << spinHCID << '\n';
  
  // Add to HCoTE
  HCoTE->AddHitsCollection( trackerHCID, trackerHC );
  HCoTE->AddHitsCollection( spinHCID, spinHC );
}

G4bool trackerSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){ 

  if( thisStep->GetPreStepPoint()->GetPhysicalVolume() ==
      thisStep->GetPostStepPoint()->GetPhysicalVolume() )
    return false;
  
  trackerHC->insert(new trackerHit(thisStep));
  spinHC->insert(new spinHit(thisStep));

  return true; 
}



void trackerSD::EndOfEvent(G4HCofThisEvent*) {
  G4int n = trackerHC->entries();
  
  // Output junk ...
  if(! (printLevel > 0 || drawLevel > 0) )
    return;

  if(printLevel > 0 && n>0){
    G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
    for(G4int i=0; i<n; ++i){
      G4cout << '\t';
      (*trackerHC)[i]->Print();
      G4cout << '\t';
      (*spinHC)[i]->Print();
    }
  }
  
  if(drawLevel > 0 && n>0)
    for(G4int i=0; i<n; ++i){
      (*trackerHC)[i]->Draw();
    }
}



G4int trackerSD::PrintLevel(G4int newLevel){
  G4int temp = printLevel;
  printLevel = newLevel;
  return temp;
}

G4int trackerSD::DrawLevel(G4int newLevel){
  G4int temp = drawLevel;
  drawLevel = newLevel;
  return temp;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

trackerSDMessenger::trackerSDMessenger(trackerSD *tsd) :
  tsd_(tsd) {

  topdir_ = new G4UIdirectory("/g2MIGTRACE/hits/");
  topdir_->SetGuidance("g2MIGTRACE specific control of Sensitive Detectors");

  dir_ = new G4UIdirectory("/g2MIGTRACE/hits/trackerSD/");
  dir_->SetGuidance("g2MIGTRACE specific control of tracking detectors");

  printLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/trackerSD/printLevel", this);
  printLevelCmd_->SetGuidance("Modify the print verbosity; 0 gives no output, greater than 0 gives printed output");
  printLevelCmd_->SetParameterName("Verbosity", true);
  printLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

  drawLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/trackerSD/drawLevel", this);
  drawLevelCmd_->SetGuidance("Modify the drawing verbosity; 0 gives no output, greater than 0 gives hit output");
  drawLevelCmd_->SetParameterName("Verbosity", true);
  drawLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

}


trackerSDMessenger::~trackerSDMessenger(){
  delete drawLevelCmd_;
  delete printLevelCmd_;
  delete dir_;
}

void trackerSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newval){ 
  
  if( cmd == printLevelCmd_ ){
    if( 0 == newval.size() )
      G4cout << "Current print verbosity level: " 
	     << tsd_->PrintLevel() << '\n';
    else
      tsd_->PrintLevel(
		       printLevelCmd_->GetNewIntValue(newval)
		       );

    return;
  }

  if( cmd == drawLevelCmd_ ){
    if( 0 == newval.size() )
      G4cout << "Current drawing verbosity level: " 
	     << tsd_->DrawLevel() << '\n';
    else
      tsd_->DrawLevel(
		      drawLevelCmd_->GetNewIntValue(newval)
		      );

    return;
  }

}

