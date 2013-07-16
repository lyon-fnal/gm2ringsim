#ifndef STATIONGEOMETRY_HH
#define STATIONGEOMETRY_HH

#include "gm2geom/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"
#include <iostream>

namespace gm2ringsim{
  struct StationGeometry : public gm2geom::GeometryBase {
    StationGeometry(std::string const &);
    void print();

    double const in = 25.4 * mm;

    const double r;
    const double v;
    const double t;
    const double r_in;
    const double r_out;
    std::vector<double> theta_out;

    const double window_angle;

    const double t_offset;
    const double r_offset;
    const double v_offset;
    const double t_rotation;
    const double r_rotation;
    const double v_rotation;
    
    const bool displayStation;
    const std::vector<double> stationColor;

    //Derived constants
    double r_c;
    double theta_in[2];
    double theta_c[2];
    
  };

}
#endif