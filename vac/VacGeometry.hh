// Vac Geometry

#ifndef VACGEOMETRY_HH
#define VACGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

#include "Geant4/globals.hh"
#include "gm2ringsim/common/conversions.hh"
#include "Geant4/G4TwoVector.hh"
#include "CLHEP/Vector/TwoVector.h"


#include <iostream>

/** @class vacChamberConstruction
 
 The vacuum chamber is a complicated shape, consisting of a central
 circular region with 2 "scalloped sections" attached on the inside
 of the central circular region; furthermore, the inner wall is not
 a uniform arc but somewhat jagged (See RING1.dwg).  The
 construction is accomplished by twice G4Union'ing a G4Tubs (the
 outer rectangular cross section torus to a union of two G4Traps
 (two identical scalloped sections).  A further complication is
 that vacuum chamber section 11 must contain the inflector and
 requires some modification to it's geometry.
 
 In addition, a non-physical beam tracking volume is constructed in
 each section for beam dynamics purposes, along with a turn counter
 volume near the inflection point.
 */



namespace gm2ringsim {
  
  struct VacGeometry : public artg4::GeometryBase {
    VacGeometry(std::string const &);
    
    double inflectorExtension(unsigned int) const;

    void print() const;
    
    enum typeToBuild { wallRegion, vacuumRegion, nTypes };
    
    const double inflectorExtensionValue;
    const double topBottomWall;
    const double outerWallThickness;
    const double torus_rmin;
    std::vector<double> torus_rmax;
    const double torus_sphi;
    const double torus_dphi;
    const double phi_a;
    const double phi_b;
    std::vector<double> wallR;
    std::vector<double> wallPhi;
    std::vector<double> vacR;
    std::vector<double> vacPhi;
    const double phi_q_subtractor;
    const bool set_phi_q;
    const double phi_q;
    std::vector<double> zz;
    std::vector<double> ext;
    const double tracker_sphi;
    const double tracker_dphi;
    const double turn_sphi;
    const double turn_dphi;
    
    const bool displayWall;
    const std::vector<double> wallColor;
    
    const bool displayVac;
    const std::vector<double> vacColor;
    
    const bool displayTracker;
    const std::vector<double> trackerColor;
    
    const bool displayTurnCounter;
    const std::vector<double> turnCounterColor;
    
    // The following constants are derived
    const double ZachIsAwesome_Z;
    std::vector<double> torus_z;
    
    G4TwoVector pt_a[2], pt_b[2], pt_c[2], pt_o[2], pt_d[2];
    double xL[2], xS[2], z[2];
    
    G4TwoVector pt_p[2], pt_r[2];
    double xI[2], xO[2];
    
    double track_rMin, track_rMax;
    double turn_rMin, turn_rMax;
    
    
  };
}

#endif
