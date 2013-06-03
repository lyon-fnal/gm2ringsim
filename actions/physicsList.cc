/** @file physicsList.cc

    Provides implementation of the required physics list for the
    simulation. 

    - Zach Hartwig 2005
    - Kevin Lynch 2009
*/


#include "gm2ringsim/actions/physicsList.hh"

#include "Geant4/G4ProcessTable.hh"
#include "Geant4/G4ProcessManager.hh"

#include "Geant4/G4DecayPhysics.hh"
#include "Geant4/G4EmStandardPhysics.hh"
#include "Geant4/G4EmExtraPhysics.hh"

#include "Geant4/G4MuonPlus.hh"
#include "Geant4/G4MuonMinus.hh"
#include "Geant4/G4PionPlus.hh"
#include "Geant4/G4PionMinus.hh"

#include "Geant4/G4Decay.hh"
#include "Geant4/G4PionDecayMakeSpin.hh"
#include "Geant4/G4DecayWithSpin.hh"

#include "Geant4/G4DecayTable.hh"
#include "Geant4/G4MuonDecayChannel.hh"
#include "Geant4/G4MuonDecayChannelWithSpin.hh"
#include "Geant4/G4MuonRadiativeDecayChannelWithSpin.hh"

#include "Geant4/G4HadronDElasticPhysics.hh"
#include "Geant4/G4QStoppingPhysics.hh"
#include "Geant4/G4IonBinaryCascadePhysics.hh"
#include "Geant4/G4NeutronTrackingCut.hh"

#include "Geant4/G4StepLimiter.hh"
#include "Geant4/G4UserSpecialCuts.hh"

#include "Geant4/G4Cerenkov.hh"
#include "Geant4/G4Scintillation.hh"
#include "Geant4/G4OpAbsorption.hh"
#include "Geant4/G4OpRayleigh.hh"
//#include "Geant4/G4OpMieHG.hh"
#include "Geant4/G4OpBoundaryProcess.hh"
#include "Geant4/G4LossTableManager.hh"
#include "Geant4/G4EmSaturation.hh"

gm2ringsim::physicsList::physicsList() : decayPhysicsList_(new G4DecayPhysics),
					 physics_(new PhysicsListVector) {
  G4cout << "gm2ringsim::physicsList::physicsList()" << G4endl;

  defaultCutValue = 1*mm;

  physics_ -> push_back(new G4EmStandardPhysics);
  
}

gm2ringsim::physicsList::~physicsList(){
  
  PhysicsListVector::const_iterator b = physics_->begin(), e = physics_->end();
  while( b!=e ){
    delete (*b);
    ++b;
  }
  physics_->clear();

  delete physics_;
  delete decayPhysicsList_;
}


void gm2ringsim::physicsList::ConstructParticle(){

  // constructs everybody and their uncles, second cousins,
  // step-children, and pets
  decayPhysicsList_->ConstructParticle();

  // wsun: optical photon already constructed in default G4DecayPhysics
}

void gm2ringsim::physicsList::ConstructProcess(){
  // TRANSPORTATION ... REQUIRED
  AddTransportation();

  // everyone can now decay .....
  decayPhysicsList_->ConstructProcess();
  // but since we don't want that, let's pull out the muon decay
  PhysicsListVector::const_iterator b = physics_->begin(), e = physics_->end();
  while( b!=e ){
    (*b)->ConstructProcess();
    ++b;
  }
}

void gm2ringsim::physicsList::SetCuts(){ 
  SetCutsWithDefault(); 
}

void gm2ringsim::physicsList::enableStepLimiter(){
  theParticleIterator->reset();

  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4double charge = particle->GetPDGCharge();
    
    //G4cout << "physicsList::enableStepLimiter - " << particleName << G4endl;
    
    if (!pmanager) {
      G4cerr << "Particle " << particleName << "without a Process Manager";
    }
    
    if (charge != 0.0) {
      // All charged particles should have a step limiter
      // to make sure that the steps do not get too long.
      pmanager->AddDiscreteProcess(new G4StepLimiter());
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    } else if (particleName == "neutron") {
      // time cuts for ONLY neutrons:
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    } else {
      // Energy cuts for all other neutral particles
      pmanager->AddDiscreteProcess(new G4UserSpecialCuts());
    }
  }
}
