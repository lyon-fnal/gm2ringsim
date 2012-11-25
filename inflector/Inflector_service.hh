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
#include "gm2ringsim/actions/spinController.hh"

#include <string>
#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"

#include "gm2ringsim/inflector/InflectorGeom.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Inflector : public artg4::DetectorBase {

    public:

        // Constructor
        Inflector(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Inflector() {};

      /** Sets the downstream aperture position in the azimuthal                  
	  direction. */
      void setAperturePosition(G4double);
      /** Set the radial position of the downstream aperture. */
      void setApertureDistance(G4double);
      /** Sets the vertical tilt angle of the inflector relative to the           
	  symmetry plane of the ring. */
      void setInflectorTiltAngle(G4double);
      /** Sets the horizontal swing angle of the inflector axis relative          
	  to the ring tangent. */
      void setInflectorSwingAngle(G4double);
      /** Sets the conductor model of the ends.                                   
                                                                              
      @bug Doesn't actually do anything useful, since we don't model          
      anything but an "average" plate.                                        
      */
      void setConductorModelType(G4String);
      /** Sets the upstream end (window, conductor, and flange)                   
	  open/closed. */
      void setUpstreamEndType(G4String);
      /** Sets the downstream end (window, conductor, and flange)                 
	  open/closed. */
      void setDownstreamEndType(G4String);
      /** Sets the presence of the upstream window. */
      void setUpstreamWindow(G4String);
      /** Sets the presence of the upstream conductor. */
      void setUpstreamConductor(G4String);
      /** Sets the presence of the upstream end flange. */
      void setUpstreamEndFlange(G4String);
      /** Sets the presence of the downstream window. */
      void setDownstreamWindow(G4String);
      /** Sets the presence of the downstream conductor. */
      void setDownstreamConductor(G4String);
      /** Sets the presence of the downstream end flange. */
      void setDownstreamEndFlange(G4String);

      /** Sets the field strength by setting the conductor current.               
                                                                              
      @bug I don't think this does anything at all.                           
      */
      void setConductorCurrent(G4double);
      /** Sets the normalization constant that converts between conductor         
      current and nominal central field value.                                
                                                                              
      @bug I don't think this does anything at all.                           
      */
      void setFieldNormalizationConstant(G4double);
      /** Prints inflector status information. */
      void getInflectorInfo();
      /** Sets the number of inflector beam aperture tracking volumes. */
      void NumTrackers(G4int n);
      /** Gets the number of inflector beam aperture tracking volumes. */
      G4int NumTrackers() const { return num_trackers; }

      /** Sets the magnetic field model.                                          
                                                                              
      Options are "vanishing/none", "simple" or uniform, and "mapped"         
      from Wuzheng Meng's detailed field maps.                                
      */
      void setMagFieldType(G4String);

      /** Regenerates the GPS Macros when the inflector geometry is               
	  rebuilt. */
      void GenerateGPSMacros();

      /** Interface to the spinController to enable/disable spin tracking         
	  in the magnetic field. */
      void enable_spintracking(bool);



    private:
      InflectorGeom infGeom_;
      G4int num_trackers;
      G4MagneticField *inflectorMagField;
      G4Mag_EqRhs *iEquation;
      G4MagIntegratorStepper *iStepper;
      G4ChordFinder *iChordFinder;
      G4FieldManager *inflectorFieldManager, *launchFieldManager;
      G4RotationMatrix *inflectorRotation;
      G4double const epsilon;
      G4int const vacuumInflectorSection;
      

      // Private overriden methods

        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;

        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;

        // CHANGE_ME: Delete the next two functions if no hits

        // Tell Art what we'll produce
      //        virtual void doCallArtProduces(art::EDProducer * producer) override;

        // Actually add the data to the event
        //virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;

      //Private Construction Methods
      void BuildCore_SandL(); // Solids and Logicals                              
      void BuildPeripherals_SandL(); // Solids and Logicals                       
      void BuildInflector(); // Physical Volumes                                  
      void BuildCryostatWalls();
      void BuildTrackingVolumes(); // all of the above                            

      void RebuildInflector();
      void DeleteInflector();

      void BuildInflectorField();
      void RebuildFieldImpl();
      void RebuildEOM();
      void AssignFieldManager();

      // All the detailed volumes
      /** @bug The construction process and member variables should be            
    redone so that we can lose all these pointers, or at least, as            
    many as is practicable.  We hardly need any of them. */

      G4LogicalVolume *inflector_L;
      G4VPhysicalVolume *inflector_P;

      G4LogicalVolume *inflectorMandrel_L;
      G4VPhysicalVolume *inflectorMandrel_P;

      G4LogicalVolume *beamChannel_L;
      G4VPhysicalVolume *beamChannel_P;

      // US=Upstream DS=Downstream                                                
      G4LogicalVolume *window_US_L;
      G4VPhysicalVolume *window_US_P;
      G4LogicalVolume *window_DS_L;
      G4VPhysicalVolume *window_DS_P;

      G4LogicalVolume *endFlange_US_L;
      G4VPhysicalVolume *endFlange_US_P;
      G4LogicalVolume *endFlange_DS_L;
      G4VPhysicalVolume *endFlange_DS_P;

      G4LogicalVolume *equivalentAl_US_L;
      G4VPhysicalVolume *equivalentAl_US_P;
      G4LogicalVolume *equivalentAl_DS_L;
      G4VPhysicalVolume *equivalentAl_DS_P;

      G4LogicalVolume *equivalentCu_US_L;
      G4VPhysicalVolume *equivalentCu_US_P;
      G4LogicalVolume *equivalentCu_DS_L;
      G4VPhysicalVolume *equivalentCu_DS_P;

      G4LogicalVolume *equivalentNbTi_US_L;
      G4VPhysicalVolume *equivalentNbTi_US_P;
      G4LogicalVolume *equivalentNbTi_DS_L;
      G4VPhysicalVolume *equivalentNbTi_DS_P;

      G4LogicalVolume *parallelCryoWall_L;
      G4VPhysicalVolume *parallelCryoWall_P;

      G4LogicalVolume *perpCryoWall_L;
      G4VPhysicalVolume *perpCryoWall_P;

      G4LogicalVolume *launchRegion_L;
      G4VPhysicalVolume *launchRegion_P;

      G4VPhysicalVolume *vacPTR;


      std::vector<G4VPhysicalVolume*> tracker_physicals;




      G4ThreeVector calc_position() const;
      G4RotationMatrix *calc_rotation();




      G4bool initialBuild;
      G4double maxStepLength;

      G4bool useConductorEquivalent;
      G4bool useUpstreamWindow, useDownstreamWindow;
      G4bool useUpstreamConductor, useDownstreamConductor;
      G4bool useUpstreamEndFlange, useDownstreamEndFlange;

      enum {VANISHING_FIELD, SIMPLE_FIELD, MAPPED_FIELD, OTHER_FIELD};
      G4int mag_field_type;

      G4double conductorCurrent;
      G4double fieldNormConst;
      G4double currentToMagFieldConversion;

      //      inflectorMessenger *InflectorMessenger;

      bool spin_tracking_;
      connection_t conn_;
      

    };
}

#endif
