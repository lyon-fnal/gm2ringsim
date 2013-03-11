#ifndef G2MUONGASSOURCE_HH
#define G2MUONGASSOURCE_HH

// Header for the Event action
/** @file PGA.hh prior to ART
    @file g2MuonGasSource_service.hh
    @author Nathan Froemming                                               
    @date 2011-2012          
    @comment Implemented in with the g2GeneralParticleSource
    @author Brendan Kiburg
    @date 2013
    @comment Implementation in ART as a separate sort of Source
      
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
//#include "Geant4/G4VPrimaryVertex.hh"

#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
// Get the helper files

namespace gm2ringsim
{
  class G2MuonGasSource : public G4VPrimaryGenerator
  {
  public:
    G2MuonGasSource();
    ~G2MuonGasSource();
    
    void GeneratePrimaryVertex(G4Event*) override;
    
    G4double randXFromE821();
    G4double randTFromE989();
    
    void SetVerbosity(bool);
    
  private:
    G4ParticleGun* muonGasGun_;
    G2GeneralParticleSource* g2GPS_;
    bool muonGasVerbosity_;
    
  }; // End G2MuonGasSource class
} //namespace gm2ringsim

#endif //G2MUONGASSOURCE_HH

