// Include guards
#ifndef DECAYEDPOSITRONACTION_SERVICE_HH
#define DECAYEDPOSITRONACTION_SERVICE_HH

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

#include <tr1/memory>

namespace gm2ringsim
{
  class DecayedPositronAction : public artg4::EventActionBase,
				  public artg4::RunActionBase ,
				  public artg4::SteppingActionBase
  {
  public:
    DecayedPositronAction(fhicl::ParameterSet const&, art::ActivityRegistry&);
    virtual ~DecayedPositronAction() {};

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
    //
    //void callArtProduces(art::EDProducer * producer) override;
    //void fillEventWithArtStuff(art::Event & e) override;

    // Event Action helpers
    
  private:
    //EventAction Stuff

    double stored_threshold_; //  The minimum radius of a muon for it to be considered
    //       'stored', in meters. Default is 7.0 m.    
    
    int Nexposed_;
    int Nkilled_;
    
    
    //ART stuff
    mf::LogInfo logInfo_;
    
  }; //end of EventAction class definition 
} //namespace gm2ringsim

#endif //DECAYEDPOSITRONACTION_SERVICE_HH

