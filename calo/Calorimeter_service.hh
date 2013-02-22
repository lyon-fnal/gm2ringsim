//
//  Calorimeter_service.h
//  g2ringsim_xcode
//
//  Created by Lawrence Gibbons on 12/21/12.
//  Copyright (c) 2012 Lawrence Gibbons. All rights reserved.
//

// Include guards
#ifndef __g2ringsim__Calorimeter_service__
#define __g2ringsim__Calorimeter_service__

#include <iostream>
#include <vector>

#include "gm2ringsim/calo/PhotodetectorSD.hh"

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
    class CalorimeterGeometry;
    
    // The class
    class Calorimeter : public artg4::DetectorBase {
        
    public:
        
        // Constructor
        Calorimeter(fhicl::ParameterSet const &, art::ActivityRegistry & );
        
        // We always need a virtual destructor
        virtual ~Calorimeter() {};
        
        // Return name for photodetector hit collection
        static G4String getPhotodetectorName() {return "PhotodetectorSD";}
        
        // Add "photon" to name
        static G4String addPhotonToName(G4String name) {return "photon" + name;}
        
    private:
        
        G4String photodetectorSDname_;
        PhotodetectorSD *photodetectorSD_;
        
        // Private overriden methods
        
        // Create the logical volumes
        virtual std::vector<G4LogicalVolume*> doBuildLVs() override;
        
        // Create the physical volumes
        virtual std::vector<G4VPhysicalVolume*> doPlaceToPVs( std::vector<G4LogicalVolume*>) override;
        
        // Convenience functions
        G4LogicalVolume* makeCalorimeterLV(CalorimeterGeometry const &caloGeom, int calorimeterNumber);
        

        // Tell Art what we'll produce
        virtual void doCallArtProduces(art::EDProducer * producer) override;
        
        // Actually add the data to the event
        virtual void doFillEventWithArtHits(G4HCofThisEvent * hc) override;
        
    };
}

#endif /* defined(__g2ringsim__Calorimeter_service__) */
