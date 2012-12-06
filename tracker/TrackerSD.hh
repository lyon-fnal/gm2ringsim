#ifndef G2MIGTRACE_TRACKERSD_HH
#define G2MIGTRACE_TRACKERSD_HH

/** @file trackerSD.hh

    Declares the sensitive detector and messenger for the in-ring beam
    trackers. 

    @author Kevin Lynch 
    @date 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "gm2ringsim/tracker/TrackerHit.hh"
//FIXME: #include "spinHit.hh"

class TrackerSDMessenger;

/** Provides the in-ring beam tracker volumes with a sensitive
    detector. 

    This detector emits both trackerHit and spinHit collections for
    position/momentum, trackerHit, and spin tracking, spinHit.

    Do NOT instantiate instances directly; utilize the SDHandleOwner
    interface to obtain a pointer to the active implementation.
    Otherwise, the enable/disable logic in Geant could go screwy. 
*/
class TrackerSD : public G4VSensitiveDetector{

public:
  TrackerSD(G4String name);
  ~TrackerSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  G4int PrintLevel() const { return printLevel_; };
  G4int PrintLevel(G4int newLevel);

  G4int DrawLevel() const { return drawLevel_; };
  G4int DrawLevel(G4int newLevel);
private:
  TrackerHitsCollection *trackerHC_;
  //FIXME:  spinHitsCollection *spinHC_;

  G4int printLevel_, drawLevel_;

  //TrackerSDMessenger *tsdm_;
};



// #include "G4UImessenger.hh"
// #include "G4UIdirectory.hh"
// #include "G4UIcmdWithAnInteger.hh"

// /** The G4UImessenger interface to the in-ring beam tracker sensitive
//     detectors. 

//     Provdes the following commands:
//     - /g2MIGTRACE/hits/TrackerSD/printLevel
//     - /g2MIGTRACE/hits/TrackerSD/drawLevel
// */
// class TrackerSDMessenger : public G4UImessenger {

// public:
//   TrackerSDMessenger(TrackerSD *tsd);
//   ~TrackerSDMessenger();

//   void SetNewValue(G4UIcommand*,G4String);

// private:
//   TrackerSD *tsd_;

//   G4UIdirectory *topdir_;
//   G4UIdirectory *dir_;
  
//   G4UIcmdWithAnInteger *printLevelCmd_;
//   G4UIcmdWithAnInteger *drawLevelCmd_;

// };


 #endif // G2MIGTRACE_TRACKERSD_HH
