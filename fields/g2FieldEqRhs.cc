/** @file g2FieldEqRhs.cc

    Provides (poor) implementations of time dependent magnetic field
    classes. 

    - Kevin Lynch 2009
*/

// swiped from G4EqMagElectricField.cc

#include "gm2ringsim/fields/g2FieldEqRhs.hh"
#include "Geant4/globals.hh"

void  
g2TimeDepMagField_EqRhs::SetChargeMomentumMass(G4double particleCharge, // e+ units
		                            G4double,
                                            G4double particleMass)
{
  fMagCof =  eplus*particleCharge*c_light ;
  fMassCof = particleMass*particleMass ; 
}



void
g2TimeDepMagField_EqRhs::EvaluateRhsGivenB(const G4double y[],
					   const G4double Field[],
					   G4double dydx[] ) const
{

  // Components of y:
  //    0-2 dr/ds, 
  //    3-5 dp/ds - momentum derivatives 
  
  G4double pSquared = y[3]*y[3] + y[4]*y[4] + y[5]*y[5] ;
  
  G4double Energy   = std::sqrt( pSquared + fMassCof );
  
  G4double pModuleInverse  = 1.0/std::sqrt(pSquared) ;
   
  G4double inverse_velocity = Energy * pModuleInverse / c_light;
  
  G4double cof1     = fMagCof*pModuleInverse ;
  
  
  dydx[0] = y[3]*pModuleInverse ;                         
  dydx[1] = y[4]*pModuleInverse ;                         
  dydx[2] = y[5]*pModuleInverse ;                        
  
  dydx[3] = cof1*(y[4]*Field[2] - y[5]*Field[1]) ;
  
  dydx[4] = cof1*(y[5]*Field[0] - y[3]*Field[2]) ; 
  
  dydx[5] = cof1*(y[3]*Field[1] - y[4]*Field[0]) ;  
  
  dydx[6] = 0.;//not used
  
  // Lab Time of flight
  dydx[7] = inverse_velocity;
}


////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////


void  
g2TimeDepMagField_SpinEqRhs::SetChargeMomentumMass(G4double charge, // e+ units
						   G4double momentum,
						   G4double particleMass)
{
  fMagCof =  eplus*particleCharge*c_light ;
  fMassCof = particleMass*particleMass ; 
  
  particleCharge = charge;

  omegac = 0.105658387*GeV/particleMass * 2.837374841e-3*(rad/cm/kilogauss);
  
  E = std::sqrt(sqr(momentum)+sqr(particleMass));
  beta  = momentum/E;
  gamma = E/particleMass;

}

#include "Geant4/G4ThreeVector.hh"

void
g2TimeDepMagField_SpinEqRhs::EvaluateRhsGivenB(const G4double y[],
					       const G4double Field[],
					       G4double dydx[] ) const
{

  // Components of dy/dx:
  //    0-2 dr/ds, 
  //    3-5 dp/ds - momentum derivatives 
  
  G4double pSquared = y[3]*y[3] + y[4]*y[4] + y[5]*y[5] ;
  
  G4double Energy   = std::sqrt( pSquared + fMassCof );
  
  G4double pModuleInverse  = 1.0/std::sqrt(pSquared) ;
   
  G4double inverse_velocity = Energy * pModuleInverse / c_light;
  
  G4double cof1     = fMagCof*pModuleInverse ;
  
  
  dydx[0] = y[3]*pModuleInverse ;                         
  dydx[1] = y[4]*pModuleInverse ;                         
  dydx[2] = y[5]*pModuleInverse ;                        
  
  dydx[3] = cof1*(y[4]*Field[2] - y[5]*Field[1]) ;
  
  dydx[4] = cof1*(y[5]*Field[0] - y[3]*Field[2]) ; 
  
  dydx[5] = cof1*(y[3]*Field[1] - y[4]*Field[0]) ;  
  
  dydx[6] = 0.;//not used
  
  // Lab Time of flight
  dydx[7] = inverse_velocity;

  dydx[8] = 0.;

  G4ThreeVector u(y[3], y[4], y[5]);
  u *= pModuleInverse;
  
  G4ThreeVector BField(Field[0],Field[1],Field[2]);

  G4double udb = anomaly*beta*gamma/(1.+gamma) * (BField * u); 
  G4double ucb = (anomaly+1./gamma)/beta;

  G4ThreeVector Spin(y[9],y[10],y[11]);
  //  G4cout << Spin << '\n';
  G4ThreeVector dSpin = 
    particleCharge*omegac*(ucb*(Spin.cross(BField))-udb*(Spin.cross(u)));

  dydx[ 9] = dSpin.x();
  dydx[10] = dSpin.y();
  dydx[11] = dSpin.z();

}



g2Mag_SpinEqRhs::g2Mag_SpinEqRhs( G4MagneticField* MagField )
  : G4Mag_EqRhs( MagField ) 
{
   anomaly = 0.0011659208;
}

g2Mag_SpinEqRhs::~g2Mag_SpinEqRhs()
{
}

void
g2Mag_SpinEqRhs::SetChargeMomentumMass(G4double particleCharge, // in e+ units
                                       G4double MomentumXc,
                                       G4double particleMass)
{
   //  To set fCof_val 
   G4Mag_EqRhs::SetChargeMomentumMass(particleCharge, MomentumXc, particleMass);

   omegac = 0.105658387*GeV/particleMass * 2.837374841e-3*(rad/cm/kilogauss);

   ParticleCharge = particleCharge;

   E = std::sqrt(sqr(MomentumXc)+sqr(particleMass));
   beta  = MomentumXc/E;
   gamma = E/particleMass;

}

void
g2Mag_SpinEqRhs::EvaluateRhsGivenB( const G4double y[],
                                    const G4double B[3],
                                          G4double dydx[] ) const
{
   G4double momentum_mag_square = sqr(y[3]) + sqr(y[4]) + sqr(y[5]);
   G4double inv_momentum_magnitude = 1.0 / std::sqrt( momentum_mag_square );
   G4double cof = FCof()*inv_momentum_magnitude;

   dydx[0] = y[3] * inv_momentum_magnitude;       //  (d/ds)x = Vx/V
   dydx[1] = y[4] * inv_momentum_magnitude;       //  (d/ds)y = Vy/V
   dydx[2] = y[5] * inv_momentum_magnitude;       //  (d/ds)z = Vz/V
   dydx[3] = cof*(y[4]*B[2] - y[5]*B[1]) ;   // Ax = a*(Vy*Bz - Vz*By)
   dydx[4] = cof*(y[5]*B[0] - y[3]*B[2]) ;   // Ay = a*(Vz*Bx - Vx*Bz)
   dydx[5] = cof*(y[3]*B[1] - y[4]*B[0]) ;   // Az = a*(Vx*By - Vy*Bx)

   G4ThreeVector u(y[3], y[4], y[5]);
   u *= inv_momentum_magnitude; 

   G4ThreeVector BField(B[0],B[1],B[2]);

   G4double udb = anomaly*beta*gamma/(1.+gamma) * (BField * u); 
   G4double ucb = (anomaly+1./gamma)/beta;

   // Initialise the values of dydx that we do not update.
   dydx[6] = dydx[7] = dydx[8] = 0.0;

   G4ThreeVector Spin(y[9],y[10],y[11]);
   //   G4cout << Spin << '\n';

   G4ThreeVector dSpin;

   dSpin = ParticleCharge*omegac*(ucb*(Spin.cross(BField))-udb*(Spin.cross(u)));

   dydx[ 9] = dSpin.x();
   dydx[10] = dSpin.y();
   dydx[11] = dSpin.z();

   return ;
}
