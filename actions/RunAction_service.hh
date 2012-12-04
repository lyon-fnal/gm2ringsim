// Header for the Run action
#ifndef RUNACTION_SERVICE_HH
#define RUNACTION_SERVICE_HH



/** @file RunAction_service.hh

    Provides the declaration of a User Run Action, as well as an
    associated messenger. 

    - Zach Hartwig 2005
    - Kevin Lynch 2009

    - Artified the g2migtrace runAction code
    - @author Brendan Kiburg
    - @date   Dec 2012
*/

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

// Get the base class
#include "artg4/actionBase/RunActionBase.hh"

#include "Geant4/G4Run.hh"
namespace gm2ringsim
{
  class RunAction : public artg4::RunActionBase
  {
  public:
    RunAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~RunAction() {};
    
    /** Resets the muon storage counter and causes begin of run setup to
	occur in the rootStorageManager and elapsed time counters. */
    virtual void beginOfRunAction(const G4Run *currentRun);
    
    
    /** Calculates capture efficiency and elapsed time, and initiates
	rootStorageManager end of run activities. */
    virtual void endOfRunAction(const G4Run *currentRun); 
    /** Used in muon storage efficiency studies.
	
	@bug ... this function really should be taken over by a class
	that tracks lots of stuff on a run by run basis. */
    void incrementMuonStorageCounter();
    
  private:  
    G4int muonStorageCounter_;  
        
  };   
} //gm2ringsim namespace
#endif

