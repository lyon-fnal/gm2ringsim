// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "artg4/services/PhysicsListServiceBase.hh"

#include "Geant4/G4String.hh"

#include <string>

class G4VUserPhysicsList;

namespace gm2ringsim {
  
  class Gm2ModularPhysicsList;

  class Gm2PhysicsListService : public artg4::PhysicsListServiceBase {
    
  public:
    Gm2PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &);
    virtual ~Gm2PhysicsListService() {};
    virtual G4VUserPhysicsList* makePhysicsList() override;
    virtual void initializePhysicsList() override;
    
  private:
    std::string muonDecayMode_;
    bool pionDecayEnabled_;
    G4String physicsListName_;
    int verboseLevel_;
    Gm2ModularPhysicsList* thePhysicsList_;
  };
  
}

#endif
