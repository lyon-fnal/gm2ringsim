#ifndef INFLECTORPGA_SERVICE_HH
#define INFLECTORPGA_SERVICE_HH

/* 
  @author  Brendan Kiburg
  @date    March 2013
  @comment Separating the Inflector Gun into it's own service in ART
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
#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
#include "gm2ringsim/actions/PGA/G2InflectorSource.hh"

namespace gm2ringsim
{
  class InflectorPGA : public artg4::PrimaryGeneratorActionBase
  {
  public:
    InflectorPGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~InflectorPGA();
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
   
  private:
    //    fhicl::ParameterSet par_g2GPS_;
    
    // Replace these settings 
    //G2GPSSettings settings_;
    G2InflectorSource* gps_;
    bool inflectorVerbosity_;
    bool StartUpstream_;
    bool StartUpstreamCryo_;
    bool StartDownstream_;
    bool StartPerfect_;
    bool GenGaussian_;
    double LaunchAngle_;
    double StorageOffset_;
    double EmittanceX_;
    double EmittanceY_;
    double BetaX_;
    double BetaY_;
    double AlphaX_;
    double AlphaY_;
    double Pmean_;
    double dPOverP_;
    double SigmaT_;
    std::string Particle_;
    int NumParticles_;
    int DecayScaleFactor_;
    std::string Polarization_;

   

  }; // End InflectorPGA class 
}
#endif // INFLECTORPGA_SERVICE_HH

