// Settings for SpinTracking

#include "gm2ringsim/actions/SpinTrackingSettings.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>

gm2ringsim::SpinTrackingSettings::SpinTrackingSettings(std::string const & detName) :
  RunSettingsBase(detName),
  spinTrackingEnabled(p.get<bool>("spinTrackingEnabled")),
  edmTrackingEnabled(p.get<bool>("edmTrackingEnabled", false)),
  charge_(p.get<int>("Q", 1)),
  eta_(p.get<double>("Eta", -1.0)),
  gm2_(p.get<double>("Gm2", -1.0))
{
  //double gm2_factor = 0.0;
  if ( eta_ <= 0.0 ) { eta_ = 0.0; }
  if ( gm2_ < 0.0 ) { gm2_ = 116592080e-11; }
  //gm2_ *= gm2_factor;
  if ( spinTrackingEnabled && edmTrackingEnabled ) { edmTrackingEnabled = false; }
  if ( edmTrackingEnabled == false ) { eta_ = 0.0; }


  G4cout << "=========== SpinTracking ===========" << G4endl;
  G4cout << "| Muon Charge        = " << charge_ << G4endl;
  G4cout << "| Spin Tracking      = " << spinTrackingEnabled << G4endl;
  G4cout << "| EDM Tracking       = " << edmTrackingEnabled << G4endl;
  G4cout << "| g-2                = " << gm2_ << G4endl;
  G4cout << "| eta                = " << eta_ << G4endl;
  G4cout << "====================================" << G4endl;


}

void gm2ringsim::SpinTrackingSettings::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("SpinTrackingSettings") << "SpinTracking settings are:"
    				      << "spinTrackingEnabled=" << spinTrackingEnabled;
  mf::LogInfo("SpinTrackingSettings") << "SpinTracking settings are:"
    				      << "edmTrackingEnabled=" << edmTrackingEnabled;

    
}

