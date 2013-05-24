#ifndef g2PreciseValues_hh
#define g2PreciseValues_hh

/** @file g2PreciseValues.hh

    Provides a number of globally useful precisely calculated values.

    - Kevin Lynch 2009
    - Nathan Froemming 2012
 */

#include "Geant4/globals.hh"
namespace gm2ringsim {
  /** Updated muon mass */
  G4double mMuon();

  /** Updated muon anomalous magnetic moment */
  G4double  aMuon();


  /** The magic magnetic field strength. */
  /** NSF: Not sure what ^this means, exactly.  The magnetic field strength is an experimental
      parameter, not a "magic" parameter.  All "magic" parameters depend on (1) the value of 
      the muon's anomalous magnetic moment, and (2) the strength of the magnetic field used 
      in the experiment. */
  G4double B_magic();

  /** Magic momentum */
  G4double  pMagic();
  G4double  P_magic();

  /** Magic radius */
  G4double  R_magic();

  /** Magic gamma */
  G4double gammaMagic();

  /** Magic beta */
  G4double betaMagic();

  /** Magic energy */
  G4double EMagic();

  /** Magic kinetic energy */
  G4double KEMagic();

  /** Magic cyclotron angular frequency */
  G4double omegaCMagic();

  /** Magic anomalous precession angular frequency */
  G4double omegaAMagic();

  /** Magic spin angular frequency */
  G4double omegaSMagic();

}//namespace gm2ringsim

//FIXME : Need to Figure out role of Messenger here
#include "Geant4/G4UImessenger.hh"
#include "Geant4/G4UIdirectory.hh"
#include "Geant4/G4UIcmdWithoutParameter.hh"

namespace gm2ringsim{
  /** A G4UImessenger that allows read access to the precision values. */
  class g2PreciseMessenger : public G4UImessenger {
  public:
    g2PreciseMessenger();
    ~g2PreciseMessenger();
    
    void SetNewValue(G4UIcommand *, G4String);
    
  private:
    
    G4UIdirectory *preciseDirectory;
    
    G4UIcmdWithoutParameter *preciseInfoCmd;
  }; //class g2PreciseMessenger

}//namespace gm2ringsim

#endif //g2PreciseValues_hh

