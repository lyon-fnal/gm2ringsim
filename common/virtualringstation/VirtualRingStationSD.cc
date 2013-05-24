/** @file VirtualRingStationSD.cc

    Implements the sensitive detector and messengers for the in-ring
    beam VirtualRingStations. 

    - Kevin Lynch 2009
*/


#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4LorentzVector.hh"

#include "gm2ringsim/common/virtualringstation/VirtualRingStationHit.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationSD.hh"
#include "gm2ringsim/common/spin/SpinHit.hh"
//#include "eventAction.hh"

gm2ringsim::VirtualRingStationSD::VirtualRingStationSD(G4String name) : 
  G4VSensitiveDetector( name ),
  printLevel_(0), drawLevel_(0)
  //  tsdm_(new VirtualRingStationSDMessenger(this)) 

{
  //  G4cout << "In VirtualRingStationSD constructor: " << name << '\n';
  
  collectionName.push_back( name );
  collectionName.push_back( name+"_spin" );

  // Register with SDManager
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->AddNewDetector(this);

}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
gm2ringsim::VirtualRingStationSD::~VirtualRingStationSD(){
}

void gm2ringsim::VirtualRingStationSD::Initialize(G4HCofThisEvent* HCoTE){
  virtualringstationHC_ = new VirtualRingStationHitsCollection
    ( SensitiveDetectorName, collectionName[0]);
  spinHC_ = new gm2ringsim::SpinHitsCollection
    ( SensitiveDetectorName, collectionName[1]);

  G4int const virtualringstationHCID = 
    G4SDManager::GetSDMpointer()->GetCollectionID(virtualringstationHC_);
  
    G4int const spinHCID =
      G4SDManager::GetSDMpointer()->GetCollectionID(spinHC_);
  
    //G4cout << "In VirtualRingStationSD::Initialize():\n";
    //G4cout << virtualringstationHCID << '\n';
    //G4cout << spinHCID << '\n';
    
  // Add to HCoTE
  HCoTE->AddHitsCollection( virtualringstationHCID, virtualringstationHC_ );
  HCoTE->AddHitsCollection( spinHCID, spinHC_ );
}

G4bool gm2ringsim::VirtualRingStationSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){ 

  if( thisStep->GetPreStepPoint()->GetPhysicalVolume() ==
      thisStep->GetPostStepPoint()->GetPhysicalVolume() )
    return false;
  
  virtualringstationHC_->insert(new VirtualRingStationHit(thisStep));
  spinHC_->insert(new gm2ringsim::SpinHit(thisStep));

  return true; 
}



void gm2ringsim::VirtualRingStationSD::EndOfEvent(G4HCofThisEvent*) {
  G4int n = virtualringstationHC_->entries();
  
  // Output junk ...
  if(! (printLevel_ > 0 || drawLevel_ > 0) )
    return;

  if(printLevel_ > 0 && n>0){
    G4cout << "Sensitive Detector: " << SensitiveDetectorName << '\n';
    for(G4int i=0; i<n; ++i){
      G4cout << '\t';
      (*virtualringstationHC_)[i]->Print();
      G4cout << '\t';
      (*spinHC_)[i]->Print();
    }
  }
  
  if(drawLevel_ > 0 && n>0)
    for(G4int i=0; i<n; ++i){
      (*virtualringstationHC_)[i]->Draw();
    }
}



G4int gm2ringsim::VirtualRingStationSD::PrintLevel(G4int newLevel){
  G4int temp = printLevel_;
  printLevel_ = newLevel;
  return temp;
}

G4int gm2ringsim::VirtualRingStationSD::DrawLevel(G4int newLevel){
  G4int temp = drawLevel_;
  drawLevel_ = newLevel;
  return temp;
}

//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
/*
VirtualRingStationSDMessenger::VirtualRingStationSDMessenger(VirtualRingStationSD *tsd) :
  tsd_(tsd) {

  topdir_ = new G4UIdirectory("/g2MIGTRACE/hits/");
  topdir_->SetGuidance("g2MIGTRACE specific control of Sensitive Detectors");

  dir_ = new G4UIdirectory("/g2MIGTRACE/hits/VirtualRingStationSD/");
  dir_->SetGuidance("g2MIGTRACE specific control of tracking detectors");

  printLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/VirtualRingStationSD/printLevel", this);
  printLevelCmd_->SetGuidance("Modify the print verbosity; 0 gives no output, greater than 0 gives printed output");
  printLevelCmd_->SetParameterName("Verbosity", true);
  printLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

  drawLevelCmd_ = 
    new G4UIcmdWithAnInteger("/g2MIGTRACE/hits/VirtualRingStationSD/drawLevel", this);
  drawLevelCmd_->SetGuidance("Modify the drawing verbosity; 0 gives no output, greater than 0 gives hit output");
  drawLevelCmd_->SetParameterName("Verbosity", true);
  drawLevelCmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

}


VirtualRingStationSDMessenger::~VirtualRingStationSDMessenger(){
  delete drawLevelCmd_;
  delete printLevelCmd_;
  delete dir_;
}

void VirtualRingStationSDMessenger::SetNewValue(G4UIcommand* cmd, G4String newval){ 
  
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
*/
