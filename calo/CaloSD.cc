/** @file CaloSD.cc
 
    Implements the "stop and kill" calorimeter sensitive detector.
 
    Ported to Art from g2migtrace file calorimeterSD.cc
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013
 */

#include "CaloSD.hh"
#include "ShowerListManager.hh"

#include "Geant4/G4ios.hh"
#include "Geant4/G4SDManager.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4TrackStatus.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4StepPoint.hh"
#include "Geant4/G4ThreeVector.hh"

gm2ringsim::CaloSD::CaloSD(G4String name) :
G4VSensitiveDetector( name ),
printLevel(0), drawLevel(0), killShowers(true)
{
    collectionName.insert( name );
    
    // Register with SDManager
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    SDman->AddNewDetector(this);    
}

// The destructor will never be called during the run of the program,
// as SDs are never removed from the SDManager during the lifetime of
// the program.
gm2ringsim::CaloSD::~CaloSD(){}

void gm2ringsim::CaloSD::Initialize(G4HCofThisEvent* HCoTE){
    
    thisHC = new CaloHitsCollection
    ( SensitiveDetectorName, collectionName[0]);
    
    G4int const thisHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(thisHC);
    
    // Add to HCoTE
    HCoTE->AddHitsCollection( thisHCID, thisHC );
    
    // keep track of particles that are entering the calorimeter
    ShowerListManager::instance().resetList();
    
}

G4bool gm2ringsim::CaloSD::ProcessHits(G4Step* thisStep, G4TouchableHistory*){
    
    // get basic information regarding the particle involved in this step
    G4Track* track = thisStep->GetTrack() ;
    int parentID = track->GetParentID() ;
    int thisID = track->GetTrackID() ;
    int pdg = track->GetDefinition()->GetPDGEncoding();
    
    bool generateCaloHit = false;
    
    if( pdg != 0 && pdg != 12 && pdg !=-14)  // reject optical photons, nu_e, and anti_nu_mu
      {
        // We'll keep track of which particles enter (and where) by recording
        // the position in CaloHits (which it already does) for only those
        // particles that enter the calorimeter volume from the outside. To work
        // properly, the bookkeeping must be coordinated between the this class and
        // xtalSD, hence the ShowerListManager

	ShowerListManager::particleStatus status = ShowerListManager::instance().addToList( thisID, parentID );
	if( status == ShowerListManager::kInitiatedShower || status == ShowerListManager::kAddedToShower ) {generateCaloHit=true;}

      }
    
    
    
    // insert information only for particles that are entering the volume (and would initiate a shower or interaction)
    if ( generateCaloHit ) {
      thisHC->insert(new CaloHit(thisStep));
    }
    
    // If killShowers = true, kill track when it enters the calo volume (i.e., turn the calo into a black box)
    if( killShowers ){
      thisStep->GetTrack()->SetTrackStatus(fStopAndKill);
    }
    
    return true;
}

void gm2ringsim::CaloSD::EndOfEvent(G4HCofThisEvent*) {
    
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


G4int gm2ringsim::CaloSD::PrintLevel(G4int newLevel){
    G4int temp = printLevel;
    printLevel = newLevel;
    return temp;
}

G4int gm2ringsim::CaloSD::DrawLevel(G4int newLevel){
    G4int temp = drawLevel;
    drawLevel = newLevel;
    return temp;
}

G4bool gm2ringsim::CaloSD::KillShowers(G4bool newLevel){
    G4bool temp = killShowers;
    killShowers = newLevel;
    return temp;
}
