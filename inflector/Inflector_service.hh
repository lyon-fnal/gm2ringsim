// Tracker detector service for 

// Include guards
#ifndef INFLECTOR_HH
#define INFLECTOR_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"

#include "Geant4/G4Box.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Torus.hh"
#include "Geant4/G4Trd.hh"
#include "Geant4/G4EllipticalTube.hh"
#include "Geant4/G4UnionSolid.hh"
#include "Geant4/G4SubtractionSolid.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4AssemblyVolume.hh"

#include "Geant4/G4MagneticField.hh"
#include "Geant4/G4Mag_EqRhs.hh"
#include "Geant4/G4MagIntegratorStepper.hh"
#include "Geant4/G4ChordFinder.hh"
#include "Geant4/G4FieldManager.hh"

//#include "constructionMaterials.hh"
#include "gm2ringsim/common/spin/SpinController.hh"

#include <string>
#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

#include "gm2ringsim/inflector/InflectorGeom.hh"
#include "gm2ringsim/inflector/InflectorSD.hh"
#include "gm2ringsim/common/ring/RingSD.hh"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"
#include "gm2ringsim/inflector/InflectorArtRecord.hh"
#include "gm2ringsim/inflector/InflectorHit.hh"

#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/common/ring/RingHit.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Inflector : public artg4::DetectorBase {

    public:

      // Constructor
      Inflector(fhicl::ParameterSet const &, art::ActivityRegistry & );
      
      // We always need a virtual destructor
      virtual ~Inflector() ;
      
      /** Prints inflector status information. */
      void getInflectorInfo();
      /** Sets the number of inflector beam aperture tracking volumes. */
      //void NumTrackers(G4int n); // was NumTrackers(G4int n); in g2migtrace
      void setNumTrackers (G4int n);

      /** Gets the number of inflector beam aperture tracking volumes. */
      G4int getNumTrackers() const ; //return num_trackers_; }
      
      /** Sets the magnetic field model.
	  Options are "vanishing/none", "simple" or uniform, and "mapped"
	  from Wuzheng Meng's detailed field maps.                                
      */
      void setMagFieldType(G4String);

      /** Regenerates the GPS Macros when the inflector geometry is
	  rebuilt. Not yet implemented int ART */
      void generateGPSMacros(); //was GenerateGPSMacros() in g2mt

      /** Interface to the spinController to enable/disable spin tracking
       	  in the magnetic field. */
      //      void enable_spintracking(bool);

      /*
      //Obsolete public: g2MIGTRACE FUNCTIONS
      
      void setAperturePosition(G4double);
      void setApertureDistance(G4double);
      void setInflectorTiltAngle(G4double);
      void setInflectorSwingAngle(G4double);
      void setConductorModelType(G4String);
      void setUpstreamEndType(G4String);
      void setDownstreamEndType(G4String);
      void setUpstreamWindow(G4String);
      void setUpstreamConductor(G4String);
      void setUpstreamEndFlange(G4String);
      void setDownstreamWindow(G4String);
      void setDownstreamConductor(G4String);
      void setDownstreamEndFlange(G4String);
      void setConductorCurrent(G4double);
      void setFieldNormalizationConstant(G4double);

      void RebuildInflector();
      void DeleteInflector();
      */


    private:
      SpinTrackingSettings sts_;
      InflectorGeom infGeom_;
      G4int num_trackers_;
      G4MagneticField *inflectorMagField_;
      G4Mag_EqRhs *iEquation_;
      G4MagIntegratorStepper *iStepper_;
      G4ChordFinder *iChordFinder_;
      G4FieldManager *inflectorFieldManager_, *launchFieldManager_;
      G4RotationMatrix *inflectorRotation_;
      G4double const epsilon_;
      G4int const vacuumInflectorSection_;
      G4bool initialBuild_;
      G4double maxStepLength_;
      G4bool useConductorEquivalent_;
      G4bool useUpstreamWindow_, useDownstreamWindow_;
      G4bool useUpstreamConductor_, useDownstreamConductor_;
      G4bool useUpstreamEndFlange_, useDownstreamEndFlange_;
      enum {VANISHING_FIELD, SIMPLE_FIELD, MAPPED_FIELD, OTHER_FIELD};
      G4int mag_field_type_;
      G4double conductorCurrent_;
      G4double fieldNormConst_;
      G4double currentToMagFieldConversion_;
      bool const spin_tracking_;
      G4String inflectorSDname_;
      G4String ringSDname_;
      InflectorSD *inflectorSD_;
      RingSD *ringSD_;

      // Private overriden methods from ART
      // Create the logical volumes
      virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

      // Create the physical volumes
      virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
      
      // CHANGE_ME: Delete the next two functions if no hits
      
      // Tell Art what we'll produce
      virtual void doCallArtProduces(art::EDProducer * producer) override;
      // Actually add the data to the event
      virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
      
      //Private Construction Methods
      void buildCore_SandL(); // Solids and Logicals                       
      void buildPeripherals_SandL(); // Solids and Logicals                

      void buildCryostatWalls_SandL(); // Solids and Logicals
      
      void buildSensitiveDetectors(); 
      
      void buildInflector( ); // Physical Volumes                       
      void buildCryostatWalls(); // Physical Volumes
      

      
      void buildTrackingVolumes(); // all of the above                         

      void buildInflectorField();
      void rebuildFieldImpl();
      void rebuildEOM();
      void assignFieldManager();

      // All the detailed volumes
      /** @bug The construction process and member variables should be         
	redone so that we can lose all these pointers, or at least, as    
	many as is practicable.  We hardly need any of them. */

      G4LogicalVolume *inflector_L_;
      G4VPhysicalVolume *inflector_P_;

      G4LogicalVolume *inflectorMandrel_L_;
      G4VPhysicalVolume *inflectorMandrel_P_;

      G4LogicalVolume *beamChannel_L_;
      G4VPhysicalVolume *beamChannel_P_;

      // US=Upstream DS=Downstream                                                
      G4LogicalVolume *window_US_L_;
      G4VPhysicalVolume *window_US_P_;
      G4LogicalVolume *window_DS_L_;
      G4VPhysicalVolume *window_DS_P_;

      G4LogicalVolume *endFlange_US_L_;
      G4VPhysicalVolume *endFlange_US_P_;
      G4LogicalVolume *endFlange_DS_L_;
      G4VPhysicalVolume *endFlange_DS_P_;

      G4LogicalVolume *equivalentAl_US_L_;
      G4VPhysicalVolume *equivalentAl_US_P_;
      G4LogicalVolume *equivalentAl_DS_L_;
      G4VPhysicalVolume *equivalentAl_DS_P_;

      G4LogicalVolume *equivalentCu_US_L_;
      G4VPhysicalVolume *equivalentCu_US_P_;
      G4LogicalVolume *equivalentCu_DS_L_;
      G4VPhysicalVolume *equivalentCu_DS_P_;

      G4LogicalVolume *equivalentNbTi_US_L_;
      G4VPhysicalVolume *equivalentNbTi_US_P_;
      G4LogicalVolume *equivalentNbTi_DS_L_;
      G4VPhysicalVolume *equivalentNbTi_DS_P_;

      G4LogicalVolume *parallelCryoWall_L_;
      G4VPhysicalVolume *parallelCryoWall_P_;

      G4LogicalVolume *perpCryoWall_L_;
      G4VPhysicalVolume *perpCryoWall_P_;

      G4LogicalVolume *launchRegion_L_;
      G4VPhysicalVolume *launchRegion_P_;

      //G4VPhysicalVolume *vacPTR_; //Was a PV in g2MIGTRACE
      G4LogicalVolume *vacPTR_; // Changed in ART since we only pass the mother LVs

      std::vector<G4VPhysicalVolume*> tracker_physicals_;
      
      G4ThreeVector calc_position() const;
      G4RotationMatrix *calc_rotation();
      //InflectorArtRecord convert(InflectorHit* );
      // inflectorMessenger *InflectorMessenger;

      connection_t conn_;

    };

  //Helper function
   InflectorArtRecord convert(InflectorHit* e);


}

#endif
