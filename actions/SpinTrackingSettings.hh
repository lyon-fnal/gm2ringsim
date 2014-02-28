// SpinTrackingSettings.hh
// @author Brendan Kiburg 
// @date Nov 2012

#ifndef SPINTRACKING_SETTINGS_HH
#define SPINTRACKING_SETTINGS_HH

#include "artg4/Core/RunSettingsBase.hh"

#include <iostream>

namespace gm2ringsim {

  enum { kSimpleField, kMappedField };
  enum { kNystromRK4, kCashKarpRKF45, kClassicalRK4, kConstRK4, kExplicitEuler, kImplicitEuler, kSimpleHeum, kSimpleRunge, kHelixExplicitEuler, kHelixHeum, kHelixImplicitEuler, kHelixSimpleRunge };

  // NystromRK4, kCashKarpRKF45, kClassicalRK4, ConstRK4, kExplicitEuler, kImplicitEuler, kSimpleHeum, kSimpleRunge :: These are good for EM fields
  // kHelixExplicitEuler, kHelixHeum, kHelixImplicitEuler, kHelixSimpleRunge :: These are good for B fields
    
  struct SpinTrackingSettings : public artg4::RunSettingsBase {
    SpinTrackingSettings(std::string const &);
    void print() const;
    inline double GetEta() { return( eta_ ); }
    inline double GetGm2() { return( gm2_ ); }
    inline int GetCharge() { return( charge_ ); }
    inline int GetStorageFieldType() { return( storage_field_type_ ); }
    inline double GetStepSize() { return( step_size_ ); }
    inline int GetStepperType() { return( stepper_type_ ); }
    inline std::string GetStepperName() { return( stepper_type_name_ ); }

    bool spinTrackingEnabled;
    bool edmTrackingEnabled;

    int charge_;
    double eta_;
    double gm2_;
    
    int storage_field_type_;
    std::string storage_field_name_;

    double step_size_;
    std::string stepper_type_name_;
    int stepper_type_;
    
  };
}

#endif
