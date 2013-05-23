#ifndef UsefulVariables_hh
#define UsefulVariables_hh

/** @file UsefulVariables.hh

    Provides a number of globally useful precisely calculated values.

    - Kevin Lynch 2009
    - Nathan Froemming 2012
 */

#include "Geant4/globals.hh"
namespace gm2ringsim {

  G4double ComputeRhat(G4double r);
  G4double ComputeXe(G4double p, G4double n);

}//namespace gm2ringsim


#endif //UsefulVariables_hh

