
// Get the PGA header
#include "gm2ringsim/actions/StackingAction_service.hh"

// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Geant4 includes
#include "Geant4/G4Track.hh"
#include "Geant4/G4UnitsTable.hh"


gm2ringsim::StackingAction::StackingAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::StackingActionBase(p.get<std::string>("name")),
  OnlyTrackPrimary_(p.get<bool>("OnlyTrackPrimary", false)),  
  TrackPrimaryDecay_(p.get<bool>("TrackPrimaryDecay", true)),
  TrackOrphans_(p.get<bool>("TrackOrphans", true)),
  PrimaryTrackCharge_(0),
  minWavelength_(p.get<double>("minWavelength",250)), //nm
  maxWavelength_(p.get<double>("maxWavelength",950)) //nm
{}

gm2ringsim::StackingAction::~StackingAction(){
}

bool gm2ringsim::StackingAction::killNewTrack( const G4Track* aTrack) {
  bool debug = false;
    
  if ( aTrack->GetTrackID() == 1 ) {
    G4ParticleDefinition *def = aTrack->GetDefinition();
    //int id = def->GetPDGEncoding();
    std::string name = def->GetParticleName();
    if ( debug ) { G4cout << "Found primary " << name << G4endl; }
    PrimaryTrackCharge_ = (int)def->GetPDGCharge();
    return( false );
  }

  // pass all non-optical tracks as normal: urgent stacking
  int pdg = aTrack->GetDefinition()->GetPDGEncoding();
  
  if ( debug ) { G4cout << "Particle: " << pdg << "\t"; }

  if ( TrackPrimaryDecay_ ) {
    int parentid = aTrack->GetParentID();
    if ( parentid == 1 ) {
      G4ParticleDefinition *def = aTrack->GetDefinition();
      int id = def->GetPDGEncoding();
      std::string name = def->GetParticleName();
      //
      // Positive Primary Particle Charge
      //
      if ( PrimaryTrackCharge_ == 1 ) {
	// Found negatively charge lepton, but a positive primary particle
	if ( id == 11 || id == 13 ) { 
	  if ( debug ) { G4cout << "Found negatively charge lepton (" << name << "), but a positive primary particle." << G4endl; }
	  return( true );
	}
	else if ( id == -11 || id == -13 ) {
	  if ( debug ) { G4cout << "Found positively charge lepton (" << name << "), and a positive primary particle." << G4endl; }
	  return( false );
	}
      }
      if ( PrimaryTrackCharge_ == -1 ) {
	// Found positively charge lepton, but a negative primary particle
	if ( id == -11 || id == -13 ) { 
	  if ( debug ) { G4cout << "Found positlvely charge lepton (" << name << "), but a negative primary particle." << G4endl; }
	  return( true);
	}
	else if ( id == 11 || id == 13 ) {
	  if ( debug ) { G4cout << "Found negatively charge lepton (" << name << "), and a negative primary particle." << G4endl; }
	  return( false );
	}
      }
      // Keep all neutrinos
      if ( id == 12 || id == -12 || id == 14 || id == -14 ) {
	if ( debug ) { G4cout << "Found neutrino " << name << " w/ parent track = " << parentid << G4endl; }
	return( false );
      }
    }
  }



  if ( TrackOrphans_ == false ) {
    if ( aTrack->GetTrackID() != 1 ) { 
      if ( aTrack->GetParentID() != 1 ) { 
	if ( debug ) { G4cout << "Not storing track [" << aTrack->GetDefinition()->GetParticleName() << "] because it's not primary decay product." << G4endl; }
	return( true );
      }
    }
  }


  if ( OnlyTrackPrimary_ ) {
    if ( aTrack->GetTrackID() != 1 ) { 
      if ( debug ) { G4cout << "Killing it because it's not primary." << G4endl; }
      return( true );
    }
  }
  
  if ( pdg == 0 ) { 
    // Kill optical photons that are outside the range of interest
    double wavelength = 0.001240 / ( aTrack->GetTotalEnergy() / MeV ) ;
    if ( wavelength < minWavelength_ || wavelength > maxWavelength_ ) {
      if ( debug ) { G4cout << "Killing it because wavelengh is out of range." << G4endl; }
      return true;
    } else {
      if ( debug ) { G4cout << "Good." << G4endl; }
      return false;
    }
  }

  if ( debug ) { G4cout << "Good." << G4endl; }

  return false;
}

/** Interface to set accepted wavelength range **/
void gm2ringsim::StackingAction::setMinimumOpticalWavelength(G4double minWavelength ) {
  minWavelength_ = minWavelength;

}
void gm2ringsim::StackingAction::setMaximumOpticalWavelength(G4double maxWavelength ) {
  maxWavelength_ = maxWavelength;
}



using gm2ringsim::StackingAction;
DEFINE_ART_SERVICE(StackingAction)
