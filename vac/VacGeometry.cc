// Geometry for the Vac

#include "gm2ringsim/vac/VacGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::VacGeometry::VacGeometry(std::string const & detName) :
  GeometryBase(detName),
  inflectorExtensionValue( p.get<double>("inflectorExtensionValue") * mm),
  topBottomWall( p.get<double>("topBottomWall") * mm),
  outerWallThickness( p.get<double>("outerWallThickness") * mm),
  torus_rmin( p.get<double>("torus_rmin") * in),
  torus_rmax( p.get<std::vector<double>>("torus_rmax") ),
  torus_sphi( p.get<double>("torus_sphi") * deg),
  torus_dphi( p.get<double>("torus_dphi") * deg),
  phi_a( p.get<double>("phi_a") * deg),
  phi_b( p.get<double>("phi_b") * deg),
  wallR( p.get<std::vector<double>>("wallR") ),
  wallPhi( p.get<std::vector<double>>("wallPhi") ),
  vacR( p.get<std::vector<double>>("vacR") ),
  vacPhi( p.get<std::vector<double>>("vacPhi") ),
  phi_q_subtractor( p.get<double>("phi_q_subtractor") * deg),
  set_phi_q( p.get<bool>("set_phi_q") ),
  phi_q( p.get<double>("phi_q") * deg),
  xz( p.get<std::vector<double>>("xz") ),
  ext( p.get<std::vector<double>>("ext") ),
  tracker_sphi( p.get<double>("tracker_sphi") * deg),
  tracker_dphi( p.get<double>("tracker_dphi") * deg),
  turn_sphi( p.get<double>("turn_sphi") * deg),
  turn_dphi( p.get<double>("turn_dphi") * deg)
{
  for (auto& entry : torus_rmax ) { entry *= in; }
  for (auto& entry : wallR ) { entry *= in; }
  for (auto& entry : wallPhi ) { entry *= deg; }
  for (auto& entry : vacR ) { entry *= in; }
  for (auto& entry : vacPhi ) { entry *= deg; }
  for (auto& entry : xz ) { entry *= in; }
  for (auto& entry : ext ) { entry *= in; }
}

double gm2ringsim::VacGeometry::inflectorExtension(unsigned int arcNum) const {
  double ret = 0.0;
  if (arcNum == 11) ret=inflectorExtensionValue;
  return ret;
}


void gm2ringsim::VacGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  std::ostringstream oss;
  oss << "  inflectorExtensionValue=" << inflectorExtensionValue << "\n";
  oss << "  topBottomWall=" << topBottomWall << "\n";
  oss << "  outerWallThickness=" << outerWallThickness << "\n";
  oss << "  torus_rmin=" << torus_rmin << "\n";
  oss << "  torus_rmax= "; for (auto entry : torus_rmax) { oss << " " << entry; }; oss << "\n";
  oss << "  torus_sphi=" << torus_sphi << "\n";
  oss << "  torus_dphi=" << torus_dphi << "\n";
  oss << "  phi_a=" << phi_a << "\n";
  oss << "  phi_b=" << phi_b << "\n";
  oss << "  wallR= "; for (auto entry : wallR) { oss << " " << entry; }; oss << "\n";
  oss << "  wallPhi= "; for (auto entry : wallPhi) { oss << " " << entry; }; oss << "\n";
  oss << "  vacR= "; for (auto entry : vacR) { oss << " " << entry; }; oss << "\n";
  oss << "  vacPhi= "; for (auto entry : vacPhi) { oss << " " << entry; }; oss << "\n";
  oss << "  phi_q_subtractor=" << phi_q_subtractor << "\n";
  oss << "  set_phi_q=" << set_phi_q << "\n";
  oss << "  phi_q=" << phi_q << "\n";
  oss << "  xz= "; for (auto entry : xz) { oss << " " << entry; }; oss << "\n";
  oss << "  ext= "; for (auto entry : ext) { oss << " " << entry; }; oss << "\n";
  oss << "  tracker_sphi=" << tracker_sphi << "\n";
  oss << "  tracker_dphi=" << tracker_dphi << "\n";
  oss << "  turn_sphi=" << turn_sphi << "\n";
  oss << "  turn_dphi=" << turn_dphi << "\n";
  mf::LogInfo("VACGEOM") << oss.str();
}

