#ifndef STACKINGACTION_SERVICE_HH
#define STACKINGACTION_SERVICE_HH

/** @file stackingAction.hh

    Provides the declaration for the user stacking action class. 

    - Lawrence Gibbons 2012
    - Brendan Kiburg Artified Nov 2012
*/

// Boilerplate for ART Service
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

// Get the base class
#include "artg4/actionBase/StackingActionBase.hh"
#include "Geant4/G4ClassificationOfNewTrack.hh"
#include "Geant4/globals.hh"

namespace gm2ringsim
{
  class StackingAction : public artg4::StackingActionBase
  {
  public:
    StackingAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~StackingAction();
    
  /** Interface to set accepted wavelength range **/
    void setMinimumOpticalWavelength(G4double minWavelength );
    void setMaximumOpticalWavelength(G4double maxWavelength );
    
  private:
    G4double minWavelength_;
    G4double maxWavelength_;
    
    bool killNewTrack( const G4Track* aTrack);

  }; // End StackingAction class 
}
#endif

