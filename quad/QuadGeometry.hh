//  QuadGeometry.hh
//  @author Brendan Kiburg 
//  @date Dec 2102

#ifndef QUAD_GEOM_HH
#define QUAD_GEOM_HH

#include "artg4/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct QuadGeometry : public artg4::GeometryBase {
    QuadGeometry(std::string const &);
    void print() const;

    bool DoScraping;
    const double ScrapeHV;
    const double StoreHV;
    std::string PlateMaterial;
    const double topBottomWidth;
    const double topBottomThickness;
    const double innerOuterWidth;
    const double innerOuterThickness;
    const double outerThickness_Q1;
    const double plateSep;
    const double outerQ1offset;
    const double curlInnerRadius;
    const double curlOuterRadius;
    const double innerCurlMajorRadius;
    const double outerCurlMajorRadius;
    const double curlHeightAboveMidplane;
    std::string SupportMaterial; 
    const int support_nPlanes;
    std::vector<double> support_rInner;
    std::vector<double> support_rOuter;
    std::vector<double> support_z;
    const double support_Sphi;
    const double support_Dphi;
    
    //FIXME : should grab these from vac geom instead or Re-Setting them here
    const double QFR_rmin;
    const double QFR_rmax;
    const double QFR_z;

    /* Calculations to define solid volumes based on measurements */
    const double vCInnerWall;
    const double vCOuterWall;
    
    // Top and bottom quad plates
    const double qPlatesTB_rMin ; 
    const double qPlatesTB_rMax ;
    const double qPlatesTB_z ;
    
    // Inner quad plate
    const double qPlatesI_rMin ; 
    const double qPlatesI_rMax ; 
    const double qPlatesI_z ; 
    
    // Outer quad plate
    const double qPlatesO_rMin ;
    const double qPlatesO_rMax ;
    const double qPlatesO_rMax_Q1 ;
    const double qPlatesO_z ;

    // (Radially) inner support machining
    const double innerSM_rMin;
    const double innerSM_rMax;
    
    // (Radially) outer support cut
    const double outerSM_rMin;
    //const double outerSM_rMax = R_magic()+vCOuterWall;
    const double outerSM_rMax;
    
    // All supports
    const double machining_z;
    const double machining_Sphi;
    const double machining_Dphi;
    
  
  // The quadrupole electrode plates, the plate supports and the
  // electric field regions are built within using 'for' loops: it is
  // therefore convenient to place the parameters into arrays that can
  // be accessed by the loop index.  The qPlates are built in following
  // order: I,O,T,B.  fRegion in following order: Q,I,O,T,B.  These are
  // controlled by order of enum variables at end of this file
    const double qPlates_rMin[4];
    const double qPlates_rMax[4];
    const double qPlates_z[4];    
    
    const double qCurl_rMin;
  
    const double qCurl_rMax;
    
    const double qCurl_majorRad[2];
  
    /* Geometrical position parameters based on dimensional measurements */
    
    // Define the azimuthal starting position (Sphi) and azimuthal span
    // (Dphi) of the quadrupoles *in the azimuthal coordinates* of the
    // vacuum chamber sections so that they are "automatically" built in
    // the correct position.  All positions are based on the official
    // g2MIGTRACE position "BmChbrPln.dwg".  Note that these azimuthal
    // positions have been updated to reflect the realistic scalloped
    // vacuum chamber walls added in revision 145.  The azimuthal 0-degree
    // of the "new" vacuum chamber coordinate system is -0.413 degrees
    // from the "old" vacuum chambers for reasons of Geant4 geometry.  
    // For references, please see: 
    // (1) "<SVNREPO>/ref/azimuthalVacCoordCorrection.jpg" 
    // (2) "<SVNREPO>/ref/azimuthalQuadPosCorrection.jpg"
    
    
    const double Sphi13;
    const double Dphi13;
    const double Sphi26;
    const double Dphi26;

    
    // Both the quad plates and the linear field regions have the same
    // azimuthal span so we'll just need two arrays instead of four
    const double quad_Sphi[2];
    const double quad_Dphi[2];

    const double qPlatesTB_posZ;
    const double qPlatePosition[4];

    // NOTE TO MYSELF The offset is necessary because geant4 places
    // G4Polycone based on their "0" z-plane.  Thus when we rotate it and
    // the outer support needs more z we have to offset to get the wide
    // portion of the support to cut off at.  Current value is a total
    // guess: like fried chickin, it needs fixin's.  ZSH (01 JUN 09)
    const double outerSupportOffset;
    const double ISupportRad;
    const double OSupportRad;

    // was const G4double angSupportPos[2][6] 
    std::vector<double> angSupportPos0;
    std::vector<double> angSupportPos1;

    
    std::vector<std::string> plateNames;

    const int numQuadRegions;
    const int numQuadSections;
    const int numPlateTypes;
    const int numSupportTypes;
    std::vector<int> numSupportPairsPerSection;

    const bool displayPlates;
    const std::vector<double> platesColor;
    
    const bool displaySupports;
    const std::vector<double> supportsColor;
    
    const bool displayQFR;
    const std::vector<double> qfrColor;

  }; //struct QuadGeometry

}// namespace gm2ringsim


#endif //QUAD_GEOM_HH
