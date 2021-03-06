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
#include "artg4/actionBase/RunActionBase.hh"
#include "artg4/actionBase/TrackingActionBase.hh"

// Other includes
#include "gm2ringsim/actions/track/TrackingActionArtRecord.hh"
#include "Geant4/G4Track.hh"
#include "Geant4/G4Run.hh"

namespace gm2ringsim {

  class TrackingAction : public artg4::TrackingActionBase,
			 public artg4::RunActionBase
  {
  public: 
    TrackingAction(fhicl::ParameterSet const&, 
		       art::ActivityRegistry&);
    virtual ~TrackingAction();
    
    /** Resets the muon storage counter and causes begin of run setup to
	occur in the rootStorageManager and elapsed time counters. */
    virtual void beginOfRunAction(const G4Run *currentRun) override;
    
    /** Calculates capture efficiency and elapsed time, and initiates
	rootStorageManager end of run activities. */
    virtual void endOfRunAction(const G4Run *currentRun) override; 

    // Overload the PreUserTrackingAction method to initialize the track and
    // add it to our collection
    virtual void preUserTrackingAction(const G4Track * currentTrack);

    // Overload the PostUserTrackingAction method to initialize the track and
    // add it to our collection
    virtual void postUserTrackingAction(const G4Track * currentTrack);

    // We want to add something to the event, so we need callArtProduces
    // and fillEventWithArtStuff.

    // Tell Art what we'll be adding to the event
    virtual void callArtProduces(art::EDProducer * producer);

    // Actually add the collection to the Art event.
    virtual void fillEventWithArtStuff(art::Event & e);
    
    // Fill ArtRecord
    void FillTrackingActionArtRecord(const G4Track * currentTrack, int status);

  private:
    
    bool OnlyTrackPrimary_;
    bool TrackPrimaryDecay_;
    bool TrackOrphans_;

    int Ndecays_;
    int Nlost_;
    int Nstore_;

    // Our collection of track hits
    std::unique_ptr<TrackingActionArtRecordCollection> myArtHits_;

    // A collection of the muon charge
    std::unique_ptr<int> myMuonCharge_;

    // A message logger for this action
    mf::LogInfo logInfo_;

    int PrimaryTrackCharge_;
  };
}
using gm2ringsim::TrackingAction;
DECLARE_ART_SERVICE(TrackingAction,LEGACY)
#endif
