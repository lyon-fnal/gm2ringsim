// PhysicsListService

#include "gm2ringsim/actions/Gm2PhysicsList_service.hh"
#include "gm2ringsim/actions/physicsList.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include <boost/algorithm/string.hpp>

#include "Geant4/G4PhysListFactory.hh"

#include "Geant4/G4ProcessTable.hh"
#include "Geant4/G4ParticleTable.hh"
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
#include "Geant4/G4PhaseSpaceDecayChannel.hh"

#include "Geant4/G4HadronDElasticPhysics.hh"
#include "Geant4/G4QStoppingPhysics.hh"
#include "Geant4/G4IonBinaryCascadePhysics.hh"
#include "Geant4/G4NeutronTrackingCut.hh"

#include "Geant4/G4StepLimiterBuilder.hh"

#include "Geant4/G4Cerenkov.hh"
#include "Geant4/G4Scintillation.hh"
#include "Geant4/G4OpAbsorption.hh"
#include "Geant4/G4OpRayleigh.hh"
//#include "Geant4/G4OpMieHG.hh"
#include "Geant4/G4OpBoundaryProcess.hh"
#include "Geant4/G4LossTableManager.hh"
#include "Geant4/G4EmSaturation.hh"

#include <iostream>
using namespace std;

gm2ringsim::Gm2PhysicsListService::Gm2PhysicsListService(fhicl::ParameterSet const & p, art::ActivityRegistry &) :
  PhysicsListServiceBase(),
  DecayMode_(p.get<std::string>("DecayMode", "sm")),
  physicsListName_(G4String(p.get<std::string>("physicsListName", ""))),
  verboseLevel_(p.get<int>("verboseLevel", 0)),
  thePhysicsList_(0),
  theParticleIterator_(G4ParticleTable::GetParticleTable()->GetIterator()),
  decayStatus_(decay_init),
  theCerenkovProcess(0),
  theScintillationProcess(0),
  theAbsorptionProcess(0),
  theRayleighScatteringProcess(0),
  theMieHGScatteringProcess(0),
  theBoundaryProcess(0)
{}

G4VUserPhysicsList* gm2ringsim::Gm2PhysicsListService::makePhysicsList() {
  
  // Construct a new Physics List
  G4PhysListFactory factory;
  G4VModularPhysicsList *mPL{0};
  if(!strcmp(physicsListName_.c_str(), "") || !strcmp(physicsListName_.c_str(), "old")){
    mPL = new physicsList;
  }
  else{
    mPL = factory.GetReferencePhysList(physicsListName_);
  }
  
  if(mPL){
    thePhysicsList_ = mPL;
  }
  else{
    throw cet::exception("Gm2PhysicsList_service") << "G4PhysListFactory did not find requested list " << physicsListName_ << ".\n";
  }

  // This used to be done in enableStepLimiter() in g2migtrace but has been implemented in the G4StepLimiterBuilder() for 
  // GEANT4 provided physics lists
  thePhysicsList_->RegisterPhysics(new G4StepLimiterBuilder());

  return thePhysicsList_;
}

void gm2ringsim::Gm2PhysicsListService::initializePhysicsList() {
  this->ConstructAdditionalProcess();

  // We'd better have a physics list by this point
  if ( ! thePhysicsList_ ) {
    throw cet::exception("Gm2PhysicsList_service") << "Trying to initialize a physics list before it has been created.\n";
  }
  
  // Set the verbose level
  if ( verboseLevel_ > 0 ) {
    this->verboseLevel( verboseLevel_ );
  }
  
  // React to decay mode
  
  // If "none" or "disable" then turn off decay
  if (DecayMode_ == "none" || DecayMode_ == "disable") {
    this->disableDecay();
  }
  
  // Isotropic decay?
  else if (DecayMode_ == "iso" ) {
    this->enableIsotropicDecay();
  }
  
  // Standard decay
  else if ( DecayMode_ == "sm" || DecayMode_ == "standard") {
    this->enableSMDecay();
  }
  
}

void gm2ringsim::Gm2PhysicsListService::ConstructAdditionalProcess(){
  disableDecay();
  this->pionDecay<G4Decay>();

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

  theParticleIterator_->reset();
  while( (*theParticleIterator_)() ){
     G4ParticleDefinition* particle = theParticleIterator_->value();
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

G4String gm2ringsim::Gm2PhysicsListService::currentDecay(){
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

void gm2ringsim::Gm2PhysicsListService::disableDecay(){
  disablePionDecay();
  disableMuonDecay();
}

void gm2ringsim::Gm2PhysicsListService::disableMuonDecay(){
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

void gm2ringsim::Gm2PhysicsListService::disablePionDecay(){

  G4ProcessTable* table = G4ProcessTable::GetProcessTable();
  G4ProcessManager *manager;
  G4VProcess *process1, *process2;

  // for pi+
  process1 = table->FindProcess("Decay",G4PionPlus::PionPlus());
  process2 = table->FindProcess("DecayWithSpin",G4PionPlus::PionPlus());
  manager = G4PionPlus::PionPlus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process2);
  } else {
    cout << "Couldn't get PionPlus process manager ... to remove decays!\n";
  }

  // for pi-
  process1 = table->FindProcess("Decay",G4PionMinus::PionMinus());
  process2 = table->FindProcess("DecayWithSpin",G4PionMinus::PionMinus());
  manager = G4PionMinus::PionMinus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process2);
  } else {
    cout << "Couldn't get PionMinus process manager ... to remove decays!\n";
  }
}

void gm2ringsim::Gm2PhysicsListService::enableIsotropicDecay(){
  if( decayStatus_ == decay_isotropic )
    return;

  disableDecay();

  this->muonDecay<G4Decay>();

  this->pionDecay<G4Decay>();

  unpolDecayChannel();

  decayStatus_ = decay_isotropic;
}

void gm2ringsim::Gm2PhysicsListService::enableSMDecay(){
  if( decayStatus_ == decay_standard )
    return;

  disableDecay();

  this->muonDecay<G4DecayWithSpin>();
  
  polDecayChannel();

  this->pionDecay<G4PionDecayMakeSpin>("DecayWithSpin");

  decayStatus_ = decay_standard;
}



/// PRIVATES

template<class T> void gm2ringsim::Gm2PhysicsListService::pionDecay(const char *name){
  G4ProcessTable* table = G4ProcessTable::GetProcessTable();
  G4ProcessManager *manager;
  G4VProcess *process1, *process2, *process3;

  // pi+
  if(strcmp(name, "")==0) process3 = new T();
  else process3 = new T(G4String(name));
  process1 = table->FindProcess("Decay",G4PionPlus::PionPlus());
  process2 = table->FindProcess("DecayWithSpin",G4PionPlus::PionPlus());
  manager = G4PionPlus::PionPlus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process3);
    manager->AddProcess(process3);
    manager->SetProcessOrdering(process3, idxPostStep);
    manager->SetProcessOrdering(process3, idxAtRest);
  }else {
    G4cerr << "Couldn't get PionPlus process manager ... to add/remove decays!\n";
  }

  // pi-
  if(strcmp(name, "")==0) process3 = new T();
  else process3 = new T(G4String(name));
  process1 = table->FindProcess("Decay",G4PionMinus::PionMinus());
  process2 = table->FindProcess("DecayWithSpin",G4PionMinus::PionMinus());
  manager = G4PionMinus::PionMinus()->GetProcessManager();

  if( manager ){
    if( process1 )
      manager->RemoveProcess(process1);
    if( process2 )
      manager->RemoveProcess(process3);
    manager->AddProcess(process3);
    manager->SetProcessOrdering(process3, idxPostStep);
    manager->SetProcessOrdering(process3, idxAtRest);
  } else {
    G4cerr << "Couldn't get PionMinux process manager ... to add/remove decays!\n";
  }
}


template<class T> void gm2ringsim::Gm2PhysicsListService::muonDecay(){
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

void gm2ringsim::Gm2PhysicsListService::unpolDecayChannel(){
  G4DecayTable *dt;

  // mu+
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannel("mu+", 1.) );
  G4MuonPlus::MuonPlus()->SetDecayTable(dt);

  // mu-
  dt = new G4DecayTable;
  dt->Insert( new G4MuonDecayChannel("mu-", 1.) );
  G4MuonMinus::MuonMinus()->SetDecayTable(dt);

  dt = new G4DecayTable;
  dt->Insert(new G4PhaseSpaceDecayChannel("pi+",0.999877,2,"mu+","nu_mu"));
  dt->Insert(new G4PhaseSpaceDecayChannel("pi+",0.000123,2,"e+","nu_e"));
  G4PionPlus::PionPlus()->SetDecayTable(dt);	     

  dt = new G4DecayTable;
  dt->Insert(new G4PhaseSpaceDecayChannel("pi-",0.999877,2,"mu-","anti_nu_mu"));
  dt->Insert(new G4PhaseSpaceDecayChannel("pi-",0.000123,2,"e-","anti_nu_e"));
  G4PionMinus::PionMinus()->SetDecayTable(dt);	     
}

void gm2ringsim::Gm2PhysicsListService::polDecayChannel(){
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

using gm2ringsim::Gm2PhysicsListService;
DEFINE_ART_SERVICE(Gm2PhysicsListService)
