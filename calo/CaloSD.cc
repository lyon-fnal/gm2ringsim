/** @file CaloSD.cc
 
    Implements the calorimeter sensitive detector.
 
    Ported to Art from g2migtrace file calorimeterSD.cc
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013, 2014
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
    
    // Get basic information about the particle involved in this step
    G4Track* track = thisStep->GetTrack();
    int trackID = track->GetTrackID();
    int parentID = track->GetParentID();
    int pdg = track->GetDefinition()->GetPDGEncoding();
    G4String preStepVolume = thisStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    G4String postStepVolume = thisStep->GetPostStepPoint()->GetPhysicalVolume()->GetLogicalVolume()->GetName();

    // Determine whether to create a calo hit for this particle
    bool generateCaloHit = false;
    
    // make sure the particle is entering the calo, not exiting
    if (preStepVolume=="calo_L" && postStepVolume=="insideCalo_L") // particle is entering calo
    {
        // add this particle to the shower list
        // TO DO: flag these particles as "shower initiators" regardless of parent status
        ShowerListManager::instance().addToList( trackID, parentID );
        
        // exclude particle types that will not cause an interaction in the calo
        if (pdg == 0 || pdg == 12 || pdg ==-14) // optical photon, nu_e, or anti_nu_mu
        {
            generateCaloHit = false;
        }
        else
        {
            generateCaloHit = true;
        }

        // If the killShowers parameter is set to true, kill track when it
        // enters the calo volume (i.e., turn the calo into a black box)
        if( killShowers ){
            track->SetTrackStatus(fStopAndKill);
        }
    }

    if ( generateCaloHit ) {
      thisHC->insert(new CaloHit(thisStep));
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
