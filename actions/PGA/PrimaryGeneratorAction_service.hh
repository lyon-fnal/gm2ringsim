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
//FIXME //#include "Root/TNtuple.h"

// Get the helper files
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"


namespace gm2ringsim
{
  class PrimaryGeneratorAction : public artg4::PrimaryGeneratorActionBase
  {
  public:
    PrimaryGeneratorAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~PrimaryGeneratorAction() { delete particleGun_; }
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
    G4double randXFromE821();
    G4double randTFromE989();

   
  private:
    G4ParticleGun* particleGun_;
    g2GeneralParticleSource* g2GPS_;
    G4ParticleGun* muonGasGun_;
    G4ParticleGun* inflectorGun;

    //  NSF (30 AUG '12): Why is this here? 
    G4RotationMatrix m_localToGlobalRot ;
    G4ThreeVector m_yhatGlobal, m_caloCenterGlobal ;
    //FIXME   //  TNtuple* m_nt ;
    int m_ntCount, m_ntTot ;
    float m_elex, m_eley, m_elez ;
    float m_elpx, m_elpy, m_elpz ;
  };   
}
#endif

