// SpinTrackingSettings.hh
// @author Brendan Kiburg 
// @date Nov 2012

#ifndef SPINTRACKING_SETTINGS_HH
#define SPINTRACKING_SETTINGS_HH

#include "artg4/Core/RunSettingsBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct SpinTrackingSettings : public artg4::RunSettingsBase {
    SpinTrackingSettings(std::string const &);
    void print() const;

    bool spinTrackingEnabled;
    
  };
}

#endif