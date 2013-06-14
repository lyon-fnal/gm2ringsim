// This file provides the implementation for an action object that records
// information on each particle track as it is created.

// Authors: Tasha Arvanitis, Adam Lyon
// Date: August 2012

// Edited: Peter Winter, Dec. 2012

#include "gm2ringsim/actions/track/TrackingAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"
#include "artg4/pluginActions/physicalVolumeStore/physicalVolumeStore_service.hh"

using std::string;

gm2ringsim::TrackingAction::TrackingAction(fhicl::ParameterSet const & p, 
					      art::ActivityRegistry &)
  : TrackingActionBase(p.get<string>("name","TrackingAction")),
    OnlyTrackPrimary_(p.get<bool>("OnlyTrackPrimary", false)),
    TrackPrimaryDecay_(p.get<bool>("TrackPrimaryDecay", true)),
    TrackOrphans_(p.get<bool>("TrackOrphans", true)),
    myArtHits_(new TrackingActionArtRecordCollection),
    myMuonCharge_(new int(2)),
    logInfo_("TrackingAction")
{
  G4cout << "================ TrackingAction ================" << G4endl;
  G4cout << "| OnlyTrackPrimary:  " << OnlyTrackPrimary_ << G4endl;
  G4cout << "| TrackPrimaryDecay: " << TrackPrimaryDecay_ << G4endl;
  G4cout << "| TrackOrphans:      " << TrackOrphans_ << G4endl;
  G4cout << "================================================" << G4endl;
}

// Destructor
gm2ringsim::TrackingAction::~TrackingAction()
{}




// Overload the PreUserTrackingAction method to initialize the track and
// add it to our collection
void gm2ringsim::TrackingAction::postUserTrackingAction(const G4Track * currentTrack)
{
  if ( currentTrack->GetTrackID() == 1 ) {   
    FillTrackingActionArtRecord(currentTrack, 1);
  }
}



// Overload the PreUserTrackingAction method to initialize the track and
// add it to our collection
void gm2ringsim::TrackingAction::preUserTrackingAction(const G4Track * currentTrack)
{
  FillTrackingActionArtRecord(currentTrack, 0);
}


// Fill ArtRecord
void gm2ringsim::TrackingAction::FillTrackingActionArtRecord(const G4Track * currentTrack, int status)
{
  bool debug = false;
  bool keep_track = true;

  if ( currentTrack->GetTrackID() == 1 ) {   
    PrimaryTrackCharge_ = (int)currentTrack->GetParticleDefinition()->GetPDGCharge(); 
    if ( debug ) { 
      G4cout << "Found trackID == 1 with Name = (" << currentTrack->GetDefinition()->GetParticleName() << ")" << G4endl;
      //G4cout << "Charge = " << currentTrack->GetDefinition()->GetPDGCharge() << G4endl;
    }
//     if ( currentTrack->GetDefinition()->GetPDGEncoding() == 13 ) { 
//       if ( debug ) {
// 	G4cout << "Found " << currentTrack->GetDefinition()->GetParticleName() << " (" << currentTrack->GetDefinition()->GetPDGEncoding() << ")" << G4endl;
//       }
//       *myMuonCharge_ = 1;
//     }
//     else {
//       *myMuonCharge_ = -1;
//       if ( debug ) {
// 	G4cout << "Found " << currentTrack->GetDefinition()->GetParticleName() << " (" << currentTrack->GetDefinition()->GetPDGEncoding() << ")" << G4endl;
//       }
//     }
  }


  // Ignore tracks for xtal volumes
  if( currentTrack->GetVolume()->GetName().find( "xtal" ) !=
      std::string::npos ) {
    keep_track = false;
    return;
  }

  if ( TrackPrimaryDecay_ ) {
    int parentid = currentTrack->GetParentID();
    if ( parentid == 1 ) {
      G4ParticleDefinition *def = currentTrack->GetDefinition();
      int id = def->GetPDGEncoding();
      std::string name = def->GetParticleName();
      //
      // Positive Primary Particle Charge
      //
      if ( PrimaryTrackCharge_ == 1 ) {
	// Found negatively charge lepton, but a positive primary particle
	if ( id == 11 || id == 13 ) { // e- or mu-
	  if ( debug ) { G4cout << "Found negatively charge lepton (" << name << "), but a positive primary particle." << G4endl; }
	  keep_track = false;
	}
	else if ( id == -11 || id == -13 ) { // e+ or mu+
	  if ( debug ) { G4cout << "Found positively charge lepton (" << name << "), and a positive primary particle." << G4endl; }
	}
      }
      if ( PrimaryTrackCharge_ == -1 ) {
	// Found positively charge lepton, but a negative primary particle
	if ( id == -11 || id == -13 ) { // e+ or mu+
	  if ( debug ) { G4cout << "Found positlvely charge lepton (" << name << "), but a negative primary particle." << G4endl; }
	  keep_track = false;
	}
	else if ( id == 11 || id == 13 ) { // e- or mu-
	  if ( debug ) { G4cout << "Found negatively charge lepton (" << name << "), and a negative primary particle." << G4endl; }
	}
      }
      // Keep all neutrinos
      if ( id == 12 || id == -12 || id == 14 || id == -14 ) {
	if ( debug ) { G4cout << "Found neutrino " << name << " w/ parent track = " << parentid << G4endl; }
      }
    }
  }


  if ( TrackOrphans_ == false ) {
    //G4cout << "TrackOrphans: " << currentTrack->GetTrackID() << " , " << currentTrack->GetParentID() << G4endl;
    if ( currentTrack->GetTrackID() != 1 ) { 
      if ( currentTrack->GetParentID() != 1 ) { 
	if ( debug ) { G4cout << "Not storing track [" << currentTrack->GetDefinition()->GetParticleName() << "] because it's not primary decay product." << G4endl; }
	keep_track = false;
      }
    }
  }

  
  if ( OnlyTrackPrimary_ ) {
    if ( currentTrack->GetTrackID() != 1 ) { 
      keep_track = false;
      if ( debug ) { G4cout << "Not storing track [" << currentTrack->GetDefinition()->GetParticleName() << "] because it's not primary." << G4endl; }
    }
  }


  //-------------------------------------
  // Don't store unwanted truth particles
  //-------------------------------------
  G4cout.precision(3);
  if ( debug && !keep_track ) { G4cout << "Not storing track [" << currentTrack->GetDefinition()->GetParticleName() << "] for some reason." << G4endl; }
  if ( keep_track == false ) { return; }
  

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
  art::ServiceHandle<artg4::PhysicalVolumeStoreService> pvs;
  tr.volumeUID = pvs->idGivenPhysicalVolume( currentTrack->GetVolume() );

  // The track status, 0 for pre and 1 for post
  tr.status = status;
  
  G4ThreeVector pos = currentTrack->GetPosition();
  G4ThreeVector mom = currentTrack->GetMomentum();
  G4ThreeVector pol = currentTrack->GetPolarization();

  //-------------------
  // Position variables
  //-------------------
  G4double rhat = ComputeRhat(&pos);
  G4double vhat = ComputeVhat(&pos);
  G4double theta = ComputeTheta(&pos);
  tr.rhat = rhat;
  tr.vhat = vhat;
  tr.theta = theta;
  tr.time = currentTrack->GetGlobalTime();


  //-------------------
  // Momentum variables
  //-------------------

  G4double prhat = ComputePrhat(&pos, &mom);
  G4double pvhat = ComputePvhat(&pos, &mom);
  tr.p = mom.mag();
  tr.prhat = prhat;
  tr.pvhat = pvhat;
  tr.e = currentTrack->GetTotalEnergy();


  if ( debug ) { G4cout << "Found track[" << currentTrack->GetParticleDefinition()->GetParticleName() << "] [t=" << currentTrack->GetGlobalTime() << "] [" << currentTrack->GetParentID() << "] : " << status << "\t" << keep_track << "\t" << prhat << "\t" << pvhat << G4endl; }


  //-----------------------
  // Polarization variables
  //-----------------------
  tr.polx = pol.x();
  tr.poly = pol.y();
  tr.polz = pol.z();
  //G4cout << tr.trackType << "\tPol[x,z] = " << pol.x() << "\t" << pol.z() << G4endl;
  
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

  // myArtHits should now be invalid and set to nullptr. But in fact
  // due to https://cdcvs.fnal.gov/redmine/issues/3601 this does not happen.
  // So need to do a release to avoid a segfault
  myArtHits_.release();
  
  // Point to a new valid collection
  myArtHits_.reset( new TrackingActionArtRecordCollection() );
}

using gm2ringsim::TrackingAction;
DEFINE_ART_SERVICE(TrackingAction)
