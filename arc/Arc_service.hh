// Tracker detector service for 

// Include guards
#ifndef ARC_HH
#define ARC_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4FieldManager.hh"

#include <vector>

#include "gm2geom/arc/ArcGeometry.hh"
// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Arc : public artg4::DetectorBase {

    public:

        // Constructor
        Arc(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Arc() ;

    private:
      SpinTrackingSettings sts_;
      bool nospin_tracking_;
      bool const spin_tracking_;
      bool const edm_tracking_;

      G4FieldManager *withoutSpin_;
      G4FieldManager *withSpin_;
      G4FieldManager *withEDM_;


      
      // Private overriden methods
      
      // Create the logical volumes
      virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
      
      // Create the physical volumes
      virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

      virtual void initialize() override;
      
      // Convenience functions
      G4LogicalVolume* makeAnArcLV(gm2geom::ArcGeometry const &, unsigned int);
      

    };
}

using gm2ringsim::Arc;
DECLARE_ART_SERVICE(Arc,LEGACY)
#endif
