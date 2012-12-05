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

// Get the base class
#include "artg4/actionBase/EventActionBase.hh"

#include "Geant4/G4Event.hh"


#include <tr1/memory>

namespace muonTrackingStatus {
  enum state { trackingMuon, storedMuon, lostMuon };
}

enum { kStored, kHitInflector, kHitLab, kBeyondStorageRegion };

namespace gm2ringsim
{
  class EventAction : public artg4::EventActionBase
  {
  public:
    EventAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~EventAction() {};
    void beginOfEventAction (const G4Event*) override;
    void endOfEventAction( const G4Event* ) override;

    
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

    
  private:
    muonTrackingStatus::state muonStorageStatus_;
    // convert this to ART service handle in the code
    //runAction *RunAction_;
    int eventStatus_;
    G4int hbFreq_, hbLength_;
    
  }; //end of EventAction class definition 
} //namespace gm2ringsim

#endif //EVENTACTION_SERVICE_HH

