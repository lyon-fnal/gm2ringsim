// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "artg4/services/PhysicsListServiceBase.hh"

#include "Geant4/G4String.hh"

#include <string>

class G4VUserPhysicsList;
class G4VModularPhysicsList;
class G4ParticleTable;
class G4PTblDicIterator;
class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace gm2ringsim {
  
  class Gm2PhysicsListService : public artg4::PhysicsListServiceBase {
    
  public:
    Gm2PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &);
    virtual ~Gm2PhysicsListService() {};
    virtual G4VUserPhysicsList* makePhysicsList() override;
    virtual void initializePhysicsList() override;
    
    void ConstructAdditionalProcess();

    /** Completely disables muon decay and pion decays */
    void disableDecay();
    /** Completely disables muon decay */
    void disableMuonDecay();
    /** Completely disables pion decay */
    void disablePionDecay();
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

    // working code
    template<class T> void pionDecay();
    template<class T> void muonDecay();
    void unpolDecayChannel();
    void polDecayChannel();

    std::string muonDecayMode_;
    bool pionDecayEnabled_;
    G4String physicsListName_;
    int verboseLevel_;
    G4VModularPhysicsList* thePhysicsList_;
    G4ParticleTable::G4PTblDicIterator *theParticleIterator_;

    enum decayStatus { decay_init, decay_none, decay_isotropic, decay_standard };
    decayStatus decayStatus_;
    
    // From N06 example
    G4Cerenkov*          theCerenkovProcess;
    G4Scintillation*     theScintillationProcess;
    G4OpAbsorption*      theAbsorptionProcess;
    G4OpRayleigh*        theRayleighScatteringProcess;
    G4OpMieHG*           theMieHGScatteringProcess;
    G4OpBoundaryProcess* theBoundaryProcess;
  };  
}

#endif
