#include "gm2ringsim/quad/QuadGeometry.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/conversions.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::QuadGeometry::QuadGeometry(std::string const & detName) :
  GeometryBase(detName),
  DoScraping( p.get<bool>("DoScraping")),
  PlateMaterial(p.get<std::string>("PlateMaterial", "Al")),
  topBottomWidth( p.get<double>("topBottomWidth") * cm),
  topBottomThickness( p.get<double>("topBottomThickness") * cm),
  innerOuterWidth( p.get<double>("innerOuterWidth") * cm),
  innerOuterThickness( p.get<double>("innerOuterThickness") * cm),
  outerThickness_Q1( p.get<double>("outerThickness_Q1") * cm),
  plateSep( p.get<double>("plateSep") * cm),
  curlInnerRadius( p.get<double>("curlInnerRadius") * cm),
  curlOuterRadius( p.get<double>("curlOuterRadius") * cm),
  innerCurlMajorRadius( p.get<double>("innerCurlMajorRadius") * cm),
  outerCurlMajorRadius( p.get<double>("outerCurlMajorRadius") * cm),
  curlHeightAboveMidplane( p.get<double>("curlHeightAboveMidplane") * cm),
  SupportMaterial( p.get<std::string>("SupportMaterial", "Macor")),
  support_nPlanes( p.get<int>("support_nPlanes") ),
  support_rInner( p.get<std::vector<double>>("support_rInner") ),
  support_rOuter( p.get<std::vector<double>>("support_rOuter") ),
  support_z( p.get<std::vector<double>>("support_z") ),
  support_Sphi( p.get<double>("support_Sphi") * degree),
  support_Dphi( p.get<double>("support_Dphi") * degree),
  QFR_rmin( p.get<double>("QFR_rmin") * in),
  QFR_rmax( p.get<double>("QFR_rmax") * in),
  QFR_z( p.get<double>("QFR_z") * m),
  vCInnerWall (R_magic() - QFR_rmin),
  vCOuterWall (QFR_rmax - R_magic()),
  qPlatesTB_rMin (R_magic() - (topBottomWidth/2)),
  qPlatesTB_rMax ( R_magic() + (topBottomWidth/2)),
  qPlatesTB_z ( (topBottomThickness/2)),
  
// Inner quad plate
  qPlatesI_rMin ( R_magic() - (plateSep/2) - innerOuterThickness),
  qPlatesI_rMax ( R_magic() - (plateSep/2)),
  qPlatesI_z ( (innerOuterWidth/2)), 
  
// Outer quad plate
  qPlatesO_rMin ( R_magic() + (plateSep/2)),
  qPlatesO_rMax ( R_magic() + (plateSep/2) + innerOuterThickness),
  qPlatesO_rMax_Q1 ( R_magic() + (plateSep/2) + outerThickness_Q1),
  qPlatesO_z ( (innerOuterWidth/2)),
  
  innerSM_rMin ( R_magic()-vCInnerWall),
  innerSM_rMax ( qPlatesI_rMin),
  
// (Radially) outer support cut
  outerSM_rMin (qPlatesO_rMax),
// outerSM_rMax = R_magic()+vCOuterWall;
  outerSM_rMax( p.get<double>("outerSM_rMax") * in),
  
// All supports
  machining_z (QFR_z),
  machining_Sphi( p.get<double>("machining_Sphi") * degree),
  machining_Dphi( p.get<double>("machining_Dphi") * degree),

  qPlates_rMin{qPlatesI_rMin,qPlatesO_rMin,qPlatesTB_rMin,qPlatesTB_rMin},
  qPlates_rMax{qPlatesI_rMax,qPlatesO_rMax,qPlatesTB_rMax,qPlatesTB_rMax},
  qPlates_z{qPlatesI_z,qPlatesO_z,qPlatesTB_z,qPlatesTB_z},
  qCurl_rMin(curlInnerRadius),
  qCurl_rMax(curlOuterRadius),
  qCurl_majorRad{innerCurlMajorRadius,outerCurlMajorRadius},
  Sphi13( p.get<double>("Sphi13") * degree),
  Dphi13( p.get<double>("Dphi13") * degree),
  Sphi26( p.get<double>("Sphi26") * degree),
  Dphi26( p.get<double>("Dphi26") * degree),
  quad_Sphi{Sphi13, Sphi26},
  quad_Dphi{Dphi13, Dphi26},
  qPlatesTB_posZ((plateSep/2) + (topBottomThickness/2)),
  qPlatePosition{0.*m, 0.*m, qPlatesTB_posZ, -qPlatesTB_posZ},
  outerSupportOffset( p.get<double>("outerSupportOffset") * cm),
  ISupportRad(R_magic()-vCInnerWall),
  OSupportRad(R_magic()+vCOuterWall-outerSupportOffset),
  angSupportPos0( p.get<std::vector<double>>("angSupportPos0") ),
  angSupportPos1( p.get<std::vector<double>>("angSupportPos1") ),
  plateNames( p.get<std::vector<std::string>>("plateNames") ),
  numQuadRegions( p.get<int>("numQuadRegions") ),
  numQuadSections( p.get<int>("numQuadSections") ),
  numPlateTypes( p.get<int>("numPlateTypes") ),
  numSupportTypes( p.get<int>("numSupportTypes") ),
  numSupportPairsPerSection( p.get<std::vector<int>>("numSupportPairsPerSection") ),
  displayPlates( p.get<bool>("displayPlates") ),
  platesColor( p.get<std::vector<double>>("platesColor") ),
  displaySupports( p.get<bool>("displaySupports") ),
  supportsColor( p.get<std::vector<double>>("supportsColor") ),
  displayQFR( p.get<bool>("displayQFR") ),
  qfrColor( p.get<std::vector<double>>("qfrColor") )
  { 
  for (auto& entry : support_rInner ) { entry *= cm; }
  for (auto& entry : support_rOuter ) { entry *= cm; }
  for (auto& entry : support_z ) { entry *= cm; }
  for (auto& entry : angSupportPos0 ) { entry *= degree; }
  for (auto& entry : angSupportPos1 ) { entry *= degree; }

}

void gm2ringsim::QuadGeometry::print() const { 
  std::ostringstream oss;
  oss << "  DoScraping=" << DoScraping << "\n";
  oss << "  topBottomWidth=" << topBottomWidth << "\n";
  oss << "  topBottomThickness=" << topBottomThickness << "\n";
  oss << "  innerOuterWidth=" << innerOuterWidth << "\n";
  oss << "  innerOuterThickness=" << innerOuterThickness << "\n";
  oss << "  outerThickness_Q1=" << outerThickness_Q1 << "\n";
  oss << "  plateSep=" << plateSep << "\n";
  oss << "  curlInnerRadius=" << curlInnerRadius << "\n";
  oss << "  curlOuterRadius=" << curlOuterRadius << "\n";
  oss << "  innerCurlMajorRadius=" << innerCurlMajorRadius << "\n";
  oss << "  outerCurlMajorRadius=" << outerCurlMajorRadius << "\n";
  oss << "  curlHeightAboveMidplane=" << curlHeightAboveMidplane << "\n";
  oss << "  SupportMaterial=" << SupportMaterial << "\n";
  oss << "  support_nPlanes=" << support_nPlanes << "\n";
  oss << "  support_rInner= "; for (auto entry : support_rInner) { oss << " " << entry; }; oss << "\n";
  oss << "  support_rOuter= "; for (auto entry : support_rOuter) { oss << " " << entry; }; oss << "\n";
  oss << "  support_z= "; for (auto entry : support_z) { oss << " " << entry; }; oss << "\n";
  oss << "  support_Sphi=" << support_Sphi << "\n";
  oss << "  support_Dphi=" << support_Dphi << "\n";
  oss << "  QFR_rmin=" << QFR_rmin << "\n";
  oss << "  QFR_rmax=" << QFR_rmax << "\n";
  oss << "  QFR_z=" << QFR_z << "\n";
  
  oss << "  vCInnerWall=" << vCInnerWall<< "\n";
  oss << "  vCOuterWall=" << vCOuterWall<< "\n";
  
  oss << "  qPlatesTB_rMin=" << qPlatesTB_rMin<< "\n"; 
  oss << "  qPlatesTB_rMax=" << qPlatesTB_rMax<< "\n";
  oss << "  qPlatesTB_z=" << qPlatesTB_z<< "\n";
 
  oss << "  qPlatesI_rMin=" << qPlatesI_rMin<< "\n"; 
  oss << "  qPlatesI_rMax=" << qPlatesI_rMax << "\n"; 
  oss << "  qPlatesI_z=" << qPlatesI_z<< "\n" ; 
 
  oss << "  qPlatesO_rMin=" << qPlatesO_rMin<< "\n";
  oss << "  qPlatesO_rMax=" << qPlatesO_rMax<< "\n" ;
  oss << "  qPlatesO_rMax_Q1=" << qPlatesO_rMax_Q1<< "\n" ;
  oss << "  qPlatesO_z=" << qPlatesO_z<< "\n" ;
  
  oss << "  innerSM_rMin=" <<innerSM_rMin<< "\n";
  oss << "  innerSM_rMax=" <<innerSM_rMax<< "\n";
 
 // (Radially) outer support cut
  oss << "  outerSM_rMin=" << outerSM_rMin<< "\n";
  oss << "  outerSM_rMax=" << outerSM_rMax << "\n";
  oss << "  machining_Sphi=" << machining_Sphi << "\n";
  oss << "  machining_Dphi=" << machining_Dphi << "\n";
  for (int i=0;i<4;++i)
    oss << "  qPlates_rMin[i]="<<qPlates_rMin[i]<<"\n";
  for (int i=0;i<4;++i)
    oss << "  qPlates_rMax[i]="<<qPlates_rMax[i]<<"\n";
  for (int i=0;i<4;++i)
    oss << "  qPlates_z[i]="<<qPlates_z[i]<<"\n";

  oss << " qCurl_rMin=" << qCurl_rMin<<"\n";
  oss << " qCurl_rMax=" << qCurl_rMax<<"\n";
  
  for (int i=0;i<2;++i)
    oss << "   qCurl_majorRad[i] ="<<qCurl_majorRad[i]<<"\n";

  oss << "  Sphi13=" << Sphi13 << "\n";
  oss << "  Dphi13=" << Dphi13 << "\n";
  oss << "  Sphi26=" << Sphi26 << "\n";
  oss << "  Dphi26=" << Dphi26 << "\n";

 for (int i=0;i<2;++i)
   oss << "   quad_Sphi[i]="<<quad_Sphi[i]<<"\n";
 for (int i=0;i<2;++i)
   oss << "   quad_Dphi[i]="<<quad_Dphi[i]<<"\n";
 
 oss << "  qPlatesTB_posZ="<<qPlatesTB_posZ<<"\n";
 for (int i=0;i<4;++i)
   oss << "  qPlatePosition[i]="<<qPlatePosition[i]<<"\n";

 oss << "  outerSupportOffset="<<outerSupportOffset<<"\n";
 oss << "  ISupportRad="<<ISupportRad<<"\n";
 oss << "  OSupportRad="<<OSupportRad<<"\n";

 oss << "  angSupportPos0= "; for (auto entry : angSupportPos0) { oss << " " << entry; }; oss << "\n";
 oss << "  angSupportPos1= "; for (auto entry : angSupportPos1) { oss << " " << entry; }; oss << "\n";

oss << "  plateNames= "; for (auto entry : plateNames) { oss << " " << entry; }; oss << "\n";

oss << "  numQuadRegions=" << numQuadRegions << "\n";
oss << "  numQuadSections=" << numQuadSections << "\n";
oss << "  numPlateTypes=" << numPlateTypes << "\n";
oss << "  numSupportTypes=" << numSupportTypes << "\n";
oss << "  numSupportPairsPerSection= "; for (auto entry : numSupportPairsPerSection) { oss << " " << entry; }; oss << "\n";

oss << "  displayPlates=" << displayPlates << "\n";
oss << "  platesColor= "; for (auto entry : platesColor) { oss << " " << entry; }; oss << "\n";
oss << "  displaySupports=" << displaySupports << "\n";
oss << "  supportsColor= "; for (auto entry : supportsColor) { oss << " " << entry; }; oss << "\n";
oss << "  displayQFR=" << displayQFR << "\n";
oss << "  qfrColor= "; for (auto entry : qfrColor) { oss << " " << entry; }; oss << "\n";

 mf::LogInfo("QuadGeometry") << oss.str();
}//QuadGeometry::print()
