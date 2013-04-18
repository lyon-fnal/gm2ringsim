#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::TracebackGeometry::TracebackGeometry(std::string const & detName) :
  GeometryBase(detName),
  tracebackZ( p.get<double>("tracebackZ")),
  strawLocation( p.get<std::vector<double>>("strawLocation")),
  strawRadialExtent( p.get<std::vector<double>>("strawRadialExtent")),
  whichTracebackLocations( p.get<std::vector<int>>("whichTracebackLocations")),
  displayTraceback( p.get<bool>("displayTraceback") ),
  tracebackColor( p.get<std::vector<double>>("tracebackColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  individualStrawColor( p.get<std::vector<double>>("individualStrawColor"))
{
    
  for (unsigned int i = 0 ; i < strawRadialExtent.size() ; i ++){
    strawRadialExtentHalf.push_back(strawRadialExtent[i]/2);
  }
  
  tracebackZHalf = tracebackZ/2;
}

void gm2ringsim::TracebackGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  tracebackZ="<< tracebackZ << "\n";
  oss << "  strawRadialExtent="; for (auto entry: strawRadialExtent) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawLocations="; for (auto entry : strawLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichTracebackLocations="; for (auto entry : whichTracebackLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
