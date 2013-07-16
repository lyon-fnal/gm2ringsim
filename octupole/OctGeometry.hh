//  OctGeometry.hh
//  @author Brendan Kiburg 
//  @date Dec 2102

#ifndef OCT_GEOM_HH
#define OCT_GEOM_HH

#include "gm2geom/Core/GeometryBase.hh"

#include <iostream>

namespace gm2ringsim {
  
  struct OctGeometry : public gm2geom::GeometryBase {
    OctGeometry(std::string const &);
    void print() const;
    //octupole field

    const double period;
    const double gradient;
    const double offset;

    //octupole construction

    const int whichSector;
    const double fieldRad;
    const double fieldStartPhi;
    const double coilLength;
    const double outerPipeDiameter;
    const double pipeThickness;
    const double pipePlaceRad;
    const double thetaSepStep;

    //derived quantities
    const double fieldDeltaPhi;
    const double outerPipeRad;
    const double innerPipeRad;
    const double thetaSep;
    const double thetaPipe[16];


    const bool display;
    std::vector<double> color;
    const bool displayTubes;
    std::vector<double> tubeColor;
    const bool displayWater;
    std::vector<double> waterColor;

  }; //struct OctGeometry

}// namespace gm2ringsim


#endif //Oct_GEOM_HH
