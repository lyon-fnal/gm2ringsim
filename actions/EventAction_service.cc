// Implementation of EventActionService

#include "gm2ringsim/actions/EventAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/globals.hh"

// Constructor
gm2ringsim::EventAction::EventAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
    artg4::EventActionBase(p.get<std::string>("name"))
{}

// EndOfEventAction
void gm2ringsim::EventAction::endOfEventAction(const G4Event* ){//evt) {

}   

using gm2ringsim::EventAction;
DEFINE_ART_SERVICE(EventAction)
