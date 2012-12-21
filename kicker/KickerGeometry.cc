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
kickPercent( p.get<double>("kickPercent") * mm),
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
kicker3C( p.get<double>("kicker3C") * nanofarad)
{
}


void gm2ringsim::KickerGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  std::ostringstream oss;
oss << "  kickTime1=" << kickTime1 << "\n";
oss << "  kickTime2=" << kickTime2 << "\n";
oss << "  kickPercent=" << kickPercent << "\n";
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
 
  mf::LogInfo("KICKERGEOM") << oss.str();
}

