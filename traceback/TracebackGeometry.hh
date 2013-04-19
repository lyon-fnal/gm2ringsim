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
    std::vector<double> strawStationSizeHalf;
    const double strawStationHeight;
    double strawStationHeightHalf;
    
    double innerRadiusOfTheStraw;
    double outerRadiusOfTheStraw;
    double heightOfTheStraw;
    double startAngleOfTheStraw;
    double spanningAngleOfTheStraw;

    const bool displayTraceback;
    std::vector<double> tracebackColor;
    const bool displayStraw;
    std::vector<double> strawColor;
    std::vector<double> individualStrawColor;
    
    
  };

}
#endif