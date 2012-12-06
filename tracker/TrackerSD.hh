#ifndef G2MIGTRACE_TRACKERSD_HH
#define G2MIGTRACE_TRACKERSD_HH

/** @file trackerSD.hh

    Declares the sensitive detector and messenger for the in-ring beam
    trackers. 

    @author Kevin Lynch 
    @date 2009
*/

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "trackerHit.hh"
#include "spinHit.hh"

class trackerSDMessenger;

/** Provides the in-ring beam tracker volumes with a sensitive
    detector. 

    This detector emits both trackerHit and spinHit collections for
    position/momentum, trackerHit, and spin tracking, spinHit.

    Do NOT instantiate instances directly; utilize the SDHandleOwner
    interface to obtain a pointer to the active implementation.
    Otherwise, the enable/disable logic in Geant could go screwy. 
*/
class trackerSD : public G4VSensitiveDetector{

public:
  trackerSD(G4String name);
  ~trackerSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  G4int PrintLevel() const { return printLevel; };
  G4int PrintLevel(G4int newLevel);

  G4int DrawLevel() const { return drawLevel; };
  G4int DrawLevel(G4int newLevel);
private:
  trackerHitsCollection *trackerHC;
  spinHitsCollection *spinHC;

  G4int printLevel, drawLevel;

  trackerSDMessenger *tsdm_;
};



#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

/** The G4UImessenger interface to the in-ring beam tracker sensitive
    detectors. 

    Provdes the following commands:
    - /g2MIGTRACE/hits/trackerSD/printLevel
    - /g2MIGTRACE/hits/trackerSD/drawLevel
*/
class trackerSDMessenger : public G4UImessenger {

public:
  trackerSDMessenger(trackerSD *tsd);
  ~trackerSDMessenger();

  void SetNewValue(G4UIcommand*,G4String);

private:
  trackerSD *tsd_;

  G4UIdirectory *topdir_;
  G4UIdirectory *dir_;
  
  G4UIcmdWithAnInteger *printLevelCmd_;
  G4UIcmdWithAnInteger *drawLevelCmd_;

};

#endif // G2MIGTRACE_TRACKERSD_HH
