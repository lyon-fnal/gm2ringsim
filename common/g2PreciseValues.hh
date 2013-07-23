#ifndef g2PreciseValues_hh
#define g2PreciseValues_hh

/** @file g2PreciseValues.hh

    Provides a number of globally useful precisely calculated values.

    - Kevin Lynch 2009
    - Nathan Froemming 2012
 */

namespace gm2ringsim {
  /** Updated muon mass */
  double mMuon();

  /** Updated muon anomalous magnetic moment */
  double  aMuon();


  /** The magic magnetic field strength. */
  /** NSF: Not sure what ^this means, exactly.  The magnetic field strength is an experimental
      parameter, not a "magic" parameter.  All "magic" parameters depend on (1) the value of 
      the muon's anomalous magnetic moment, and (2) the strength of the magnetic field used 
      in the experiment. */
  double B_magic();

  /** Magic momentum */
  double  pMagic();
  double  P_magic();

  /** Magic radius */
  double  R_magic();

  /** Magic gamma */
  double gammaMagic();

  /** Magic beta */
  double betaMagic();

  /** Magic energy */
  double EMagic();

  /** Magic kinetic energy */
  double KEMagic();

  /** Magic cyclotron angular frequency */
  double omegaCMagic();

  /** Magic anomalous precession angular frequency */
  double omegaAMagic();

  /** Magic spin angular frequency */
  double omegaSMagic();

}//namespace gm2ringsim


#endif //g2PreciseValues_hh

