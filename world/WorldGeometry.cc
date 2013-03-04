// Geometry for the world

#include "gm2ringsim/world/WorldGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>

gm2ringsim::WorldGeometry::WorldGeometry(std::string const & detName) :
  GeometryBase(detName),
  world_x(p.get<double>("world_x") * mm),
  world_y(p.get<double>("world_y") * mm),
  world_z(p.get<double>("world_z") * mm),
  display( p.get<bool>("display")),
  worldColor( p.get<std::vector<double>>("worldColor"))
{}

void gm2ringsim::WorldGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("WorldGeometry") << "World geometry is"
                             << " world_x=" << world_x
                             << " world_y=" << world_y
                             << " world_z=" << world_z;  
}

