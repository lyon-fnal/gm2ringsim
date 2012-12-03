#ifndef FiberHarpGeometry_hh__
#define FiberHarpGeometry_hh__

#include "artg4/Core/GeometryBase.hh"

#include <string>

namespace gm2ringsim{
  struct FiberHarpGeometry : public artg4::GeometryBase {
    
    FiberHarpGeometry(std::string);

    const double fiberLength;
    const double fiberRadius;
    const double fiberSpacing;
    const double supportLength;
    const double supportWidth;
    const double supportDepth;
    const int nFibers;
    const double coll_z;
    const int nHarps;
    std::vector<double> azimuthalPos;
    std::vector<double> vacWallPos;
    std::vector<double> harpType;
    
    const bool display;
    std::vector<double> fiberHarpColor;
    void print();
  }; 
}

#endif // SAMPLEGEOM_HH    
