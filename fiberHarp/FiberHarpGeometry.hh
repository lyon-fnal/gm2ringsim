#ifndef FiberHarpGeometry_hh__
#define FiberHarpGeometry_hh__

/** @file FiberHarpGeometry.hh

    Implements the geometry data for the fiber harps

    @author Peter Winter
    @date 2012
*/

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
    const double RMagicScale;
    const int nHarps;
    std::vector<double> azimuthalPos;
    std::vector<int> vacWallPos;
    std::vector<int> harpType;
    
    const bool display;
    std::vector<double> fiberHarpColor;
    void print();
  }; 
}

#endif // SAMPLEGEOM_HH    
