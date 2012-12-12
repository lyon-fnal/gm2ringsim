// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "artg4/services/PhysicsListServiceBase.hh"

namespace gm2ringsim {
  

  class PhysicsListService : public artg4::PhysicsListServiceBase {
    
  public:
    PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &);    
    virtual ~PhysicsListService() {};
  };
  
}

#endif
