#ifndef G2FIELDEQRHS_HH
#define G2FIELDEQRHS_HH

/** @file g2FieldEqRhs.hh

    Provides right hand side equations of motion for time dependent
    magnetic fields.

    - Kevin Lynch 2009
*/

#include "Geant4/G4Mag_EqRhs.hh"
#include "Geant4/G4MagneticField.hh"

/** Provides for a time dependent magnetic field. */
class g2TimeDepMagField_EqRhs : public G4Mag_EqRhs
{
public:  // with description
  
  g2TimeDepMagField_EqRhs(G4MagneticField *BField )
    : G4Mag_EqRhs( BField ) {}
  
  void SetChargeMomentumMass(G4double particleCharge, // in e+ units
			     G4double MomentumXc,
			     G4double mass);
  
  void EvaluateRhsGivenB(const G4double y[],
			 const G4double Field[3],
			 G4double dydx[] ) const;
  // Given the value of the magnetic field, this function 
  // calculates the value of the derivative dydx.
  
private:
  
  G4double        fMagCof ;
  G4double        fMassCof;
};


////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////

// Fixme ... this whole implementation is a fucking disaster...

/** Provides for a time dependent magnetic field with spin evolution. */
class g2TimeDepMagField_SpinEqRhs : public G4Mag_EqRhs
{
public:  // with description
  
  g2TimeDepMagField_SpinEqRhs(G4MagneticField *BField )
    : G4Mag_EqRhs( BField ), anomaly(0.0011659208) {}
  
  void SetChargeMomentumMass(G4double particleCharge, // in e+ units
			     G4double MomentumXc,
			     G4double mass);
  
  void EvaluateRhsGivenB(const G4double y[],
			 const G4double Field[3],
			 G4double dydx[] ) const;
  // Given the value of the magnetic field, this function 
  // calculates the value of the derivative dydx.
  
private:
  
  G4double        fMagCof ;
  G4double        fMassCof;
  G4double        anomaly;

  G4double        beta;
  G4double        gamma;
  G4double        E;
  G4double        omegac;
  G4double        particleCharge;
};


// Straight copy of G4Mag_SpinEqRhs, so that I can do debug printing

class g2Mag_SpinEqRhs : public G4Mag_EqRhs
{
   public:  // with description

     g2Mag_SpinEqRhs( G4MagneticField* MagField );
    ~g2Mag_SpinEqRhs();
       // Constructor and destructor. No actions.

     void SetChargeMomentumMass(G4double particleCharge, // in e+ units
                                G4double MomentumXc,
                                G4double mass); 

     void EvaluateRhsGivenB( const  G4double y[],
                             const  G4double B[3],
                                    G4double dydx[] ) const;
       // Given the value of the magnetic field B, this function 
       // calculates the value of the derivative dydx.

     inline void SetAnomaly(G4double a) { anomaly = a; }
     inline G4double GetAnomaly() const { return anomaly; }
       // set/get magnetic anomaly

   private:

     G4double omegac;
     G4double anomaly;
     G4double ParticleCharge;

     G4double E;
     G4double gamma;
     G4double beta;
};


#endif // G2FIELDEQRHS_HH
