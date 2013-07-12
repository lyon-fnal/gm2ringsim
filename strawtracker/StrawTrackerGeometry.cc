#include "gm2ringsim/strawtracker/StrawTrackerGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>
#include <math.h>



gm2ringsim::StrawTrackerGeometry::StrawTrackerGeometry(std::string const & detName) :
  GeometryBase(detName),
  whichScallopLocations( p.get<std::vector<int>>("whichScallopLocations")),
  strawStationLocation( p.get<std::vector<double>>("strawStationLocation")),
  strawStationSize( p.get<std::vector<double>>("strawStationSize")),
  strawStationPiping( p.get<double>("strawStationPiping")),
  strawStationOffset( p.get<std::vector<double>>("strawStationOffset")),
  strawStationType( p.get<std::vector<int>>("strawStationType")),
  strawView( p.get<double>("strawView")),
  strawLayers( p.get<double>("strawLayers")),
  strawStationHeight( p.get<double>("strawStationHeight")),
  strawStationWidth( p.get<std::vector<double>>("strawStationWidth")),
  innerRadiusOfTheStraw( p.get<double>("innerRadiusOfTheStraw") ),
  outerRadiusOfTheStraw( p.get<double>("outerRadiusOfTheStraw") ),
  heightOfTheStraw( p.get<double>("heightOfTheStraw") * cm),
  startAngleOfTheStraw( p.get<double>("startAngleOfTheStraw") * deg),
  spanningAngleOfTheStraw( p.get<double>("spanningAngleOfTheStraw") *deg ),
  dist_btwn_wires( p.get<double>("dist_btwn_wires") *mm ),
  layer_angle( p.get<double>("layer_angle") *deg),
  x_position_straw0( p.get<std::vector<double>>("x_position_straw0")),
  y_position( p.get<std::vector<double>>("y_position")),
  displayStation( p.get<bool>("displayStation")),
  stationColor( p.get<std::vector<double>>("stationColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor"))
  
{
  
  strawStationHeightHalf = strawStationHeight/2;
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){
    strawStationSizeHalf.push_back(strawStationSize[i]/2);
    strawStationWidthHalf.push_back(strawStationWidth[i]/2);
    strawStationLocation[i] = 1436-strawStationLocation[i];
    
  }
  
  for (unsigned int i = 0 ; i < strawStationSize.size() ; i ++){

    straw_station_center_from_edge.push_back(strawStationSizeHalf[i] + strawStationOffset[i] + strawStationPiping);
  }
  
  lengthOfTheStraw = heightOfTheStraw/cos(layer_angle);
  halfLengthOfTheStraw = lengthOfTheStraw/2;
  halfHeightOfTheStraw = heightOfTheStraw/2;
  
  for (unsigned int i = 0; i<y_position.size(); i++){
    y_position[i] = y_position[i] - strawStationWidthHalf[0];
  }
  
  delta_x = halfHeightOfTheStraw*tan(layer_angle);
  number_of_stations = strawStationSize.size() * whichScallopLocations.size();
}


int gm2ringsim::StrawTrackerGeometry::Plane(int station, int view, int layer) {
  
  return station*(strawView+strawLayers) + view*2 + layer;
  
}


double gm2ringsim::StrawTrackerGeometry::wirePosition(int plane, int wire, int view){
  
  double x =  x_position_straw0[plane%4] + wire*dist_btwn_wires;
  if (view == 0) x = x - delta_x;
  else x = x+delta_x;
  
  return x;
}

double gm2ringsim::StrawTrackerGeometry::yPosition(int plane){
  return y_position[plane%4];
}

void gm2ringsim::StrawTrackerGeometry::print() const{
  std::ostringstream oss;
  
  oss << "  strawStationHeight="<< strawStationHeight << "\n";
  oss << "  strawStationSize="; for (auto entry: strawStationSize) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawStationLocations="; for (auto entry : strawStationLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichScallopLocations="; for (auto entry : whichScallopLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  lengthOfStraw=" <<lengthOfTheStraw << "\n";
  mf::LogInfo("STRAWTRACKERGEOMETRY") << oss.str();
  
 
}
