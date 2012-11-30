#ifndef G2MIGTRACE_INFLECTORSD_HH
#define G2MIGTRACE_INFLECTORSD_HH

/** @file inflectorSD.hh

    Provides declaration of a tracking sensitive detector,
    inflectorSD, and a controlling messenger, inflectorSDMessenger. 

    @author Kevin Lynch
    @date 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "gm2ringsim/inflector/inflectorHit.hh"

class inflectorSDMessenger;

/** A sensitive detector similar to the trackerSD for tracking of
    particles inside the inflector beam aperture. */
class inflectorSD : public G4VSensitiveDetector{

public:
  inflectorSD(G4String name);
  ~inflectorSD();
  
  void Initialize(G4HCofThisEvent*);
  G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  void EndOfEvent(G4HCofThisEvent*);

  G4int PrintLevel() const { return printLevel; };
  G4int PrintLevel(G4int newLevel);

  G4int DrawLevel() const { return drawLevel; };
  G4int DrawLevel(G4int newLevel);
private:
  inflectorHitsCollection *thisHC;

  G4int printLevel, drawLevel;

  inflectorSDMessenger *tsdm_;
};



#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UIcmdWithAnInteger.hh"

/** A G4UImessenger interface to control the inflectorSD sensitive
    detector class. 

    Provides the following commands:
    - /g2MIGTRACE/hits/inflectorSD/printLevel
    - /g2MIGTRACE/hits/inflectorSD/drawLevel
*/
class inflectorSDMessenger : public G4UImessenger {

public:
  inflectorSDMessenger(inflectorSD *tsd);
  ~inflectorSDMessenger();

  void SetNewValue(G4UIcommand*,G4String);

private:
  inflectorSD *tsd_;

  G4UIdirectory *topdir_;
  G4UIdirectory *dir_;
  
  G4UIcmdWithAnInteger *printLevelCmd_;
  G4UIcmdWithAnInteger *drawLevelCmd_;

};

#endif // G2MIGTRACE_INFLECTORSD_HH
