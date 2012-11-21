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

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

// Get the base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"

#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"

namespace gm2ringsim
{
    class PrimaryGeneratorAction : public artg4::PrimaryGeneratorActionBase
    {
        public:
            PrimaryGeneratorAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
            virtual ~PrimaryGeneratorAction() { delete particleGun_; }
      
            void initialize() override;

            void generatePrimaries( G4Event* ) override;
        
        private:
            G4ParticleGun* particleGun_;
    };   
}
#endif

