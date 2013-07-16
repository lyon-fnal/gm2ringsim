// Tracker detector service for 

// Include guards
#ifndef STRAWTRACKER_HH
#define STRAWTRACKER_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "messagefacility/MessageLogger/MessageLogger.h"
#include "gm2ringsim/strawtracker/StrawSD.hh"

#include "gm2geom/strawtracker/StrawTrackerGeometry.hh"
#include "gm2ringsim/vac/VacGeometry.hh"

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
        gm2strawtracker::StrawTrackerGeometry geom_;
     
        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

    };
}

#endif
