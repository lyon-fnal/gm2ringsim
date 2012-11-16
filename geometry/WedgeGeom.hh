#ifndef WEDGE_HH
#define WEDGE_HH

#include "gm2ringsim/geometry/GeometryBase.hh"
#include <string>

namespace gm2ringsim{
  struct WedgeGeometry : public GeometryBase {
    WedgeGeometry(std::string);
    const int length;
    const double angle;
    
    void Print();
  };
}






#endif // WEDGE_HH
