#ifndef STATIONGEOMETRY_HH
#define STATIONGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"
#include <iostream>


namespace gm2ringsim{
  struct StationGeometry : public artg4::GeometryBase {
    StationGeometry(std::string const &);
    
    const double r;
    const double v;
    const double t;
    
    void print();
  };

}
#endif