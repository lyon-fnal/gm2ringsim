#ifndef G2MIGTRACE_PHYSICSLISTMESSENGER_HH
#define G2MIGTRACE_PHYSICSLISTMESSENGER_HH

/** @file physicsListMessenger.hh

    Provides the declarations for the messenger interface to the
    custom physics options.

    - Kevin Lynch 2009
*/

#include "Geant4/G4UImessenger.hh"

#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UIcmdWithAString.hh"



/** A Geant4/G4UImessenger that controls the custom physics options for the
    simulation.

    Provides control of muon spin decay mode:
    - /g2MIGTRACE/spin_decays/muonDecayMode

    Spin tracking is controlled by the spinController and the
    spinMessenger.
*/
namespace gm2ringsim{ 
class physicsList;

  class physicsListMessenger : public G4UImessenger {
    
  public:
    physicsListMessenger(physicsList*);
    ~physicsListMessenger();
    
    void SetNewValue(G4UIcommand *, G4String);
    
  private:
    physicsList *pl_;
    
    G4UIdirectory *dir_;
    G4UIcmdWithAString *decayCmd_;
  }; // class physicsListMessenger

} //namespace gm2ringsim

#endif // G2MIGTRACE_PHYSICSLISTMESSENGER_HH
