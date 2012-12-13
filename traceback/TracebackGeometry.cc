#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::TracebackGeometry::TracebackGeometry(std::string const & detName) :
  GeometryBase(detName),
  r( p.get<double>("r") * mm),
  v( p.get<double>("v") * mm),
  t( p.get<double>("t") * mm),
  r_in( p.get<double>("r_in") * in),
  r_out( p.get<double>("r_out") * in),
  window_angle( p.get<double>("window_angle") * deg),
  theta_out( p.get<std::vector<double>>("theta_out") ),
  t_offset( p.get<double>("t_offset") * mm),
  r_offset( p.get<double>("r_offset") * mm),
  v_offset( p.get<double>("v_offset") * mm),
  t_rotation( p.get<double>("t_rotation") * deg),
  r_rotation( p.get<double>("r_rotation") * deg),
  v_rotation( p.get<double>("v_rotation") * deg),
  traceback_radial( p.get<std::vector<double>>("traceback_radial")),
  traceback_z( p.get<double>("traceback_z")),
  traceback_theta( p.get<double>("traceback_theta")),
  traceback_radial_shift_angle( p.get<double>("traceback_radial_shift_angle") * deg),
  strawLocation( p.get<std::vector<int>>("strawLocation")),
  displayTraceback( p.get<bool>("displayTraceback") ),
  tracebackColor( p.get<std::vector<double>>("tracebackColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  r_c((r_in + r_out)/2)
{
  for (auto& entry : theta_out ) { entry *= deg; }
  for (auto& entry : theta_in ) { entry *= deg; }
  for (auto& entry : theta_c ) { entry *= deg; }
  
  //Derived quantities
  
  for (unsigned int i = 0; i < 2; ++i ) {
    theta_in[i] = (theta_out[i] - 0.12*deg);
    theta_c[i] = (theta_in[i] + theta_out[i])/2;
  }
  
  for (unsigned int i = 0 ; i < 22; ++i){
    traceback_radial_half[i] = traceback_radial[i]/2;
  }

  traceback_z_half = traceback_z/2;
  traceback_theta_half = traceback_theta/2;
  tan_traceback_radial_shift_angle = tan(traceback_radial_shift_angle);
}

void gm2ringsim::TracebackGeometry::print() const{
  std::ostringstream oss;
  oss << "  r=" << r << "\n";
  oss << "  v=" << v << "\n";
  oss << "  t=" << t << "\n";
  oss << "  r_in=" << r_in << "\n";
  oss << "  r_out=" << r_out << "\n";
  oss << "  window_angle=" << window_angle << "\n";
  oss << "  theta_out= "; for (auto entry : theta_out) { oss << " " << entry; }; oss << "\n";
  oss << "  t_offset=" << t_offset << "\n";
  oss << "  r_offset=" << r_offset << "\n";
  oss << "  v_offset=" << v_offset << "\n";
  oss << "  t_rotation=" << t_rotation << "\n";
  oss << "  r_rotation=" << r_rotation << "\n";
  oss << "  v_rotation=" << v_rotation << "\n";
  oss << "  traceback_radial= "; for (auto entry : traceback_radial) { oss << " " << entry; }; oss << "\n";
  oss << "  traceback_z="<< traceback_z << "\n";
  oss << "  traceback_theta="<< traceback_theta << "\n";
  oss << "  strawLocations="; for (auto entry : strawLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
