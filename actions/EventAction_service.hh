// Header for the Event action

#ifndef EVENTACTION_SERVICE_HH
#define EVENTACTION_SERVICE_HH

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

// Get the base class
#include "artg4/actionBase/EventActionBase.hh"

#include "Geant4/G4Event.hh"
namespace gm2ringsim
{
    class EventAction : public artg4::EventActionBase
    {
        public:
            EventAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
            virtual ~EventAction() {};

            void endOfEventAction( const G4Event* ) override;
        
    };   
}
#endif

