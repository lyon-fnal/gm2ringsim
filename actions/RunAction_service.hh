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
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

// This is necessary because the time libraries are implemented                               
// differently on the MAC                                                                     
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#else
#include <time.h>
#endif


// Get the base class
#include "artg4/actionBase/RunActionBase.hh"

// Geant stuff
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

#ifdef __MACH__

    mach_timespec_t start_;
    mach_timespec_t end_;
    
    clock_serv_t cclock_;
    mach_timespec_t mts_;
    
    void mach_clock_get_start_time();
    void mach_clock_get_end_time();
#else
    // timespec values for the start and end of the run;
    timespec start_;
    timespec end_;
    
    clockid_t clockID_;
    // A method to find the difference between two timespec values  
    
#endif
    // The diff will use the internal variables rather than being passed
    // variables, and I will protect against MAC/LINUX timespec types
    double clockDiff();

    // COMMON to MAC and LINUX
    G4int muonStorageCounter_;  
    mf::LogInfo logInfo_;
    
  };   
} //gm2ringsim namespace
#endif

