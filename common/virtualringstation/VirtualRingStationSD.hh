#ifndef G2MIGTRACE_VIRTUALRINGSTATIONSD_HH
#define G2MIGTRACE_VIRTUALRINGSTATIONSD_HH

/** @file virtualringstationSD.hh

    Declares the sensitive detector and messenger for the in-ring beam
    virtualringstations. 

    @author Kevin Lynch 
    @date 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationHit.hh"
#include "gm2ringsim/common/spin/SpinHit.hh"

//class gm2ringsim::VirtualRingStationSDMessenger;

namespace gm2ringsim{ 
  /** Provides the in-ring beam virtualringstation volumes with a sensitive
      detector. 
      
      This detector emits both virtualringstationHit and spinHit collections for
      position/momentum, virtualringstationHit, and spin tracking, spinHit.
      
      Do NOT instantiate instances directly; utilize the SDHandleOwner
      interface to obtain a pointer to the active implementation.
      Otherwise, the enable/disable logic in Geant could go screwy. 
  */
  class VirtualRingStationSD : public G4VSensitiveDetector{
    
  public:
    VirtualRingStationSD(G4String name);
    ~VirtualRingStationSD();
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
    G4int PrintLevel() const { return printLevel_; };
    G4int PrintLevel(G4int newLevel);
    
    G4int DrawLevel() const { return drawLevel_; };
    G4int DrawLevel(G4int newLevel);
  private:
    VirtualRingStationHitsCollection *virtualringstationHC_;
    gm2ringsim::SpinHitsCollection *spinHC_;
    
    G4int printLevel_, drawLevel_;
    
    //VirtualRingStationSDMessenger *tsdm_;
  }; //class VirtualRingStationSD

}//namespace gm2ringsim



// #include "G4UImessenger.hh"
// #include "G4UIdirectory.hh"
// #include "G4UIcmdWithAnInteger.hh"

// /** The G4UImessenger interface to the in-ring beam virtualringstation sensitive
//     detectors. 

//     Provdes the following commands:
//     - /g2MIGTRACE/hits/VirtualRingStationSD/printLevel
//     - /g2MIGTRACE/hits/VirtualRingStationSD/drawLevel
// */
// class VirtualRingStationSDMessenger : public G4UImessenger {

// public:
//   VirtualRingStationSDMessenger(VirtualRingStationSD *tsd);
//   ~VirtualRingStationSDMessenger();

//   void SetNewValue(G4UIcommand*,G4String);

// private:
//   VirtualRingStationSD *tsd_;

//   G4UIdirectory *topdir_;
//   G4UIdirectory *dir_;
  
//   G4UIcmdWithAnInteger *printLevelCmd_;
//   G4UIcmdWithAnInteger *drawLevelCmd_;

// };


 #endif // G2MIGTRACE_VIRTUALRINGSTATIONSD_HH
