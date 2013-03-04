// NO2PhysicsList

#ifndef N02PHYSICS_LIST_HH
#define N02PHYSICS_LIST_HH

#include "Geant4/G4VUserPhysicsList.hh"

namespace gm2ringsim {
  
  class N02PhysicsList : public G4VUserPhysicsList {
    
  public:
    N02PhysicsList();
    virtual ~N02PhysicsList() {};
    
    // The remaining member functions come out of the Geant example code
    
  protected:
    void ConstructParticle() override;
    void ConstructProcess() override;
    
    void SetCuts() override;
    
    // these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void AddStepMax();
    
  };
  
}



#endif
