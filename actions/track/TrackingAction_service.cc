// This file provides the implementation for an action object that records
// information on each particle track as it is created.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

// Edited: Peter Winter, Dec. 2012

#include "gm2ringsim/actions/track/TrackingAction_service.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

using std::string;

gm2ringsim::TrackingAction::TrackingAction(fhicl::ParameterSet const & p, 
					      art::ActivityRegistry &)
  : TrackingActionBase(p.get<string>("name","TrackingAction")),
    myArtHits_(new TrackingActionArtRecordCollection),
    logInfo_("TrackingAction")
{ }

// Destructor
gm2ringsim::TrackingAction::~TrackingAction()
{}


// Overload the PreUserTrackingAction method to initialize the track and
// add it to our collection
void gm2ringsim::TrackingAction::
preUserTrackingAction(const G4Track * currentTrack)
{
  // Ignore tracks for xtal volumes
  if( currentTrack->GetVolume()->GetName().find( "xtal" ) !=
      std::string::npos ) {
    return ;
  }
  
  // Create a hit
  TrackingActionArtRecord tr;
  
  // Fill it based on the track.
  tr.trackType = std::string(currentTrack->GetDefinition()->GetParticleName());
  tr.trackID = currentTrack->GetTrackID();
  tr.parentTrackID = currentTrack->GetParentID();

  //TODO
  // Omit turns for now
  //  tr.turn = turnCounter::getInstance().turns();

  // Get the volume ID
  art::ServiceHandle<artg4::PhysicalVolumeStore> pvs;
  tr.volumeUID = pvs->uidForPhysicalVolume( currentTrack->GetVolume() );
  
  G4ThreeVector pos = currentTrack->GetPosition();
  tr.rhat = std::sqrt(pos.x()*pos.x() + pos.z()*pos.z()) - R_magic();
  tr.vhat = pos.y();
  tr.theta = std::atan2(pos.z(),pos.x());
  if( tr.theta < 0 )
    tr.theta+= 2.*M_PI;
  tr.time = currentTrack->GetGlobalTime();

  G4ThreeVector mom = currentTrack->GetMomentum();
  tr.p = mom.mag();
  tr.prhat = 
    (mom.x()*pos.x() + mom.z()*pos.z()) /
    sqrt(pos.x()*pos.x()+pos.z()*pos.z()) / 
    tr.p;
  tr.pvhat = mom.y()/mom.mag();

  // Add the hit to our collection
  myArtHits_->push_back(tr);
}

// Tell Art what we produce
void gm2ringsim::TrackingAction::callArtProduces(art::EDProducer * producer)
{
  producer -> produces<TrackingActionArtRecordCollection>();
}

// Add our collection to the Art event.
// ???
// I believe that when we do this, we lose possession of the collection, which
// is a distinctly good thing.
// ???
void gm2ringsim::TrackingAction::fillEventWithArtStuff(art::Event & e)
{

  // Make a unique pointer for the tracking object

  e.put(std::move(myArtHits_));
}

using gm2ringsim::TrackingAction;
DEFINE_ART_SERVICE(TrackingAction)
