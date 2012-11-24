
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
  minWavelength_(p.get<double>("minWavelength",250)), //nm
  maxWavelength_(p.get<double>("maxWavelength",950)) //nm
{}

gm2ringsim::StackingAction::~StackingAction(){
}

bool gm2ringsim::StackingAction::killNewTrack( const G4Track* ) { 
  return false; 
}

G4ClassificationOfNewTrack gm2ringsim::StackingAction::ClassifyNewTrack(const G4Track* aTrack){
  // pass all non-optical tracks as normal: urgent stacking
  int pdg = aTrack->GetDefinition()->GetPDGEncoding();
  
  // Use only for muon tracker (TG)
  //if ( pdg == 13 || pdg == -13 ) { return( fUrgent ); }
  //else { return( fKill ); }
  
  if ( pdg != 0 ) { return fUrgent; }

  // Kill optical photons that are outside the range of interest
  double wavelength = 0.001240 / ( aTrack->GetTotalEnergy() / MeV ) ;
  if ( wavelength < minWavelength_ || wavelength > maxWavelength_ ) {
    return fKill;
  } else {
    return fUrgent;
  }

  return fUrgent;

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
