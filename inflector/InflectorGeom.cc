// Geometry for the world

#include "gm2ringsim/inflector/InflectorGeom.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::InflectorGeom::InflectorGeom(std::string const & detName) :
  GeometryBase(detName),
  inflectorTotalLength( p.get<double>("inflectorTotalLength") * mm),
  inflectorTotalWidth( p.get<double>("inflectorTotalWidth") * mm),
  inflectorTotalHeight( p.get<double>("inflectorTotalHeight") * mm),
  apertureWidth( p.get<double>("apertureWidth") * mm),
  apertureInnerHeight( p.get<double>("apertureInnerHeight") * mm),
  apertureOuterHeight( p.get<double>("apertureOuterHeight") * mm),
  flangeWidth( p.get<double>("flangeWidth") * mm),
  equivalentAlWidth( p.get<double>("equivalentAlWidth") * mm),
  equivalentCuWidth( p.get<double>("equivalentCuWidth") * mm),
  equivalentNbTiWidth( p.get<double>("equivalentNbTiWidth") * mm),
  parWall_X( p.get<double>("parWall_X") * cm),
  parWall_Y( p.get<double>("parWall_Y") * cm),
  parWall_Z( p.get<double>("parWall_Z") * cm),
  perpWall_X( p.get<double>("perpWall_X") * cm),
  perpWall_Y( p.get<double>("perpWall_Y") * cm),
  perpWall_Z( p.get<double>("perpWall_Z") * cm),
  cryoWindow_X( p.get<double>("cryoWindow_X") * cm),
  cryoWindow_Y( p.get<double>("cryoWindow_Y") * cm),
  cryoWindow_Z( p.get<double>("cryoWindow_Z") * cm),
  cryoWindow_posZ( p.get<double>("cryoWindow_posZ") * mm),
  perpWall_alpha( p.get<double>("perpWall_alpha") * mm),
  perpWall_beta( p.get<double>("perpWall_beta") * mm),
  perpWall_gamma( p.get<double>("perpWall_gamma") * deg),
  perpWall_posR( p.get<double>("perpWall_posR") * cm),
  perpWall_angle( p.get<double>("perpWall_angle") * deg),
  perpWall_posZ( p.get<double>("perpWall_posZ") * mm),
  muonRadiusToInnerCryoWindowEdge( p.get<double>("muonRadiusToInnerCryoWindowEdge") * mm),
  outerCryoWindowEdgeToOuterInflectorEdge( p.get<double>("outerCryoWindowEdgeToOuterInflectorEdge") * mm),
  outerInflectorEdgeToCenterBeamAperture( p.get<double>("outerInflectorEdgeToCenterBeamAperture") * mm),
  centerBeamApertureToInflectorCenter( p.get<double>("centerBeamApertureToInflectorCenter") * mm),
  launchWidth( p.get<double>("launchWidth") * cm),
  trackerThickness( p.get<double>("trackerThickness") * mm),
  cryo_angular( p.get<double>("cryo_angular") * deg),
  cryo_rotation( p.get<double>("cryo_rotation") * deg)

{}

void gm2ringsim::InflectorGeom::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("InflectorGeom") << "Inflector geometry is" ;
  std::ostringstream oss;
  oss << "  inflectorTotalLength=" << inflectorTotalLength << "\n";
  oss << "  inflectorTotalWidth=" << inflectorTotalWidth << "\n";
  oss << "  inflectorTotalHeight=" << inflectorTotalHeight << "\n";
  oss << "  apertureWidth=" << apertureWidth << "\n";
  oss << "  apertureInnerHeight=" << apertureInnerHeight << "\n";
  oss << "  apertureOuterHeight=" << apertureOuterHeight << "\n";
  oss << "  flangeWidth=" << flangeWidth << "\n";
  oss << "  equivalentAlWidth=" << equivalentAlWidth << "\n";
  oss << "  equivalentCuWidth=" << equivalentCuWidth << "\n";
  oss << "  equivalentNbTiWidth=" << equivalentNbTiWidth << "\n";
  oss << "  parWall_X=" << parWall_X << "\n";
  oss << "  parWall_Y=" << parWall_Y << "\n";
  oss << "  parWall_Z=" << parWall_Z << "\n";
  oss << "  perpWall_X=" << perpWall_X << "\n";
  oss << "  perpWall_Y=" << perpWall_Y << "\n";
  oss << "  perpWall_Z=" << perpWall_Z << "\n";
  oss << "  cryoWindow_X=" << cryoWindow_X << "\n";
  oss << "  cryoWindow_Y=" << cryoWindow_Y << "\n";
  oss << "  cryoWindow_Z=" << cryoWindow_Z << "\n";
  oss << "  cryoWindow_posZ=" << cryoWindow_posZ << "\n";
  oss << "  perpWall_alpha=" << perpWall_alpha << "\n";
  oss << "  perpWall_beta=" << perpWall_beta << "\n";
  oss << "  perpWall_gamma=" << perpWall_gamma << "\n";
  oss << "  perpWall_posR=" << perpWall_posR << "\n";
  oss << "  perpWall_angle=" << perpWall_angle << "\n";
  oss << "  perpWall_posZ=" << perpWall_posZ << "\n";
  oss << "  muonRadiusToInnerCryoWindowEdge=" << muonRadiusToInnerCryoWindowEdge << "\n";
  oss << "  outerCryoWindowEdgeToOuterInflectorEdge=" << outerCryoWindowEdgeToOuterInflectorEdge << "\n";
  oss << "  outerInflectorEdgeToCenterBeamAperture=" << outerInflectorEdgeToCenterBeamAperture << "\n";
  oss << "  centerBeamApertureToInflectorCenter=" << centerBeamApertureToInflectorCenter << "\n";
  oss << "  launchWidth=" << launchWidth << "\n";
  oss << "  trackerThickness=" << trackerThickness << "\n";
  oss << "  cryo_angular=" << cryo_angular << "\n";
  oss << "  cryo_rotation=" << cryo_rotation << "\n";
  mf::LogInfo("CATEGORY") << oss.str();
}

