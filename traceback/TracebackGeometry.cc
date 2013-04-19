#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::TracebackGeometry::TracebackGeometry(std::string const & detName) :
  GeometryBase(detName),
  whichTracebackLocations( p.get<std::vector<int>>("whichTracebackLocations")),
  strawStationLocation( p.get<std::vector<double>>("strawStationLocation")),
  strawStationSize( p.get<std::vector<double>>("strawStationSize")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  heightOfTheStraw( p.get<double>("heightOfTheStraw") * cm),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  displayTraceback( p.get<bool>("displayTraceback") ),
  tracebackColor( p.get<std::vector<double>>("tracebackColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  individualStrawColor( p.get<std::vector<double>>("individualStrawColor"))
  
{
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
  }
  
  strawStationHeightHalf = strawStationHeight/2;
}

void gm2ringsim::TracebackGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichTracebackLocations="; for (auto entry : whichTracebackLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
