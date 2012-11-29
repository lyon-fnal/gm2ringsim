#include "gm2ringsim/station/StationGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::StationGeometry::StationGeometry(std::string const & detName) :
  GeometryBase(detName),
  r( p.get<double>("r") * mm),
  v( p.get<double>("v") * mm),
  t( p.get<double>("t") * mm){
    
}

void gm2ringsim::StationGeometry::print() {
  std::ostringstream oss;
  oss << "  r=" << r << "\n";
  oss << "  v=" << v << "\n";
  oss << "  t=" << t << "\n";
  mf::LogInfo("STATIONGEOMETRY") << oss.str();

}