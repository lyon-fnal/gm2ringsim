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

void gm2ringsim::ComputeXZFromRhatTheta(G4double *x, G4double *z, G4double rhat, G4double theta)
{
  *x = (rhat + R_magic())*TMath::Cos(theta);
  *z = (rhat + R_magic())*TMath::Sin(theta);
}

void gm2ringsim::ComputeRhatThetaFromXZ(G4double *rhat, G4double *theta, G4double x, G4double z)
{
  *rhat  = ComputeRhat(x, z);
  *theta = ComputeTheta(x, z);
}

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

G4double gm2ringsim::ComputeRho(TVector3 *pos)
{
  G4double rho = std::sqrt(pos->X()*pos->X() + pos->Z()*pos->Z());

  return( rho );
}

G4double gm2ringsim::ComputeRho(const G4ThreeVector *pos)
{
  G4double rho = std::sqrt(pos->x()*pos->x() + pos->z()*pos->z());

  return( rho );
}

G4double gm2ringsim::ComputeRhat(G4double x, G4double z)
{
  G4double r = std::sqrt(x*x + z*z);
  return( ComputeRhat(r) );
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

G4double gm2ringsim::ComputeRhat(TVector3 *pos)
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

G4double gm2ringsim::ComputeVhat(TVector3 *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double vhat = pos->Y();
  return( vhat );
}

G4double gm2ringsim::ComputeVhat(const G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  G4double vhat = pos->y();
  return( vhat );
}

G4double gm2ringsim::ConvertTheta(double theta)
{
  if ( theta >= TMath::Pi() )  { theta -= TMath::TwoPi(); }
  if ( theta <= -TMath::Pi() ) { theta += TMath::TwoPi(); }

  return( theta );
}

G4double gm2ringsim::ComputeTheta(G4ThreeVector *pos)
{
  if ( pos == NULL ) { return( -1.0 ); }

  return( ComputeTheta(pos->x(), pos->z()) );
}

G4double gm2ringsim::ComputeTheta(double x, double z)
{
  // Theta starts at in `up` direction and moves clockwise
  // Otherwise it's a right-handed coordinate system
  // "x" is the "y" coordinate here and "z" is the "x" coordiante for atan2
  G4double theta = std::atan2(x,z);
  theta = TMath::Pi()/2.0 - theta;
  if( theta < 0 )
    theta+= 2.*M_PI;


  return( theta );
}

G4double gm2ringsim::ComputeFieldTheta(double x, double y)
{
  // Theta starts at in `out` direction (i.e., +x, y=0)
  // x and y are treated in the usual coordinates here
  G4double theta = std::atan2(y,x);
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


G4double gm2ringsim::ComputePrhat(TVector3 *pos, TVector3 *mom)
{
  if ( pos == NULL || mom == NULL ) { return( -1.0 ); }

  G4double rho = ComputeRho(pos);
  G4double pmag = mom->Mag();
  
  //-----------------
  // prhat = mom.dot(pos)/(|mom|*|pos|)
  //-----------------
  G4double prhat = (mom->X()*pos->X() + mom->X()*pos->Z());
  prhat /= (rho*pmag);

  return( prhat );
}


G4double gm2ringsim::ComputePvhat(TVector3 *pos, TVector3 *mom)
{
  if ( pos == NULL || mom == NULL ) { return( -1.0 ); }

  //-----------------
  // pvhat = mom->y()/(|mom|)
  //-----------------
  G4double pmag = mom->Mag();
  G4double pvhat = mom->Y()/pmag;

  return( pvhat );
}

G4double gm2ringsim::ComputeMomFromXe(G4double xe, G4double n)
{
  G4double mom = (xe * P_magic() * (1 - n) / R_magic()) + P_magic();

  return( mom );
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

void gm2ringsim::ComputeGEANTPosition(double *posx, double *posy, double *posz, double r, double vhat, double theta)
{
  *posy = vhat;
  *posx = (r + R_magic())*TMath::Cos(theta);
  *posz = (r + R_magic())*TMath::Sin(theta);
}


void gm2ringsim::ComputeGEANTMomentum(double *momx, double *momy, double *momz, double mom_radial, double mom_vertical, double mom_azimuth, double theta)
{
  *momx = mom_radial * TMath::Cos(theta) - mom_azimuth * TMath::Sin(theta);
  *momy = mom_vertical;
  *momz = mom_radial * TMath::Sin(theta) + mom_azimuth * TMath::Cos(theta);
}

G4double gm2ringsim::Energy(G4double gamma)
{
  static G4double const E_ = gamma*mMuon();
  //  G4cout << "E = " << E_/MeV << " MeV" << G4endl;
  return E_;
}

G4double gm2ringsim::Mom(G4double gamma)
{
  static G4double const E_ = Energy(gamma);
  static G4double const Mom_ = TMath::Sqrt(E_*E_ - mMuon()*mMuon());

  return( Mom_ );
}

G4double gm2ringsim::Gamma(G4double mom)
{
  static G4double const E_ = TMath::Sqrt(mom*mom + mMuon()*mMuon());
  static G4double const Gamma_ = E_/mMuon();

  return( Gamma_ );
}

G4double gm2ringsim::R(G4double gamma, G4double n)
{
  static G4double const Mom_ = Mom(gamma);
  return( ComputeXe(Mom_, n) + R_magic() );
}

G4double gm2ringsim::Beta(G4double gamma)
{
  static G4double const gamma_2_ = gamma*gamma;
  static G4double const beta_ = std::sqrt( 1.-1./gamma_2_ );
  //  G4cout << "beta = " << beta_ << G4endl;
  return beta_;
}

G4double gm2ringsim::ComputeOmegaCGivenMomFieldIndex(G4double mom, G4double n)
{
  static G4double const gamma_ = Gamma(mom);
  static G4double const omegaC_ = Beta(gamma_)*c_light/R(gamma_, n);
  //  G4cout << "omegaC = " << omegaC_ << " rad/ns" << G4endl;
  return omegaC_;
}

G4double gm2ringsim::ComputeOmegaCGivenMomRadius(G4double mom, G4double radius)
{
  static G4double const gamma_ = Gamma(mom);
  static G4double const omegaC_ = Beta(gamma_)*c_light/radius;
  //  G4cout << "omegaC = " << omegaC_ << " rad/ns" << G4endl;
  return omegaC_;
}

G4double gm2ringsim::ComputeOmegaAGivenMomFieldIndex(G4double mom, G4double n)
{
  static G4double const gamma_= Gamma(mom);
  static G4double const omegaC = ComputeOmegaCGivenMomFieldIndex(mom, n);
  static G4double const omegaA_ = aMuon()*gamma_ * omegaC;
  //  G4cout << "omegaA = " << omegaA_ << " rad/ns" << G4endl;
  return omegaA_;
}

G4double gm2ringsim::ComputeOmegaAGivenMomRadius(G4double mom, G4double radius)
{
  static G4double const gamma_= Gamma(mom);
  static G4double const omegaC = ComputeOmegaCGivenMomRadius(mom, radius);
  static G4double const omegaA_ = aMuon()*gamma_ * omegaC;
  //  G4cout << "omegaA = " << omegaA_ << " rad/ns" << G4endl;
  return omegaA_;
}

G4double gm2ringsim::ComputeOmegaSGivenMomFieldIndex(G4double mom, G4double n)
{
  static G4double const gamma_ = Gamma(mom);
  static G4double const omegaS_ = ComputeOmegaCGivenMomFieldIndex(gamma_, n) + ComputeOmegaAGivenMomFieldIndex(gamma_, n);
  //  G4cout << "omegaS = " << omegaS_ << " rad/ns" << G4endl;
  return omegaS_;
}

G4double gm2ringsim::ComputeOmegaSGivenMomRadius(G4double mom, G4double radius)
{
  static G4double const gamma_ = Gamma(mom);
  static G4double const omegaS_ = ComputeOmegaCGivenMomRadius(gamma_, radius) + ComputeOmegaAGivenMomRadius(gamma_, radius);
  //  G4cout << "omegaS = " << omegaS_ << " rad/ns" << G4endl;
  return omegaS_;
}


