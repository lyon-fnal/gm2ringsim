#ifndef physicsList_hh
#define physicsList_hh

/** @file physicsList.hh

    Provides the required interface and declarations for control of
    the physics in the simulation.

    Based in part on examples/extended/field/field04.  

    Let me send a big shout out to the Geant collaboration, who figure
    that control of physics processes is just so obvious and trivial
    that they can't be bothered to document this stuff, or answer
    questions with more than "you should use our predefined lists".
    Way to go!  You've only cost me days of dredging through your
    poorly documented, and mostly non-functional, examples.  That LXe
    example, whose physics list is f*(&ed, was particularly helpful!

    I'm not bitter,  Nope, not me.  Not at all!

    - Zach Hartwig 2005
    - Kevin Lynch 2009
 */

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/G4VPhysicsConstructor.hh"
#include "gm2ringsim/actions/physicsListMessenger.hh"

/** Provides the required physics lists. 

    Interacts with a messenger class to enable/disable pion and muon
    decay, and to set whether those decays are isotropic or account
    for spin.
*/

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace gm2ringsim{
  class physicsList : public G4VModularPhysicsList
  {
    
  public:
    physicsList();
    ~physicsList();
    
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    
    
    /** Completely disables muon decay and pion decays */
    void disableDecay();
    /** Enables isotropic muon and pion decays. */
    void enableIsotropicDecay();
    /** Enables full standard model, spin dependent muon and pion decays */
    void enableSMDecay();
    /** Returns a status string containing the sping decay type. */
    G4String currentDecay();
    
    /** Gets the output verbosity that propogates to each contained
	physics process. */
    G4int verboseLevel() const { return verboseLevel_; }
    /** Sets the output verbosity that propogates to each contained
	physics process. */
    G4int verboseLevel(G4int level);
    
  private:
    void enableStepLimiter();
    
    // working code
    template<class T> void pionDecay();
    template<class T> void muonDecay();
    void unpolDecayChannel();
    void polDecayChannel();
    
    typedef std::vector<G4VPhysicsConstructor*>  PhysicsListVector;
    
    G4int verboseLevel_;
    
    enum decayStatus { decay_init, decay_none, decay_isotropic, decay_standard };
    decayStatus decayStatus_;
    
    G4VPhysicsConstructor*  decayPhysicsList_;
    
    PhysicsListVector* physics_;
    
    physicsListMessenger *mess_;
    
    // From N06 example
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpMieHG*           theMieHGScatteringProcess;    G4OpBoundaryProcess* theBoundaryProcess;
  };//class physicsList
  
} //namespace physicsList

#endif
