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

#include "gm2ringsim/calo/PhotodetectorHit.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonHit.hh"
#include "gm2ringsim/calo/PhotodetectorArtRecord.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonArtRecord.hh"

// Constructor for the service
gm2ringsim::Calorimeter::Calorimeter(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
DetectorBase(p,
             p.get<std::string>("name", "calorimeter"),
             p.get<std::string>("category", "calorimeter"),
             p.get<std::string>("mother_category", "station")),
    photodetectorSD_(artg4::getSensitiveDetector<PhotodetectorSD>(getPhotodetectorName()))
{}

G4LogicalVolume* gm2ringsim::Calorimeter::makeCalorimeterLV(const CalorimeterGeometry& caloGeom, int calorimeterNumber ) {
    
    // 1/2 widths of the calorimeter box dimensions.  It includes space for a 
    // small buffer between this volume and the enclosed crystals, as well as
    // a gap between the crystals to enable the optical surface wrappings
    G4double const r_2 = caloGeom.radial/2.    + caloGeom.crystalCaloBuffer;
    G4double const t_2 = caloGeom.thickness/2. + caloGeom.crystalCaloBuffer;
    G4double const v_2 = caloGeom.vertical/2.  + caloGeom.crystalCaloBuffer;
    G4Box *calo_S = new G4Box("calo_S", r_2, v_2, t_2 );
                              
    G4LogicalVolume *calo_L =
    new G4LogicalVolume(calo_S,
                        artg4Materials::Vacuum1(),
                        "calo_L" );
    
    
    artg4::setVisAtts(calo_L, caloGeom.displayCalorimeterBox, caloGeom.calorimeterColor);
    
    // we will continue to make the full calorimeter here, rather than having
    // having many services for each layer of volume
    
    
    // 1st build a volume that that let's us the optical surfaces define surfaces
    //
    // Coordinates are weird: +y is the vertical direction, +z is into calo,
    // +x is still radially out

    G4Box *caloBound_S = new G4Box("caloBound_S",
                                   r_2,
                                   v_2,
                                   t_2 );
 
    G4LogicalVolume *caloBound_L = new G4LogicalVolume(caloBound_S,
                                                       artg4Materials::Vacuum1(),
                                                       "caloBound_L");
    artg4::setVisAtts(caloBound_L, caloGeom.displayCalorimeterBox, caloGeom.calorimeterColor);
    
    // place the physical bounding volume
    G4ThreeVector pos = G4ThreeVector(0,0,0);
    std::string calorimeterLabel( boost::str( boost::format("PbF2Bounding[%02d]") % calorimeterNumber ));
    G4String name = calorimeterLabel;
    // Place the bounding volume.
    G4PVPlacement* caloBound_P =
          new G4PVPlacement(0, pos, caloBound_L, name, calo_L, false, 0);

    
    // Now add the array of actual PbF2 crystals
    double wrappingGap             = caloGeom.wrappingGap ;
    double xtalWidth               = caloGeom.xtalWidth;
    double xtalHeight              = caloGeom.xtalHeight;
    double xtalDepth               = caloGeom.xtalDepth ;
    double opticalCouplingDepth    = caloGeom.opticalCouplingDepth ;
    double photodetectorDepth      = caloGeom.photodetectorDepth ;
    std::string photodetectorShape = caloGeom.photodetectorShape ;
    double photodetectorSize       = caloGeom.photodetectorSize ;
    

    G4ThreeVector xhat( 1., 0., 0. ) ;
    G4ThreeVector yhat( 0., 1., 0. ) ;
    G4ThreeVector zhat( 0., 0., 1. ) ;

    double wrappingDepth = caloGeom.crystalCaloBuffer;
    double totalDepth = wrappingDepth + xtalDepth + opticalCouplingDepth
                      + photodetectorDepth;
    if ( totalDepth > 2*t_2 ) {
        throw cet::exception("Daughters of Calorimeter bounding volume exceed depth of calo volume") << "\n   total daughter depth = " << totalDepth / cm
                          << " cm \n    bounding volume depth = " << 2*t_2 / cm << " cm";
    }

    // Xtal optical surfaces.  Sides, front and back can all be specified independently
    G4String frontOpticalMaterial = caloGeom.frontWrapping;
    G4String sideOpticalMaterial  = caloGeom.sideWrapping;
    G4String backOpticalMaterial  = caloGeom.backWrapping;

    
    // create the locations of the wrapping and crystal coordinate systems
    // in the bounding box mother volume
    // --- front wrapping
    double wrappingDepthCenter = t_2 - wrappingDepth / 2. ;
    G4ThreeVector wrappingOrigin( 0., 0., -wrappingDepthCenter );
    // --- PbF2 crystals
    double xtalDepthCenter = wrappingDepthCenter - ( wrappingDepth + xtalDepth ) / 2.;
    G4ThreeVector xtalOrigin( 0., 0., -xtalDepthCenter ) ;
    // --- crystal / photodetector coupling goo
    double opticalCouplingDepthCenter = xtalDepthCenter -
                                      ( xtalDepth + opticalCouplingDepth ) / 2. ;
    G4ThreeVector opticalCouplingOrigin( 0., 0., -opticalCouplingDepthCenter ) ;
    // --- photodetector
    double photodetectorDepthCenter = opticalCouplingDepthCenter -
                                     (opticalCouplingDepth + photodetectorDepth ) / 2.;
    G4ThreeVector photodetectorOrigin( 0., 0., -photodetectorDepthCenter ) ;

    
    // make a thin box filled with our "optical vacuum" that allows us to
    // specify the front wrapping surface for the crystals.  The front wrapping
    // volume covers the entire front edge of the  caloBound volume and is
    // wrappingDepth deep:
    // -- the box
    G4Box* frontWrapping_S = new G4Box("frontWrapping_S",
                                       caloGeom.radial/2,
                                       caloGeom.vertical/2,
                                       wrappingDepth/2);
    // -- the logical volume
    G4LogicalVolume* frontWrapping_L = new G4LogicalVolume( frontWrapping_S,
                                                           artg4Materials::Vacuum1(),
                                                           "frontWrapping_L" );

    // Place the front wrapping volume at the front of the calo mother volume.  
    G4PVPlacement* frontWrapping_P =
       new G4PVPlacement( 0,
                         wrappingOrigin,
                         frontWrapping_L,
                         "frontWrapping_P",
                         caloBound_L,
                         false,
                         0);

    // now make a thin volume that is nominally filled with the optical vacuum
    // so that we can specify the back face optical surface.  This will also
    // hold the optical coupling volumes, so that we get the correct optical
    // properties between crystal and couplant (the back wrapping will only
    // come into effect where photons go from PbF2 to "optical vacuum".  This
    // volume also covers the entire caloBound surface
    // --- the shape
    G4Box* backWrapping_S = new G4Box("backWrapping_S",
                                      caloGeom.radial/2,
                                      caloGeom.vertical/2,
                                      opticalCouplingDepth/2);
    // --- now the logical volume
    G4LogicalVolume* backWrapping_L =
       new G4LogicalVolume( backWrapping_S,
                           artg4Materials::Vacuum1(),
                           "backWrapping_L" );
    // --- and placed into the caloBound volume against the very back
    // The physical volume gets registered, and we never
    // need a local version of pointer, so simply do a "new" without assignment
    G4PVPlacement* backWrapping_P =
       new G4PVPlacement( 0,
                         opticalCouplingOrigin,
                         backWrapping_L,
                         "backWrapping_P",
                         caloBound_L,
                         false,
                         0);


    // Color the front and back wrapping volumes cyan so we can ensure they
    // are in the right spot:
    artg4::setVisAtts(frontWrapping_L, caloGeom.displayWrappingVolumes, caloGeom.wrappingColor);
    artg4::setVisAtts(backWrapping_L,  caloGeom.displayWrappingVolumes, caloGeom.wrappingColor);

    // now place the individual crystals, optical coupling volumes and photodetectors
    int xtalCount = 0 ;
    int nXtalRows = caloGeom.nXtalRows;
    int nXtalCols = caloGeom.nXtalCols;
    for( int irow = 0 ; irow < nXtalRows ; ++irow )
    {
        // note that in the offset, we account for a wrappingGap around the
        // crystals.  This differs from the g2migtrace implementation, where the
        // crystal volumes were made smaller
        G4ThreeVector offset = -( double( nXtalCols - 1 ) ) / 2.
        * (xtalWidth + wrappingGap) * xhat
        + ( double( nXtalRows - 1 ) / 2. - double( irow ) )
        * (xtalHeight + wrappingGap) * yhat ;

        G4ThreeVector xtalPos            = xtalOrigin             + offset ;
        G4ThreeVector opticalCouplingPos = offset ; // only the offset, since it goes into the backwrapping volume
        G4ThreeVector photodetectorPos   = photodetectorOrigin    + offset ;
        
        for( int icol = 0 ; icol < nXtalCols; ++icol, ++xtalCount )
        {
            // Coordinates are weird: +y is the vertical direction, +z is into calo,
            // +x is still radially out, so y and z are switched in many places
            
            // Xtal volume -- need 2*epsilon gap on the sides for reflective surface
            G4Box* xtal_S = new G4Box("xtal_S",
                                       xtalWidth / 2.,
                                       xtalHeight / 2.,
                                       xtalDepth / 2.) ;
            G4LogicalVolume* xtal_L = new G4LogicalVolume( xtal_S,
                                                          artg4Materials::PbF2(),
                                                          "xtal_L" ) ;

            artg4::setVisAtts(xtal_L, caloGeom.displayCrystalArray, caloGeom.xtalColor);

            
            // Photodetector and optical coupling volumes
            
            // Declare variables to avoid them going out of scope after the if/else,
            // which will define the shapes (cylinder for phototube, square for sipm)
            G4LogicalVolume* opticalCoupling_L ;
            G4LogicalVolume* photodetector_L ;

            // Case 1: square photodetector (and optical coupling) -- a la SiPM
            if( photodetectorShape.compare("Square") == 0 ) {
                // --- optical coupling volume: bicron grease, epoxy, or ...
                G4Box* opticalCoupling_S = new G4Box( "opticalCoupling_S",
                                                      photodetectorSize / 2.,
                                                      photodetectorSize / 2.,
                                                      opticalCouplingDepth / 2. ) ;
                opticalCoupling_L = new G4LogicalVolume(opticalCoupling_S,
                                              artg4Materials::BicronBC630(),
                                              "opticalCoupling_L" ) ;
                
                // --- now photodetector volume
                G4Box* photodetector_S = new G4Box( "photodetector_S",
                                                   photodetectorSize / 2.,
                                                   photodetectorSize / 2.,
                                                   photodetectorDepth / 2. ) ;
                photodetector_L = new G4LogicalVolume( photodetector_S,
                                            artg4Materials::Borosilicate(),
                                            "photodetector_L" ) ;
                
            // Casee 2: cylindrical photodetector
            } else if ( photodetectorShape.compare("Circle") == 0 ) {
                G4Tubs* opticalCoupling_S = new G4Tubs( "opticalCoupling_S",
                                                        0, photodetectorSize / 2,
                                                        opticalCouplingDepth / 2,
                                                        0, 360. * deg);
                opticalCoupling_L = new G4LogicalVolume( opticalCoupling_S,
                                                         artg4Materials::BicronBC630(),
                                                         "opticalCoulpling_L");

                G4Tubs* photodetector_S = new G4Tubs( "photodetector_S",
                                                     0, photodetectorSize / 2,
                                                     photodetectorDepth / 2,
                                                     0, 360. * deg);
                photodetector_L = new G4LogicalVolume( photodetector_S,
                                                      artg4Materials::Borosilicate(),
                                                      "photodetector_L" ) ;
 
            // Whoops! Wrong shape specified
            } else {
                throw cet::exception("Unknown photodetector shape: ") << photodetectorShape << " in MakeCalorimeterLV";
            }
            
            artg4::setVisAtts(opticalCoupling_L, caloGeom.displayCrystalArray, caloGeom.opticalCouplingColor);
            artg4::setVisAtts(photodetector_L, caloGeom.displayCrystalArray, caloGeom.photodetectorColor);

            
            // Now place the photodetector and optical coupling volumes
            std::ostringstream ocName;
            ocName << "opticalCoupling[" << std::setfill('0') << std::setw(2) << calorimeterNumber+1 << ']';

            // G4PVPlacement* opticalCoupling_P =
            new G4PVPlacement(0,
                              opticalCouplingPos,
                              opticalCoupling_L,
                              ocName.str(),
                              backWrapping_L,
                              false,
                              xtalCount ) ;

            std::ostringstream pdName;
            pdName << "photodetector[" << std::setfill('0') << std::setw(2) << calorimeterNumber+1 << ']';
            // G4PVPlacement* pmt_P =
            new G4PVPlacement( 0,
                              photodetectorPos,
                              photodetector_L,
                              pdName.str(),
                              caloBound_L,
                              false,
                              xtalCount ) ;
            
            
            // Finally place the crystal itself
            std::ostringstream xtalName;
            xtalName << "xtal[" << std::setfill('0') << std::setw(2) << calorimeterNumber+1 << ']';
            G4PVPlacement* xtal_P =
            new G4PVPlacement( 0,
                              xtalPos,
                              xtal_L,
                              xtalName.str(),
                              caloBound_L,
                              false,
                              xtalCount ) ;
            
            // Increment the positions to the end of the next crystal:
            xtalPos            += (xtalWidth + wrappingGap) * xhat ;
            opticalCouplingPos += (xtalWidth + wrappingGap) * xhat ;
            photodetectorPos   += (xtalWidth + wrappingGap) * xhat ;
        
            
            // Still need to set the sensitive detector here!
            photodetector_L->SetSensitiveDetector(photodetectorSD_);
            
            // Wrap the crystals with materials specified:
            // --- Define the optical surface for the long side :
            if( sideOpticalMaterial.compare("None") != 0 ) {
                G4OpticalSurface* sideXtalSurface =
                   artg4Materials::findOpticalByName(sideOpticalMaterial);
                // G4LogicalBorderSurface* sideXtalSurface_P =
                new G4LogicalBorderSurface( "PbF2SideSurface",
                                           xtal_P,
                                           caloBound_P,
                                           sideXtalSurface);
            }
            
            // Define the optical surface for the front wrapping:
            if( frontOpticalMaterial.compare("None") != 0 ) {
                G4OpticalSurface* frontXtalSurface =
                artg4Materials::findOpticalByName(frontOpticalMaterial);
                // G4LogicalBorderSurface* frontXtalSurface_P =
                new G4LogicalBorderSurface( "PbF2FrontSurface",
                                           xtal_P,
                                           frontWrapping_P,
                                           frontXtalSurface);
            }
            // Define the optical surface for the back wrapping:
            if( backOpticalMaterial.compare("None") != 0 ) {
                G4OpticalSurface* backXtalSurface = 
                artg4Materials::findOpticalByName(backOpticalMaterial);
                // G4LogicalBorderSurface* backXtalSurface_P =
                new G4LogicalBorderSurface( "PbF2BackSurface",
                                           xtal_P,
                                           backWrapping_P,
                                           backXtalSurface);
            }
            
        } // end of loop over column
        
        
    } // end of loop over rows (irow)

    // finally, return the calorimeter mother volume that we have created and filled
    return calo_L;

}

// Build the logical volumes
std::vector<G4LogicalVolume *> gm2ringsim::Calorimeter::doBuildLVs() {
    
    // create the calorimeter geometry, which initializes itself from the parameter set
    CalorimeterGeometry caloGeom(myName());
    caloGeom.print();
    

    // Create the vector of logical volumes
    std::vector<G4LogicalVolume*> calorimeterLVs;
    
    // Build the logical volumes
    for ( unsigned int calorimeterNumber = 0; calorimeterNumber != 24; ++calorimeterNumber ) {
        // Push this into the vector
        calorimeterLVs.push_back( makeCalorimeterLV(caloGeom, calorimeterNumber ));
    }
    
    return calorimeterLVs;
    
}

// Build the physical volumes
std::vector<G4VPhysicalVolume *> gm2ringsim::Calorimeter::doPlaceToPVs( std::vector<G4LogicalVolume*> stations) {
    
    StationGeometry stationGeom(motherCategory());
    CalorimeterGeometry caloGeom(myName());
    
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
    G4double centerLocation = (caloGeom.thickness + caloGeom.wrappingGap - stationGeom.t) / 2;
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
    producer->produces<PhotodetectorArtRecordCollection>(category());
    producer->produces<PhotodetectorPhotonArtRecordCollection>(category());
}

// Actually add the data to the event
void gm2ringsim::Calorimeter::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    doFillEventWithPhotodetectorHits(hc);
    doFillEventWithPhotodetectorPhotonHits(hc);
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
