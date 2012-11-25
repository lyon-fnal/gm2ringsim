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
  zz( p.get<std::vector<double>>("zz") ),
  ext( p.get<std::vector<double>>("ext") ),
  tracker_sphi( p.get<double>("tracker_sphi") * deg),
  tracker_dphi( p.get<double>("tracker_dphi") * deg),
  turn_sphi( p.get<double>("turn_sphi") * deg),
  turn_dphi( p.get<double>("turn_dphi") * deg),
  ZachIsAwesome_Z(topBottomWall-outerWallThickness),
  torus_z( {topBottomWall, topBottomWall - outerWallThickness} )
{
  for (auto& entry : torus_rmax ) { entry *= in; }
  for (auto& entry : wallR ) { entry *= in; }
  for (auto& entry : wallPhi ) { entry *= deg; }
  for (auto& entry : vacR ) { entry *= in; }
  for (auto& entry : vacPhi ) { entry *= deg; }
  for (auto& entry : zz ) { entry *= in; }
  for (auto& entry : ext ) { entry *= in; }
  
  // Derived quantities
  pt_a[wallRegion].setR(wallR[0]);
  pt_a[wallRegion].setPhi(wallPhi[0]);
  pt_b[wallRegion].setR(wallR[1]);
  pt_b[wallRegion].setPhi(wallPhi[1]);
  pt_c[wallRegion].setR(wallR[2]);
  pt_c[wallRegion].setPhi(wallPhi[2]);
  pt_o[wallRegion].setR(wallR[3]);
  pt_o[wallRegion].setPhi(wallPhi[3]);

  pt_a[vacuumRegion].setR(vacR[0]);
  pt_a[vacuumRegion].setPhi(vacPhi[0]);
  pt_b[vacuumRegion].setR(vacR[1]);
  pt_b[vacuumRegion].setPhi(vacPhi[1]);
  pt_c[vacuumRegion].setR(vacR[2]);
  pt_c[vacuumRegion].setPhi(vacPhi[2]);
  pt_o[vacuumRegion].setR(vacR[3]);
  pt_o[vacuumRegion].setPhi(wallPhi[3]);
  
  const double alpha = 1.05;
  for (unsigned int i = 0; i < 2; ++i ) {
    pt_d[i] = pt_b[i] + alpha*(pt_c[i] - pt_b[i]);
    
    xS[i] = (pt_a[i]-pt_b[i]).mag();
    z[i] = (pt_d[i]-pt_b[i]).mag()*std::cos(phi_b);
    xL[i] = xS[i] + z[i]*(std::tan(phi_b)+std::tan(phi_a));
    
    // Now, calculate inner scallop data (the little right angle bit)
    
    G4TwoVector what = (pt_c[i] - pt_b[i]).unit();
    G4TwoVector nhat = what;
    nhat.rotate(90.*degree);
    nhat = nhat.unit();
    
    pt_r[i] = pt_c[i] - ext[i]*nhat;
    
    G4double const c = std::cos(15.*degree), s = std::sin(15.*degree);
    G4double const m =
    (s*pt_c[i].x()-c*pt_c[i].y())/
    (c*nhat.y()-s*nhat.x());
    
    pt_p[i] = pt_c[i] + m*nhat;
    
    xI[i] = (pt_p[i]-pt_r[i]).mag();
    xO[i] = xI[i] - zz[i]*std::tan(phi_q);
  }
  
  // trackers
  track_rMin = pt_a[vacuumRegion].r();
  turn_rMin = track_rMin;

  track_rMax = torus_rmax[vacuumRegion];
  turn_rMax = track_rMax;
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
  oss << "  zz= "; for (auto entry : zz) { oss << " " << entry; }; oss << "\n";
  oss << "  ext= "; for (auto entry : ext) { oss << " " << entry; }; oss << "\n";
  oss << "  tracker_sphi=" << tracker_sphi << "\n";
  oss << "  tracker_dphi=" << tracker_dphi << "\n";
  oss << "  turn_sphi=" << turn_sphi << "\n";
  oss << "  turn_dphi=" << turn_dphi << "\n";
  mf::LogInfo("VACGEOM") << oss.str();
}

