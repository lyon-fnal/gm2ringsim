// Geometry for the inflector

#include "gm2ringsim/inflector/InflectorGeom.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

#include "gm2ringsim/inflector/inflectorGeometry.hh"

gm2ringsim::InflectorGeom::InflectorGeom(std::string const & detName) :
  GeometryBase(detName),
  ig(),
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
  CryoWallMaterial( p.get<std::string>("CryoWallMaterial")),
  parWall_X( p.get<double>("parWall_X") * cm),
  parWall_Y( p.get<double>("parWall_Y") * cm),
  parWall_Z( p.get<double>("parWall_Z") * cm),
  parWall_alpha( p.get<double>("parWall_alpha") * degree),
  parWall_beta( p.get<double>("parWall_beta") * degree),
  parWall_gamma( p.get<double>("parWall_gamma") * degree),
  parWall_posX( p.get<double>("parWall_posX_length") * cm * 
		std::cos(parWall_gamma) ), // Assume this is based on parWall_gamma. In g2MIGTRACE, this was hard coded as cos(24*degree)
  parWall_posY( p.get<double>("parWall_posY_length") * cm *
		std::sin(parWall_gamma) ), // and sin(24*degree)
  parWall_posZ( p.get<double>("parWall_posZ") * cm),
  perpWall_X( p.get<double>("perpWall_X") * cm),
  perpWall_Y( p.get<double>("perpWall_Y") * cm),
  perpWall_Z( p.get<double>("perpWall_Z") * cm),
  cryoWindow_X( p.get<double>("cryoWindow_X") * cm),
  cryoWindow_Y( p.get<double>("cryoWindow_Y") * cm),
  cryoWindow_Z( p.get<double>("cryoWindow_Z") * cm),
// next two quantities already in proper units. Do not multiply again
  cryoWindow_posX(-perpWall_X + cryoWindow_X), 
  cryoWindow_posY(perpWall_Y - cryoWindow_Y),
  cryoWindow_posZ( p.get<double>("cryoWindow_posZ") * mm),
  perpWall_alpha( p.get<double>("perpWall_alpha") * mm),
  perpWall_beta( p.get<double>("perpWall_beta") * mm),
  perpWall_gamma( p.get<double>("perpWall_gamma") * deg),
  perpWall_posR( p.get<double>("perpWall_posR") * cm),
  perpWall_angle( p.get<double>("perpWall_angle") * deg),
  perpWall_posX((perpWall_posR)*std::cos(perpWall_angle)),
  perpWall_posY((perpWall_posR)*std::sin(perpWall_angle)),
  perpWall_posZ( p.get<double>("perpWall_posZ") * mm),
  centerRingToMuonOrbit( R_magic()),
  muonRadiusToInnerCryoWindowEdge( p.get<double>("muonRadiusToInnerCryoWindowEdge") * mm),
  outerCryoWindowEdgeToOuterInflectorEdge( p.get<double>("outerCryoWindowEdgeToOuterInflectorEdge") * mm),
  outerInflectorEdgeToCenterBeamAperture( p.get<double>("outerInflectorEdgeToCenterBeamAperture") * mm),
  centerBeamApertureToInflectorCenter( p.get<double>("centerBeamApertureToInflectorCenter") * mm),
  inflector_X((inflectorTotalHeight / 2) + 1*mm),
  inflector_Y((inflectorTotalLength / 2) + 3*cm),
  inflector_Z((inflectorTotalWidth / 2) + 1*mm),

  inflectorMandrel_X(inflectorTotalHeight / 2),
  inflectorMandrel_Y(ig.mandrel_length() / 2),
  inflectorMandrel_Z(inflectorTotalWidth / 2),
  
  beamChannel1_X(apertureInnerHeight / 2),
  beamChannel1_Y(ig.mandrel_length() / 2),
  beamChannel1_Z((apertureWidth/2) / 2),
  beamChannel2_X1(apertureOuterHeight / 2),
  beamChannel2_X2(apertureInnerHeight / 2),
  beamChannel2_Y1(ig.mandrel_length() / 2),
  beamChannel2_Y2(ig.mandrel_length() / 2),
  beamChannel2_Z((apertureWidth/2) / 2),
  
   window_X(inflectorTotalHeight / 2),
   window_Y(ig.window_thickness() / 2),
   window_Z(inflectorTotalWidth / 2),

   flange_X(inflectorTotalHeight / 2),
   flange_Y(flangeWidth / 2),
   flange_Z(inflectorTotalWidth / 2),

   launchWidth( p.get<double>("launchWidth") * cm),
 launch_X(inflectorTotalHeight / 2),
   launch_Y(launchWidth / 2),
   launch_Z(inflectorTotalWidth / 2),
   equivalentPlate_X(inflectorTotalHeight / 2),
   equivalentPlate_Z(inflectorTotalWidth / 2),
  trackerThickness( p.get<double>("trackerThickness") * mm),
  beamChannel2_offset((apertureWidth / 2)),
 beamChannel_offset(centerBeamApertureToInflectorCenter + beamChannel1_Z),

   windowPlacement((ig.mandrel_length() / 2) + (ig.window_thickness() / 2)),

   eqAlPlacement((ig.mandrel_length() / 2) + ig.window_thickness()
    + (equivalentAlWidth / 2)),

   eqCuPlacement((ig.mandrel_length() / 2) + ig.window_thickness()
    + equivalentAlWidth + (equivalentCuWidth /2 )),

   eqNbTiPlacement((ig.mandrel_length() / 2) + ig.window_thickness()
    + equivalentAlWidth + equivalentCuWidth + (equivalentNbTiWidth / 2)),

   flangePlacement((ig.mandrel_length() / 2) + ig.window_thickness()
    + ig.conductor_thickness() + (ig.window_thickness() / 2)),

   launchPlacement((ig.mandrel_length() / 2) + (2*ig.window_thickness())
    + ig.conductor_thickness() + (launchWidth / 2)),

  cryo_angular( p.get<double>("cryo_angular") * deg),
  cryo_rotation( p.get<double>("cryo_rotation") * deg),
  num_trackers( p.get<int>("num_trackers")),
  epsilon(p.get<double>("epsilon") * deg ),
  vacuumInflectorSection(p.get<int>("vacuumInflectorSection")),
  maxStepLength(p.get<double>("maxStepLength") * mm),
  useConductorEquivalent(p.get<bool>("useConductorEquivalent")),
  useUpstreamWindow(p.get<bool>("useUpstreamWindow")),
  useDownstreamWindow(p.get<bool>("useDownstreamWindow")),
  useUpstreamConductor(p.get<bool>("useUpstreamConductor")),
  useDownstreamConductor(p.get<bool>("useDownstreamConductor")),
  useUpstreamEndFlange(p.get<bool>("useUpstreamEndFlange")),
  useDownstreamEndFlange(p.get<bool>("useDownstreamEndFlange")),
  conductorCurrent(p.get<double>("conductorCurrent") * ampere),
  fieldNormConst(p.get<double>("fieldNormConst") * gauss) ,
  currentToMagFieldConversion((p.get<double>("currentToField_gauss") * gauss ) /
			      (p.get<double>("currentToField_amp") * ampere ) )
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
  oss << "  parWall_alpha=" << parWall_alpha << "\n";
  oss << "  parWall_beta=" << parWall_beta << "\n";
  oss << "  parWall_gamma=" << parWall_gamma << "\n";
  oss << "  parWall_posX=" << parWall_posX << "\n";
  oss << "  parWall_posY=" << parWall_posY << "\n";
  oss << "  parWall_posZ=" << parWall_posZ << "\n";
  oss << "  perpWall_X=" << perpWall_X << "\n";
  oss << "  perpWall_Y=" << perpWall_Y << "\n";
  oss << "  perpWall_Z=" << perpWall_Z << "\n";
  oss << "  cryoWindow_X=" << cryoWindow_X << "\n";
  oss << "  cryoWindow_Y=" << cryoWindow_Y << "\n";
  oss << "  cryoWindow_Z=" << cryoWindow_Z << "\n";
  oss << "  cryoWindow_posX=" << cryoWindow_posX << "\n";
  oss << "  cryoWindow_posY=" << cryoWindow_posY << "\n";
  oss << "  cryoWindow_posZ=" << cryoWindow_posZ << "\n";
  oss << "  perpWall_alpha=" << perpWall_alpha << "\n";
  oss << "  perpWall_beta=" << perpWall_beta << "\n";
  oss << "  perpWall_gamma=" << perpWall_gamma << "\n";
  oss << "  perpWall_posR=" << perpWall_posR << "\n";
  oss << "  perpWall_angle=" << perpWall_angle << "\n";
  oss << "  perpWall_posX=" << perpWall_posX << "\n";
  oss << "  perpWall_posY=" << perpWall_posY << "\n";
  oss << "  perpWall_posZ=" << perpWall_posZ << "\n";
  oss << "  centerRingToMuonOrbit=" <<  centerRingToMuonOrbit << "\n";
  oss << "  muonRadiusToInnerCryoWindowEdge=" << muonRadiusToInnerCryoWindowEdge << "\n";
  oss << "  outerCryoWindowEdgeToOuterInflectorEdge=" << outerCryoWindowEdgeToOuterInflectorEdge << "\n";
  oss << "  outerInflectorEdgeToCenterBeamAperture=" << outerInflectorEdgeToCenterBeamAperture << "\n";
  oss << "  centerBeamApertureToInflectorCenter=" << centerBeamApertureToInflectorCenter << "\n";
  oss << " inflector_X=" <<inflector_X << "\n";
    oss << " inflector_Y=" <<inflector_Y << "\n";
    oss << " inflector_Z=" << inflector_Z << "\n";

    oss << " inflectorMandrel_X=" <<inflectorMandrel_X << "\n";
    oss << " inflectorMandrel_Y=" << inflectorMandrel_Y<< "\n";
    oss << " inflectorMandrel_Z =" <<inflectorMandrel_Z << "\n";

    oss << " beamChannel1_X=" <<beamChannel1_X << "\n"; 
    oss << " beamChannel1_Y=" <<beamChannel1_Y << "\n";
    oss << " beamChannel1_Z=" <<beamChannel1_Z << "\n"; 
    oss << " beamChannel2_X1=" <<beamChannel2_X1 << "\n"; 
    oss << " beamChannel2_X2=" <<beamChannel2_X2 << "\n"; 
    oss << " beamChannel2_Y1=" << beamChannel2_Y1 << "\n"; 
    oss << " beamChannel2_Y2=" <<beamChannel2_Y2 << "\n"; 
    oss << " beamChannel2_Z=" <<beamChannel2_Z << "\n"; 
    
    oss << " window_X=" <<window_X << "\n"; 
    oss << " window_Y=" <<window_Y << "\n"; 
    oss << " window_Z=" <<window_Z << "\n"; 

    oss << " flange_X=" <<flange_X << "\n"; 
    oss << " flange_Y=" <<flange_Y << "\n"; 
    oss << " flange_Z=" <<flange_Z << "\n"; 


    oss << " launchWidth=" <<launchWidth << "\n";
   oss << "  launch_X=" <<launch_X << "\n"; 
    oss << " launch_Y=" <<launch_Y << "\n"; 
    oss << " launch_Z=" <<launch_Z << "\n"; 
    oss << " equivalentPlate_X=" <<equivalentPlate_X << "\n"; 
    oss << " equivalentPlate_Z=" <<equivalentPlate_Z << "\n"; 

    oss << " trackerThickness=" << trackerThickness<< "\n";
    oss << " beamChannel2_offset=" <<beamChannel2_offset << "\n"; 
    oss << " beamChannel_offset=" <<beamChannel_offset << "\n"; 

    oss << " windowPlacement=" <<windowPlacement << "\n"; 
    
    oss << " eqAlPlacement=" <<eqAlPlacement << "\n"; 
    oss << " eqCuPlacement=" <<eqCuPlacement << "\n"; 

    oss << " eqNbTiPlacement=" <<eqNbTiPlacement << "\n"; 

    oss << " flangePlacement=" <<flangePlacement << "\n"; 
    
    oss << " launchPlacement=" <<launchPlacement << "\n"; 
    
    oss << " cryo_angular=" <<cryo_angular << "\n";
    oss << " cryo_rotation=" <<cryo_rotation << "\n";

    oss << "  num_trackers=" << num_trackers << "\n";
    oss << "  epsilon=" <<epsilon << "\n";
    oss << "  vacuumInflectorSection=" <<vacuumInflectorSection << "\n";
    
    oss << "maxStepLength=" << maxStepLength << "\n";
    oss << "useConductorEquivalent=" <<useConductorEquivalent << "\n";
    oss << "useUpstreamWindow=" <<useUpstreamWindow << "\n";
    oss << "useDownstreamWindow=" <<useDownstreamWindow << "\n";
    oss << "useUpstreamConductor=" <<useUpstreamConductor << "\n";
    oss << "useDownstreamConductor=" <<useDownstreamConductor << "\n";
    oss << "useUpstreamEndFlange=" <<useUpstreamEndFlange << "\n";
    oss << "useDownstreamEndFlange=" <<useDownstreamEndFlange << "\n";
    oss << "conductorCurrent=" <<conductorCurrent <<"\n";
    oss << "fieldNormConst=" << fieldNormConst << "\n";
    oss << "currentToMagFieldConversion=" << currentToMagFieldConversion << "\n";
  mf::LogInfo("CATEGORY") << oss.str();
}

