// Settings for SpinTracking

#include "gm2ringsim/actions/SpinTrackingSettings.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>

gm2ringsim::SpinTrackingSettings::SpinTrackingSettings(std::string const & detName) :
  RunSettingsBase(detName),
  spinTrackingEnabled(p.get<bool>("spinTrackingEnabled"))

{}

void gm2ringsim::SpinTrackingSettings::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("SpinTrackingSettings") << "SpinTracking settings are:"
				      << "spinTrackingEnabled=" << spinTrackingEnabled;
}

