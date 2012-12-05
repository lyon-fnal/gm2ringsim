// Header for the Event action
#ifndef EVENTACTION_SERVICE_HH
#define EVENTACTION_SERVICE_HH

/** @file EventAction_service.hh
    
    Provides the declaration of a User Event Action, as well as an
    associated messenger. 
    
    - Zach Hartwig 2005
    - Kevin Lynch 2009
    
    - Artified the g2migtrace eventAction code
    - @author Brendan Kiburg
    - @date   Dec 2012
*/

#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

// Geant stuff
#include "Geant4/G4Run.hh"

// Get the base class(es)
#include "artg4/actionBase/RunActionBase.hh"
#include "artg4/actionBase/EventActionBase.hh"

#include "Geant4/G4Event.hh"
#include "gm2ringsim/actions/EventRecord.hh"

#include <tr1/memory>

namespace muonTrackingStatus {
  enum state { trackingMuon, storedMuon, lostMuon };
}

enum { kStored, kHitInflector, kHitLab, kBeyondStorageRegion };

namespace gm2ringsim
{
  class EventAndRunAction : public artg4::EventActionBase,
		      public artg4::RunActionBase 
  {
  public:
    EventAndRunAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~EventAndRunAction() {};
    void beginOfEventAction (const G4Event*) override;
    void endOfEventAction( const G4Event* ) override;

    /** Resets the muon storage counter and causes begin of run setup to
	occur in the rootStorageManager and elapsed time counters. */
    virtual void beginOfRunAction(const G4Run *currentRun);
    
    /** Calculates capture efficiency and elapsed time, and initiates
	rootStorageManager end of run activities. */
    virtual void endOfRunAction(const G4Run *currentRun); 
    
    
    void callArtProduces(art::EDProducer * producer) override;
    void fillEventWithArtStuff(art::Event & e) override;
    
    
    /** Setter for successful storage. */
    void successfulStorage();
    /** Setter for failed storage */
    void unsuccessfulStorage();
    void unsuccessfulStorage(G4String reason);
    /** Getter for the current muon storage status. */
    muonTrackingStatus::state getMuonStorageStatus() {return muonStorageStatus_;};
    
    inline void setEventStatus(int status) { eventStatus_ = status; }
    inline int getEventStatus() { return( eventStatus_ ); }
    
    /** Get the printout heartbeat frequency (events between
	printouts). */
    G4int hbFrequency() const { return hbFreq_; }
    /** Sets the printout heartbeat frequency (events between
	printouts). */
    G4int hbFrequency(G4int hbf) { G4int old = hbFreq_; hbFreq_ = hbf; return old; }

    //RunAction Stuff

    /** Used in muon storage efficiency studies.
	
	@bug ... this function really should be taken over by a class
	that tracks lots of stuff on a run by run basis. */
    void incrementMuonStorageCounter();


    
  private:
    //EventAction Stuff
    muonTrackingStatus::state muonStorageStatus_;
    int eventStatus_;
    G4int hbFreq_, hbLength_;
    
    // COMMON to MAC and LINUX
    G4int muonStorageCounter_;  
    mf::LogInfo logInfo_;
    
  }; //end of EventAction class definition 
} //namespace gm2ringsim

#endif //EVENTACTION_SERVICE_HH

