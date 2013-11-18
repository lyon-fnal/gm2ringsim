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
  
  void ComputeXZFromRhatTheta(G4double *x, G4double *z, G4double rhat, G4double theta);
  
  G4double ComputeZhat(double dt, int turn);

  G4double ComputeRho(TVector3 *pos);
  G4double ComputeRho(G4ThreeVector *pos);
  G4double ComputeRho(const G4ThreeVector *pos);

  G4double ComputeTheta(G4ThreeVector *pos);
  G4double ComputeTheta(double x, double z);
  G4double ConvertTheta(double theta);

  G4double ComputeRhat(const G4Track *track);
  G4double ComputeRhat(G4Track *track);
  G4double ComputeRhat(G4ThreeVector *pos); 
  G4double ComputeRhat(TVector3 *pos);  
  G4double ComputeRhat(const G4ThreeVector *pos);  
  G4double ComputeRhat(G4double r);

  G4double ComputeVhat(G4Track *track);
  G4double ComputeVhat(const G4Track *track);
  G4double ComputeVhat(TVector3 *pos);
  G4double ComputeVhat(G4ThreeVector *pos);
  G4double ComputeVhat(const G4ThreeVector *pos);


  G4double ComputeMomFromXe(G4double p, G4double n);
  G4double ComputeXe(G4double p, G4double n);

  G4double ComputePrhat(G4ThreeVector *pos, G4ThreeVector *mom);
  G4double ComputePvhat(G4ThreeVector *pos, G4ThreeVector *mom);
  G4double ComputePrhat(TVector3 *pos, TVector3 *mom);
  G4double ComputePvhat(TVector3 *pos, TVector3 *mom);

  // Polarizaton Function
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta);
  void ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z);
  void InitializeBeamCoordinates(TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam);
  void RotateRingToBeamCoordinates(double theta, TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam);

  void ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz);
  void ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz, double theta);
  void ComputeMomentum(double *mom_azimuth, double *mom_radial, double *mom_vertical, double momx, double momy, double momz, double x, double z);

  void ComputeGEANTPosition(double *posx, double *posy, double *posz, double r, double vhat, double theta);
  void ComputeGEANTMomentum(double *momx, double *momy, double *momz, double mom_radial, double mom_vertical, double mom_azimuth, double theta);

  G4double Energy(G4double gamma);
  G4double Mom(G4double gamma);
  G4double Gamma(G4double mom);
  G4double R(G4double gamma, G4double n);
  G4double Beta(G4double gamma);
  G4double ComputeOmegaCGivenMomFieldIndex(G4double mom, G4double n);
  G4double ComputeOmegaCGivenMomRadius(G4double mom, G4double radius);
  inline G4double ComputeFrequencyCGivenMomFieldIndex(G4double mom, G4double n) { return( ComputeOmegaCGivenMomFieldIndex(mom, n)/TMath::TwoPi() ); }
  inline G4double ComputeFrequencyCGivenMomRadius(G4double mom, G4double radius) { return( ComputeOmegaCGivenMomRadius(mom, radius)/TMath::TwoPi() ); }
  G4double ComputeOmegaAGivenMomFieldIndex(G4double mom, G4double n);
  G4double ComputeOmegaAGivenMomRadius(G4double mom, G4double n);
  inline G4double ComputeFrequencyAGivenMomFieldIndex(G4double mom, G4double n) { return( ComputeOmegaAGivenMomFieldIndex(mom, n)/TMath::TwoPi() ); }
  inline G4double ComputeFrequencyAGivenMomRadius(G4double mom, G4double radius) { return( ComputeOmegaAGivenMomRadius(mom, radius)/TMath::TwoPi() ); }
  G4double ComputeOmegaSGivenMomFieldIndex(G4double mom, G4double n);
  G4double ComputeOmegaSGivenMomRadius(G4double mom, G4double radius);
  inline G4double ComputeFrequencySGivenMomFieldIndex(G4double mom, G4double n) { return( ComputeOmegaSGivenMomFieldIndex(mom, n)/TMath::TwoPi() ); }
  inline G4double ComputeFrequencySGivenMomRadius(G4double mom, G4double radius) { return( ComputeOmegaSGivenMomRadius(mom, radius)/TMath::TwoPi() ); }





}//namespace gm2ringsim


#endif //UsefulVariables_hh

