#ifndef Gm2ModularPhysicsList_hh
#define Gm2ModularPhysicsList_hh

/** @file Gm2ModularPhysicsList.hh

    Provides the required interface and declarations for control of
    the physics in the simulation.

    Based in part on examples/extended/field/field04.  

*/

#include "Geant4/globals.hh"
#include "Geant4/G4VModularPhysicsList.hh"
#include "Geant4/G4VPhysicsConstructor.hh"

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace gm2ringsim
{
  class Gm2ModularPhysicsList : public G4VModularPhysicsList
  {
    
  public:
    Gm2ModularPhysicsList();
    Gm2ModularPhysicsList(G4VModularPhysicsList *mPL);
    ~Gm2ModularPhysicsList();
    
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    
    const G4VPhysicsConstructor* GetPhysicsConstructor() const { return decayPhysicsList_; };
    
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

    void Initialize();
    
    typedef std::vector<G4VPhysicsConstructor*>  PhysicsListVector;
    
    G4int verboseLevel_;
    
    enum decayStatus { decay_init, decay_none, decay_isotropic, decay_standard };
    decayStatus decayStatus_;
    
    G4VPhysicsConstructor*  decayPhysicsList_;
    
    PhysicsListVector* physics_;
    
    // From N06 example
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpMieHG*           theMieHGScatteringProcess;    G4OpBoundaryProcess* theBoundaryProcess;
  };//class Gm2ModularPhysicsList
  
} //namespace Gm2ModularPhysicsList

#endif
