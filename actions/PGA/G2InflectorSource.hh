#ifndef G2INFLECTORSOURCE_HH
#define G2INFLECTORSOURCE_HH

// Header for the Event action
/** @file PGA.hh prior to ART
    @file g2InflectorSource_service.hh
    @author Nathan Froemming                                               
    @date 2011-2012          
    @comment Implemented in with the g2GeneralParticleSource
    @author Brendan Kiburg
    @date 2013
    @comment Implementation in ART as a separate sort of Source
      
*/

// Boilerplate for ART Service
#include "fhiclcpp/ParameterSet.h"
#include "art/Framework/Services/Registry/ActivityRegistry.h"

// Get the base class
#include "artg4/actionBase/PrimaryGeneratorActionBase.hh"

// Get the Geant4 code
#include "Geant4/G4Event.hh"
#include "Geant4/G4ParticleGun.hh"
#include "Geant4/G4RotationMatrix.hh"
//#include "Geant4/G4VPrimaryVertex.hh"

#include "TF1.h"
#include "TRandom3.h"

// Inflector
#include "gm2geom/inflector/InflectorGeom.hh"

#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
// Get the helper files

namespace gm2ringsim
{
  class G2InflectorSource : public G4VPrimaryGenerator
  {
  public:
    G2InflectorSource();
    ~G2InflectorSource();
    
    void GeneratePrimaryVertex(G4Event*) override;

    void TransportTwiss(double alpha, double beta, double gamma, double s,
			double *new_alpha, double *new_beta, double *new_gamma);

    void SetVerbosity(bool);
    

    void SetStartUpstream( bool aBool ){ StartUpstream=aBool; }
    bool GetStartUpstream( ){ return StartUpstream; }
    
    void SetStartUpstreamCryo( bool aBool ){ StartUpstreamCryo=aBool; }
    bool GetStartUpstreamCryo( ){ return StartUpstreamCryo; }
    
    void SetStartDownstream( bool aBool ){ StartDownstream=aBool; }
    bool GetStartDownstream( ){ return StartDownstream; }
    
    void SetStartPerfect( bool aBool ){ StartPerfect=aBool; }
    bool GetStartPerfect( ){ return StartPerfect; }
    
    void SetGenGaussian( bool aBool ){ GenGaussian=aBool; }
    bool GetGenGaussian( ){ return GenGaussian; }
    
    void SetLaunchAngle( double aDouble ){ LaunchAngle=aDouble; }
    double GetLaunchAngle( ){ return LaunchAngle; }
    
    void SetStorageOffset( double aDouble ){ StorageOffset=aDouble; }
    double GetStorageOffset( ){ return StorageOffset; }
    
    void SetEmittanceX( double aDouble ){ EmittanceX=aDouble; }
    double GetEmittanceX( ){ return EmittanceX; }
    
    void SetEmittanceY( double aDouble ){ EmittanceY=aDouble; }
    double GetEmittanceY( ){ return EmittanceY; }
    
    void SetBetaX( double aDouble ){ BetaX=aDouble; }
    double GetBetaX( ){ return BetaX; }
    
    void SetBetaY( double aDouble ){ BetaY=aDouble; }
    double GetBetaY( ){ return BetaY; }
    
    void SetAlphaX( double aDouble ){ AlphaX=aDouble; }
    double GetAlphaX( ){ return AlphaX; }
    
    void SetAlphaY( double aDouble ){ AlphaY=aDouble; }
    double GetAlphaY( ){ return AlphaY; }
    
    void SetPmean( double aDouble ){ Pmean=aDouble; }
    double GetPmean( ){ return Pmean; }
    
    void SetdPOverP( double aDouble ){ dPOverP=aDouble; }
    double GetdPOverP( ){ return dPOverP; }
    
    void SetSigmaT( double aDouble ){ SigmaT=aDouble; }
    double GetSigmaT( ){ return SigmaT; }
    
    void SetFlatDecayTime( bool aBool ){ FlatDecayTime=aBool; }
    bool GetFlatDecayTime( ){ return FlatDecayTime; }
    
    void SetMaxDecayTime( double aDouble ){ MaxDecayTime=aDouble; }
    double GetMaxDecayTime( ){ return MaxDecayTime; }
    
    void SetParticle( std::string aString ){ Particle_=aString; }
    std::string GetParticle(){ return Particle_; }
 
    void SetNumParticles( int aInt ){ NumParticles_=aInt; }
    int GetNumParticles( ){ return NumParticles_; }
 
    void SetDecayScaleFactor( int aInt ){ DecayScaleFactor_=aInt; }
    int GetDecayScaleFactor( ){ return DecayScaleFactor_; }
    
    void SetPolarization( std::string aString ){ Polarization_=aString; }
    std::string GetPolarization( ){ return Polarization_; }
    

    G4double randTFromE989();

  private:
    G4ParticleGun* inflectorGun_;
    G2GeneralParticleSource* g2GPS_;
    gm2geom::InflectorGeom infGeom_;
    bool Verbosity_;
    bool first_event;

    G4double avg_x, avg_y, avg_xprime, avg_yprime;
    G4double sum_x, sum_y, sum_xprime, sum_yprime;
    G4int ngen;

    bool StartUpstream;
    bool StartUpstreamCryo;
    bool StartDownstream;
    bool StartPerfect;
    bool GenGaussian;
    double LaunchAngle;
    double StorageOffset;
    double EmittanceX;
    double EmittanceY;
    double BetaX;
    double BetaY;
    double AlphaX;
    double AlphaY;
    double Pmean;
    double dPOverP;
    double SigmaT;
    bool FlatDecayTime;
    double MaxDecayTime;
    std::string Particle_;
    int NumParticles_;
    int DecayScaleFactor_;
    std::string Polarization_;

    TF1 *fsz;
    TF1 *fsphi;
    TRandom3 *gRandom_;
    
  }; // End G2InflectorSource class
} //namespace gm2ringsim

#endif //G2INFLECTORSOURCE_HH

