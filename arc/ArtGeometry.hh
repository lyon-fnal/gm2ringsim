// Arc Geometry

#ifndef ARCGEOMETRY_HH
#define ARCGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct ArcGeometry : public artg4::GeometryBase {
    ArcGeometry(std::string const &);
    void print() const;
    
    const G4double arc_rMin;
    const G4double arc_rMax;
    const G4double arc11_rExtension;
    const G4double arc_z;
    const G4double arc_Sphi;
    const G4double arc_Dphi;
  };
}

#endif
