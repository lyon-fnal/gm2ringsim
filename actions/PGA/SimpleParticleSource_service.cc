// Implementation of PrimaryGenService

#include "gm2ringsim/actions/PGA/SimpleParticleSource_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"

#include <iostream>

// Constructor
gm2ringsim::SimpleParticleSource::SimpleParticleSource(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
artg4::PrimaryGeneratorActionBase(p.get<std::string>("name", "SimpleParticleSource")),
particleGun_( 0 ),   // Must not intialize here because Geant isn't ready yet
particleType_( p.get<std::string>("particleType") ),
energy_( p.get<double>("energy") ),
direction_( p.get<std::vector<double>>("direction") ),
position_( p.get<std::vector<double>>("position") )
{
}

// initialize
void gm2ringsim::SimpleParticleSource::initialize() {
  
  particleGun_ = new G4ParticleGun(1);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleType_);
  
  particleGun_->SetParticleDefinition(particle);
  particleGun_->SetParticleMomentumDirection(G4ThreeVector(direction_[0], direction_[1], direction_[2]));
  particleGun_->SetParticleEnergy(energy_*GeV);
}

// EndOfPrimaryGen
void gm2ringsim::SimpleParticleSource::generatePrimaries(G4Event* evt) {
  
  particleGun_->SetParticlePosition(G4ThreeVector(position_[0]*mm, position_[1]*mm, position_[2]*mm));
  
  particleGun_->GeneratePrimaryVertex(evt);
}

using gm2ringsim::SimpleParticleSource;
DEFINE_ART_SERVICE(SimpleParticleSource)
