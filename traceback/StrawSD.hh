#ifndef StrawSD_hh
#define StrawSD_hh

/** @file StrawSD.hh

    Provides the declaration of a straw sensitive detector, and a
    messenger interface to control it.

    @author Kevin Lynch
    @date 2011
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"

#include "gm2ringsim/traceback/StrawHit.hh"

//class StrawSDMessenger;

namespace gm2ringsim {
  /** A sensitive detector for the Straws. */
  class StrawSD : public G4VSensitiveDetector{
    
  public:
    StrawSD(G4String name);
    ~StrawSD();
    
    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
    G4int PrintLevel() const { return printLevel; };
    G4int PrintLevel(G4int newLevel);
    
    G4int DrawLevel() const { return drawLevel; };
    G4int DrawLevel(G4int newLevel);
  private:
    StrawHitsCollection *thisHC;
    
    G4int printLevel, drawLevel;
    
    //    StrawSDMessenger *hsdm_;
  }; // class StrawSD

}//namespace gm2ringsim

//#include "Geant4/G4UImessenger.hh"
//#include "Geant4/G4UIdirectory.hh"
//#include "Geant4/G4UIcmdWithAnInteger.hh"

/** A G4UImessenger interface to control the StrawSD.

    Currently provides the following commands:
    - /g2MIGTRACE/hits/StrawSD/printLevel
    - /g2MIGTRACE/hits/StrawSD/drawLevel
*/
/* class StrawSDMessenger : public G4UImessenger {

public:
  StrawSDMessenger(StrawSD *rsd);
  ~StrawSDMessenger();

  void SetNewValue(G4UIcommand*,G4String);

private:
  StrawSD *rsd_;

  G4UIdirectory *topdir_;
  G4UIdirectory *dir_;
  
  G4UIcmdWithAnInteger *printLevelCmd_;
  G4UIcmdWithAnInteger *drawLevelCmd_;

};
*/

#endif // StrawSD_hh
