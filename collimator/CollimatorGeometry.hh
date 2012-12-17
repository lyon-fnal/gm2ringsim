#ifndef CollimatorGeometry_hh__
#define CollimatorGeometry_hh__

/** @file CollimatorGeometry.hh

    Implements the geometry data for the collimators

    @author Peter Winter
    @date 2012
*/

#include "artg4/Core/GeometryBase.hh"

#include <string>

// There are three types of collimators
namespace gm2ringsim {
  enum e_collimator_type {FULL, HALF_LRO, HALF_SRO, OFF};
  std::vector<const char*> e_collimator_names = { "Collimator(full)", "Collimator(half-lro)", 
						  "Collimator(half-sro)", ""};
}

namespace gm2ringsim{
  struct CollimatorGeometry : public artg4::GeometryBase {
    
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

    const double coll_Sphi[3];
    const double coll_Dphi[3];

    const bool display;
    std::vector<double> collimatorColor;
    void print();
  }; 
}

#endif // SAMPLEGEOM_HH    
