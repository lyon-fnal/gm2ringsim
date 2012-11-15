#ifndef SAMPLEGEOM_HH
#define SAMPLEGEOM_HH

#include "gm2ringsim/geometry/GeometryBase.hh"
#include <string>

namespace gm2ringsim{
  struct SampleGeometry : public GeometryBase {
    SampleGeometry(std::string);
    const int sampleInt;
    const double sampleDouble;
    
    void Print();
  };
}






#endif // SAMPLEGEOM_HH
