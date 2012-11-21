/** @file PGA.cc prior to ART
    @file PrimaryGeneratorAction_service.cc
    Implements PGA, the required primary generator action.
    
    @author Zach Hartwig
    @author Kevin Lynch
    @date 2005-2009
    @author Nathan Froemming
    @date August 2012
    @author Brendan Kiburg
    @date 2012 Port to ART                                                                                
*/

#include "gm2ringsim/actions/PrimaryGeneratorAction_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Constructor
gm2ringsim::PrimaryGeneratorAction::PrimaryGeneratorAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
    artg4::PrimaryGeneratorActionBase(p.get<std::string>("name")),
    particleGun_( 0 )   // Must not intialize here because Geant isn't ready yet
{}

// initialize
void gm2ringsim::PrimaryGeneratorAction::initialize() {
  
  particleGun_ = new G4ParticleGun(1);
  //G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  //  G4ParticleDefinition* particle = particleTable->FindParticle("proton");
  
  //  particleGun_->SetParticleDefinition(particle);
  particleGun_->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun_->SetParticleEnergy(3.0*GeV);
  
}

// EndOfPrimaryGeneratorAction
void gm2ringsim::PrimaryGeneratorAction::generatePrimaries(G4Event* evt) {

    particleGun_->GeneratePrimaryVertex(evt);
}

using gm2ringsim::PrimaryGeneratorAction;
DEFINE_ART_SERVICE(PrimaryGeneratorAction)
