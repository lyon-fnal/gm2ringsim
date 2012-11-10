// Here we define the physics list

#ifndef PHYSICSLIST_SERVICE_HH
#define PHYSICSLIST_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "Geant4/G4VUserPhysicsList.hh"

namespace gm2ringsim {

  class PhysicsList : public G4VUserPhysicsList {
    
  public:
    PhysicsList();
    
    virtual ~PhysicsList() {};
    
    // The remaining member functions come out of the Geant example code
    
  protected:
    void ConstructParticle();
    void ConstructProcess();
    
    void SetCuts();
    
    // these methods Construct particles
    
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void AddStepMax();
    
  };
  
  
  class PhysicsListService {
  public:
    PhysicsListService( fhicl::ParameterSet const &, art::ActivityRegistry & );
    
    virtual ~PhysicsListService() {};
    
  };

}


#endif
