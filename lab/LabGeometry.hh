//
//  LabGeometry.h
//  gm2ringsim_xcode
//
//  Created by Adam Lyon on 11/16/12.
//  Copyright (c) 2012 Adam Lyon. All rights reserved.
//

#ifndef LABGEOMETRY_HH
#define LABGEOMETRY_HH

#include "gm2ringsim/geometry/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct LabGeometry : public GeometryBase {
    LabGeometry(std::string);
    void print() const;
    
    const double lab_x, lab_y, lab_z;
    
  }
}

#endif
