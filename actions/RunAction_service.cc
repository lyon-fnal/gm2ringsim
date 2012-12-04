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
#include <time.h>
#include <vector>

//#include "rootStorageManager.hh"
//#include "g2UniqueObjectManager.rhh"

namespace {
  clockid_t const clkid = CLOCK_REALTIME;
  timespec start, end;
  
  // make sure the RSM is initialized before we exit PreInit state, or
  // the Messenger directories won't be populated correctly....
  //FIXME: Address in ART rootStorageManager& rsm = rootStorageManager::getInstance();
}

double diff(timespec start, timespec end)
{
  timespec temp;
  if ((end.tv_nsec-start.tv_nsec)<0) {
    temp.tv_sec = end.tv_sec-start.tv_sec-1;
    temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
  } else {
    temp.tv_sec = end.tv_sec-start.tv_sec;
    temp.tv_nsec = end.tv_nsec-start.tv_nsec;
  }
  return temp.tv_sec + temp.tv_nsec/1e9;
}



// Constructor
gm2ringsim::RunAction::RunAction(fhicl::ParameterSet const& p, art::ActivityRegistry&) :
  artg4::RunActionBase(p.get<std::string>("name")),
  muonStorageCounter_(0)
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

  clock_gettime(clkid, &start);
}

// Called at the end of each run.                                                   
void gm2ringsim::RunAction::endOfRunAction(const G4Run *){//currentRun) {

  clock_gettime(clkid, &end);

  G4double totalEvents = currentRun -> GetNumberOfEvent();
  G4double captureEfficiency = (muonStorageCounter*100.) / totalEvents;
  G4double dcE = std::sqrt(muonStorageCounter)*100/totalEvents;
  
  G4cout << "\n  Muons injected : "
	 << totalEvents << "\n"
	 << "  Muons stored : "
	 << muonStorageCounter << "\n"
	 <<"  Capture Efficiency : (" 
	 << captureEfficiency << " +/- "
	 << dcE << ")%\n\n";
  
  G4cout << "Elapsed time this run: "
	 << diff(start, end) << "sec\n";

  //FIXME: ARTIFY
  // No unique object manager or rootstoragemanager yet
  /*G4cout << "Unique Objects in Manager: "
	 << rsm.getUOM().count() << '\n';
  
  rsm.EndOfRun();
  */

}   

void gm2ringsim::RunAction::incrementMuonStorageCounter();
    

using gm2ringsim::RunAction;
DEFINE_ART_SERVICE(RunAction)
