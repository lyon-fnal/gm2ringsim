// TrackActionService is the service that provides the track action. It 
// records information on each track as it is created, and can output this
// information in a collection
// To use this action, all you need to do is put it in the services section
// of the configuration file, like this:
// 
// services: { 
//   ...
//   user: {
//     TrackActionService: {}
//     ...
//   }
// }

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

// Include guard
#ifndef TRACKINGACTION_SERVICE_HH
#define TRACKINGACTION_SERVICE_HH

// Includes for general action service
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Framework/Principal/Event.h"

// Get the base class
#include "artg4/actionBase/TrackingActionBase.hh"

// Other includes
#include "gm2ringsim/actions/track/TrackingActionArtRecord.hh"
#include "Geant4/G4Track.hh"

namespace gm2ringsim {

  class TrackingAction : public artg4::TrackingActionBase {
  public: 
    TrackingAction(fhicl::ParameterSet const&, 
		       art::ActivityRegistry&);
    virtual ~TrackingAction();

    // Overload the PreUserTrackingAction method to initialize the track and
    // add it to our collection
    virtual void preUserTrackingAction(const G4Track * currentTrack);

    // We want to add something to the event, so we need callArtProduces
    // and fillEventWithArtStuff.

    // Tell Art what we'll be adding to the event
    virtual void callArtProduces(art::EDProducer * producer);

    // Actually add the collection to the Art event.
    virtual void fillEventWithArtStuff(art::Event & e);

  private:
    // Our collection of track hits
    std::unique_ptr<TrackingActionArtRecordCollection> myArtHits_;

    // A message logger for this action
    mf::LogInfo logInfo_;
  };
}

#endif
