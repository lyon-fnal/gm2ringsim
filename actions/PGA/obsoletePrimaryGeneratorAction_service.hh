// NOTE: THIS WAS THE INITIAL ATTEMPT TO PORT OVER THE PGA FROM 
// G2MIGTRACE
// THIS HAS BEEN SPLIT UP INTO TWO NEW SERVICES
// G2PGA_SERVICE , AND
// MUONGASPGA_SERVICE
#ifndef PRIMARYGENERATORACTION_SERVICE_HH
#define PRIMARYGENERATORACTION_SERVICE_HH

// Header for the Event action
/** @file PGA.hh prior to ART
    @file PrimaryGeneratorAction_service.hh
    Contains the minimal required primary generator declarations,
    class PGA                                                    
    @author Zach Hartwig                                                      
    @author Kevin Lynch                                                     
    @date 2005-2009                                                        
    @author Nathan Froemming                                              
    @date 2011-2012                                                      
    @author Brendan Kiburg
    @date 2012 Port to ART
      
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

namespace gm2ringsim
{
  class PrimaryGeneratorAction : public artg4::PrimaryGeneratorActionBase
  {
  public:
    PrimaryGeneratorAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~PrimaryGeneratorAction();
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
    G4double randXFromE821();
    G4double randTFromE989();
   
  private:
    fhicl::ParameterSet par_g2GPS_;
    G2GeneralParticleSource* g2GPS_;
    G4ParticleGun* muonGasGun_;
    G4ParticleGun* inflectorGun_;
    bool muonGasVerbosity_;


  }; // End PrimaryGeneratorAction class 
}
using gm2ringsim::PrimaryGeneratorAction;
DECLARE_ART_SERVICE(PrimaryGeneratorAction,LEGACY)

#endif

