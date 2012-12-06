#ifndef G2MIGTRACE_TURNCOUNTERSD_HH
#define G2MIGTRACE_TURNCOUNTERSD_HH

/** @file turnCounterSD.hh
    
    Provides the declaration of the turnCounterSD.
    
    - Kevin Lynch 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

/** Provides the declaration of a turn counting sensitive detector.

    This SD interacts with the turnCounter.  This class is responsible
    for doing the hard work to ensure that the turn counter is only
    incremented by the right particles traveling in the right
    direction. 

    Don't instantiate this directly; obtain a handle through the
    SDHandleOwner.  Otherwise, the enable/disable logic in Geant's
    Sensitive Detector Manager can get screwed up.
*/
class turnCounterSD : public G4VSensitiveDetector{

public:
  turnCounterSD(G4String name);
  ~turnCounterSD();

  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

};

#endif // G2MIGTRACE_TURNCOUNTERSD_HH
