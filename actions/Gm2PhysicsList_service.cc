// PhysicsListService

#include "gm2ringsim/actions/Gm2PhysicsList_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2ringsim/actions/physicsList.hh"
#include "gm2ringsim/actions/Gm2ModularPhysicsList.hh"

#include <boost/algorithm/string.hpp>

#include "Geant4/G4PhysListFactory.hh"

// these are only needed for testing
#include "Geant4/G4MuonPlus.hh"
#include "Geant4/G4PionPlus.hh"
#include "Geant4/G4ProcessManager.hh"


gm2ringsim::Gm2PhysicsListService::Gm2PhysicsListService(fhicl::ParameterSet const & p, art::ActivityRegistry &) :
  PhysicsListServiceBase(),
  muonDecayMode_(p.get<std::string>("muonDecayMode", "")),
  pionDecayEnabled_(p.get<bool>("pionDecayEnabled", true)),
  physicsListName_(G4String(p.get<std::string>("physicsListName", "FTFP_BERT"))),
  verboseLevel_(p.get<int>("verboseLevel", 0)),
  thePhysicsList_(0)
{}

G4VUserPhysicsList* gm2ringsim::Gm2PhysicsListService::makePhysicsList() {
  
  // Construct a new Physics List
  //thePhysicsList_ = new physicsList();
  
  G4PhysListFactory factory;
  G4VModularPhysicsList *mPL = factory.GetReferencePhysList(physicsListName_);

  if(mPL){
    thePhysicsList_ = new Gm2ModularPhysicsList(mPL);
  }
  else{
    throw cet::exception("Gm2PhysicsList_service") << "G4PhysListFactory did not find requested list " << physicsListName_ << ".\n";
  }

  return thePhysicsList_;
}

void gm2ringsim::Gm2PhysicsListService::initializePhysicsList() {
  
  // We'd better have a physics list by this point
  if ( ! thePhysicsList_ ) {
    throw cet::exception("Gm2PhysicsList_service") << "Trying to initialize a physics list before it has been created.\n";
  }
  
  // Set the verbose level
  if ( verboseLevel_ > 0 ) {
    thePhysicsList_->verboseLevel( verboseLevel_ );
  }
  
  // React to muon decay mode
  
  // If "none" or "disable" then turn off decay
  if (muonDecayMode_ == "none" || muonDecayMode_ == "disable") {
    thePhysicsList_->disableMuonDecay();
  }
  
  // Isotropic decay?
  else if (muonDecayMode_ == "iso" ) {
    thePhysicsList_->enableIsotropicDecay();
  }
  
  // Standard decay
  else if ( muonDecayMode_ == "sm" || muonDecayMode_ == "standard") {
    thePhysicsList_->enableSMDecay();
  }
  
  if(!pionDecayEnabled_) thePhysicsList_->disablePionDecay();

  printf("========= At the end of initializePhyscsList()\n");
  printf("========= mu+:\n");  
  G4MuonPlus::MuonPlus()->GetProcessManager()->DumpInfo();

  printf("\n\n========= pi+:\n");  
  G4PionPlus::PionPlus()->GetProcessManager()->DumpInfo();
}

using gm2ringsim::Gm2PhysicsListService;
DEFINE_ART_SERVICE(Gm2PhysicsListService)
