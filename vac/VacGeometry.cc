// Geometry for the Vac

#include "gm2ringsim/vac/VacGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>

gm2ringsim::VacGeometry::VacGeometry(std::string const & detName) :
  GeometryBase(detName),
  vac_rMin(         p.get<double>("vac_rMin")         * mm),
  vac_rMax(         p.get<double>("vac_rMax")         * mm),
  vac11_rExtension( p.get<double>("vac11_rExtension") * mm),
  vac_z(            p.get<double>("vac_z")            * mm),
  vac_Sphi(         p.get<double>("vac_Sphi")         * deg),
  vac_Dphi(         p.get<double>("vac_Dphi")         * deg)
  {}

void gm2ringsim::VacGeometry::print() const {
  
  // Use the MessageLogger (see https://cdcvs.fnal.gov/redmine/projects/novaart/wiki/Using_the_Framework#MessageLogger )
  mf::LogInfo("VacGeometry") << "Vac geometry is" << "\n"
  << " vac_rMin="         << vac_rMin << "\n"
  << " vac_rMax="         << vac_rMax << "\n"
  << " vac11_rExtension=" << vac11_rExtension << "\n"
  << " vac_z="            << vac_z << "\n"
  << " vac_Sphi="         << vac_Sphi << "\n"
  << " vac_Dphi="         << vac_Dphi;
}

