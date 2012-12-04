// Implementation of RunActionService
/** @file RunAction_service.cc

    Provides definitions of the User Run Action class members.

    - Zach Hartwig 2005
    - Kevin Lynch 2009

    - Artified the g2migtrace runAction.cc files
    @author Brendan Kiburg
    @date Dec 2012
*/


#include "gm2ringsim/actions/RunAction_service.hh"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/ActionHolder_service.hh"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/globals.hh"

#include "Geant4/G4TrajectoryContainer.hh"
#include "Geant4/G4NavigationHistory.hh"
#include "Geant4/G4RunManager.hh"

#include <iomanip>
#include <vector>




//#include "rootStorageManager.hh"
//#include "g2UniqueObjectManager.rhh"

#ifndef __MACH__
namespace {
  //  clockid_t const clkid = CLOCK_REALTIME;
  //  clock_t const clkid = CLOCK_REALTIME;
  
  // make sure the RSM is initialized before we exit PreInit state, or
  // the Messenger directories won't be populated correctly....
  //FIXME: Address in ART rootStorageManager& rsm = rootStorageManager::getInstance();
}
#endif


double gm2ringsim::RunAction::clockDiff()
{
  // mach_timespec_t and timespec both have the internal vars:
  // tv_nsec and tv_sec, so just let the compiler figure out the
  // type of temp and do the same calculation. In the end, we just
  // return a double

#ifdef __MACH__
  mach_timespec_t temp;
#else
  timespec temp;
#endif
  
  if ((end_.tv_nsec-start_.tv_nsec)<0) {
    temp.tv_sec = end_.tv_sec-start_.tv_sec-1;
    temp.tv_nsec = 1000000000+end_.tv_nsec-start_.tv_nsec;
  } else {
    temp.tv_sec = end_.tv_sec-start_.tv_sec;
    temp.tv_nsec = end_.tv_nsec-start_.tv_nsec;
  }
  return temp.tv_sec + temp.tv_nsec/1e9;
}


// Constructor
gm2ringsim::RunAction::RunAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::RunActionBase(p.get<std::string>("name")),
  start_(), // These have different types on MAC and LINUX
  end_(),   // but they are defined in the same relative order
#ifdef __MACH__
  cclock_(),
  mts_(),
#else
  clockID_(CLOCK_REALTIME),
#endif
  muonStorageCounter_(0),
  logInfo_("ClockAction")
{ }

void gm2ringsim::RunAction::beginOfRunAction(const G4Run *currentRun) {
  // Initialization per run
  muonStorageCounter_ = 0;
  //FIXME:TEMP to use currentRun
  std::cout<<"currentRunID is "<<currentRun->GetRunID()<<std::endl;
  
  // FIXME: ARTIFY
  // Cannot use this block yet because RootStorageManager not implemented
  /* if( !rsm.BeginOfRun(currentRun->GetRunID()) ){
     G4cout << "The Root Storage Manager had a problem in BeginOfRunAction()."
     << "  Aborting Run....\n";
     G4RunManager::GetRunManager()->AbortRun();
     }*/
#ifdef __MACH__
  mach_clock_get_start_time();
#else 
  clock_gettime(clockID_, &start_);
#endif

}

// Called at the end of each run.                                               

#ifdef __MACH__

void gm2ringsim::RunAction::mach_clock_get_start_time(){
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock_);
  clock_get_time(cclock_, &start_);
  mach_port_deallocate(mach_task_self(), cclock_); 
}
void gm2ringsim::RunAction::mach_clock_get_end_time(){
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock_);
  clock_get_time(cclock_, &end_);
  mach_port_deallocate(mach_task_self(), cclock_); 
}
#endif

    
void gm2ringsim::RunAction::endOfRunAction(const G4Run *currentRun) {
#ifdef __MACH__
  mach_clock_get_end_time();
#else 
  clock_gettime(clockID_, &end_);
#endif
  
  G4double totalEvents = currentRun -> GetNumberOfEvent();
  G4double captureEfficiency = (muonStorageCounter_*100.) / totalEvents;
  G4double dcE = std::sqrt(muonStorageCounter_)*100/totalEvents;
  
  G4cout << "\n  Muons injected : "
	 << totalEvents << "\n"
	 << "  Muons stored : "
	 << muonStorageCounter_ << "\n"
	 <<"  Capture Efficiency : (" 
	 << captureEfficiency << " +/- "
	 << dcE << ")%\n\n";
  
  G4cout << "Elapsed time this run: "
	 << clockDiff() << "sec\n";
  
  //FIXME: ARTIFY
  // No unique object manager or rootstoragemanager yet
  /*G4cout << "Unique Objects in Manager: "
	 << rsm.getUOM().count() << '\n';
  
  rsm.EndOfRun();
  */
  
} //RunAction::endOfRunAction(const G4Run*)
  
  void gm2ringsim::RunAction::incrementMuonStorageCounter()
  {++muonStorageCounter_;} 
    

using gm2ringsim::RunAction;
DEFINE_ART_SERVICE(RunAction)
