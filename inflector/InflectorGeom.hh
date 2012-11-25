//
//  WorldGeometry.h
//  gm2ringsim_xcode
//
//  Created by Adam Lyon on 11/16/12.
//  Copyright (c) 2012 Adam Lyon. All rights reserved.
//

#ifndef LABGEOMETRY_HH
#define LABGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct InflectorGeom : public artg4::GeometryBase {
    InflectorGeom(std::string const &);
    void print() const;

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
    const double parWall_X;
    const double parWall_Y;
    const double parWall_Z;
    const double perpWall_X;
    const double perpWall_Y;
    const double perpWall_Z;
    const double cryoWindow_X;
    const double cryoWindow_Y;
    const double cryoWindow_Z;
    const double cryoWindow_posZ;
    const double perpWall_alpha;
    const double perpWall_beta;
    const double perpWall_gamma;
    const double perpWall_posR;
    const double perpWall_angle;
    const double perpWall_posZ;
    const double muonRadiusToInnerCryoWindowEdge;
    const double outerCryoWindowEdgeToOuterInflectorEdge;
    const double outerInflectorEdgeToCenterBeamAperture;
    const double centerBeamApertureToInflectorCenter;
    const double launchWidth;
    const double trackerThickness;
    const double cryo_angular;
    const double cryo_rotation;
    
    
  };
}

#endif
