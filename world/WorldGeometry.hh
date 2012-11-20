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
  
  struct WorldGeometry : public artg4::GeometryBase {
    WorldGeometry(std::string const &);
    void print() const;
    
    const double world_x, world_y, world_z;
    
  };
}

#endif
