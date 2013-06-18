// Traceback Geometry

#ifndef STRAWTRACKERGEOMETRY_HH
#define STRAWTRACKERGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"

#include <iostream>

namespace gm2ringsim {

  /*struct strawKey{
    
  }
  
  struct Plane{
    int view;
    int layer;
    int station;
  }*/
  
  struct StrawTrackerGeometry : public artg4::GeometryBase {
    StrawTrackerGeometry(std::string const & detName="strawtracker");
    
    void print() const;
    
    int Plane(int station, int view, int layer);
    double wirePosition(int plane, int wire, int view);
    double yPosition(int plane);
    
    std::vector<int> whichScallopLocations;
    std::vector<double> strawStationLocation;
    std::vector<double> strawStationSize;
    std::vector<double> strawStationOffset;
    std::vector<int> strawStationType;
    int strawView;
    int strawLayers;
    std::vector<double> strawStationSizeHalf;
    const double strawStationHeight;
    double strawStationHeightHalf;
    double strawStationWidth;
    double strawStationWidthHalf;
    
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
    double heightOfTheStraw;
    double lengthOfTheStraw;
    double halfHeightOfTheStraw;
    double halfLengthOfTheStraw;
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    double dist_btwn_wires;
    double straw_diameter;
    double layer_angle;
    double delta_x;
    
    int number_of_stations; 
    std::vector<double> straw_station_center_from_edge;

    std::vector<double> x_position_straw0;
    std::vector<double> y_position;
    
    const bool displayStation;
    std::vector<double> stationColor;
    const bool displayStraw;
    std::vector<double> strawColor;
    
    
  };

}
#endif