// Geometry for the world

#include "gm2ringsim/world/ArcGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>

gm2ringsim::ArcGeometry::ArcGeometry(std::string const & detName) :
  GeometryBase(detName),
  arc_rMin(p.get<double>(arc_rMin) * mm),
  arc_rMax(p.get<double>(arc_rMax) * mm),
  arc11_rExtension(p.get<double>(arc11_rExtension) * mm),
  arc_z(p.get<double>(arc_z) * mm),
  arc_Sphi(p.get<double>(arc_Sphi) * deg),
  arc_Dphi(p.get<double>(arc_Dphi) * deg)
  {}

void gm2ringsim::ArcGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("ArcGeometry") << "Arc geometry is"
  << " arc_rMin=" << arc_rMin
  << " arc_rMax=" << arc_rMax
  << " arc11_rExtension=" << arc11_rExtension
  << " arc_z=" << arc_z
  << " arc_Sphi=" << arc_Sphi
  << " arc_Dphi=" << arc_Dphi;
}

