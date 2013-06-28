//  InflectorGeom.hh
//  @author Brendan Kiburg 
//  @date Nov 2102

#ifndef INFLECTOR_GEOM_HH
#define INFLECTOR_GEOM_HH

#include "artg4/Core/GeometryBase.hh"

#include "gm2ringsim/inflector/inflectorGeometry.hh"
#include <iostream>

namespace gm2ringsim {
  
  struct InflectorGeom : public artg4::GeometryBase {
    InflectorGeom(std::string const & detName="inflector");
    void print() const;
    
    
    inflectorGeometry ig;
    
    const double inflectorTotalLength;
    const double inflectorTotalWidth;
    const double inflectorTotalHeight;

    const double apertureWidth;
    const double apertureInnerHeight;
    const double apertureOuterHeight;

    const double flangeWidth;
    const double equivalentAlWidth;
    const double equivalentCuWidth;
    const double equivalentNbTiWidth;

    // X&Y constants were divided by 2. Hardcoded post-division 11/2012
    const std::string CryoWallMaterial;
    const double parWall_X;
    const double parWall_Y;
    const double parWall_Z;
    
    const double parWall_alpha;
    const double parWall_beta;
    const double parWall_gamma;

    const double parWall_posX;
    const double parWall_posY;
    const double parWall_posZ;
    


    const double perpWall_X;
    const double perpWall_Y;
    const double perpWall_Z;

    const double cryoWindow_X;
    const double cryoWindow_Y;
    const double cryoWindow_Z;
 
    const double cryoWindow_posX ;
    const double cryoWindow_posY ;
    const double cryoWindow_posZ;

    const double perpWall_alpha;
    const double perpWall_beta;
    const double perpWall_gamma;

    const double perpWall_posR;
    const double perpWall_angle;
  
    const double perpWall_posX;
    const double perpWall_posY;
    const double perpWall_posZ;
    
    const double centerRingToMuonOrbit;
    const double muonRadiusToInnerCryoWindowEdge;
    const double outerCryoWindowEdgeToOuterInflectorEdge;
    const double outerInflectorEdgeToCenterBeamAperture;
    const double centerBeamApertureToInflectorCenter;

    const double inflector_X;
    const double inflector_Y;
    const double inflector_Z;

    const double inflectorMandrel_X;
    const double inflectorMandrel_Y;
    const double inflectorMandrel_Z ;

    const double beamChannel1_X; 
    const double beamChannel1_Y;
    const double beamChannel1_Z; 
    const double beamChannel2_X1; 
    const double beamChannel2_X2; 
    const double beamChannel2_Y1; 
    const double beamChannel2_Y2; 
    const double beamChannel2_Z; 
    
    const double window_X; 
    const double window_Y; 
    const double window_Z; 

    const double flange_X; 
    const double flange_Y; 
    const double flange_Z; 


    const double launchWidth;
   const double  launch_X; 
    const double launch_Y; 
    const double launch_Z; 
    const double equivalentPlate_X; 
    const double equivalentPlate_Z; 

    const double trackerThickness;
    const double beamChannel2_offset; 
    const double beamChannel_offset; 

    const double windowPlacement; 
    
    const double eqAlPlacement; 
    const double eqCuPlacement; 

    const double eqNbTiPlacement; 

    const double flangePlacement; 
    
    const double launchPlacement; 
    
    const double cryo_angular;
    const double cryo_rotation;

    const int num_trackers;
    const double epsilon;
    const int vacuumInflectorSection;
    const double maxStepLength;
    const bool useConductorEquivalent;
    const bool useUpstreamWindow;
    const bool useDownstreamWindow;
    const bool useUpstreamConductor;
    const bool useDownstreamConductor;
    const bool useUpstreamEndFlange;
    const bool useDownstreamEndFlange;

    const double conductorCurrent;
    const double fieldNormConst;
    const double currentToMagFieldConversion;
    
  };
}

#endif
