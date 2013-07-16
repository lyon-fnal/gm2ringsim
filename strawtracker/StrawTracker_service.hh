// Tracker detector service for 

// Include guards
#ifndef STRAWTRACKER_HH
#define STRAWTRACKER_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "messagefacility/MessageLogger/MessageLogger.h"
#include "gm2ringsim/strawtracker/StrawSD.hh"

#include "gm2ringsim/strawtracker/StrawTrackerGeometry.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class StrawTracker : public artg4::DetectorBase {

    public:

        // Constructor
        StrawTracker(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~StrawTracker() {};

    private:
        StrawTrackerGeometry geom_;
     
        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

        // CHANGE_ME: Delete the next two functions if no hits

        // Tell Art what we'll produce
        //virtual void doCallArtProduces(art::EDProducer * producer) override;

        // Actually add the data to the event
        //virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
      
      // Convenience functions
        //G4UnionSolid* buildScallopSolid();

    };
}

#endif
