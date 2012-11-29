#include "gm2ringsim/station/StationGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::StationGeometry::StationGeometry(std::string const & detName) :
  GeometryBase(detName),
  r( p.get<double>("r") * mm),
  v( p.get<double>("v") * mm),
  t( p.get<double>("t") * mm),
  r_in( p.get<double>("r_in") * in),
  r_out( p.get<double>("r_out") * in),
  theta_out( p.get<std::vector<double>>("theta_out") ),
  window_angle( p.get<double>("window_angle") * deg),
  t_offset( p.get<double>("t_offset")),
  r_offset( p.get<double>("r_offset")),
  v_offset( p.get<double>("v_offset")),
  t_rotation( p.get<double>("t_rotation")),
  r_rotation( p.get<double>("r_rotation")),
  v_rotation( p.get<double>("v_rotation")),
  r_c((r_in + r_out)/2)
{

  for (auto& entry : theta_out ) { entry *= deg; }
  for (auto& entry : theta_in ) { entry *= deg; }
  for (auto& entry : theta_c ) { entry *= deg; }
    
  //Derived quantities
  std::vector<double> theta_in;
  std::vector<double> theta_c;
  
  theta_in[0] = theta_out[0] - 0.12*deg;
  theta_in[1] = theta_out[1] - 0.12*deg;
		
  theta_c[0] = (theta_in[0] + theta_out[0])/2;
  theta_c[1] = (theta_in[1] + theta_in[1])/2;
  
}

void gm2ringsim::StationGeometry::print() {
  std::ostringstream oss;
  oss << "  r=" << r << "\n";
  oss << "  v=" << v << "\n";
  oss << "  t=" << t << "\n";
	
	oss << "  r_in=" << r_in << "\n";
	oss << "  r_out=" << r_out << "\n";
	oss << "  r_c=" << r_c << "\n";

	
  mf::LogInfo("STATIONGEOMETRY") << oss.str();

}
