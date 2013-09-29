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

#include "gm2geom/calo/CalorimeterGeometry.hh"
using gm2geom::CalorimeterGeometry;
#include "gm2geom/station/StationGeometry.hh"

#include "gm2ringsim/calo/CaloHit.hh"
#include "gm2ringsim/calo/XtalHit.hh"
#include "gm2ringsim/calo/XtalPhotonHit.hh"
#include "gm2ringsim/calo/PhotodetectorHit.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonHit.hh"

#include "gm2ringsim/calo/CaloArtRecord.hh"
#include "gm2ringsim/calo/XtalArtRecord.hh"
#include "gm2ringsim/calo/XtalPhotonArtRecord.hh"
#include "gm2ringsim/calo/PhotodetectorArtRecord.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonArtRecord.hh"

// Constructor for the service
gm2ringsim::Calorimeter::Calorimeter(fhicl::ParameterSet const & p, art::ActivityRegistry & ) :
    DetectorBase(p,
             p.get<std::string>("name", "calorimeter"),
             p.get<std::string>("category", "calorimeter"),
             p.get<std::string>("mother_category", "station")),
    stationGeomName_(p.get<std::string>("stationGeomName", "")), // For calo placement, use a geometry different from the mother category
    killAllParticles_(p.get<bool>("killAllParticles", true)),
    killOpticalPhotons_(p.get<bool>("killOpticalPhotons", true)),
    caloSD_(0),
    xtalSD_(0),
    photodetectorSD_(0)
{
  if ( stationGeomName_.empty() ) {
    stationGeomName_ = motherCategory();
  }
    
    // determine what kinds of hits to produce (always produce calo hits)
    if (killAllParticles_)
    {
        produceXtalHits_ = false;
        producePhotodetectorHits_ = false;
    }
    else if (killOpticalPhotons_)
    {
        produceXtalHits_ = true;
        producePhotodetectorHits_ = false;
    }
    else // all particles are being tracked, including optical photons
    {
        produceXtalHits_ = true;
        producePhotodetectorHits_ = true;
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
    bool diffuser                    = caloGeom.diffuser ;
    double diffuserDepth             = caloGeom.diffuserDepth ;
    G4String frontOpticalMaterial    = caloGeom.frontWrapping ;
    G4String sideOpticalMaterial     = caloGeom.sideWrapping ;
    G4String backOpticalMaterial     = caloGeom.backWrapping ;
    bool frontWrappingHole           = caloGeom.frontWrappingHole ;
    double frontWrappingHoleSize     = caloGeom.frontWrappingHoleSize ;
    
    
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
    // The order is: gap, diffuser, front wrapping, PbF2, optical coupling / backwrapping, photodetector
        
    // --- distance from caloBound center to the center of each volume
    double diffuserDepthCenter = t_2 - wrappingGap - diffuserDepth / 2. ;
    double wrappingDepthCenter = diffuserDepthCenter -
                                ( diffuserDepth + wrappingGap ) / 2. ;
    double xtalDepthCenter = wrappingDepthCenter -
                                ( wrappingGap + xtalDepth ) / 2.;
    double opticalCouplingDepthCenter = xtalDepthCenter -
                                ( xtalDepth + opticalCouplingDepth ) / 2. ;
    double photodetectorDepthCenter = opticalCouplingDepthCenter -
                                (opticalCouplingDepth + photodetectorDepth ) / 2.;
    
    // --- vector for placing each volume inside caloBound
    G4ThreeVector diffuserOrigin( 0., 0., -diffuserDepthCenter );
    G4ThreeVector wrappingOrigin( 0., 0., -wrappingDepthCenter );
    G4ThreeVector xtalOrigin( 0., 0., -xtalDepthCenter ) ;
    G4ThreeVector opticalCouplingOrigin( 0., 0., -opticalCouplingDepthCenter ) ;
    G4ThreeVector photodetectorOrigin( 0., 0., -photodetectorDepthCenter ) ;
    
    // Make the front wrapping volume: a thin box filled with our "optical vacuum"
    // that allows us to specify the front wrapping material for the crystals.
    // The front wrapping volume covers the entire front edge of the crystal
    // array and is wrappingGap deep
    
    // The "optical vacuum" is just vacuum with an index of refraction defined
    // Optical boundary processes don't work correctly unless both materials
    // involved have indices of refraction
    
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
    
    if (diffuser){
        // Make the diffuser volume: unpolished silica diffuser sheet in front of
        // the crystal array -- proposed component of laser calibration system.
        // Separated from the PbF2 crystals by a small gap (the front wrapping
        // volume accomplishes this).
        
        // --- diffuser shape
        G4Box* diffuser_S = new G4Box("diffuser_S",
                                      r_2,
                                      v_2,
                                      diffuserDepth/2);
        // --- diffuser logical volume
        G4LogicalVolume* diffuser_L =
        new G4LogicalVolume( diffuser_S,
                            artg4Materials::Quartz(),
                            "diffuser_L");
        // --- diffuser visual attributes
        artg4::setVisAtts(diffuser_L, caloGeom.displayWrappingVolumes, caloGeom.diffuserColor);
        
        // --- place diffuser volume inside caloBound volume
        G4PVPlacement* diffuser_P =
        new G4PVPlacement( 0,
                          diffuserOrigin,
                          diffuser_L,
                          "diffuser_P",
                          caloBound_L,
                          false,
                          0);

        // Define optical surfaces for the diffuser
        //     polished side facing away from crystals (front surface)
        //     ground side facing toward crystals (back surface)
        
        // --- Diffuser front (smooth)
        G4OpticalSurface* frontDiffuserSurface =
        artg4Materials::findOpticalByName("EtchedGlass");
        new G4LogicalBorderSurface( "SurfaceExitingDiffuserFront",
                                   diffuser_P,
                                   caloBound_P,
                                   frontDiffuserSurface);
        new G4LogicalBorderSurface( "SurfaceEnteringDiffuserFront",
                                   caloBound_P,
                                   diffuser_P,
                                   frontDiffuserSurface);
        // -- Diffuse back (rough)
        G4OpticalSurface* backDiffuserSurface =
        artg4Materials::findOpticalByName("GroundGlass");
        new G4LogicalBorderSurface( "SurfaceExitingDiffuserBack",
                                   diffuser_P,
                                   frontWrapping_P,
                                   backDiffuserSurface);
        new G4LogicalBorderSurface( "SurfaceEnteringDiffuserBack",
                                   frontWrapping_P,
                                   diffuser_P,
                                   backDiffuserSurface);
    }

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
    int nXtalsPerCalo = nXtalRows * nXtalCols;
    
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
        G4ThreeVector frontWrappingHolePos = wrappingGap/4.*zhat + offset ;
        
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
                                  (calorimeterNumber * nXtalsPerCalo) + xtalCount ) ;
            
            if (frontWrappingHole){
                // Create a square hole in the wrapping in front of each crystal.
                //    This volume is thinner than the front wrapping volume and
                //    is flush with the xtal, to allow us to define a different
                //    optical surface at its interface with the xtal volume
                
                // --- front wrapping hole shape
                G4Box* frontWrappingHole_S = new G4Box("frontWrappingHole_S",
                                                       frontWrappingHoleSize / 2.,
                                                       frontWrappingHoleSize / 2.,
                                                       wrappingGap / 4.) ; // half the thickness of front wrapping
                
                // --- front wrapping hole logical volume
                G4LogicalVolume* frontWrappingHole_L =
                new G4LogicalVolume( frontWrappingHole_S,
                                    artg4Materials::Vacuum1(),
                                    "frontWrappingHole_L" );
                
                // --- front wrapping hole visual attributes (same as front wrapping)
                artg4::setVisAtts(frontWrappingHole_L, caloGeom.displayWrappingVolumes, caloGeom.wrappingColor);
                
                // -- place front wrapping hole volume inside front wrapping volume
                G4PVPlacement* frontWrappingHole_P =
                new G4PVPlacement( 0,
                                  frontWrappingHolePos,
                                  frontWrappingHole_L,
                                  "frontWrappingHole_P",
                                  frontWrapping_L,
                                  false,
                                  0);
                
                // --- optical surface for xtal-hole interface
                G4OpticalSurface* wrappingHoleSurface =
                artg4Materials::findOpticalByName("Open");
                //G4LogicalBorderSurface* xtaltoFrontHoleSurface_P =
                new G4LogicalBorderSurface( "PbF2ToFrontHole",
                                           xtal_P,
                                           frontWrappingHole_P,
                                           wrappingHoleSurface);
                //G4LogicalBorderSurface* frontHoletoXtalSurface_P =
                new G4LogicalBorderSurface( "FrontHoleToPbF2",
                                           frontWrappingHole_P,
                                           xtal_P,
                                           wrappingHoleSurface);
            }
            
            // --- declare optical coupling and photodetector logical volumes outside if statement
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
            
            // -- include optical coupling volume with xtalSD
            opticalCoupling_L->SetSensitiveDetector(xtalSD_);
            
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
                                  (calorimeterNumber * nXtalsPerCalo) + xtalCount ) ;

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
                              (calorimeterNumber * nXtalsPerCalo) + xtalCount ) ;
            
            // Define optical surfaces for crystal
            
            // --- Define the optical surface for the long side of crystals
                G4OpticalSurface* sideXtalSurface =
                   artg4Materials::findOpticalByName(sideOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2ExitingSideSurface",
                                           xtal_P,
                                           caloBound_P,
                                           sideXtalSurface);
                G4OpticalSurface* sideXtalReverseSurface =
                artg4Materials::findOpticalByName(sideOpticalMaterial+"Reverse");
                new G4LogicalBorderSurface( "PbF2EnteringSideSurface",
                                           caloBound_P,
                                           xtal_P,
                                           sideXtalReverseSurface);

            
            // --- Define the optical surface for the front wrapping
                G4OpticalSurface* frontXtalSurface =
                    artg4Materials::findOpticalByName(frontOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2ExitingFrontSurface",
                                           xtal_P,
                                           frontWrapping_P,
                                           frontXtalSurface);
                G4OpticalSurface* frontXtalReverseSurface =
                artg4Materials::findOpticalByName(frontOpticalMaterial+"Reverse");
                new G4LogicalBorderSurface( "PbF2EnteringFrontSurface",
                                           frontWrapping_P,
                                           xtal_P,
                                           frontXtalReverseSurface);
            
            // --- Define the optical surface for the back wrapping
                G4OpticalSurface* backXtalSurface = 
                    artg4Materials::findOpticalByName(backOpticalMaterial);
                new G4LogicalBorderSurface( "PbF2ExitingBackSurface",
                                           xtal_P,
                                           backWrapping_P,
                                           backXtalSurface);
                G4OpticalSurface* backXtalReverseSurface =
                artg4Materials::findOpticalByName(backOpticalMaterial+"Reverse");
                new G4LogicalBorderSurface( "PbF2EnteringBackSurface",
                                           backWrapping_P,
                                           xtal_P,
                                           backXtalReverseSurface);
            
            
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
                new G4LogicalBorderSurface( "OpticalCouplingToPbF2",
                                           opticalCoupling_P,
                                           xtal_P,
                                           xtalOpticalCouplingSurface);
            
            
            // --- Increment positions to the next column
            xtalPos              += (xtalWidth + wrappingGap) * xhat ;
            opticalCouplingPos   += (xtalWidth + wrappingGap) * xhat ;
            photodetectorPos     += (xtalWidth + wrappingGap) * xhat ;
            frontWrappingHolePos += (xtalWidth + wrappingGap) * xhat ;
            
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

    // set whether CaloSD allows particles to enter the calorimeter volume for further tracking
    caloSD_->KillShowers(killAllParticles_);
    
    // set whether XtalSD kills optical photon tracks
    xtalSD_->killOpticalPhotons(killOpticalPhotons_);
    
    // make sure sensitive detectors have the right number of xtals/photodetectors
    int nCalo = caloGeom.nCalorimeters;
    int nXtalsPerCalo = caloGeom.nXtalRows * caloGeom.nXtalCols;
    int nXtalsTotal = nCalo * nXtalsPerCalo;
    xtalSD_->setTotalXtals(nXtalsTotal);
    photodetectorSD_->setTotalPhotodetectors(nXtalsTotal);
    xtalSD_->setXtalsPerCalo(nXtalsPerCalo);
    photodetectorSD_->setPhotodetectorsPerCalo(nXtalsPerCalo);

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
    
    CalorimeterGeometry caloGeom; // "calorimeter" is the default parameter
    
    std::vector<G4VPhysicalVolume*> calorimeterPVs;
    calorimeterPVs.resize(lvs().size());

    
    // Calorimeter coordinates:
    //      +x radial out
    //      +y vertical up
    //      +z downstream into calorimeter

    // Station (mother volume) coordinates:
    //      +x radial out
    //      +y downstream into calorimeter
    //      +z vertical down
    
    // position and rotation of calo volume inside mother volume
    G4ThreeVector pos(0.0, 0.0, 0.0);
    G4RotationMatrix* caloRot = new G4RotationMatrix;
    
    if (caloGeom.placeInStation)
    {
        gm2geom::StationGeometry stationGeom(stationGeomName_);
        
        // find the location of the center of the calorimeter volume along the station's thickness
        // (depth) dimension so that front of calo volume is flush with front of station
        G4double centerLocation = (caloGeom.thickness + 2*caloGeom.crystalCaloBuffer - stationGeom.t) / 2;
        pos.setY(centerLocation);
        
        // create the rotation matrix that we need for placing the calo LV's into the stations
        caloRot->rotateX(M_PI/2*rad); // Rotates by pi/2 radians around x-axis
    }
    
    // manual position offset from fcl parameters
    pos.setX( pos.x() + caloGeom.positionOffsetX );
    pos.setY( pos.y() + caloGeom.positionOffsetY );
    pos.setZ( pos.z() + caloGeom.positionOffsetZ );    

    // loop over the logical volumes
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
                                                    calorimeterNum ) );
        
        
        calorimeterNum++;
    }
    return calorimeterPVs;
}


// Declare to Art what we are producing
void gm2ringsim::Calorimeter::doCallArtProduces(art::EDProducer * producer) {
    producer->produces<CaloArtRecordCollection>(category());
    
    if (produceXtalHits_)
    {
        producer->produces<XtalArtRecordCollection>(category());
        producer->produces<XtalPhotonArtRecordCollection>(category());
    }
    if (producePhotodetectorHits_)
    {
        producer->produces<PhotodetectorArtRecordCollection>(category());
        producer->produces<PhotodetectorPhotonArtRecordCollection>(category());
    }
}

// Actually add the data to the event
void gm2ringsim::Calorimeter::doFillEventWithArtHits(G4HCofThisEvent * hc) {
    doFillEventWithCaloHits(hc);
    
    if (produceXtalHits_)
    {
        doFillEventWithXtalHits(hc);
        doFillEventWithXtalPhotonHits(hc);
    }
    
    if (producePhotodetectorHits_)
    {
        doFillEventWithPhotodetectorHits(hc);
        doFillEventWithPhotodetectorPhotonHits(hc);
    }
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
                                    e->local_pos.y(), // vertical coordinate
                                    e->local_pos.z(), // thickness coordinate
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
