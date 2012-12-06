#ifndef G2MIGTRACE_RINGSD_HH
#define G2MIGTRACE_RINGSD_HH

/** @file ringSD.hh

    Declares a sensitive detector to record energy loss data along
    with an associated messenger.

    - Kevin Lynch 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

#include "gm2ringsim/common/ring/RingHit.hh"

class RingSDMessenger;

/** A Sensitive Detector to record energy loss data in the ring. 

    Don't instantiate this directly; obtain a handle through the
    SDHandleOwner.  Otherwise, the enable/disable logic in Geant's
    Sensitive Detector Manager can get screwed up.
*/
class RingSD : public G4VSensitiveDetector{

public:
  RingSD(G4String name);
  ~RingSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  G4int printLevel() const { return printLevel_; };
  G4int printLevel(G4int newLevel);

  G4int drawLevel() const { return drawLevel_; };
  G4int drawLevel(G4int newLevel);
private:
  RingHitsCollection *thisHC_;

  G4int printLevel_, drawLevel_;

  //  RingSDMessenger *rsdm_;
};



// #include "G4UImessenger.hh"
// #include "G4UIdirectory.hh"
// #include "G4UIcmdWithAnInteger.hh"

// /** A G4UImessenger interface to control the RingSD configuration.

//     Provides the following commands:
//     - /g2MIGTRACE/hits/RingSD/printLevel
//     - /g2MIGTRACE/hits/RingSD/drawLevel
// */
// class RingSDMessenger : public G4UImessenger {

// public:
//   RingSDMessenger(RingSD *rsd);
//   ~RingSDMessenger();

//   void SetNewValue(G4UIcommand*,G4String);

// private:
//   RingSD *rsd_;

//   G4UIdirectory *topdir_;
//   G4UIdirectory *dir_;
  
//   G4UIcmdWithAnInteger *printLevelCmd_;
//   G4UIcmdWithAnInteger *drawLevelCmd_;

// };


#endif // G2MIGTRACE_RINGSD_HH
