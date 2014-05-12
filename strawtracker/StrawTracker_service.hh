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
#include "gm2geom/vac/VacGeometry.hh"

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
        gm2geom::StrawTrackerGeometry geom_;
        gm2geom::VacGeometry vacg;

        void getXYCoordinatesForPlacement(double distAlongScallop, double distShift, double &x, double&y);     
        // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

    };
}
using gm2ringsim::StrawTracker;
DECLARE_ART_SERVICE(StrawTracker,LEGACY)
#endif
