/** @file FiberHarpSD.cc

    Implements the sensitive detector for the fiber harps.

    Derived from work of Kevin Lynch, 2011, Fred Gray, 2012

    @author Peter Winter
    @date 2012
*/

#include "FiberHarpSD.hh"

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackStatus.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4ThreeVector.hh"

FiberHarpSD::FiberHarpSD(G4String name) : 
  G4VSensitiveDetector( name ),
  printLevel(0), drawLevel(0){
  // TODO  hsdm_(new FiberHarpSDMessenger(this)) {
  collectionName.insert( name );

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
FiberHarpSD::~FiberHarpSD(){
}

void FiberHarpSD::Initialize(G4HCofThisEvent* HCoTE){

  thisHC = new FiberHarpHitsCollection
    ( SensitiveDetectorName, collectionName[0]);

  G4int const thisHCID = 
    G4SDManager::GetSDMpointer()->GetCollectionID(thisHC);

  // Add to HCoTE
  HCoTE->AddHitsCollection( thisHCID, thisHC );
}

G4bool FiberHarpSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){ 
  thisHC->insert(new FiberHarpHit(thisStep));
  return true;
}

void FiberHarpSD::EndOfEvent(G4HCofThisEvent*) {

  G4int n = thisHC->entries();

  if(! (printLevel > 0 || drawLevel > 0) )
    return;

  if(printLevel > 0 && n>0){
    G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
    for(G4int i=0; i<n; ++i){
      G4cout << '\t';
      (*thisHC)[i]->Print();
    }
  }
  
  if(drawLevel > 0 && n>0)
    for(G4int i=0; i<n; ++i)
      (*thisHC)[i]->Draw();
  
}

G4int FiberHarpSD::PrintLevel(G4int newLevel){
  G4int temp = printLevel;
  printLevel = newLevel;
  return temp;
}

G4int FiberHarpSD::DrawLevel(G4int newLevel){
  G4int temp = drawLevel;
  drawLevel = newLevel;
  return temp;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

/*
FiberHarpSDMessenger::FiberHarpSDMessenger(FiberHarpSD *tsd) :
  rsd_(tsd) {

  topdir_ = new G4UIdirectory("/g2MIGTRACE/hits/");
  topdir_->SetGuidance("g2MIGTRACE specific control of Sensitive Detectors");

  dir_ = new G4UIdirectory("/g2MIGTRACE/hits/FiberHarpSD/");
  dir_->SetGuidance("g2MIGTRACE specific control of tracking detectors");

  printLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/FiberHarpSD/printLevel", this);
  printLevelCmd_->SetGuidance("Modify the print verbosity; 0 gives no output, greater than 0 gives printed output");
  printLevelCmd_->SetParameterName("Verbosity", true);
  printLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

  drawLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/FiberHarpSD/drawLevel", this);
  drawLevelCmd_->SetGuidance("Modify the drawing verbosity; 0 gives no output, greater than 0 gives hit output");
  drawLevelCmd_->SetParameterName("Verbosity", true);
  drawLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

}

FiberHarpSDMessenger::~FiberHarpSDMessenger(){
  delete drawLevelCmd_;
  delete printLevelCmd_;
  delete dir_;
}

void FiberHarpSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newval){ 
  
  if( cmd == printLevelCmd_ ){
    if( 0 == newval.size() )
      G4cout << "Current print verbosity level: " 
	     << rsd_->PrintLevel() << '\n';
    else
      rsd_->PrintLevel(
		       printLevelCmd_->GetNewIntValue(newval)
		       );

    return;
  }

  if( cmd == drawLevelCmd_ ){
    if( 0 == newval.size() )
      G4cout << "Current drawing verbosity level: " 
	     << rsd_->DrawLevel() << '\n';
    else
      rsd_->DrawLevel(
		      drawLevelCmd_->GetNewIntValue(newval)
		      );

    return;
  }

}

*/
