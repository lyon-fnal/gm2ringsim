/** @file InflectorSD.cc

    Provides the member implementations of the InflectorSD and
    InflectorSDMessenger classes.

    @author Kevin Lynch
    @date 2009
*/

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4LorentzVector.hh"

#include "gm2ringsim/inflector/InflectorHit.hh"
#include "gm2ringsim/inflector/InflectorSD.hh"

gm2ringsim::InflectorSD::InflectorSD(G4String name) : 
  G4VSensitiveDetector( name ),
  printLevel(0), drawLevel(0),
  tsdm_(new InflectorSDMessenger(this)) {
  collectionName.insert( name );

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
gm2ringsim::InflectorSD::~InflectorSD(){
}

void gm2ringsim::InflectorSD::Initialize(G4HCofThisEvent* HCoTE){

  thisHC = new InflectorHitsCollection
    ( SensitiveDetectorName, collectionName[0]);

  G4int const thisHCID = 
      G4SDManager::GetSDMpointer()->GetCollectionID(thisHC);

  //  G4cout << "In gm2ringsim::InflectorSD::Initialize():\n";
  //  G4cout << (int)thisHC << ' ' << thisHCID << '\n';

  // Add to HCoTE
  HCoTE->AddHitsCollection( thisHCID, thisHC );
}

G4bool gm2ringsim::InflectorSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){ 

  if( thisStep->GetPreStepPoint()->GetPhysicalVolume() ==
      thisStep->GetPostStepPoint()->GetPhysicalVolume() )
    return false;
  
  thisHC->insert(new InflectorHit(thisStep));

  return true; 
}



void gm2ringsim::InflectorSD::EndOfEvent(G4HCofThisEvent*) {
  G4int n = thisHC->entries();
  
  // Output junk ...
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



G4int gm2ringsim::InflectorSD::PrintLevel(G4int newLevel){
  G4int temp = printLevel;
  printLevel = newLevel;
  return temp;
}

G4int gm2ringsim::InflectorSD::DrawLevel(G4int newLevel){
  G4int temp = drawLevel;
  drawLevel = newLevel;
  return temp;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////

gm2ringsim::InflectorSDMessenger::InflectorSDMessenger(InflectorSD *tsd) :
  tsd_(tsd) {

  topdir_ = new G4UIdirectory("/g2MIGTRACE/hits/");
  topdir_->SetGuidance("g2MIGTRACE specific control of Sensitive Detectors");

  dir_ = new G4UIdirectory("/g2MIGTRACE/hits/InflectorSD/");
  dir_->SetGuidance("g2MIGTRACE specific control of tracking detectors");

  printLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/InflectorSD/printLevel", this);
  printLevelCmd_->SetGuidance("Modify the print verbosity; 0 gives no output, greater than 0 gives printed output");
  printLevelCmd_->SetParameterName("Verbosity", true);
  printLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

  drawLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/InflectorSD/drawLevel", this);
  drawLevelCmd_->SetGuidance("Modify the drawing verbosity; 0 gives no output, greater than 0 gives hit output");
  drawLevelCmd_->SetParameterName("Verbosity", true);
  drawLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

}


gm2ringsim::InflectorSDMessenger::~InflectorSDMessenger(){
  delete drawLevelCmd_;
  delete printLevelCmd_;
  delete dir_;
}

void gm2ringsim::InflectorSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newval){ 
  
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

