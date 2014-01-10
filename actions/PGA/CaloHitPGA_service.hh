#ifndef CALOHITPGA_SERVICE_HH
#define CALOHITPGA_SERVICE_HH

/** @file CaloHitPGA_service.hh
 
 Based on SimpleParticleSource_service.hh
 
 This particle gun extracts the calo hits from the input file. For each calo hit, it creates a primary vertex 
 with the stored particle type, position, and momentum.
 
 This allows the simulation to be done in two stages:
 
    1. Do simulations with calorimeter tracking turned off; tracks entering the calorimeter volume are killed 
       and stored in calo hits.
 
    2. Use this particle gun to do the full calorimeter simulation for the tracks previously stored.
 
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
    class CaloHitPGA : public artg4::PrimaryGeneratorActionBase
    {
    public:
        CaloHitPGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
        virtual ~CaloHitPGA();
        
        void initialize() override;
        
        void generatePrimaries( G4Event* ) override;
        
    private:
        G4ParticleGun* particleGun_;
        
    }; // End CaloHitPGA class
    
} // gm2ringsim namespace

using gm2ringsim::CaloHitPGA;
DECLARE_ART_SERVICE(CaloHitPGA,LEGACY)
#endif // CALOHITPGA_SERVICE_HH

