// Implementation of PrimaryGenService

#include "gm2ringsim/actions/PGA/SimpleParticleSource_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"

// Constructor
gm2ringsim::SimpleParticleSource::SimpleParticleSource(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
artg4::PrimaryGeneratorActionBase(p.get<std::string>("name", "SimpleParticleSource")),
particleGun_( 0 )   // Must not intialize here because Geant isn't ready yet
{}

// initialize
void gm2ringsim::SimpleParticleSource::initialize() {
  
  particleGun_ = new G4ParticleGun(1);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");
  
  particleGun_->SetParticleDefinition(particle);
  particleGun_->SetParticleMomentumDirection(G4ThreeVector(0.,1.,0.));
  particleGun_->SetParticleEnergy(0.5*GeV);
}

// EndOfPrimaryGen
void gm2ringsim::SimpleParticleSource::generatePrimaries(G4Event* evt) {
  
  G4double position = -0.45*100;
  particleGun_->SetParticlePosition(G4ThreeVector(0.0*cm, position, 0.0*cm));
  
  particleGun_->GeneratePrimaryVertex(evt);
}

using gm2ringsim::SimpleParticleSource;
DEFINE_ART_SERVICE(SimpleParticleSource)
