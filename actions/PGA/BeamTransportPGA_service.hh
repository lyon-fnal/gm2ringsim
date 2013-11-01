#ifndef BEAMTRANSPORTPGA_SERVICE_HH
#define BEAMTRANSPORTPGA_SERVICE_HH

/* 
  @author  Brendan Kiburg
  @date    March 2013
  @comment Separating the BeamTransport Gun into it's own service in ART
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
#include "gm2ringsim/actions/PGA/G2GPSSettings.hh"
#include "gm2ringsim/actions/PGA/G2BeamTransportSource.hh"

namespace gm2ringsim
{
  class BeamTransportPGA : public artg4::PrimaryGeneratorActionBase
  {
  public:
    BeamTransportPGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~BeamTransportPGA();
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
   
  private:
//    fhicl::ParameterSet par_g2GPS_;
    
    // Replace these settings 
    G2GPSSettings settings_;
    G2BeamTransportSource* gps_;
    double EmittanceX_;
    double EmittanceY_;
    double BetaX_;
    double BetaY_;
    double AlphaX_;
    double AlphaY_;
    bool UseConstBeta_;
    bool UseDispersion_;
    bool UseFNALt0_;
    double Pmean_;
    double dPOverP_;
    double SigmaT_;
    bool FlatDecayTime_;
    double MaxDecayTime_;
    std::string Particle_;
    int NumParticles_;
    int DecayScaleFactor_;
    std::string Polarization_;
    bool NoTransMom_;
    bool PosOffset_;
    bool StartPerfect_;
    double RotAngle_;
    int Kick_;
    bool Debug_;

  }; // End BeamTransportPGA class 
}
#endif // BEAMTRANSPORTPGA_SERVICE_HH

