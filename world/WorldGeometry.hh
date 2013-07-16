//
//  WorldGeometry.h
//  gm2ringsim_xcode
//
//  Created by Adam Lyon on 11/16/12.
//  Copyright (c) 2012 Adam Lyon. All rights reserved.
//

#ifndef LABGEOMETRY_HH
#define LABGEOMETRY_HH

#include "gm2geom/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct WorldGeometry : public gm2geom::GeometryBase {
    WorldGeometry(std::string const &);
    void print() const;
    
    const double world_x, world_y, world_z;
    
    const bool display;
    const std::vector<double> worldColor;

    
  };
}

#endif
