#ifndef G2BEAMTRANSPORTSOURCE_HH
#define G2BEAMTRANSPORTSOURCE_HH

// Header for the Event action
/** @file PGA.hh prior to ART
    @file g2BeamTransportSource_service.hh
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
#include "TH1.h"
#include "TH2.h"
#include "TFile.h"
#include "TRandom3.h"

// Inflector
#include "gm2geom/inflector/InflectorGeom.hh"

#include "gm2ringsim/actions/PGA/G2GeneralParticleSource.hh"
// Get the helper files

namespace gm2ringsim
{
  class G2BeamTransportSource : public G4VPrimaryGenerator
  {
  public:
    G2BeamTransportSource();
    ~G2BeamTransportSource();
    
    void GeneratePrimaryVertex(G4Event*) override;
    
    G4double randTFromE989();
    
    void SetVerbosity(bool);
    void SetDebug(bool);

    double GenerateRandomXe();
    double GenerateRandomdPoverP();
    
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
    
    void SetUseConstBeta( bool aBool ){ UseConstBeta=aBool; }
    bool GetUseConstBeta( ){ return UseConstBeta; }
    
    void SetUseDispersion( bool aBool ){ UseDispersion=aBool; }
    bool GetUseDispersion( ){ return UseDispersion; }
    
    void SetUseFNALt0( bool aBool ){ UseFNALt0=aBool; }
    bool GetUseFNALt0( ){ return UseFNALt0; }
    
    void SetFlatDecayTime( bool aBool ){ FlatDecayTime=aBool; }
    bool GetFlatDecayTime( ){ return FlatDecayTime; }
    
    void SetMaxDecayTime( double aDouble ){ MaxDecayTime=aDouble; }
    double GetMaxDecayTime( ){ return MaxDecayTime; }
    
    void SetParticle( std::string aString ){ Particle_=aString; }
    std::string GetParticle(){ return Particle_; }
 
    void SetPolarization( std::string aString ){ Polarization_=aString; }
    std::string GetPolarization( ){ return Polarization_; }
    
    void SetNoTransMom( bool aBool ){ NoTransMom_=aBool; }
    bool GetNoTransMom( ){ return NoTransMom_; }
    
    void SetPosOffset( bool aBool ){ PosOffset_=aBool; }
    bool GetPosOffset( ){ return PosOffset_; }
    
    void SetStartPerfect( bool aBool ){ StartPerfect_=aBool; }
    bool GetStartPerfect( ){ return StartPerfect_; }
    
    void SetRotAngle( double aDouble ){ RotAngle_=aDouble; }
    double GetRotAngle( ){ return RotAngle_; }
    
    void SetKick( double aInt ){ Kick_=aInt; }
    double GetKick( ){ return Kick_; }
    
    void SetPerfectMatch( bool aBool ){ PerfectMatch_=aBool; }
    bool GetPerfectMatch( ){ return PerfectMatch_; }

    void TransportMatrix(double *M, int var, double arclength, double s0);
    void Transport(double *newX, double *newXprime, double *newdelta, int var, double arclength, double s0);
    double Beta(int var, double arclength);
    void MaxAmplitudes(double arclength, double *MaxX, double *MaxY, double *MaxXprime, double *MaxYprime, double *s0X, double *s0Y);
    double Alpha(int var, double arclength);
    double Eta(int var, double arclength);
    double Etap(int var, double arclength);
    double SingleTurnArcLength(double arclength);
    double PhaseAdvance(int var, double arclength);
    double CBOValue(double time);

    //Double_t betaFunction(Double_t *x, Double_t *par);    

  private:
    G4ParticleGun* beamTransportGun_;
    G2GeneralParticleSource* g2GPS_;
    gm2geom::InflectorGeom infGeom_;
    bool beamTransportVerbosity_;
    bool debug_;
    bool first_event;
    bool fill_;
    
    G4double sum_xe, sum_x, sum_y, sum_xprime, sum_yprime;
    G4double rms_xe, rms_x, rms_y, rms_xprime, rms_yprime;
    G4int ngen;

    double EmittanceX;
    double EmittanceY;
    double BetaX;
    double BetaY;
    double AlphaX;
    double AlphaY;
    bool UseConstBeta;
    bool UseDispersion;
    bool UseFNALt0;
    bool FlatDecayTime;
    double MaxDecayTime;
    std::string Particle_;
    std::string Polarization_;
    bool NoTransMom_;
    bool PosOffset_;
    bool StartPerfect_;
    double RotAngle_;
    int Kick_;
    bool PerfectMatch_;

    TF1 *fsz;
    TF1 *fsphi;
    TRandom3 *gRandom_;
    TF1 *fBetaX;
    TF1 *fBetaY;
    TF1 *fEtaX;
    TF1 *fEtapX;
    TF1 *fAlphaX;
    TF1 *fAlphaY;
    TF1 *fBetaX_n18;
    TF1 *fBetaY_n18;
    TF1 *fEtaX_n18;
    TF1 *fEtapX_n18;
    TF1 *fAlphaX_n18;
    TF1 *fAlphaY_n18;
    TF1 *fCBO;
    TF1 *fSingleTurnPhaseAdvanceX;
    TF1 *fSingleTurnPhaseAdvanceY;
    TF1 *fSingleTurnPhaseAdvanceX_n18;
    TF1 *fSingleTurnPhaseAdvanceY_n18;

    TFile *f;
    TH1F *h_Ax;
    TH1F *h_Ay;
    TH1F *h_xe;
    TH1F *h_pol;
    TH2F *h_mom_rand;
    TH2F *h_time_pol;
    TH1F *h_x;
    TH1F *h_x_cbo;
    TH1F *h_xp_cbo;
    TH2F *h_xprimex;
    TH2F *h_yprimey;
    TH2F *h_xy;
    TH1F *h_y;
    TH1F *h_xp;
    TH1F *h_yp;
    TH1F *h_pz;
    TH1F *h_pr;
    TH1F *h_emitX;
    TH1F *h_emitY;
    TH1F *h_turn;
    TH1F *h_t0;
    TH1F *h_t0off;
    TH1F *h_time;
    TH1F *h_time_m_t0;
    TH1F *h_time_tmod;
    TH1F *h_time_decay_tmod;
    TH1F *h_time_VS0;

  }; // End G2BeamTransportSource class
} //namespace gm2ringsim

#endif //G2BEAMTRANSPORTSOURCE_HH

