#include "gm2ringsim/fiberHarp/FiberHarpGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"

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
  azimuthalPos( p.get<std::vector<double>>("azimuthalPos"))
{}


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

  mf::LogInfo("FiberHarpGeometry") << oss.str();
}

