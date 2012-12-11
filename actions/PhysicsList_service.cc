// PhysicsListService

#include "gm2ringsim/actions/PhysicsList_service.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "gm2ringsim/actions/N02PhysicsList.hh"
#include "gm2ringsim/actions/physicsList.hh"
#include <memory>

gm2ringsim::PhysicsListService::PhysicsListService(fhicl::ParameterSet const &, art::ActivityRegistry &) :
  PhysicsListServiceBase( std::unique_ptr<G4VUserPhysicsList>(new 
      //physicsList))
							      N02PhysicsList) )
{
  //  physicsList_.ConstructLeptons();
}

using gm2ringsim::PhysicsListService;
DEFINE_ART_SERVICE(PhysicsListService)
