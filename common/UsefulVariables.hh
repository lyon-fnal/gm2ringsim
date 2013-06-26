#ifndef UsefulVariables_hh
#define UsefulVariables_hh

/** @file UsefulVariables.hh

    Provides a number of globally useful quantities

    - Thomas Gadfort '13
 */


#include "TVector3.h"
#include "TMath.h"
#include "Geant4/G4ThreeVector.hh"
#include "Geant4/G4Track.hh"

#include "Geant4/globals.hh"
namespace gm2ringsim {
  
  G4double ComputeZhat(double dt, int turn);
  G4double ComputeR(G4ThreeVector *pos);
  G4double ComputeTheta(G4ThreeVector *pos);
  G4double ComputeRhat(const G4Track *track);
  G4double ComputeRhat(G4Track *track);
  G4double ComputeRhat(G4ThreeVector *pos);
  G4double ComputeRhat(G4double r);
  G4double ComputeVhat(G4Track *track);
  G4double ComputeVhat(const G4Track *track);
  G4double ComputeVhat(G4ThreeVector *pos);
  G4double ComputeXe(G4double p, G4double n);
  G4double ComputePrhat(G4ThreeVector *pos, G4ThreeVector *mom);
  G4double ComputePvhat(G4ThreeVector *pos, G4ThreeVector *mom);

  // Polarizaton Function
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);
  void InitializeBeamCoordinates(TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam);
  void RotateRingToBeamCoordinates(double theta, TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam);


}//namespace gm2ringsim


#endif //UsefulVariables_hh

