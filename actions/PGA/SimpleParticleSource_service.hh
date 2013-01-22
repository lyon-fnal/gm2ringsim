// Header for the Event action

#ifndef SIMPLEPARTICLESOURCE_SERVICE_HH
#define SIMPLEPARTICLESOURCE_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"


// Get the base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"

#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"

namespace gm2ringsim
{
  class SimpleParticleSource : public artg4::PrimaryGeneratorActionBase
  {
  public:
    SimpleParticleSource(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~SimpleParticleSource() { delete particleGun_; }
    
    void initialize() override;
    
    void generatePrimaries( G4Event* ) override;
    
  private:
    G4ParticleGun* particleGun_;
  };
}
#endif

