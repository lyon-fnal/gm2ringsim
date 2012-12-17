#include "gm2ringsim/collimator/CollimatorGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

/** @file CollimatorGeometry.cc

    Implements the geometry data for the fiber harps

    @author Peter Winter
    @date 2012
*/


#include "Geant4/globals.hh"

#include <iostream>

gm2ringsim::CollimatorGeometry::CollimatorGeometry(std::string detName) :
  GeometryBase(detName),
  coll_rMin( p.get<double>("coll_rMin") * m),
  coll_rMax( p.get<double>("coll_rMax") * m),
  coll_z( p.get<double>("coll_z") * m),
  coll_Sphi_Full( p.get<double>("coll_Sphi_Full") * deg),
  coll_Dphi_Full( p.get<double>("coll_Dphi_Full") * deg),
  coll_Sphi_Half( p.get<double>("coll_Sphi_Half") * deg),
  coll_Dphi_Half( p.get<double>("coll_Dphi_Half") * deg),
  nCollimators( p.get<double>("nCollimators") ),
  azimuthalPos( p.get<std::vector<double>>("azimuthalPos") ),
  cVacWallArray( p.get<std::vector<int>>("cVacWallArray") ),
  coll_Sphi{coll_Sphi_Full, coll_Sphi_Half, coll_Sphi_Half},
  coll_Dphi{coll_Dphi_Full, coll_Dphi_Half, coll_Dphi_Half},
  display( p.get<bool>("display") ),
  collimatorColor( p.get<std::vector<double>>("collimatorColor") )
{
  for (auto& entry : azimuthalPos ) { entry *= deg; }

  if(azimuthalPos.size() != (unsigned int)nCollimators) throw cet::exception("CollimatorGeometry") << 
    "Wrong geometry input: Size of azimuthalPos array is " << azimuthalPos.size() << 
    " and not equals nHarps=" << nCollimators << std::endl;
  if(cVacWallArray.size() != (unsigned int)nCollimators) throw cet::exception("CollimatorGeometry") << 
    "Wrong geometry input: Size of cVacWallArray array is " << cVacWallArray.size() << 
    " and not equals nHarps=" << nCollimators << std::endl;

  std::vector<std::string> collimatorName = p.get<std::vector<std::string>>("collimatorType");

  if(collimatorName.size() != (unsigned int)nCollimators) 
    throw cet::exception("CollimatorGeometry") << 
      "Wrong geometry input: Size of collimatorName array is " << collimatorName.size() << 
      " and not equals nHarps=" << nCollimators << std::endl;
    
  for ( auto entry : collimatorName){
    int collType = -1;
    if(entry == "FULL") collType = FULL;
    if(entry == "HALF_LRO") collType = HALF_LRO;
    if(entry == "HALF_SRO") collType = HALF_SRO;
    if(entry == "OFF") collType = OFF;
    
    if(collType == -1) throw cet::exception("CollimatorGeometry") << "collimatorType entry " << entry << 
			 " not FULL, HALF_LRO, HALF_SRO, OFF!\n";
    
    collimatorType.push_back(collType);
  }

  print();
}


void gm2ringsim::CollimatorGeometry::print(){
  std::ostringstream oss;

  oss << "  coll_rMin=" << coll_rMin << "\n";
  oss << "  coll_rMax=" << coll_rMax << "\n";
  oss << "  coll_z=" << coll_z << "\n";
  oss << "  coll_Sphi_Full=" << coll_Sphi_Full << "\n";
  oss << "  coll_Dphi_Full=" << coll_Dphi_Full << "\n";
  oss << "  coll_Sphi_Half=" << coll_Sphi_Half << "\n";
  oss << "  coll_Dphi_Half=" << coll_Dphi_Half << "\n";
  oss << "  nCollimators=" << nCollimators << "\n";
  oss << "  azimuthalPos= "; for (auto entry : azimuthalPos) { oss << " " << entry; }; oss << "\n";
  oss << "  cVacWallArray= "; for (auto entry : cVacWallArray) { oss << " " << entry; }; oss << "\n";
  oss << "  collimatorType= "; for (auto entry : collimatorType) { oss << " " << entry; }; oss << "\n";
  oss << "  display=" << display << "\n";
  oss << "  collimatorColor= "; for (auto entry : collimatorColor) { oss << " " << entry; }; oss << "\n";
  
  mf::LogInfo("CollimatorGeometry") << oss.str();
}

