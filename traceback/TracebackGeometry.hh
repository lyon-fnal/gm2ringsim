// Traceback Geometry

#ifndef TRACEBACKGEOMETRY_HH
#define TRACEBACKGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct TracebackGeometry : public artg4::GeometryBase {
    TracebackGeometry(std::string const & detName="traceback");
    
    void print() const;
    
    std::vector<int> whichTracebackLocations;
    std::vector<double> strawStationLocation;
    std::vector<double> strawStationSize;
    std::vector<int> strawStationType;
    std::vector<double> strawStationSizeHalf;
    const double strawStationHeight;
    double strawStationHeightHalf;
    double strawStationWidth;
    double strawStationWidthHalf;
    
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
    double heightOfTheStraw;
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    double dist_btwn_wires;
    double straw_spacing;
    double straw_diameter;
    
    std::vector<double> x_position_straw0;
    std::vector<double> y_position;
    
    const bool displayStation;
    std::vector<double> stationColor;
    const bool displayStraw;
    std::vector<double> strawColor;
    
    
  };

}
#endif