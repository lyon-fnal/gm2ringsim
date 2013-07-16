#ifndef CollimatorGeometry_hh__
#define CollimatorGeometry_hh__

/** @file CollimatorGeometry.hh

    Implements the geometry data for the collimators

    @author Peter Winter
    @date 2012
*/

#include "gm2geom/Core/GeometryBase.hh"

#include <string>

namespace gm2ringsim{
  struct CollimatorGeometry : public gm2geom::GeometryBase {
    
    CollimatorGeometry(std::string);

    const double coll_rMin;
    const double coll_rMax;
    const double coll_z;
    const double coll_Sphi_Full;
    const double coll_Dphi_Full;
    const double coll_Sphi_Half;
    const double coll_Dphi_Half;
    const int nCollimators;
    std::vector<double> azimuthalPos;
    std::vector<int> cVacWallArray;
    std::vector<int> collimatorType;
    const std::vector<std::string> collimatorName;

    const double coll_Sphi[3];
    const double coll_Dphi[3];

    const bool display;
    std::vector<double> collimatorColor;

    void print();
  }; 
}

#endif // SAMPLEGEOM_HH    
