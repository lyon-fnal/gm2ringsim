#ifndef FiberHarpSD_hh
#define FiberHarpSD_hh

/** @file FiberHarpSD.hh

    Provides the declaration of a fiber harp sensitive detector.

    Derived from work of Kevin Lynch, 2011 and Fred Gray, 2012.

    @author Peter Winter
    @date 2012
*/

#include "FiberHarpHit.hh"

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

class FiberHarpSDMessenger;

/** A sensitive detector for the fiberHarps. */
class FiberHarpSD : public G4VSensitiveDetector{

public:
  FiberHarpSD(G4String name);
  ~FiberHarpSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  G4int PrintLevel() const { return printLevel; };
  G4int PrintLevel(G4int newLevel);

  G4int DrawLevel() const { return drawLevel; };
  G4int DrawLevel(G4int newLevel);
private:
  FiberHarpHitsCollection *thisHC;

  G4int printLevel, drawLevel;
};

#endif // FiberHarpSD_hh
