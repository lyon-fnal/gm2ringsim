// PhysicsListService

#include "gm2ringsim/actions/Gm2PhysicsList_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2ringsim/actions/N02PhysicsList.hh"
#include "gm2ringsim/actions/physicsList.hh"

#include <boost/algorithm/string.hpp>

gm2ringsim::Gm2PhysicsListService::Gm2PhysicsListService(fhicl::ParameterSet const & p, art::ActivityRegistry &) :
  PhysicsListServiceBase(),
  muonDecayMode_(p.get<std::string>("muonDecayMode", "")),
  verboseLevel_(p.get<int>("verboseLevel", 0))
{}

G4VUserPhysicsList* gm2ringsim::Gm2PhysicsListService::makePhysicsList() const {
  
  // Construct a new Physics List
  physicsList* pl = new physicsList();
  
  // Set the verbose level
  if ( verboseLevel_ > 0 ) {
    pl->verboseLevel( verboseLevel_ );
  }
  
  // React to muon decay mode
  
  // If "none" or "disable" then turn off decay
  if (muonDecayMode_ == "none" || muonDecayMode_ == "disable") {
    pl->disableDecay();
  }
  
  // Isotropic decay?
  else if (muonDecayMode_ == "iso" ) {
    pl->enableIsotropicDecay();
  }
  
  // Standard decay
  else if ( muonDecayMode_ == "sm" || muonDecayMode_ == "standard") {
    pl->enableSMDecay();
  }
  
  // Turn on lepton construction
  //pl->ConstructLeptons();  ???
  
  return pl;
}

using gm2ringsim::Gm2PhysicsListService;
DEFINE_ART_SERVICE(Gm2PhysicsListService)
