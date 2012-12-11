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
    std::vector<double> traceback_radial;
    const double traceback_z;
    const double traceback_theta;
    const double traceback_radial_shift_angle;
    
    const bool displayTraceback;
    std::vector<double> tracebackColor;
    

    //Derived constants
    double r_c;
    double theta_in[2];
    double theta_c[2];
    double traceback_radial_half[22];
    double traceback_z_half;
    double traceback_theta_half;
    double tan_traceback_radial_shift_angle;

  };

}
#endif