// Tracker detector service for 

// Include guards
#ifndef KICKER_HH
#define KICKER_HH

// Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

#include "art/Framework/Core/EDProducer.h"

#include "messagefacility/MessageLogger/MessageLogger.h"

#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4Mag_EqRhs.hh"
#include "Geant4/G4MagIntegratorStepper.hh"
#include "Geant4/G4ChordFinder.hh"

#include "artg4/material/Materials.hh"
#include "gm2ringsim/kicker/KickerField.hh"
#include "gm2ringsim/common/spin/SpinController.hh"
#include "gm2ringsim/actions/SpinTrackingSettings.hh"

#include <vector>

// Get the base class for the service
#include "artg4/Core/DetectorBase.hh"
#include "gm2ringsim/kicker/KickerGeometry.hh"

// Within a namespace
namespace gm2ringsim {

    // The class
    class Kicker : public artg4::DetectorBase {

          public:

        // Constructor
        Kicker(fhicl::ParameterSet const &, art::ActivityRegistry & );

        // We always need a virtual destructor
        virtual ~Kicker() {}

    private:



      SpinTrackingSettings sts_;
      const bool spin_tracking_;
      KickerGeometry kg_;
      enum {KICKER1 = 0, KICKER2 = 1, KICKER3 = 2};
      G4int const numKickers;
      G4int const numKickerObjects;
      enum {KICK_LCR, KICK_SQUARE, KICK_OTHER};
      G4int kickType;
      G4double squareMag[3];
      G4bool initialBuild;

      G4VPhysicalVolume *kicker_P[3][3];

      KickModifier *modifier[3];
      G4MagneticField *kickerMagField[3];
      G4Mag_EqRhs *iEquation[3];
      G4MagIntegratorStepper *iStepper[3];
      G4ChordFinder *iChordFinder[3];
      G4FieldManager *manager[3];
      
      enum {NO_MODIFIER, MORSE_MODIFIER};
      enum kicker_region_type { FIELDREGION, INNERPLATE, OUTERPLATE };
      G4int which_modifier;
      

      // Previously public methods in g2migtrace
      void constructKickers(const std::vector<G4VPhysicalVolume *>& VacH);
      
      /** Interface to set the kicker module voltages in the LCR model. */
      void setKickerHV(G4double, G4double, G4double);
      /** Interface to set the kicker module application percentage; works          
	  in parallel with the kicker HV setting. */
      void setKickPercent(G4double, G4double, G4double);
      /** Jigger the kicker pulse in time */
      void setKickerOffsetTime(G4double, G4double, G4double);
      /** Modify the kicker module capacitance value in the LCR pulse               
	  model. */
      void setCircuitCapacitance(G4double, G4double, G4double);
      /** Modify the kicker module inductance value in the LCR pulse                
	  model. */
      void setCircuitInductance(G4double, G4double, G4double);
      /** Modify the kicker module resistance value in the LCR pulse                
	  model. */
      void setCircuitResistance(G4double, G4double, G4double);
      /** Choose the kicker model.                                                    Available models are LCR and Square. */
      void setKickType(G4String);
      /** Set the kicker field modifier module, which provides geometric            
	  modification of the field distribution.                                   
	  
	  Available modifiers are the unmodified and Morse. */
      void setKickModifier(G4String);
      /** Set the value of the square pulse magnitude. */
      void setSquarePulseMagnitude(G4double, G4double, G4double);
      
      /** Interface to get the current values of the kicker parameters. */
      void getKickerInfo();
      
      /** Interface to the spinController to enable/disable spin tracking           
	  in the fields. */
      void enable_spintracking(bool);
      
      // private methods in g2migtrace
      void buildKickerObjects(std::vector<G4VPhysicalVolume*> const&);
      
      void buildKickerPlates(G4VPhysicalVolume*, G4int, G4int, char *);
      
      void buildKickerFields();
      
      void figureProperOffsetTime();
      void modifyKickerFields();
      


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

    };
}

#endif
