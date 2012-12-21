// Kicker Geometry

#ifndef KICKERGEOMETRY_HH
#define KICKERGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"
#include "Geant4/G4TwoVector.hh"
#include "CLHEP/Vector/TwoVector.h"


#include <iostream>


namespace gm2ringsim {
  
  struct KickerGeometry : public artg4::GeometryBase {
    KickerGeometry(std::string const &);
    
const double kickTime1;
const double kickTime2;
const double kickPercent;
const double kPlate1HV;
const double kPlate2HV;
const double kPlate3HV;
const double kicker1L;
const double kicker1R;
const double kicker1C;
const double kicker2L;
const double kicker2R;
const double kicker2C;
const double kicker3L;
const double kicker3R;
const double kicker3C;

    void print() const;
  }; //struct KickerGeometry
} //namespace gm2ringsim

#endif
