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

gm2ringsim::physicsList::physicsList() : verboseLevel_(1),
			     decayStatus_(decay_init),
			     decayPhysicsList_(new G4DecayPhysics),
			     physics_(new PhysicsListVector) {
  defaultCutValue = 1*mm;
  //FIXME: This breaks in ART
  //physics_ -> push_back(new G4EmStandardPhysics);
  
  // This was commented out in g2MIGTRACE already
  /*
  G4EmExtraPhysics *eep = new G4EmExtraPhysics;
  G4String on("on");
  eep->Synch(on);
  eep->GammaNuclear(on);
  eep->MuonNuclear(on);
  physics_->push_back(eep);

  physics_->push_back(new G4HadronDElasticPhysics );
  physics_->push_back(new  G4QStoppingPhysics );
  physics_->push_back(new G4IonBinaryCascadePhysics );
  physics_->push_back(new G4NeutronTrackingCut );
  */
  verboseLevel(verboseLevel_);

  theCerenkovProcess           = NULL;
  theScintillationProcess      = NULL;
  theAbsorptionProcess         = NULL;
  theRayleighScatteringProcess = NULL;
  theMieHGScatteringProcess    = NULL;
  theBoundaryProcess           = NULL;
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

  disableDecay();
  this->pionDecay<G4Decay>();

  enableStepLimiter();

  decayStatus_ = decay_none;  

  // ~~~~~ wsun: add optical photon processes, from ExN06PhysicsList::ConstructOp()

  theCerenkovProcess           = new G4Cerenkov("Cerenkov");
  theScintillationProcess      = new G4Scintillation("Scintillation");
  theAbsorptionProcess         = new G4OpAbsorption();
  theRayleighScatteringProcess = new G4OpRayleigh();
//  theMieHGScatteringProcess    = new G4OpMieHG();
  theBoundaryProcess           = new G4OpBoundaryProcess();

  //  theBoundaryProcess->SetVerboseLevel(2);

  theCerenkovProcess->SetMaxNumPhotonsPerStep(20);
  theCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  
  theScintillationProcess->SetScintillationYieldFactor(1.);
  theScintillationProcess->SetTrackSecondariesFirst(true);

  // Use Birks Correction in the Scintillation process

  G4EmSaturation* emSaturation = G4LossTableManager::Instance()->EmSaturation();
  theScintillationProcess->AddSaturation(emSaturation);

  G4OpticalSurfaceModel themodel = unified;
  theBoundaryProcess->SetModel(themodel);

  theParticleIterator->reset();
  while( (*theParticleIterator)() ){
     G4ParticleDefinition* particle = theParticleIterator->value();
     G4ProcessManager* pmanager = particle->GetProcessManager();
     G4String particleName = particle->GetParticleName();
     if (theCerenkovProcess->IsApplicable(*particle)) {
	pmanager->AddProcess(theCerenkovProcess);
	pmanager->SetProcessOrdering(theCerenkovProcess,idxPostStep);
     }
     if (theScintillationProcess->IsApplicable(*particle)) {
	pmanager->AddProcess(theScintillationProcess);
	pmanager->SetProcessOrderingToLast(theScintillationProcess, idxAtRest);
	pmanager->SetProcessOrderingToLast(theScintillationProcess, idxPostStep);
     }
     if (particleName == "opticalphoton") {
	G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
	pmanager->AddDiscreteProcess(theAbsorptionProcess);
	pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
//	pmanager->AddDiscreteProcess(theMieHGScatteringProcess);
	pmanager->AddDiscreteProcess(theBoundaryProcess);
     }
  }
}

void gm2ringsim::physicsList::SetCuts(){ 
  SetCutsWithDefault(); 
}

G4String gm2ringsim::physicsList::currentDecay(){
  G4String t;
  switch( decayStatus_ ){
  case decay_init:
    t = "initializing";
    break;
  case decay_none:
    t = "none";
    break;
  case decay_isotropic:
    t = "isotropic";
    break;
  case decay_standard:
    t = "standard model";
    break;
  default:
    t = "oops!";
  }

  return t;
}

G4int gm2ringsim::physicsList::verboseLevel(G4int level) { 
  G4int t(verboseLevel_); 
  verboseLevel_ = level; 
  SetVerboseLevel(verboseLevel_);

  PhysicsListVector::const_iterator b = physics_->begin(), e = physics_->end();
  while( b!=e ){
    (*b)->SetVerboseLevel(verboseLevel_);
    ++b;
  }

  return t; 
}




/// PRIVATES

void gm2ringsim::physicsList::disableDecay(){
  if( decayStatus_ == decay_none )
    return;

  G4ProcessTable* table = G4ProcessTable::GetProcessTable();
  G4ProcessManager *manager;
  G4VProcess *process1, *process2;

  // for mu+

  process1 = table->FindProcess("Decay",G4MuonPlus::MuonPlus());
  process2 = table->FindProcess("DecayWithSpin",G4MuonPlus::MuonPlus());
  manager = G4MuonPlus::MuonPlus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process2);
  } else {
    G4cerr << "Couldn't get MuonPlus process manager ... to remove decays!\n";
  }

  // for mu-
  process1 = table->FindProcess("Decay",G4MuonMinus::MuonMinus());
  process2 = table->FindProcess("DecayWithSpin",G4MuonMinus::MuonMinus());
  manager = G4MuonMinus::MuonMinus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process2);
  } else {
    G4cerr << "Couldn't get MuonMinus process manager ... to remove decays!\n";
  }

  decayStatus_ = decay_none;
}


void gm2ringsim::physicsList::enableIsotropicDecay(){
  if( decayStatus_ == decay_isotropic )
    return;

  disableDecay();

  this->muonDecay<G4Decay>();

  unpolDecayChannel();

  this->pionDecay<G4Decay>();

  decayStatus_ = decay_isotropic;
}

void gm2ringsim::physicsList::enableSMDecay(){
  if( decayStatus_ == decay_standard )
    return;

  disableDecay();

  this->muonDecay<G4DecayWithSpin>();

  polDecayChannel();

  this->pionDecay<G4PionDecayMakeSpin>();

  decayStatus_ = decay_standard;
}


template<class T> void gm2ringsim::physicsList::pionDecay(){
  G4ProcessTable* table = G4ProcessTable::GetProcessTable();
  G4ProcessManager *manager;
  G4VProcess *process1, *process2;

  // pi+
  process2 = new T();
  process1 = table->FindProcess("Decay",G4PionPlus::PionPlus());
  manager = G4PionPlus::PionPlus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    manager->AddProcess(process2);
    manager->SetProcessOrdering(process2, idxPostStep);
    manager->SetProcessOrdering(process2, idxAtRest);
  }else {
    G4cerr << "Couldn't get PionPlus process manager ... to add/remove decays!\n";
  }

  // pi-
  process2 = new T();
  process1 = table->FindProcess("Decay",G4PionMinus::PionMinus());
  manager = G4PionMinus::PionMinus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    manager->AddProcess(process2);
    manager->SetProcessOrdering(process2, idxPostStep);
    manager->SetProcessOrdering(process2, idxAtRest);
  } else {
    G4cerr << "Couldn't get PionMinux process manager ... to add/remove decays!\n";
  }
}


template<class T> void gm2ringsim::physicsList::muonDecay(){
  G4ProcessManager *manager;
  G4VProcess *process;

  // mu+  
  manager = G4MuonPlus::MuonPlus()->GetProcessManager();
  process = new T();

  if( manager ){
    manager->AddProcess(process);
    manager->SetProcessOrdering(process, idxPostStep);
    manager->SetProcessOrdering(process, idxAtRest);
  } else {
    G4cerr << "Couldn't get MuonPlus process manager ... to add decays!\n";
  }

  // mu-
  manager = G4MuonMinus::MuonMinus()->GetProcessManager();
  process = new T();

  if( manager ){
    manager->AddProcess(process);
    manager->SetProcessOrdering(process, idxPostStep);
    manager->SetProcessOrdering(process, idxAtRest);
  } else {
    G4cerr << "Couldn't get MuonPlus process manager ... to add decays!\n";
  }

}

void gm2ringsim::physicsList::unpolDecayChannel(){
  G4DecayTable *dt;

  // mu+
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannel("mu+", 1.) );
  G4MuonPlus::MuonPlus()->SetDecayTable(dt);

  // mu-
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannel("mu-", 1.) );
  G4MuonMinus::MuonMinus()->SetDecayTable(dt);
}

void gm2ringsim::physicsList::polDecayChannel(){
  G4DecayTable *dt;

  // mu+
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannelWithSpin("mu+", 0.986) );
  dt->Insert( new G4MuonRadiativeDecayChannelWithSpin("mu+", 0.014) );
  G4MuonPlus::MuonPlus()->SetDecayTable(dt);

  // mu-
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannelWithSpin("mu-", 0.986) );
  dt->Insert( new G4MuonRadiativeDecayChannelWithSpin("mu-", 0.014) );
  G4MuonMinus::MuonMinus()->SetDecayTable(dt);
}


void gm2ringsim::physicsList::enableStepLimiter(){
  theParticleIterator->reset();

  while ((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    G4double charge = particle->GetPDGCharge();
    
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
