#ifndef G2PGA_SERVICE_HH
#define G2PGA_SERVICE_HH

// Header for the Event action
/** @file PGA.hh prior to ART
    @file G2PGA_service.hh
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
#include "gm2ringsim/actions/PGA/g2GeneralParticleSource.hh"

namespace gm2ringsim
{
  class G2PGA : public artg4::PrimaryGeneratorActionBase
  {
  public:
    G2PGA(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~G2PGA();
    
    void initialize() override;
    void generatePrimaries( G4Event* ) override;
   
  private:
    fhicl::ParameterSet par_g2GPS_;
    g2GeneralParticleSource* g2GPS_;



  }; // End G2PGA class 
}
#endif // G2PGA_SERVICE_HH

