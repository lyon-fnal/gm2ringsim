// PhysicsListService

#include "gm2ringsim/actions/Gm2PhysicsList_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2ringsim/actions/physicsList.hh"

#include <boost/algorithm/string.hpp>

gm2ringsim::Gm2PhysicsListService::Gm2PhysicsListService(fhicl::ParameterSet const & p, art::ActivityRegistry &) :
  PhysicsListServiceBase(),
  muonDecayMode_(p.get<std::string>("muonDecayMode", "")),
  verboseLevel_(p.get<int>("verboseLevel", 0)),
  thePhysicsList_(0)
{}

G4VUserPhysicsList* gm2ringsim::Gm2PhysicsListService::makePhysicsList() {
  
  // Construct a new Physics List
  thePhysicsList_ = new physicsList();
  
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
    thePhysicsList_->disableDecay();
  }
  
  // Isotropic decay?
  else if (muonDecayMode_ == "iso" ) {
    thePhysicsList_->enableIsotropicDecay();
  }
  
  // Standard decay
  else if ( muonDecayMode_ == "sm" || muonDecayMode_ == "standard") {
    thePhysicsList_->enableSMDecay();
  }
  
}

using gm2ringsim::Gm2PhysicsListService;
DEFINE_ART_SERVICE(Gm2PhysicsListService)
