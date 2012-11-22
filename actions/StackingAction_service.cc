
// Get the PGA header
#include "gm2ringsim/actions/StackingAction_service.hh"

// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


gm2ringsim::StackingAction::StackingAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
    artg4::StackingActionBase(p.get<std::string>("name"))
{}

gm2ringsim::StackingAction::~StackingAction(){
}

bool gm2ringsim::StackingAction::killNewTrack( const G4Track* ) { return false; }

using gm2ringsim::StackingAction;
DEFINE_ART_SERVICE(StackingAction)
