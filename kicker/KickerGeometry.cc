// Geometry for the Kicker

#include "gm2ringsim/kicker/KickerGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::KickerGeometry::KickerGeometry(std::string const & detName) :
  GeometryBase(detName),
  kickTime1( p.get<double>("kickTime1") * ns),
  kickTime2( p.get<double>("kickTime2") * ns),
  //  kickPercent( p.get<double>("kickPercent") * mm),
  kPlate1HV( p.get<double>("kPlate1HV") * kilovolt),
  kPlate2HV( p.get<double>("kPlate2HV") * kilovolt),
  kPlate3HV( p.get<double>("kPlate3HV") * kilovolt),
  kicker1L( p.get<double>("kicker1L") * henry),
  kicker1R( p.get<double>("kicker1R") * ohm),
  kicker1C( p.get<double>("kicker1C") * nanofarad),
  kicker2L( p.get<double>("kicker2L") * henry),
  kicker2R( p.get<double>("kicker2R") * ohm),
  kicker2C( p.get<double>("kicker2C") * nanofarad),
  kicker3L( p.get<double>("kicker3L") * henry),
  kicker3R( p.get<double>("kicker3R") * ohm),
  kicker3C( p.get<double>("kicker3C") * nanofarad),
  radius( p.get<double>("radius") * m),
  width( p.get<double>("width") * m),
  thickness( p.get<double>("thickness") * m),
  separation( p.get<double>("separation") * m),
  k1Sphi( p.get<double>("k1Sphi") * degree),
  k2Sphi( p.get<double>("k2Sphi") * degree),
  k3Sphi( p.get<double>("k3Sphi") * degree),
  kPlates_Dphi( p.get<double>("kPlates_Dphi") * deg),
  arc2( p.get<int>("arc2") ),
  arc3( p.get<int>("arc3") ),
  kickerHV( p.get<std::vector<double>>("kickerHV") ),
  kickPercent( p.get<std::vector<double>>("kickPercent") ),
  squareMag( p.get<std::vector<double>>("squareMag") ),
  circuitC( p.get<std::vector<double>>("circuitC") ),
  circuitL( p.get<std::vector<double>>("circuitL") ),
  circuitR( p.get<std::vector<double>>("circuitR") ),
  kickerOffsetTime( p.get<std::vector<double>>("kickerOffsetTime") ),
  kickerProperOffsetTime( p.get<std::vector<double>>("kickerProperOffsetTime") ),

// Derived parameters
// Defines the field-region                                                   
  kPlatesFR_rMin(radius - (separation/2)),
  kPlatesFR_rMax(radius + (separation/2)),
  kPlatesFR_z((separation/2)),
  
// Defines the inner kicker plate                                             
  kPlatesI_rMin(radius - (separation/2) - thickness),
  kPlatesI_rMax(radius - (separation/2)),
  kPlatesI_z((width/2)),
  
// Defines the outer kicker plate                                             
  kPlatesO_rMin(radius + (separation/2)),
  kPlatesO_rMax(radius + (separation/2) + thickness),
  kPlatesO_z((width/2)),
  displayPlates( p.get<bool>("displayPlates") ),
  platesColor( p.get<std::vector<double>>("platesColor") )
{  

  for (auto& entry : kickerHV ) { entry *= kilovolt; }
  for (auto& entry : squareMag ) { entry *= gauss; }
  for (auto& entry : circuitC ) { entry *= nanofarad; }
  for (auto& entry : circuitL ) { entry *= henry; }
  for (auto& entry : circuitR ) { entry *= ohm; }
  for (auto& entry : kickerOffsetTime ) { entry *= ns;}
  for (auto& entry : kickerProperOffsetTime ) { entry *= ns;}
  kPlates_rMin.push_back(kPlatesFR_rMin);
  kPlates_rMin.push_back(kPlatesI_rMin);
  kPlates_rMin.push_back(kPlatesO_rMin);
  kPlates_rMax.push_back(kPlatesFR_rMax);
  kPlates_rMax.push_back(kPlatesI_rMax);
  kPlates_rMax.push_back(kPlatesO_rMax);
  kPlates_z.push_back(kPlatesFR_z);
  kPlates_z.push_back(kPlatesI_z);
  kPlates_z.push_back(kPlatesO_z);  
  kPlates_Sphi.push_back(k1Sphi);
  kPlates_Sphi.push_back(k2Sphi);
  kPlates_Sphi.push_back(k3Sphi);

  kVacWallArray.push_back(arc2);
  kVacWallArray.push_back(arc2);
  kVacWallArray.push_back(arc3);
  
}


void gm2ringsim::KickerGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  std::ostringstream oss;
  oss << "  kickTime1=" << kickTime1 << "\n";
  oss << "  kickTime2=" << kickTime2 << "\n";
  //  oss << "  kickPercent=" << kickPercent << "\n";
  oss << "  kPlate1HV=" << kPlate1HV << "\n";
  oss << "  kPlate2HV=" << kPlate2HV << "\n";
  oss << "  kPlate3HV=" << kPlate3HV << "\n";
  oss << "  kicker1L=" << kicker1L << "\n";
  oss << "  kicker1R=" << kicker1R << "\n";
  oss << "  kicker1C=" << kicker1C << "\n";
  oss << "  kicker2L=" << kicker2L << "\n";
  oss << "  kicker2R=" << kicker2R << "\n";
  oss << "  kicker2C=" << kicker2C << "\n";
  oss << "  kicker3L=" << kicker3L << "\n";
  oss << "  kicker3R=" << kicker3R << "\n";
  oss << "  kicker3C=" << kicker3C << "\n";
  
  oss << "  radius=" << radius << "\n";
  oss << "  width=" << width << "\n";
  oss << "  thickness=" << thickness << "\n";
  oss << "  separation=" << separation << "\n";
  oss << "  k1Sphi=" << k1Sphi << "\n";
  oss << "  k2Sphi=" << k2Sphi << "\n";
  oss << "  k3Sphi=" << k3Sphi << "\n";
  oss << "  kPlates_Dphi=" << kPlates_Dphi << "\n";
  oss << "  arc2=" << arc2 << "\n";
  oss << "  arc3=" << arc3 << "\n";
  
  
  oss << "  kPlatesFR_rMin=" <<kPlatesFR_rMin << "\n";
  oss << "  kPlatesFR_rMax=" <<  kPlatesFR_rMax << "\n";
  oss << "  kPlatesFR_z=" <<  kPlatesFR_z << "\n";
  oss << "  kPlatesI_rMin=" <<  kPlatesI_rMin << "\n";
  oss << "  kPlatesI_rMax=" <<  kPlatesI_rMax << "\n";
  oss << "  kPlatesI_z=" <<  kPlatesI_z << "\n";
  oss << "  kPlatesO_rMin=" <<  kPlatesO_rMin << "\n";
  oss << "  kPlatesO_rMax=" <<  kPlatesO_rMax << "\n";
  oss << "  kPlatesO_z=" <<  kPlatesO_z << "\n";
  


  oss << "  kickerHV= "; for (auto entry : kickerHV) { oss << " " << entry; }; oss << "\n";
  oss << "  kickPercent= "; for (auto entry : kickPercent) { oss << " " << entry; }; oss << "\n";
  oss << "  squareMag= "; for (auto entry : squareMag) { oss << " " << entry; }; oss << "\n";
  oss << "  circuitC= "; for (auto entry : circuitC) { oss << " " << entry; }; oss << "\n";
  oss << "  circuitL= "; for (auto entry : circuitL) { oss << " " << entry; }; oss << "\n";
  oss << "  circuitR= "; for (auto entry : circuitR) { oss << " " << entry; }; oss << "\n";  

  oss << "  kickerOffsetTime= "; for (auto entry : kickerOffsetTime) { oss << " " << entry; }; oss << "\n";  
  oss << "  kickerProperOffsetTime= "; for (auto entry : kickerProperOffsetTime) { oss << " " << entry; }; oss << "\n";  

  oss << "   kPlates_rMin=" ;  for (auto entry : kPlates_rMin){  oss << " " << entry; }; oss << "\n";
oss << "   kPlates_rMax=" ;  for (auto entry : kPlates_rMax){  oss << " " << entry; }; oss << "\n";
oss << "   kPlates_z=" ;  for (auto entry : kPlates_z){  oss << " " << entry; }; oss << "\n";
oss << "   kPlates_Sphi=" ;  for (auto entry : kPlates_Sphi){  oss << " " << entry; }; oss << "\n";


oss << "  displayPlates=" << displayPlates << "\n";
oss << "  platesColor= "; for (auto entry : platesColor) { oss << " " << entry; }; oss << "\n";
  
  mf::LogInfo("KICKERGEOM") << oss.str();
}

