// Vac Geometry

#ifndef VACGEOMETRY_HH
#define VACGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct TracebackGeometry : public artg4::GeometryBase {
    TracebackGeometry(std::string const &);
    
    void print() const;

    const double r;
    const double v;
    const double t;
    const double r_in;
    const double r_out;
    const double window_angle;
    std::vector<double> theta_out;
    const double t_offset;
    const double r_offset;
    const double v_offset;
    const double t_rotation;
    const double r_rotation;
    const double v_rotation;
    const bool displayTraceback;
    std::vector<double> tracebackColor;

  };

}
#endif