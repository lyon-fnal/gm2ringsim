/** @file Calorimeter_service.cc
 
 Ported to Art from g2migtrace file stationConstruction.cc
    @author Kevin Lynch
    @date 2011
 
 @author Lawrence Gibbons
 @date 2012
 
 @author Robin Bjorkquist
 @date 2013
 */

#include "artg4/material/Materials.hh"
#include "artg4/util/util.hh"

#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "Geant4/G4Box.hh"
#include "Geant4/G4Tubs.hh"
#include "Geant4/G4PVPlacement.hh"
#include "Geant4/G4VisAttributes.hh"
#include "Geant4/G4UserLimits.hh"
#include "Geant4/G4OpticalSurface.hh"
#include "Geant4/G4LogicalBorderSurface.hh"

#include "boost/format.hpp"


#include "Calorimeter_service.hh"

#include "gm2ringsim/calo/CalorimeterGeometry.hh"
#include "gm2ringsim/station/StationGeometry.hh"

#include "gm2ringsim/calo/CaloHit.hh"
#include "gm2ringsim/calo/XtalHit.hh"
#include "gm2ringsim/calo/XtalPhotonHit.hh"
#include "gm2ringsim/calo/PhotodetectorHit.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonHit.hh"

#include "gm2ringsim/calo/arthits/CaloArtRecord.hh"
#include "gm2ringsim/calo/arthits/XtalArtRecord.hh"
#include "gm2ringsim/calo/arthits/XtalPhotonArtRecord.hh"
#include "gm2ringsim/calo/arthits/PhotodetectorArtRecord.hh"
#include "gm2ringsim/calo/arthits/PhotodetectorPhotonArtRecord.hh"

// Constructor for the service
gm2ringsim::Calorimeter::Calorimeter(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
             p.get<std::string>("name", "calorimeter"),
             p.get<std::string>("category", "calorimeter"),
             p.get<std::string>("mother_category", "station")),
    stationGeomName_(p.get<std::string>("stationGeomName", "")), // Use if the mother category isn't a station (e.g. test beam)
    caloSD_(0),
    xtalSD_(0),
    photodetectorSD_(0)
{
  if ( stationGeomName_.empty() ) {
    stationGeomName_ = motherCategory();
  }
}

G4LogicalVolume* gm2ringsim::Calorimeter::makeCalorimeterLV(const CalorimeterGeometry& caloGeom, int calorimeterNumber ) {
    
    // Get dimensions and settings from the CalorimeterGeometry

    double crystalCaloBuffer         = caloGeom.crystalCaloBuffer ;    
    double wrappingGap               = caloGeom.wrappingGap ;
    double xtalWidth                 = caloGeom.xtalWidth ;
    double xtalHeight                = caloGeom.xtalHeight ;
    double xtalDepth                 = caloGeom.xtalDepth ;
    double opticalCouplingDepth      = caloGeom.opticalCouplingDepth ;
    std::string photodetectorShape   = caloGeom.photodetectorShape ;
    double photodetectorSize         = caloGeom.photodetectorSize ;
    double photodetectorDepth        = caloGeom.photodetectorDepth ;
    G4String frontOpticalMaterial = caloGeom.frontWrapping ;
    G4String sideOpticalMaterial  = caloGeom.sideWrapping ;
    G4String backOpticalMaterial  = caloGeom.backWrapping ;
    
    // --- Dimensions of all the stuff that goes inside the calorimeter volume
    G4double const r_2               = caloGeom.radial/2. ;
    G4double const v_2               = caloGeom.vertical/2. ;
    G4double const t_2               = caloGeom.thickness/2. ;
    
    // --- Define coordinate vectors
    G4ThreeVector xhat( 1., 0., 0. ) ;
    G4ThreeVector yhat( 0., 1., 0. ) ;
    G4ThreeVector zhat( 0., 0., 1. ) ;

    // --- Calorimeter coordinates:
    //          +x radial out
    //          +y vertical up
    //          +z downstream into calorimeter
    
    
    // Make the main calo volume
    
    // Dimensions include the "crystalCaloBuffer" on all sides; we want the
    // main calo volume to be slightly larger than its contents so that all
    // particles entering the calorimeter will pass through the calo volume
    // (and be seen by the CaloSD) before entering any sub-volume.

    // --- calo shape
    G4Box *calo_S = new G4Box("calo_S",
                              r_2 + crystalCaloBuffer,
                              v_2 + crystalCaloBuffer,
                              t_2 + crystalCaloBuffer );
    
    // --- calo logical volume
    G4LogicalVolume *calo_L =
    new G4LogicalVolume(calo_S,
                        artg4Materials::Vacuum1(),
                        "calo_L" );
    
    // --- calo visual attributes
    artg4::setVisAtts(calo_L, caloGeom.displayCalorimeterBox, caloGeom.calorimeterColor);
    
    // --- calo sensitive detector
    calo_L->SetSensitiveDetector( caloSD_ );
    
    
    // Now build a volume that is slightly smaller than the calo volume,
    // but will tightly enclose the crystals, photodetectors, and gaps.

    // --- caloBound shape
    G4Box *caloBound_S = new G4Box("caloBound_S",
                                   r_2 ,
                                   v_2 ,
                                   t_2 );
 
    // --- caloBound logical volume
    G4LogicalVolume *caloBound_L = new G4LogicalVolume(caloBound_S,
                                                       artg4Materials::Vacuum1(),
                                                       "caloBound_L");
    
    // --- caloBound visual attributes
    artg4::setVisAtts(caloBound_L, caloGeom.displayCalorimeterBox, caloGeom.calorimeterColor);
    
    // --- place caloBound volume inside calo volume
    G4ThreeVector pos = G4ThreeVector(0,0,0);
    std::string calorimeterLabel( boost::str( boost::format("PbF2Bounding[%02d]") % calorimeterNumber ));
    G4String name = calorimeterLabel;
    G4PVPlacement* caloBound_P =
          new G4PVPlacement(0, pos, caloBound_L, name, calo_L, false, 0);

    
    // Calculate the locations of sub-volumes inside the caloBound volume.
    // The order is: front wrapping, PbF2, optical coupling / backwrapping, photodetector
        
    // --- distance from caloBound center to the center of each volume
    double wrappingDepthCenter = t_2 - wrappingGap / 2. ;
    double xtalDepthCenter = wrappingDepthCenter -
                                ( wrappingGap + xtalDepth ) / 2.;
    double opticalCouplingDepthCenter = xtalDepthCenter -
                                ( xtalDepth + opticalCouplingDepth ) / 2. ;
    double photodetectorDepthCenter = opticalCouplingDepthCenter -
                                (opticalCouplingDepth + photodetectorDepth ) / 2.;
    
    // --- vector for placing each volume inside caloBound
    G4ThreeVector wrappingOrigin( 0., 0., -wrappingDepthCenter );
    G4ThreeVector xtalOrigin( 0., 0., -xtalDepthCenter ) ;
    G4ThreeVector opticalCouplingOrigin( 0., 0., -opticalCouplingDepthCenter ) ;
    G4ThreeVector photodetectorOrigin( 0., 0., -photodetectorDepthCenter ) ;
    

    // Make the front wrapping volume: a thin box filled with our "optical vacuum"
    // that allows us to specify the front wrapping material for the crystals.
    // The front wrapping volume covers the entire front edge of the caloBound
    // volume and is wrappingGap deep
    
    // The "optical vacuum" is just vacuum with an index of refraction defined
    // Optical boundary processes don't work correctly unless both materials
    // invol
    
    // --- front wrapping shape
    G4Box* frontWrapping_S = new G4Box("frontWrapping_S",
                                       r_2,
                                       v_2,
                                       wrappingGap/2);
    // --- front wrapping logical volume
    G4LogicalVolume* frontWrapping_L =  
        new G4LogicalVolume( frontWrapping_S,
                             artg4Materials::Vacuum1(),
                             "frontWrapping_L" );
    // --- front wrapping visual attributes
    artg4::setVisAtts(frontWrapping_L, caloGeom.displayWrappingVolumes, caloGeom.wrappingColor);
    
    // --- place front wrapping volume inside caloBound volume.  
    G4PVPlacement* frontWrapping_P =
        new G4PVPlacement( 0,
                           wrappingOrigin,
                           frontWrapping_L,
                           "frontWrapping_P",
                           caloBound_L,
                           false,
                           0);

    // Make the back wrapping volume: a thin box filled with the optical vacuum
    // that allows us to specify the back wrapping material for the crystals.
    // This volume will contain the optical coupling volumes, which need to
    // be flush with the crystal surfaces. The back wrapping optical surface
    // will only come into effect where photons go from PbF2 to "optical vacuum".
    // This volume is the same depth as the optical coupling.
    
    // --- back wrapping shape
    G4Box* backWrapping_S = new G4Box("backWrapping_S",
                                      r_2,
                                      v_2,
                                      opticalCouplingDepth/2);
    // --- back wrapping logical volume
    G4LogicalVolume* backWrapping_L =
       new G4LogicalVolume( backWrapping_S,
                           artg4Materials::Vacuum1(),
                           "backWrapping_L" );
    
    // --- back wrapping visual attributes
    artg4::setVisAtts(backWrapping_L,  caloGeom.displayWrappingVolumes, caloGeom.wrappingColor);

    // --- place back wrapping volume inside caloBound volume
    G4PVPlacement* backWrapping_P =
       new G4PVPlacement( 0,
                         opticalCouplingOrigin,
                         backWrapping_L,
                         "backWrapping_P",
                         caloBound_L,
                         false,
                         0);

    
    // Place the individual crystals, optical coupling volumes and photodetectors
    
    int xtalCount = 0 ;
    int nXtalRows = caloGeom.nXtalRows;
    int nXtalCols = caloGeom.nXtalCols;
    
    // --- loop over rows, starting with bottom (smallest y coordinate)
    for( int irow = 0 ; irow < nXtalRows ; ++irow )
    {
        // --- calculate x-y position of first xtal inside caloBound volume
        // Note that in the offset, we account for a wrappingGap around the
        // crystals.  This differs from the g2migtrace implementation, where the
        // crystal volumes were made smaller
        G4ThreeVector offset =
          - double( nXtalCols - 1 ) / 2. * (xtalWidth + wrappingGap) * xhat
        - ( double( nXtalRows - 1 ) / 2. - double( irow ) )
                                        * (xtalHeight + wrappingGap) * yhat ;

        // --- coordinates for placing xtal, optical coupling, and photodetectors inside mother volumes
        G4ThreeVector xtalPos = xtalOrigin + offset ;
        G4ThreeVector opticalCouplingPos = offset ; // only the offset, since it goes into the backwrapping volume
        G4ThreeVector photodetectorPos = photodetectorOrigin + offset ;
        
        // --- loop over columns, starting with radial inside (smallest x coord)
        for( int icol = 0 ; icol < nXtalCols; ++icol, ++xtalCount )
        {

            // --- xtal shape
            G4Box* xtal_S = new G4Box("xtal_S",
                                       xtalWidth / 2.,
                                       xtalHeight / 2.,
                                       xtalDepth / 2.) ;
            
            // --- xtal logical volume
            G4LogicalVolume* xtal_L = new G4LogicalVolume( xtal_S,
                                                          artg4Materials::PbF2(),
                                                          "xtal_L" ) ;

            // --- xtal visual attributes
            artg4::setVisAtts(xtal_L, caloGeom.displayCrystalArray, caloGeom.xtalColor);
            
            // --- xtal sensitive detector
            xtal_L->SetSensitiveDetector(xtalSD_);
            
            // --- place xtal volume inside caloBound volume
            std::ostringstream xtalName;
            xtalName << "xtal[" << std::setfill('0') << std::setw(2) << calorimeterNumber << ']';
            G4PVPlacement* xtal_P =
                new G4PVPlacement( 0,
                                  xtalPos,
                                  xtal_L,
                                  xtalName.str(),
                                  caloBound_L,
                                  false,
                                  xtalCount ) ;
            

            // --- declare optical coupling and photodetector logical volumes outside if statemnt
            G4LogicalVolume* opticalCoupling_L ;
            G4LogicalVolume* photodetector_L ;

            // Case 1: square photodetector and optical coupling (e.g., SiPM)
            if( photodetectorShape.compare("Square") == 0 ) {
                // --- optical coupling shape
                G4Box* opticalCoupling_S = new G4Box( "opticalCoupling_S",
                                                      photodetectorSize / 2.,
                                                      photodetectorSize / 2.,
                                                      opticalCouplingDepth / 2. ) ;
                // --- optical coupling logical volume
                opticalCoupling_L = new G4LogicalVolume(opticalCoupling_S,
                                              artg4Materials::BicronBC630(),
                                              "opticalCoupling_L" ) ;
                // --- photodetector shape
                G4Box* photodetector_S = new G4Box( "photodetector_S",
                                                   photodetectorSize / 2.,
                                                   photodetectorSize / 2.,
                                                   photodetectorDepth / 2. ) ;
                // --- photodetector logical volume
                photodetector_L = new G4LogicalVolume( photodetector_S,
                                            artg4Materials::Borosilicate(),
                                            "photodetector_L" ) ;
                
            // Case 2: cylindrical photodetector (e.g., phototube)
            } else if ( photodetectorShape.compare("Circle") == 0 ) {
                // --- optical coupling shape
                G4Tubs* opticalCoupling_S = new G4Tubs( "opticalCoupling_S",
                                                        0, photodetectorSize / 2,
                                                        opticalCouplingDepth / 2,
                                                        0, 360. * deg);
                // --- optical coupling logical volume
                opticalCoupling_L = new G4LogicalVolume( opticalCoupling_S,
                                                         artg4Materials::BicronBC630(),
                                                         "opticalCoupling_L");
                // --- photodetector shape
                G4Tubs* photodetector_S = new G4Tubs( "photodetector_S",
                                                     0, photodetectorSize / 2,
                                                     photodetectorDepth / 2,
                                                     0, 360. * deg);
                // --- photodetector logical volume
                photodetector_L = new G4LogicalVolume( photodetector_S,
                                                      artg4Materials::Borosilicate(),
                                                      "photodetector_L" ) ;
 
            // Whoops! Invalid photodetector shape
            } else {
                throw cet::exception("Unknown photodetector shape: ") << photodetectorShape << " in MakeCalorimeterLV";
            }
            
            // --- optical coupling visual attributes
            artg4::setVisAtts(opticalCoupling_L, caloGeom.displayCrystalArray, caloGeom.opticalCouplingColor);
            
            // --- photodetector visual attributes
            artg4::setVisAtts(photodetector_L, caloGeom.displayCrystalArray, caloGeom.photodetectorColor);
            
            // --- photodetector sensitive detector
            photodetector_L->SetSensitiveDetector(photodetectorSD_);

            // --- place optical coupling volume inside backWrapping volume
            std::ostringstream ocName;
            ocName << "opticalCoupling[" << std::setfill('0') << std::setw(2) << calorimeterNumber << ']';

            G4PVPlacement* opticalCoupling_P =
                new G4PVPlacement(0,
                                  opticalCouplingPos,
                                  opticalCoupling_L,
                                  ocName.str(),
                                  backWrapping_L,
                                  false,
                                  xtalCount ) ;

            // --- place photodetector volume inside caloBound volume
            std::ostringstream pdName;
            pdName << "photodetector[" << std::setfill('0') << std::setw(2) << calorimeterNumber << ']';

            // G4PVPlacement* photodetector_P =
            new G4PVPlacement( 0,
                              photodetectorPos,
                              photodetector_L,
                              pdName.str(),
                              caloBound_L,
                              false,
                              xtalCount ) ;
            
            // --- Define the optical surface for the long side
                G4OpticalSurface* sideXtalSurface =
                   artg4Materials::findOpticalByName(sideOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2SideSurface",
                                           xtal_P,
                                           caloBound_P,
                                           sideXtalSurface);
            
            // --- Define the optical surface for the front wrapping
                G4OpticalSurface* frontXtalSurface =
                    artg4Materials::findOpticalByName(frontOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2FrontSurface",
                                           xtal_P,
                                           frontWrapping_P,
                                           frontXtalSurface);
            
            // --- Define the optical surface for the back wrapping
                G4OpticalSurface* backXtalSurface = 
                    artg4Materials::findOpticalByName(backOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2BackSurface",
                                           xtal_P,
                                           backWrapping_P,
                                           backXtalSurface);
            
            // --- Ensure faceted surface between xtal and optical coupling...
            G4OpticalSurface* xtalOpticalCouplingSurface =
                artg4Materials::findOpticalByName("Open");
            //G4LogicalBorderSurface* xtaltoOpticalCouplingSurface_P =
                new G4LogicalBorderSurface( "PbF2ToOpticalCoupling",
                                           xtal_P,
                                           opticalCoupling_P,
                                           xtalOpticalCouplingSurface);
            // --- ...and between optical coupling and xtal
            //G4LogicalBorderSurface* opticalCouplingtoXtalSurface_P =
                new G4LogicalBorderSurface( "PbF2ToGrease2",
                                           opticalCoupling_P,
                                           xtal_P,
                                           xtalOpticalCouplingSurface);
            
            
            // --- Increment positions to the next column
            xtalPos            += (xtalWidth + wrappingGap) * xhat ;
            opticalCouplingPos += (xtalWidth + wrappingGap) * xhat ;
            photodetectorPos   += (xtalWidth + wrappingGap) * xhat ;
            
        } // end of loop over column
    } // end of loop over rows

    
    // Return the calorimeter mother volume that we have created and filled
    return calo_L;
}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Calorimeter::doBuildLVs() {
    
    // create the calorimeter geometry, which initializes itself from the parameter set
    CalorimeterGeometry caloGeom; // "calorimeter" is the default parameter
    caloGeom.print();
    
    // sensitive detectors
    caloSD_ = artg4::getSensitiveDetector<CaloSD>(getCaloName());
    xtalSD_ = artg4::getSensitiveDetector<XtalSD>(getXtalName());
    photodetectorSD_ = artg4::getSensitiveDetector<PhotodetectorSD>(getPhotodetectorName());

    // set whether CaloSD kills showers
    caloSD_->KillShowers(caloGeom.killShowers);
    
    // make sure sensitive detectors have the right number of xtals/photodetectors
    int nCalo = caloGeom.nCalorimeters;
    int nXtals = nCalo * caloGeom.nXtalRows * caloGeom.nXtalCols;
    xtalSD_->setXtalNum(nXtals);
    photodetectorSD_->setPhotodetectorNum(nXtals);

    // Create the vector of logical volumes
    std::vector<G4LogicalVolume*> calorimeterLVs;
    
    // Build the logical volumes
    for (  int calorimeterNumber = 0; calorimeterNumber != nCalo; ++calorimeterNumber ) {
        // Push this into the vector
        calorimeterLVs.push_back( makeCalorimeterLV(caloGeom, calorimeterNumber ));
    }
    
    return calorimeterLVs;
    
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Calorimeter::doPlaceToPVs( std::vector<G4LogicalVolume*> stations) {
    
    // Calorimeter coordinates:
    //      +x radial out
    //      +y vertical up
    //      +z downstream into calorimeter

    // Station (mother volume) coordinates:
    //      +x radial out
    //      +y downstream into calorimeter
    //      +z vertical down
    
    StationGeometry stationGeom(stationGeomName_);
    CalorimeterGeometry caloGeom; // "calorimeter" is the default parameter
    
/*    if ( stations.size() != lvs().size() ) {
        throw cet::exception("# of Calorimeter logical volumes (")
           << lvs().size()
           << ") differs from # of stations (" << stations.size() << ")";
    }
 */
    
    std::vector<G4VPhysicalVolume*> calorimeterPVs;
    calorimeterPVs.resize(lvs().size());
    

    
    // find the location of the center of the calorimeter volume along the station's thickness
    // (depth) dimension
    G4double centerLocation = (caloGeom.thickness + 2*caloGeom.crystalCaloBuffer - stationGeom.t) / 2;
    G4ThreeVector pos( 0, centerLocation, 0 );
    
    // create the rotation matrix that we need for placing the calo LV's into the stations
    G4RotationMatrix* caloRot = new G4RotationMatrix;
    caloRot->rotateX(M_PI/2*rad); //Rotates by pi/2 radians around x-axis

    //loop over the logical volumes
    unsigned int calorimeterNum = 0;
    for ( auto aCalorimeterLV : lvs() ) {
        
        // We must name the calorimeter including its calorimeter number
        std::string calorimeterLabel( boost::str( boost::format("CalorimeterNumber[%02d]") % calorimeterNum ));


        calorimeterPVs.push_back( new G4PVPlacement(caloRot,
                                                    pos,
                                                    aCalorimeterLV,
                                                    calorimeterLabel,
                                                    stations[ calorimeterNum ],
                                                    false,
                                                    0 ) );
        
        
        calorimeterNum++;
    }
    return calorimeterPVs;
}


// Declare to Art what we are producing
void gm2ringsim::Calorimeter::doCallArtProduces(art::EDProducer * producer) {
    producer->produces<CaloArtRecordCollection>(category());
    producer->produces<XtalArtRecordCollection>(category());
    producer->produces<XtalPhotonArtRecordCollection>(category());
    producer->produces<PhotodetectorArtRecordCollection>(category());
    producer->produces<PhotodetectorPhotonArtRecordCollection>(category());
}

// Actually add the data to the event
void gm2ringsim::Calorimeter::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    doFillEventWithCaloHits(hc);
    doFillEventWithXtalHits(hc);
    doFillEventWithXtalPhotonHits(hc);
    doFillEventWithPhotodetectorHits(hc);
    doFillEventWithPhotodetectorPhotonHits(hc);
}

void gm2ringsim::Calorimeter::doFillEventWithCaloHits(G4HCofThisEvent * hc) {
    std::unique_ptr<CaloArtRecordCollection> myArtHits(new CaloArtRecordCollection);
    
    // Find the collection ID for the hits
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    
    // The string here is unfortunately a magic constant. It's the string used
    // by the sensitive detector to identify the collection of hits.
    G4int collectionID = fSDM->GetCollectionID(getCaloName());
    CaloHitsCollection* myCollection =
    static_cast<CaloHitsCollection*>(hc->GetHC(collectionID));
    
    // Check whether the collection exists
    if (NULL != myCollection) {
        std::vector<CaloHit*> geantHits = *(myCollection->GetVector());
        
        for ( auto e : geantHits ) {
            // Copy this hit into the Art hit
            myArtHits->emplace_back( e->turnNum,
                                    e->caloNum,
                                    e->trackID,
                                    e->global_pos.x(), // radial coordinate
                                    e->global_pos.y(), // thickness coordinate
                                    e->global_pos.z(), // vertical coordinate
                                    e->time,
                                    e->global_mom.x(),
                                    e->global_mom.y(),
                                    e->global_mom.z() );
        }
    }
    else {
        throw cet::exception("Calorimeter") << "Null collection of Geant calorimeter hits"
        << ", aborting!" << std::endl;
    }
    
    // Now that we have our collection of artized hits, add them to the event.
    // Get the event from the detector holder service
    art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
    art::Event & e = detectorHolder -> getCurrArtEvent();
    
    // Put the hits into the event
    e.put(std::move(myArtHits), category());
    
}

void gm2ringsim::Calorimeter::doFillEventWithXtalHits(G4HCofThisEvent * hc) {
    std::unique_ptr<XtalArtRecordCollection> myArtHits(new XtalArtRecordCollection);
    
    // Find the collection ID for the hits
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    
    // The string here is unfortunately a magic constant. It's the string used
    // by the sensitive detector to identify the collection of hits.
    G4int collectionID = fSDM->GetCollectionID(getXtalName());
    XtalHitsCollection* myCollection =
    static_cast<XtalHitsCollection*>(hc->GetHC(collectionID));
    
    // Check whether the collection exists
    if (NULL != myCollection) {
        std::vector<XtalHit*> geantHits = *(myCollection->GetVector());
        
        for ( auto e : geantHits ) {
            // Copy this hit into the Art hit
            myArtHits->emplace_back( e->turnNum,
                                    e->caloNum,
                                    e->xtalNum,
                                    e->trackID,
                                    e->parentID,
                                    e->local_pos.x(), // radial coordinate
                                    e->local_pos.z(), // thickness coordinate
                                    e->local_pos.y(), // vertical coordinate
                                    e->time,
                                    e->local_mom.x(),
                                    e->local_mom.z(),
                                    e->local_mom.y(),
                                    e->energy,
                                    e->energyDep,
                                    e->trackLength,
                                    e->pdgID,
                                    e->nphoton,
                                    e->ephoton );
        }
    }
    else {
        throw cet::exception("Xtal") << "Null collection of Geant xtal hits"
        << ", aborting!" << std::endl;
    }
    
    // Now that we have our collection of artized hits, add them to the event.
    // Get the event from the detector holder service
    art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
    art::Event & e = detectorHolder -> getCurrArtEvent();
    
    // Put the hits into the event
    e.put(std::move(myArtHits), category());
    
}

void gm2ringsim::Calorimeter::doFillEventWithXtalPhotonHits(G4HCofThisEvent * hc) {
    std::unique_ptr<XtalPhotonArtRecordCollection>
                    myArtHits(new XtalPhotonArtRecordCollection);
    
    // Find the collection ID for the hits
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    
    // The string here is unfortunately a magic constant. It's the string used
    // by the sensitive detector to identify the collection of hits.
    G4int collectionID = fSDM->GetCollectionID(addPhotonToName(getXtalName()));
    XtalPhotonHitsCollection* myCollection =
    static_cast<XtalPhotonHitsCollection*>(hc->GetHC(collectionID));
    
    // Check whether the collection exists
    if (NULL != myCollection) {
        std::vector<XtalPhotonHit*> geantHits = *(myCollection->GetVector());
        
        for ( auto e : geantHits ) {
            // Copy this hit into the Art hit
            myArtHits->emplace_back( e->caloNum,
                                    e->xtalNum,
                                    e->trackID,
                                    e->local_pos.x(), // radial coordinate
                                    e->local_pos.z(), // thickness coordinate
                                    e->local_pos.y(), // vertical coordinate
                                    e->cosTheta,
                                    e->phi,
                                    e->energy,
                                    e->transmitted,
                                    e->detected );
        }
    }
    else {
        throw cet::exception("Xtal") << "Null collection of Geant xtal photon hits"
        << ", aborting!" << std::endl;
    }
    
    // Now that we have our collection of artized hits, add them to the event.
    // Get the event from the detector holder service
    art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
    art::Event & e = detectorHolder -> getCurrArtEvent();
    
    // Put the hits into the event
    e.put(std::move(myArtHits), category());
    
}

void gm2ringsim::Calorimeter::doFillEventWithPhotodetectorHits(G4HCofThisEvent * hc) {
    std::unique_ptr<PhotodetectorArtRecordCollection> myArtHits(new PhotodetectorArtRecordCollection);
    
    // Find the collection ID for the hits
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    
    // The string here is unfortunately a magic constant. It's the string used
    // by the sensitive detector to identify the collection of hits.
    G4int collectionID = fSDM->GetCollectionID(getPhotodetectorName());
    PhotodetectorHitsCollection* myCollection =
    static_cast<PhotodetectorHitsCollection*>(hc->GetHC(collectionID));
    
    // Check whether the collection exists
    if (NULL != myCollection) {
        std::vector<PhotodetectorHit*> geantHits = *(myCollection->GetVector());
        
        for ( auto e : geantHits ) {
            // Copy this hit into the Art hit
            myArtHits->emplace_back( e->turnNum,
                                    e->caloNum,
                                    e->photodetectorNum,
                                    e->trackID,
                                    e->local_pos.x(), // radial coordinate
                                    e->local_pos.z(), // thickness coordinate
                                    e->local_pos.y(), // vertical coordinate
                                    e->time,
                                    e->nphoton,
                                    e->energy );
        }
    }
    else {
        throw cet::exception("Photodetector") << "Null collection of Geant photodetector hits"
        << ", aborting!" << std::endl;
    }
    
    // Now that we have our collection of artized hits, add them to the event.
    // Get the event from the detector holder service
    art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
    art::Event & e = detectorHolder -> getCurrArtEvent();
    
    // Put the hits into the event
    e.put(std::move(myArtHits), category());
    
}

void gm2ringsim::Calorimeter::doFillEventWithPhotodetectorPhotonHits(G4HCofThisEvent * hc) {
    std::unique_ptr<PhotodetectorPhotonArtRecordCollection> myArtHits(new PhotodetectorPhotonArtRecordCollection);
    
    // Find the collection ID for the hits
    G4SDManager* fSDM = G4SDManager::GetSDMpointer();
    
    // The string here is unfortunately a magic constant. It's the string used
    // by the sensitive detector to identify the collection of hits.
    G4int collectionID = fSDM->GetCollectionID(addPhotonToName(getPhotodetectorName()));
    PhotodetectorPhotonHitsCollection* myCollection =
    static_cast<PhotodetectorPhotonHitsCollection*>(hc->GetHC(collectionID));
    
    // Check whether the collection exists
    if (NULL != myCollection) {
        std::vector<PhotodetectorPhotonHit*> geantHits = *(myCollection->GetVector());
        
        for ( auto e : geantHits ) {
            // Copy this hit into the Art hit
            myArtHits->emplace_back( e->turnNum,
                                    e->caloNum,
                                    e->photodetectorNum,
                                    e->trackID,
                                    e->local_pos.x(), // radial coordinate
                                    e->local_pos.z(), // thickness coordinate
                                    e->local_pos.y(), // vertical coordinate
                                    e->time,
                                    e->local_mom.x(),
                                    e->local_mom.z(),
                                    e->local_mom.y(),
                                    e->energy,
                                    e->accepted );
        }
    }
    else {
        throw cet::exception("Photodetector") << "Null collection of Geant photodetector photon hits"
        << ", aborting!" << std::endl;
    }
    
    // Now that we have our collection of artized hits, add them to the event.
    // Get the event from the detector holder service
    art::ServiceHandle<artg4::DetectorHolderService> detectorHolder;
    art::Event & e = detectorHolder -> getCurrArtEvent();
    
    // Put the hits into the event
    e.put(std::move(myArtHits), category());
    
}


using gm2ringsim::Calorimeter;
DEFINE_ART_SERVICE(Calorimeter)
