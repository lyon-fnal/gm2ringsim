// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "artg4/services/PhysicsListServiceBase.hh"

#include <string>

class G4VUserPhysicsList;

namespace gm2ringsim {
  

  class Gm2PhysicsListService : public artg4::PhysicsListServiceBase {
    
  public:
    Gm2PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &);
    virtual ~PhysicsListService() {};
    virtual G4VUserPhysicsList* makePhysicsList() const override;
    
  private:
    std::string muonDecayMode_;
    int verboseLevel_;
  };
  
}

#endif
