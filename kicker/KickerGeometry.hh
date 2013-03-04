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
    //    const double kickPercent; // included in vector below
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
    
    const double radius;
    const double width;
    const double thickness;
    const double separation;
    const double k1Sphi;
    const double k2Sphi;
    const double k3Sphi;
    const double kPlates_Dphi;
    const int arc2;
    const int arc3;
    std::vector<double> kickerHV;
    std::vector<double> kickPercent;
    std::vector<double> squareMag;
    std::vector<double> circuitC;
    std::vector<double> circuitL;
    std::vector<double> circuitR;

    std::vector<double> kickerOffsetTime;
    std::vector<double> kickerProperOffsetTime;

    // Defines the field-region
    const G4double kPlatesFR_rMin;
    const G4double kPlatesFR_rMax;
    const G4double kPlatesFR_z;
    
    // Defines the inner kicker plate
    const G4double kPlatesI_rMin;
    const G4double kPlatesI_rMax;
    const G4double kPlatesI_z;
    
    // Defines the outer kicker plate 
    const G4double kPlatesO_rMin;
    const G4double kPlatesO_rMax;
    const G4double kPlatesO_z;
    
    std::vector<double> kPlates_rMin;
    std::vector<double> kPlates_rMax;
    std::vector<double> kPlates_z;
    std::vector<double> kPlates_Sphi;
    std::vector<int> kVacWallArray;


    const bool displayPlates;
    const std::vector<double> platesColor;

    void print() const;
  }; //struct KickerGeometry
} //namespace gm2ringsim

#endif
