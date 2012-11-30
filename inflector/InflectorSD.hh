#ifndef G2MIGTRACE_INFLECTORSD_HH
#define G2MIGTRACE_INFLECTORSD_HH

/** @file InflectorSD.hh

    Provides declaration of a tracking sensitive detector,
    InflectorSD, and a controlling messenger, InflectorSDMessenger. 

    @author Kevin Lynch
    @date 2009
*/

#include "Geant4/G4VSensitiveDetector.hh"
#include "Geant4/G4Step.hh"
#include "Geant4/G4HCofThisEvent.hh"
#include "gm2ringsim/inflector/inflectorHit.hh"

class InflectorSDMessenger;

/** A sensitive detector similar to the trackerSD for tracking of
    particles inside the inflector beam aperture. */
class InflectorSD : public G4VSensitiveDetector{

public:
  InflectorSD(G4String name);
  ~InflectorSD();
  
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

  InflectorSDMessenger *tsdm_;
};



#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UIcmdWithAnInteger.hh"

/** A G4UImessenger interface to control the InflectorSD sensitive
    detector class. 

    Provides the following commands:
    - /g2MIGTRACE/hits/InflectorSD/printLevel
    - /g2MIGTRACE/hits/InflectorSD/drawLevel
*/
class InflectorSDMessenger : public G4UImessenger {

public:
  InflectorSDMessenger(InflectorSD *tsd);
  ~InflectorSDMessenger();

  void SetNewValue(G4UIcommand*,G4String);

private:
  InflectorSD *tsd_;

  G4UIdirectory *topdir_;
  G4UIdirectory *dir_;
  
  G4UIcmdWithAnInteger *printLevelCmd_;
  G4UIcmdWithAnInteger *drawLevelCmd_;

};

#endif // G2MIGTRACE_INFLECTORSD_HH
