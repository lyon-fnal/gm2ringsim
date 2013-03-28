
#ifndef CaloSD_hh
#define CaloSD_hh

/** @file CaloSD.hh
 
    Provides the declaration of a "stops and kills" calorimeter
    sensitive detector.
 
    Ported to Art from g2migtrace file calorimeterSD.hh
        @author Kevin Lynch
        @date 2009

    @author Robin Bjorkquist
    @date 2013
 */

#include "CaloHit.hh"

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

/** A sensitive detector for the calorimeters.
 
    This class will output various information via a CaloHit for
    each external (to the calorimeter) particle that enters the
    calorimeter mother volume.
 
    If you want to speed the simulation, you can now "stop and kill"
    the external tracks here to avoid the shower simulation, which is
    very time consuming.
 
 */

namespace gm2ringsim {
    
    class CaloSD : public G4VSensitiveDetector{
        
    public:
        CaloSD(G4String name);
        ~CaloSD();
        
        void Initialize(G4HCofThisEvent*);
        G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        void EndOfEvent(G4HCofThisEvent*);
        
        G4int PrintLevel() const { return printLevel; };
        G4int PrintLevel(G4int newLevel);
        
        G4int DrawLevel() const { return drawLevel; };
        G4int DrawLevel(G4int newLevel);
        
        G4bool KillShowers() const { return killShowers; };
        G4bool KillShowers(G4bool newLevel);
        
    private:
        CaloHitsCollection *thisHC;
        
        G4int printLevel, drawLevel;
        
        G4bool killShowers;
    };
    
} // namespace gm2ringsim

#endif // CaloSD_hh
