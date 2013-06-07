// MuonStorageStatusActionService is the service that provides the muonStorageStatus action. It        
// records information on each muonStorageStatus as it is created, and can output this
// information in a collection
// To use this action, all you need to do is put it in the services section                             // of the configuration file, like this:
// <pre>
// services: {
//   ...
//   user: {
//     MuonStorageStatusAction: {}
//     ...
//   }
// }
// </pre>

// Include guards
#ifndef MUONSTORAGESTATUSACTION_SERVICE_HH
#define MUONSTORAGESTATUSACTION_SERVICE_HH

/** @file EventAction_service.hh
    Combines the functionality of the sevral g2MIGTRACE classes:
          runAction.cc, eventAction.cc and steppingAction.cc

    @author  Zach Hartwig
    @date    2005
    @author  Kevin Lynch
    @date    2009
    
    Moved to the ART environment by
    @authors Tasha Arvanitis, Adam Lyon                                         
    @date   August 2012 
    
    Integrated with the gm2ringsim port by
    @author Brendan Kiburg
    @date   Dec 2012
*/

// ART includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Core/EDProducer.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Geant stuff
#include "Geant4/G4Run.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4Step.hh"

// Get the base class(es)
#include "artg4/actionBase/RunActionBase.hh"
#include "artg4/actionBase/EventActionBase.hh"
#include "artg4/actionBase/SteppingActionBase.hh"

// ART Event output
#include "gm2ringsim/actions/muonStorageStatus/EventRecord.hh"

#include <tr1/memory>

namespace muonTrackingStatus {
  enum state { trackingMuon, storedMuon, lostMuon };
}

enum { kStored, kHitInflector, kHitLab, kBeyondStorageRegion };

namespace gm2ringsim
{
  class MuonStorageStatusAction : public artg4::EventActionBase,
				  public artg4::RunActionBase ,
				  public artg4::SteppingActionBase
  {
  public:
    MuonStorageStatusAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~MuonStorageStatusAction() {};

    void beginOfEventAction (const G4Event*) override;
    void endOfEventAction( const G4Event* ) override;
    
    /** Resets the muon storage counter and causes begin of run setup to
	occur in the rootStorageManager and elapsed time counters. */
    virtual void beginOfRunAction(const G4Run *currentRun) override;
    
    /** Calculates capture efficiency and elapsed time, and initiates
	rootStorageManager end of run activities. */
    virtual void endOfRunAction(const G4Run *currentRun) override; 

    void userSteppingAction(const G4Step *) override;

    // ART output defined here

    void callArtProduces(art::EDProducer * producer) override;
    void fillEventWithArtStuff(art::Event & e) override;

    // Event Action helpers
    
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
    void incrementMuonKillCounter();


    //SteppingAction Stuff
    void setStoredMuonTurns(G4int );


    
  private:
    //EventAction Stuff
    muonTrackingStatus::state muonStorageStatus_;
    int eventStatus_;
    G4int hbFreq_, hbLength_;
    
    //RunAction Stuff
    G4int muonStorageCounter_;  
    G4int muonKillCounter_;  

    //SteppingAction Stuff
    float turnsForStorage_; //Double to allow for fractional turns 
    // The number of turns required to consider a  muon 'stored'.
    // Default is 2160 (5 muon decay lifetimes in the lab frame).
    
    double stored_rmin_; //  The minimum radius of a muon for it to be considered
    //       'stored', in meters. Default is 7.0 m.    
    
    double stored_rmax_; // The maximum radius of a muon for it to be considered
    //       'stored', in meters Default is 8.0 m.
    

    double stored_y_; //The maximum distance (in meters) a muon can deviate
    //       vertically from a centered path to still be considered stored.
    //       Default is 0.074 m.
    
    //ART stuff
    mf::LogInfo logInfo_;

    
  }; //end of EventAction class definition 
} //namespace gm2ringsim

#endif //MUONSTORAGESTATUSACTION_SERVICE_HH

