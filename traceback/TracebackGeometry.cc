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
  strawStationType( p.get<std::vector<int>>("strawStationType")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  strawStationWidth( p.get<double>("strawStationWidth")),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  heightOfTheStraw( p.get<double>("heightOfTheStraw") * cm),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  dist_btwn_wires( p.get<double>("dist_btwn_wires") *mm ),
  straw_spacing( p.get<double>("straw_spacing") * mm),
  straw_diameter( p.get<double>("straw_diameter") *mm),
  x_position_straw0( p.get<std::vector<double>>("x_position_straw0")),
  y_position( p.get<std::vector<double>>("y_position")),
  displayStation( p.get<bool>("displayStation")),
  stationColor( p.get<std::vector<double>>("stationColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor"))
  
{
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
  }
  
    
  strawStationHeightHalf = strawStationHeight/2;
  strawStationWidthHalf = strawStationWidth/2;
  
  for (unsigned int i = 0; i<y_position.size(); i++){
    y_position[i] = y_position[i] - strawStationWidthHalf;
  }
  
  
}

void gm2ringsim::TracebackGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichTracebackLocations="; for (auto entry : whichTracebackLocations) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
