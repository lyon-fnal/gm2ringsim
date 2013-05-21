#include "gm2ringsim/common/UsefulVariables.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "Geant4/G4ParticleTable.hh"
#include <cmath>

/** @file UsefulVariables.cc

    Provides the definitions and properly ordered static
    initializations for the precision values.

    - Zach   Hartwig   2005
    - Kevin  Lynch     2009
    - Nathan Froemming 2012
    - Brendan Kiburg   Dec 2012 Moving to ART
*/

G4double gm2ringsim::ComputeRhat(G4double r)
{
  G4double rhat = r - R_magic();
  return rhat;
}

G4double gm2ringsim::ComputeXe(G4double p, G4double n)
{
  G4double xe = (p - P_magic()) * R_magic() / (P_magic() * ( 1 - n ));
  return xe;
}
