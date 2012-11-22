// Vac Geometry

#ifndef VACGEOMETRY_HH
#define VACGEOMETRY_HH

#include "artg4/Core/GeometryBase.hh"

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
    
    double inflectorExtension(unsigned int arcNum) const {
      double ret = 0.0;
      if (arcNum == 11) ret=inflectorExtensionVal;
      return ret;
    } 
    
    
    void print() const;
    
    const double inflectorExtensionVal;
    const double topBottomWall;
    const double outerWallThickness;
    const double torus_rmin;
    const double torus_rmax[];
    const double torus_sphi;
    const double torus_dphi;
    
    const double wallR[];
    const double wallPhi[];
    const double vacR[];
    const double vacPhi[];
    
                         
  };
}

#endif
