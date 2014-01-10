#ifndef CALOELECTRONPGA_SERVICE_HH
#define CALOELECTRONPGA_SERVICE_HH

/** @file CaloElectronPGA_service.hh

 Based on SimpleParticleSource_service.hh
 This will generate electrons aimed at one of the calorimeters.
 Simple version with hard-coded position, momentum, and particle energy for now.
 
 @author  Robin Bjorkquist
 @date    2013

 */

// Boilerplate for ART Service
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

// Get the base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"

// Get the Geant4 code
#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"

namespace gm2ringsim
{
    class CaloElectronPGA : public artg4::PrimaryGeneratorActionBase
    {
    public:
        CaloElectronPGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
        virtual ~CaloElectronPGA();
        
        void initialize() override;
        
        void generatePrimaries( G4Event* ) override;
        
    private:
        G4ParticleGun* particleGun_;
                
    }; // End CaloElectronPGA class
    
} // gm2ringsim namespace

using gm2ringsim::CaloElectronPGA;
DECLARE_ART_SERVICE(CaloElectronPGA, LEGACY)
#endif // CALOELECTRONPGA_SERVICE_HH

