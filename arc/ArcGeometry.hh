// Arc Geometry

#ifndef ARCGEOMETRY_HH
#define ARCGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct ArcGeometry : public artg4::GeometryBase {
    ArcGeometry(std::string const &);
    void print() const;
    
    const double arc_rMin;
    const double arc_rMax;
    const double arc11_rExtension;
    const double arc_z;
    const double arc_Sphi;
    const double arc_Dphi;
  };
}

#endif
