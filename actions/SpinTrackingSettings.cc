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
  gm2_(p.get<double>("Gm2", -1.0)),
  storage_field_name_(p.get<std::string>("StorageFieldType", "Simple")),
  step_size_(p.get<double>("StepSize", 0.01*mm)),
  stepper_type_name_(p.get<std::string>("StepperType", "ClassicalRK4"))
{
  //double gm2_factor = 0.0;
  if ( eta_ <= 0.0 ) { eta_ = 0.0; }
  if ( gm2_ < 0.0 ) { gm2_ = 116592080e-11; }
  //gm2_ *= gm2_factor;
  if ( spinTrackingEnabled && edmTrackingEnabled ) { edmTrackingEnabled = false; }
  if ( edmTrackingEnabled == false ) { eta_ = 0.0; }

  if ( storage_field_name_ == "Simple" ) { storage_field_type_ = kSimpleField; }
  else if ( storage_field_name_ == "Mapped" ) { storage_field_type_ = kMappedField; }
  else {
    G4cout << "Storage Field Type [" << storage_field_name_ << "] is unknown." << G4endl;
    G4cout << "Assuming Pure Dipole." << G4endl;
    storage_field_type_ = kSimpleField;
  }
  
  //G4MagneticField * pMagField;  // Your field - Defined elsewhere
  //G4double                 distanceConst = 2.5 * millimeter; 
  //G4MagneticField * pCachedMagField= new G4CachedMagneticField(  pMagField,  distanceConst); 

  if ( stepper_type_name_ == "ClassicalRK4" ) { stepper_type_ = kClassicalRK4; } // 4th order
  else if ( stepper_type_name_ == "CashKarpRKF45" ) { stepper_type_ = kCashKarpRKF45; }
  else if ( stepper_type_name_ == "NystromRK4" ) { stepper_type_ = kNystromRK4; } // 4th order
  else if ( stepper_type_name_ == "ConstRK4" ) { stepper_type_ = kConstRK4; } // 4th order
  else if ( stepper_type_name_ == "ExplicitEuler" ) { stepper_type_ = kExplicitEuler; } // 1st order
  else if ( stepper_type_name_ == "ImplicitEuler" ) { stepper_type_ = kHelixImplicitEuler; } // 2nd order
  else if ( stepper_type_name_ == "SimpleHeum" ) { stepper_type_ = kSimpleHeum; } // 3rd order
  else if ( stepper_type_name_ == "SimpleRunge" ) { stepper_type_ = kSimpleRunge; } // 2nd order

  else if ( stepper_type_name_ == "HelixExplicitEuler" ) { stepper_type_ = kExplicitEuler; } // 1st order
  else if ( stepper_type_name_ == "HelixHeum" ) { stepper_type_ = kHelixHeum; } // 3rd order
  else if ( stepper_type_name_ == "HelixImplicitEuler" ) { stepper_type_ = kHelixImplicitEuler; } // 2nd order
  else if ( stepper_type_name_ == "HelixSimpleRunge" ) { stepper_type_ = kHelixSimpleRunge; }
  else {
    G4cout << "Stepper Type [" << stepper_type_name_ << "] is unknown." << G4endl;
    G4cout << "Assuming ClassicalRK4." << G4endl;
    stepper_type_ = kClassicalRK4;
  }
  
  G4cout << "=========== SpinTracking ===========" << G4endl;
  G4cout << "| Muon Charge        = " << charge_ << G4endl;
  G4cout << "| Spin Tracking      = " << spinTrackingEnabled << G4endl;
  G4cout << "| EDM Tracking       = " << edmTrackingEnabled << G4endl;
  G4cout << "| g-2                = " << gm2_ << G4endl;
  G4cout << "| eta                = " << eta_ << G4endl;
  G4cout << "| Storage Field      = " << storage_field_name_ << G4endl;
  G4cout << "| Step Size          = " << step_size_ << G4endl;
  G4cout << "| Stepper Type       = " << stepper_type_name_ << G4endl;
  G4cout << "====================================" << G4endl;


}

void gm2ringsim::SpinTrackingSettings::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("SpinTrackingSettings") << "SpinTracking settings are:"
    				      << "spinTrackingEnabled=" << spinTrackingEnabled;
  mf::LogInfo("SpinTrackingSettings") << "SpinTracking settings are:"
    				      << "edmTrackingEnabled=" << edmTrackingEnabled;

    
}

