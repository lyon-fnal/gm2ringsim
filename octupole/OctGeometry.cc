#include "gm2ringsim/octupole/OctGeometry.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/conversions.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::OctGeometry::OctGeometry(std::string const & detName) :
  GeometryBase(detName),
  whichSector( p.get<double>("whichSector") * mm),
  fieldRad( p.get<double>("fieldRad") * cm),
  fieldStartPhi( p.get<double>("fieldStartPhi") * degree),
  coilLength( p.get<double>("coilLength") * cm),
  outerPipeDiameter( p.get<double>("outerPipeDiameter") * mm),
  pipeThickness( p.get<double>("pipeThickness") * mm),
  pipePlaceRad( p.get<double>("pipePlaceRad") * cm),
  thetaSepStep( p.get<double>("thetaSepStep") * mm),

  fieldDeltaPhi( coilLength / R_magic()),
  outerPipeRad( outerPipeDiameter / 2.),
  innerPipeRad( outerPipeRad - pipeThickness ),
  thetaSep( thetaSepStep * outerPipeRad / pipePlaceRad ),
// theta = 0 is radially inward, progressing UP (clockwise from
// upstream, that is)
/** @bug I've made up these theta values ... should get "real"
    values from Shatunov. *///theta
  thetaPipe{
  // Quadrant 1
  pi/8.-thetaSep, pi/8.+thetaSep, 
    3.*pi/8.-thetaSep, 3.*pi/8.+thetaSep,
    // Quadrant 2
    pi/2.+pi/8.-thetaSep, pi/2.+pi/8.+thetaSep, 
    pi/2.+3.*pi/8.-thetaSep, pi/2.+3.*pi/8.+thetaSep,
    // Quadrant 3
    pi+pi/8.-thetaSep, pi+pi/8.+thetaSep, 
    pi+3.*pi/8.-thetaSep, pi+3.*pi/8.+thetaSep,
    // Quadrant 4
    3.*pi/2.+pi/8.-thetaSep, 3.*pi/2.+pi/8.+thetaSep, 
    3.*pi/2.+3.*pi/8.-thetaSep, 3.*pi/2.+3.*pi/8.+thetaSep},

  display( p.get<bool>("display") ),
  color( p.get<std::vector<double>>("color") ),
  displayTubes( p.get<bool>("displayTubes") ),
  tubeColor( p.get<std::vector<double>>("tubeColor") ),
  displayWater( p.get<bool>("displayWater") ),
  waterColor( p.get<std::vector<double>>("waterColor") )
{ 
    //  for (auto& entry : angSupportPos1 ) { entry *= degree; }

}

void gm2ringsim::OctGeometry::print() const { 
  std::ostringstream oss;
oss << "  whichSector=" << whichSector << "\n";
oss << "  fieldRad=" << fieldRad << "\n";
oss << "  fieldStartPhi=" << fieldStartPhi << "\n";
oss << "  coilLength=" << coilLength << "\n";
oss << "  outerPipeDiameter=" << outerPipeDiameter << "\n";
oss << "  pipeThickness=" << pipeThickness << "\n";
oss << "  pipePlaceRad=" << pipePlaceRad << "\n";
oss << "  thetaSepStep=" << thetaSepStep << "\n";

oss << "  fieldDeltaPhi=" << fieldDeltaPhi << "\n";
oss << "  outerPipeRad=" << outerPipeRad << "\n";
oss << "  innerPipeRad=" << innerPipeRad << "\n";
oss << "  thetaSep=" << thetaSep << "\n";
oss << "  display=" << display << "\n";
oss << "  color= "; for (auto entry : color) { oss << " " << entry; }; oss << "\n";

oss << "  displayTubes=" << displayTubes << "\n";
oss << "  tubeColor= "; for (auto entry : tubeColor) { oss << " " << entry; }; oss << "\n";

oss << "  displayWater=" << displayWater << "\n";
oss << "  waterColor= "; for (auto entry : waterColor) { oss << " " << entry; }; oss << "\n";

 mf::LogInfo("QuadGeometry") << oss.str();
}//OctGeometry::print()
