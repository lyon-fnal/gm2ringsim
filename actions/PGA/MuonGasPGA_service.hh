#ifndef MUONGASPGA_SERVICE_HH
#define MUONGASPGA_SERVICE_HH

/* 
  @author  Brendan Kiburg
  @date    March 2013
  @comment Separating the MuonGas Gun into it's own service in ART
*/

// Boilerplate for ART Service
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

// Get the base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"

// Get the Geant4 code
#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4RotationMatrix.hh"

// Get the helper files
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"
#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"
#include "gm2ringsim/actions/PGA/g2MuonGasSource.hh"

namespace gm2ringsim
{
  class MuonGasPGA : public artg4::PrimaryGeneratorActionBase
  {
  public:
    MuonGasPGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~MuonGasPGA();
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
   
  private:
//    fhicl::ParameterSet par_g2GPS_;
    
    // Replace these settings 
    G2GPSSettings settings_;
    g2MuonGasSource* gps_;
    bool muonGasVerbosity_;

  }; // End MuonGasPGA class 
}
#endif // MUONGASPGA_SERVICE_HH

