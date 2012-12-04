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
  displayTraceback( p.get<bool>("displayTraceback") ),
  tracebackColor( p.get<std::vector<double>>("tracebackColor") )
{
  for (auto& entry : theta_out ) { entry *= deg; }
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
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
