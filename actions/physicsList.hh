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

class G4Cerenkov;
class G4Scintillation;
class G4OpAbsorption;
class G4OpRayleigh;
class G4OpMieHG;
class G4OpBoundaryProcess;

namespace gm2ringsim
{
  class physicsList : public G4VModularPhysicsList
  {
    
  public:
    physicsList();
    ~physicsList();
    
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    
    
  private:
    void enableStepLimiter();
    
    typedef std::vector<G4VPhysicsConstructor*>  PhysicsListVector;
    
    G4VPhysicsConstructor*  decayPhysicsList_;
    
    PhysicsListVector* physics_;
  };//class physicsList
  
} //namespace physicsList

#endif
