// Include guards
#ifndef OCTUPOLE_HH
#define OCTUPOLE_HH

/** @file Octupole_service.hh
    octupoleConstruction.hh contains declarations for the PNPI
    pulsed magnetic octupole CBO mixing device

    @author Kevin Lynch
    @date 2009

    Artized by
    @author Brendan Kiburg
    @date 2013

*/


// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4FieldManager.hh"
#include "Geant4/G4MagneticField.hh"


#include <vector>
#include <tr1/array>

#include "gm2ringsim/common/spin/SpinController.hh"
#include "gm2ringsim/octupole/OctupoleField.hh"
#include "gm2ringsim/octupole/OctGeometry.hh"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Octupole : public artg4::DetectorBase {

    public:

      // Constructor
      Octupole(fhicl::ParameterSet const &, art::ActivityRegistry & );
      // We always need a virtual destructor
      virtual ~Octupole() {};
      
      //bool is_enabled() const { return enabled_; }
      
      
    private:
      //bool enabled_; // not really needed since fcl will determine if enabled
      SpinTrackingSettings sts_;
      bool nospin_tracking_;
      bool spin_tracking_;
      bool edm_tracking_;
      OctGeometry og_;
      G4FieldManager *withSpin_, *withoutSpin_, *withEDM_;
      G4LogicalVolume *fieldRegion_L_;
      G4VPhysicalVolume *fieldRegion_P_;
      std::tr1::array<G4LogicalVolume *, 16> tubes_L_, water_L_;
      std::tr1::array<G4VPhysicalVolume *, 16> tubes_P_, water_P_;

      // Private overriden methods
      // Create the logical volumes
      virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
      
      // Create the physical volumes
      virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
      
      void buildLogicals();
      void buildPhysicals(std::vector<G4LogicalVolume*> motherLVs);
      void destroyPhysicals();
      void buildFieldManagers();
      void do_enable_spintracking();
      OctupoleField *GetField() const;
      
      
      
      // CHANGE_ME: Delete the next two functions if no hits
      
      // Tell Art what we'll produce
      // virtual void doCallArtProduces(art::EDProducer * producer) override;
      
      // Actually add the data to the event
      // virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
      
    };
}

#endif //OCTUPOLE_HH
