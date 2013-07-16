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

G4double gm2ringsim::ComputeZhat(double dt, int turn)
{
  // Net angle around the ring (should probably be theta-hat)
  double omega_T = gm2ringsim::omegaCMagic()*(dt); // omega x (time - t0)
  G4double zhat = omega_T - turn*TMath::TwoPi(); // subtract off expected zhat for pmagic
  
  if ( zhat > TMath::Pi() )  { zhat -= TMath::TwoPi(); }
  if ( zhat < -TMath::Pi() ) { zhat += TMath::TwoPi(); }

  return( zhat );
}

G4double gm2ringsim::ComputeRhat(const G4Track *track)
{
  const G4ThreeVector pos = track->GetPosition();

  G4double r = std::sqrt(pos.x()*pos.x() + pos.z()*pos.z());

  return( ComputeRhat(r) );
}

G4double gm2ringsim::ComputeRhat(G4Track *track)
{
  const G4ThreeVector pos = track->GetPosition();

  G4double r = std::sqrt(pos.x()*pos.x() + pos.z()*pos.z());

  return( ComputeRhat(r) );
}

G4double gm2ringsim::ComputeRho(G4ThreeVector *pos)
{
  G4double rho = std::sqrt(pos->x()*pos->x() + pos->z()*pos->z());

  return( rho );
}

G4double gm2ringsim::ComputeRho(const G4ThreeVector *pos)
{
  G4double rho = std::sqrt(pos->x()*pos->x() + pos->z()*pos->z());

  return( rho );
}

G4double gm2ringsim::ComputeRhat(G4double r)
{
  G4double rhat = r - R_magic();
  return rhat;
}

G4double gm2ringsim::ComputeRhat(G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double rho = ComputeRho(pos);
  return( ComputeRhat(rho) );
}

G4double gm2ringsim::ComputeRhat(const G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double rho = ComputeRho(pos);
  return( ComputeRhat(rho) );
}

G4double gm2ringsim::ComputeVhat(const G4Track *track)
{
  const G4ThreeVector pos = track->GetPosition();

  G4double vhat = pos.y();
  return( vhat );
}

G4double gm2ringsim::ComputeVhat(G4Track *track)
{
  const G4ThreeVector pos = track->GetPosition();

  G4double vhat = pos.y();
  return( vhat );
}

G4double gm2ringsim::ComputeVhat(G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double vhat = pos->y();
  return( vhat );
}

G4double gm2ringsim::ComputeVhat(const G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double vhat = pos->y();
  return( vhat );
}

G4double gm2ringsim::ComputeTheta(G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  // Theta starts at in `up` direction and moves clockwise
  // Otherwise it's a right-handed coordinate system
  G4double theta = std::atan2(pos->z(),pos->x());
  if( theta < 0 )
    theta+= 2.*M_PI;

  return( theta );
}


G4double gm2ringsim::ComputePrhat(G4ThreeVector *pos, G4ThreeVector *mom)
{
  if ( pos == NULL || mom == NULL ) { return( -1.0 ); }

  G4double rho = ComputeRho(pos);
  G4double pmag = mom->mag();
  
  //-----------------
  // prhat = mom.dot(pos)/(|mom|*|pos|)
  //-----------------
  G4double prhat = (mom->x()*pos->x() + mom->z()*pos->z());
  prhat /= (rho*pmag);

  return( prhat );
}


G4double gm2ringsim::ComputePvhat(G4ThreeVector *pos, G4ThreeVector *mom)
{
  if ( pos == NULL || mom == NULL ) { return( -1.0 ); }

  //-----------------
  // pvhat = mom->y()/(|mom|)
  //-----------------
  G4double pmag = mom->mag();
  G4double pvhat = mom->y()/pmag;

  return( pvhat );
}


G4double gm2ringsim::ComputeXe(G4double p, G4double n)
{
  G4double xe = (p - P_magic()) * R_magic() / (P_magic() * ( 1 - n ));
  return xe;
}


void gm2ringsim::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz)
{  
  *pol_vertical = poly;
  *pol_radial   = polx;
  *pol_azimuth  = polz; 
}

void gm2ringsim::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double theta)
{
  // Theta is defined going counter clockwise around the ring starting at x = +1 and z = 0;
  TVector3 xAxis_beam, yAxis_beam, zAxis_beam;
  RotateRingToBeamCoordinates(theta, &xAxis_beam, &yAxis_beam, &zAxis_beam);
  TVector3 pol_ring(polx, poly, polz);
  *pol_vertical = pol_ring.Dot(yAxis_beam);
  *pol_radial   = pol_ring.Dot(xAxis_beam);
  *pol_azimuth  = pol_ring.Dot(zAxis_beam);
}

void gm2ringsim::ComputePolarization(double *pol_azimuth, double *pol_radial, double *pol_vertical, double polx, double poly, double polz, double x, double z)
{
  double theta_nom = TMath::ATan2(x, z);
  double theta = TMath::Pi()/2.0 - theta_nom;
  
  ComputePolarization(pol_azimuth, pol_radial, pol_vertical, polx, poly, polz, theta);
}

void gm2ringsim::InitializeBeamCoordinates(TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam)
{
  xAxis_beam->SetXYZ(1 , 0 , 0);
  yAxis_beam->SetXYZ(0 , 1 , 0);
  zAxis_beam->SetXYZ(0 , 0 , 1);
}


void gm2ringsim::RotateRingToBeamCoordinates(double theta, TVector3 *xAxis_beam, TVector3 *yAxis_beam, TVector3 *zAxis_beam)
{
  // Ring Coordinates          Beam Coordinates 
  // (y out of page)           (z out of page)
  //    +x                         +y
  //-z      +z          ---->  -x      +x
  //    -x                         -y

  InitializeBeamCoordinates(xAxis_beam, yAxis_beam, zAxis_beam);
  xAxis_beam->RotateY(-theta);
  zAxis_beam->RotateY(-theta);
}
