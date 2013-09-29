/** @file Calorimeter_service.hh
 
    Ported to Art from g2migtrace file stationConstruction.hh
        @author Kevin Lynch
        @date 2011
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013
 */

// Include guards
#ifndef __g2ringsim__Calorimeter_service__
#define __g2ringsim__Calorimeter_service__

#include <iostream>
#include <vector>

#include "gm2ringsim/calo/CaloSD.hh"
#include "gm2ringsim/calo/XtalSD.hh"
#include "gm2ringsim/calo/PhotodetectorSD.hh"
#include "gm2geom/calo/CalorimeterGeometry.hh"

// Art Includes
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Core/EDProducer.h"
#include "messagefacility/MessageLogger/MessageLogger.h"


// Geant 4 includes
#include "Geant4/G4HCofThisEvent.hh"
#include "Geant4/G4LogicalVolume.hh"
#include "Geant4/G4VPhysicalVolume.hh"


// the base class for the service
#include "artg4/Core/DetectorBase.hh"

// Within a namespace
namespace gm2ringsim {
    
    // The class
    class Calorimeter : public artg4::DetectorBase {
        
    public:
        
        // Constructor
        Calorimeter(fhicl::ParameterSet const &, art::ActivityRegistry & );
        
        // We always need a virtual destructor
        virtual ~Calorimeter() {};
        
        // Return names for hit collections
        static G4String getCaloName() {return "CaloSD";}
        static G4String getXtalName() {return "XtalSD";}
        static G4String getPhotodetectorName() {return "PhotodetectorSD";}
        
        // Add "photon" to name
        static G4String addPhotonToName(G4String name) {return "photon" + name;}
        
    private:
      
        std::string stationGeomName_;

        bool killAllParticles_;
        bool killOpticalPhotons_;
        
        bool produceXtalHits_;
        bool producePhotodetectorHits_;
        
        CaloSD *caloSD_;
        XtalSD *xtalSD_;
        PhotodetectorSD *photodetectorSD_;
        
        // Private overriden methods
        
        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
        
        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
        
        // Convenience functions
        G4LogicalVolume* makeCalorimeterLV(gm2geom::CalorimeterGeometry const &caloGeom, int calorimeterNumber);
        

        // Tell Art what we'll produce
        virtual void doCallArtProduces(art::EDProducer * producer) override;
        
        // Actually add the data to the event
        virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
        
        // Add calorimeter hits to the event
        void doFillEventWithCaloHits(G4HCofThisEvent * hc);

        // Add xtal hits to the event
        void doFillEventWithXtalHits(G4HCofThisEvent * hc);
        
        // Add xtal photon hits to the event
        void doFillEventWithXtalPhotonHits(G4HCofThisEvent *hc);

        // Add photodetector hits to the event
        void doFillEventWithPhotodetectorHits(G4HCofThisEvent * hc);
        
        // Add photodetector photon hits to the event
        void doFillEventWithPhotodetectorPhotonHits(G4HCofThisEvent *hc);
        
    };
}

#endif /* defined(__g2ringsim__Calorimeter_service__) */
