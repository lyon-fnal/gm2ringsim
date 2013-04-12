// Traceback Geometry

#ifndef TRACEBACKGEOMETRY_HH
#define TRACEBACKGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct TracebackGeometry : public artg4::GeometryBase {
    TracebackGeometry(std::string const &);
    
    void print() const;

    //const double r;
    //const double v;
    //const double t;
    //const double rIn;
    //const double rOut;
    //const double windowAngle;
    //std::vector<double> thetaOut;
    //const double tOffset;
    //const double rOffset;
    //const double vOffset;
    //const double tRotation;
    //const double rRotation;
    //const double vRotation;
    const double tracebackZ;
    //const double tracebackTheta;
    std::vector<double> strawLocation;
    std::vector<double> strawRadialExtent;
    std::vector<double> strawRadialExtentHalf;
    std::vector<int> whichTracebackLocations;
    const bool displayTraceback;
    std::vector<double> tracebackColor;
    const bool displayStraw;
    std::vector<double> strawColor;
    

    //Derived constants
    //double rC;
    //double thetaIn[2];
    //double thetaC[2];
    double tracebackZHalf;
    //double tracebackThetaHalf;
    //double tanTracebackRadialShiftAngle;

  };

}
#endif