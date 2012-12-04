#include "gm2ringsim/fiberHarp/FiberHarpGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

/** @file FiberHarpGeometry.cc

    Implements the geometry data for the fiber harps

    @author Peter Winter
    @date 2012
*/


#include "Geant4/globals.hh"

#include <iostream>

gm2ringsim::FiberHarpGeometry::FiberHarpGeometry(std::string detName) :
  GeometryBase(detName),
  fiberLength( p.get<double>("fiberLength") * mm),
  fiberRadius( p.get<double>("fiberRadius") * mm),
  fiberSpacing( p.get<double>("fiberSpacing") * mm),
  supportLength( p.get<double>("supportLength") * mm),
  supportWidth( p.get<double>("supportWidth") * mm),
  supportDepth( p.get<double>("supportDepth") * mm),
  nFibers( p.get<double>("nFibers")),
  coll_z( p.get<double>("coll_z") * m),
  nHarps( p.get<int>("nHarps")),
  azimuthalPos( p.get<std::vector<double>>("azimuthalPos")),
  vacWallPos( p.get<std::vector<double>>("vacWallPos") ),
  harpType( p.get<std::vector<double>>("harpType") ),
  display( p.get<bool>("display")),
  fiberHarpColor( p.get<std::vector<double>>("fiberHarpColor"))
{
  for (auto& entry : azimuthalPos ){ 
    entry *= deg; 
  }
}


void gm2ringsim::FiberHarpGeometry::print(){
  std::ostringstream oss;

  mf::LogInfo("FiberHarpGeometry") << "Fiber harp geometry is" ;
  oss << "  fiberLength=" << fiberLength << "\n";
  oss << "  fiberRadius=" << fiberRadius << "\n";
  oss << "  fiberSpacing=" << fiberSpacing << "\n";
  oss << "  supportLength=" << supportLength << "\n";
  oss << "  supportWidth=" << supportWidth << "\n";
  oss << "  supportDepth=" << supportDepth << "\n";
  oss << "  nFibers=" << nFibers << "\n";
  oss << "  coll_z=" << coll_z << "\n";
  oss << "  nHarps=" << nHarps << "\n";
  oss << "  azimuthalPos= "; for (auto entry : azimuthalPos) { oss << " " << entry; }; oss << "\n";
  oss << "  vacWallPos= "; for (auto entry : vacWallPos) { oss << " " << entry; }; oss << "\n";
  oss << "  harpType= "; for (auto entry : harpType) { oss << " " << entry; }; oss << "\n";
  oss << "  display=" << display << "\n";

  mf::LogInfo("FiberHarpGeometry") << oss.str();
}

