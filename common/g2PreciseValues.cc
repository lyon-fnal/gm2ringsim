#include "gm2ringsim/common/g2PreciseValues.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/globals.hh"
#include <cmath>

/** @file g2PreciseValues.cc

    Provides the definitions and properly ordered static
    initializations for the precision values.

    - Zach   Hartwig   2005
    - Kevin  Lynch     2009
    - Nathan Froemming 2012
    - Brendan Kiburg   Dec 2012 Moving to ART
*/

double gm2ringsim::mMuon()
{
  //  NSF: g2MIGTRACE uses an old version of GEANT4.  The value below is more up-to-date.  Source = J. Beringer 
  //  et al. (Particle Data Group, http://pdg.lbl.gov), Phys. Rev. D 86, 010001 (2012).
  static double const mMuon_ = 105.6583715;
  //  G4cout << "mMuon = " << mMuon_/MeV << " MeV/c^2" << G4endl;
  return mMuon_;
}

double gm2ringsim::aMuon()

{
  //  NSF: g2MIGTRACE uses an old version of GEANT4.  The value below is more up-to-date.  Source = J. Beringer 
  //  et al. (Particle Data Group, http://pdg.lbl.gov), Phys. Rev. D 86, 010001 (2012).
  static double const aMuon_ = 11659208.9e-10; // avg. of mu+/-
  //  G4cout << "aMuon = " << aMuon_ << G4endl;
  return aMuon_;
}

/** @bug This should be calculated, and actually used in the storage field! */
double gm2ringsim::B_magic() // NSF: B_magic = B_magic(R_magic(P_magic(aMuon()))) gotten from (used in) field map?
{
  static double const B_ = 1.451269*tesla; // magnitude of E821 field, used as default
  //  G4cout << "B_ " << B_ << '\n';
  return B_;
}

//  NSF: All "magic" parameters (below) are a function of (1) the anomalous magnetic moment 
//  of the muon, and (2) the value of the magnetic field used in the experiment. That's it.

double gm2ringsim::P_magic()
{
  return( pMagic() );
}

double gm2ringsim::pMagic()
{
  //  NSF: The easiest way to calculate....
  static double const pMagic_ = mMuon()/std::sqrt( aMuon() );
  //  G4cout << "pMagic = " << pMagic_/GeV << " GeV/c" << G4endl;
  return pMagic_;
}

double gm2ringsim::R_magic()
{
  static double const R_magic_ = pMagic()/( c_light*B_magic() );
  //  G4cout << "R_magic = " << R_magic_/m << " meters" << G4endl;
  return R_magic_;
}

double gm2ringsim::gammaMagic()
{
  static double const gammaMagic_ = std::sqrt( 1.+1./aMuon() );
  //  G4cout << "gammaMagic = " << gammaMagic_ << G4endl;
  return gammaMagic_;
}


double gm2ringsim::betaMagic()
{
  static double const gammaMagic_2_ = gammaMagic()*gammaMagic();
  static double const betaMagic_ = std::sqrt( 1.-1./gammaMagic_2_ );
  //  G4cout << "betaMagic = " << betaMagic_ << G4endl;
  return betaMagic_;
}

double gm2ringsim::EMagic()

{
  static double const EMagic_ = gammaMagic()*mMuon();
  //  G4cout << "EMagic = " << EMagic_/MeV << " MeV" << G4endl;
  return EMagic_;
}

double gm2ringsim::KEMagic()
{
  static double const KEMagic_ = EMagic() - mMuon();
  //  G4cout << "KEMagic = " << KEMagic_/MeV << " MeV" << G4endl;
  return KEMagic_;
}

double gm2ringsim::omegaCMagic()
{
  static double const omegaCMagic_ = betaMagic()*c_light/R_magic();
  //  G4cout << "omegaCMagic = " << omegaCMagic_ << " rad/ns" << G4endl;
  return omegaCMagic_;
}

double gm2ringsim::omegaAMagic()
{
  static double const omegaAMagic_ = aMuon()*gammaMagic()*betaMagic()*c_light/R_magic();
  //  G4cout << "omegaAMagic = " << omegaAMagic_ << " rad/ns" << G4endl;
  return omegaAMagic_;
}

double gm2ringsim::omegaSMagic()
{
  static double const omegaSMagic_ = omegaCMagic() + omegaAMagic();
  //  G4cout << "omegaSMagic = " << omegaSMagic_ << " rad/ns" << G4endl;
  return omegaSMagic_;
}


