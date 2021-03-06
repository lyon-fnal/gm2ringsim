// Get the PGA header
#include "gm2ringsim/actions/PGA/obsoletePrimaryGeneratorAction_service.hh"

// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

// Geant includes
#include "Geant4/G4Event.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/globals.hh"
#include "Geant4/Randomize.hh"

#include "Geant4/G4strstreambuf.hh"

// g2migtrace helpers

#include "gm2ringsim/actions/PGA/G2BeamTransportSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"

#define randFromDistr CLHEP::RandGeneral

enum {kX, kY};

#include <iostream>
#include <cmath>

#include "TMath.h"
#include "TH1.h"


Double_t CBOFit(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*sin(TMath::TwoPi()*par[2]*xx + par[3]);
  return f;
}

Double_t betaFunctionFit(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*sin(par[2]*xx + par[3]);
  return f;
}

Double_t SingleTurnPhaseAdvanceFit(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*xx;
  return f;
}

using std::endl;

// Constructor
gm2ringsim::G2BeamTransportSource::G2BeamTransportSource() :
  beamTransportGun_(),
  g2GPS_(),
  infGeom_("inflector"),
  beamTransportVerbosity_ (0),
  debug_(false),
  first_event(true),
  sum_xe(0.0),
  sum_x(0.0),
  sum_y(0.0),
  sum_xprime(0.0),
  sum_yprime(0.0),
  rms_xe(0.0),
  rms_x(0.0),
  rms_y(0.0),
  rms_xprime(0.0),
  rms_yprime(0.0),
  ngen(0),
  EmittanceX(40.0),
  EmittanceY(40.0),
  BetaX(19.1),
  BetaY(7.6),
  AlphaX(0.0),
  AlphaY(0.0),
  UseConstBeta(false),
  UseFNALt0(true),
  FlatDecayTime(false),
  MaxDecayTime(-1.0),
  Particle_("mu+"),
  Polarization_("E821"),
  NoTransMom_(false),
  PosOffset_(false),
  StartPerfect_(false),
  RotAngle_(0.0),
  Kick_(0)
{  
  beamTransportGun_ = new G4ParticleGun();
  g2GPS_ = new G2GeneralParticleSource();

  fsz   = new TF1("fsz", "-(3.68923)+(1.48959*x)", 3.06, 3.13);
  fsphi = new TF1("fsphi", "(((1788.72-(196.394*cos(x)))-(321.191*cos(2*x)))+(68.1887*cos(3*x)))+(((587.76*cos(4*x))-(94.1914*cos(5*x)))-(106.777*cos(6*x)))", -3.14159, 3.14159);

  
  // These are for +/-24 kV on the quads
  fBetaX = new TF1("mybetaX",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaX->SetParameter(0, 7.67067);
  fBetaX->SetParameter(1, 0.247307);
  fBetaX->SetParameter(2, 0.562465);
  fBetaX->SetParameter(3, 1.56706);
  
  fBetaY = new TF1("mybetaY",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaY->SetParameter(0, 18.8466);
  fBetaY->SetParameter(1, 0.496336);
  fBetaY->SetParameter(2, 0.562772);
  fBetaY->SetParameter(3, -1.58141);

  fEtaX = new TF1("myetaX",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fEtaX->SetParameter(0, 8.26733);
  fEtaX->SetParameter(1, 0.110575);
  fEtaX->SetParameter(2, 0.562724);
  fEtaX->SetParameter(3, 1.56124);


  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceX = new TF1("myPhaseAdvanceX",SingleTurnPhaseAdvanceFit,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceX->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceX->SetParameter(1, 0.130514);

  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceY = new TF1("myPhaseAdvanceY",SingleTurnPhaseAdvanceFit,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceY->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceY->SetParameter(1, 0.0530519);



  //
  // These are for n=0.18
  //
  fBetaX_n18 = new TF1("mybetaX_n18",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaX_n18->SetParameter(0, 7.86248);
  fBetaX_n18->SetParameter(1, 0.290301);
  fBetaX_n18->SetParameter(2, 0.562481);
  fBetaX_n18->SetParameter(3, 1.613);

  fBetaY_n18 = new TF1("mybetaY_n18",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaY_n18->SetParameter(0, 18.34);
  fBetaY_n18->SetParameter(1, 0.59329);
  fBetaY_n18->SetParameter(2, 0.584521);
  fBetaY_n18->SetParameter(3, -1.78);
  
  fEtaX_n18 = new TF1("myetaX_n18",betaFunctionFit,0,TMath::TwoPi()*R_magic()/1000,4);
  fEtaX_n18->SetParameter(0, 8.68405);
  fEtaX_n18->SetParameter(1, 0.134382);
  fEtaX_n18->SetParameter(2, 0.562523);
  fEtaX_n18->SetParameter(3, 1.61181);


  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceX_n18 = new TF1("myPhaseAdvanceX_n18",SingleTurnPhaseAdvanceFit,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceX_n18->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceX_n18->SetParameter(1, 0.127387);

  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceY_n18 = new TF1("myPhaseAdvanceY_n18",SingleTurnPhaseAdvanceFit,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceY_n18->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceY_n18->SetParameter(1, 0.0545287);


  fCBO = NULL;
  fill_ = false;

  double timeperturn = 1/ComputeFrequencyCGivenMomRadius(P_magic(), R_magic())/microsecond;
  int maxturns_ = 101;
  double maxt = maxturns_* timeperturn;
  int numhitsperturn = 150;


  if ( fill_ ) {
    f = new TFile("beamtransport.root", "RECREATE");
  }
  h_Ax = new TH1F("hAx", "", 450, 0, 45.0*mm);
  h_Ay = new TH1F("hAy", "", 450, 0, 45.0*mm);
  h_xe = new TH1F("hx", "", 900, -45.0*mm, 45.0*mm);
  h_mom_rand = new TH2F("hmom_rand", "", 120, 0.97, 1.03, 100, 0.0, 1.0);
  h_pol = new TH1F("hpol", "", 100, -1, 1);
  h_time_pol = new TH2F("htime_pol", "", maxturns_*numhitsperturn, 0.0, maxt, 100, -1, 1);
  h_x_cbo = new TH1F("hx_cbo", "", 900, -45.0*mm, 45.0*mm);
  h_xp_cbo = new TH1F("hxp_cbo", "", 1000, -0.1, 0.1 );
  h_x = new TH1F("hx", "", 900, -45.0*mm, 45.0*mm);
  h_y = new TH1F("hy", "", 900, -45.0*mm, 45.0*mm);
  h_xp = new TH1F("hxp", "", 400, -20.0, 20.0);
  h_yp = new TH1F("hyp", "", 400, -20.0, 20.0);
  h_pz = new TH1F("hpz", "", 100, -1.0, 1.0);
  h_pr = new TH1F("hpz", "", 100, -1.0, 1.0);
  h_emitX = new TH1F("hemitX", "", 50, 0, 50);
  h_emitY = new TH1F("hemitY", "", 50, 0, 50);
  h_turn = new TH1F("hturn", "", 10000, 0.0, 10000.0);
  h_t0 = new TH1F("ht0", "", 400, -100.0, 300.0);
  h_t0off = new TH1F("ht0off", "", 400, -100.0, 300.0);
  h_time = new TH1F("htime", "", maxturns_*numhitsperturn, 0.0, maxt);
  h_time_tmod = new TH1F("htime_tmod", "", maxturns_*numhitsperturn, 0.0, maxt);
  h_time_decay_tmod = new TH1F("htime_decay_tmod", "", maxturns_*numhitsperturn, 0.0, maxt);
  h_time_m_t0 = new TH1F("htime_m_t0", "", maxturns_*numhitsperturn, 0.0, maxt);
  h_time_VS0 = new TH1F("htime_VS0", "", maxturns_*numhitsperturn, 0.0, maxt);
}

gm2ringsim::G2BeamTransportSource::~G2BeamTransportSource() 
{
  if ( 1 ) {
    G4cout << G4endl;
    G4cout << "====================================================================" << G4endl;
    G4cout << "======================  MUONGAS GUN STATS  =========================" << G4endl;
    G4cout << "====================================================================" << G4endl;
    
    G4cout << "   N   = " << ngen << G4endl;
//     G4cout << "  <xe> = " << sum_xe / ngen <<     " +/- " << TMath::Sqrt(rms_xe/ngen) << G4endl;
//     G4cout << "  <x>  = " << sum_x / ngen <<      " +/- " << TMath::Sqrt(rms_x/ngen) << G4endl;
//     G4cout << "  <y>  = " << sum_y / ngen <<      " +/- " << TMath::Sqrt(rms_y/ngen) << G4endl;
//     G4cout << "  <x'> = " << sum_xprime / ngen << " +/- " << TMath::Sqrt(rms_xprime/ngen) << G4endl;
//     G4cout << "  <y'> = " << sum_yprime / ngen << " +/- " << TMath::Sqrt(rms_yprime/ngen) << G4endl;

    G4cout << "  <Xe>: " << h_xe->GetMean() << " +/- " << h_xe->GetMeanError() << "\tRMS: " << h_xe->GetRMS() << G4endl;
    G4cout << "  <x>:  " << h_x->GetMean() << " +/- " << h_x->GetMeanError() << "\tRMS: " << h_x->GetRMS() << G4endl;
    G4cout << "  <y>:  " << h_y->GetMean() << " +/- " << h_y->GetMeanError() << "\tRMS: " << h_y->GetRMS() << G4endl;
    G4cout << "  <x'>: " << h_xp->GetMean() << " +/- " << h_xp->GetMeanError() << "\tRMS: " << h_xp->GetRMS() << G4endl;
    G4cout << "  <y'>: " << h_yp->GetMean() << " +/- " << h_yp->GetMeanError() << "\tRMS: " << h_yp->GetRMS() << G4endl;
    G4cout << "  <t0>: " << h_t0->GetMean() << " +/- " << h_t0->GetMeanError() << "\tRMS: " << h_t0->GetRMS() << G4endl;
    G4cout << "  <t>:  " << h_time->GetMean() << " +/- " << h_time->GetMeanError() << "\tRMS: " << h_time->GetRMS() << G4endl;
    
    G4cout << G4endl;

    G4cout << "  Min/Max (x,cbo):  " << h_x_cbo->GetBinCenter(h_x_cbo->FindFirstBinAbove(1)) << " / " << h_x_cbo->GetBinCenter(h_x_cbo->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (x',cbo): " << h_xp_cbo->GetBinCenter(h_xp_cbo->FindFirstBinAbove(1)) << " / " << h_xp_cbo->GetBinCenter(h_xp_cbo->FindLastBinAbove(1)) << " mrad." << G4endl;
    G4cout << "  Min/Max (Ax):     " << h_Ax->GetBinCenter(h_Ax->FindFirstBinAbove(1)) << " / " << h_Ax->GetBinCenter(h_Ax->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (Ay):     " << h_Ay->GetBinCenter(h_Ay->FindFirstBinAbove(1)) << " / " << h_Ay->GetBinCenter(h_Ay->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (xe):     " << h_xe->GetBinCenter(h_xe->FindFirstBinAbove(1)) << " / " << h_xe->GetBinCenter(h_xe->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (x):      " << h_x->GetBinCenter(h_x->FindFirstBinAbove(1)) << " / " << h_x->GetBinCenter(h_x->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (y):      " << h_y->GetBinCenter(h_y->FindFirstBinAbove(1)) << " / " << h_y->GetBinCenter(h_y->FindLastBinAbove(1)) << " mm." << G4endl;
    G4cout << "  Min/Max (x'):     " << h_xp->GetBinCenter(h_xp->FindFirstBinAbove(1)) << " / " << h_xp->GetBinCenter(h_xp->FindLastBinAbove(1)) << " mrad." << G4endl;
    G4cout << "  Min/Max (y'):     " << h_yp->GetBinCenter(h_yp->FindFirstBinAbove(1)) << " / " << h_yp->GetBinCenter(h_yp->FindLastBinAbove(1)) << " mrad." << G4endl;
    G4cout << "  Min/Max (eX):     " << h_emitX->GetBinCenter(h_emitX->FindFirstBinAbove(1)) << " / " << h_emitX->GetBinCenter(h_emitX->FindLastBinAbove(1)) << " mrad." << G4endl;
    G4cout << "  Min/Max (eY):     " << h_emitY->GetBinCenter(h_emitY->FindFirstBinAbove(1)) << " / " << h_emitY->GetBinCenter(h_emitY->FindLastBinAbove(1)) << " mrad." << G4endl;
    G4cout << "  Min/Max (turn):   " << h_turn->GetBinCenter(h_turn->FindFirstBinAbove(1)) << " / " << h_turn->GetBinCenter(h_turn->FindLastBinAbove(1)) << " turns." << G4endl;
    G4cout << "  Min/Max (t0):     " << h_t0->GetBinCenter(h_t0->FindFirstBinAbove(1)) << " / " << h_t0->GetBinCenter(h_t0->FindLastBinAbove(1)) << " ns." << G4endl;
    G4cout << "  Min/Max (t0'):    " << h_t0off->GetBinCenter(h_t0off->FindFirstBinAbove(1)) << " / " << h_t0off->GetBinCenter(h_t0off->FindLastBinAbove(1)) << " ns." << G4endl;
    G4cout << "  Min/Max (time):   " << h_time->GetBinCenter(h_time->FindFirstBinAbove(1)) << " / " << h_time->GetBinCenter(h_time->FindLastBinAbove(1)) << " us." << G4endl;
  }
  if ( fill_ ) {
    h_pol->Write();
    h_time_pol->Write();
    h_x_cbo->Write();
    h_xp_cbo->Write();
    h_xe->Write();
    h_time->Write();
    h_time_VS0->Write();
    h_time_tmod->Write();
    h_time_decay_tmod->Write();
    h_time_m_t0->Write();
    h_t0->Write();
    f->Write();
    f->Close();
  }
}

void gm2ringsim::G2BeamTransportSource::SetVerbosity(bool verbose){
  beamTransportVerbosity_ = verbose;
}

void gm2ringsim::G2BeamTransportSource::SetDebug(bool debug){
  debug_ = debug;
}

double gm2ringsim::G2BeamTransportSource::CBOValue(double time)
{
  if ( fCBO == NULL ) {
    double maxt = 10000;
    fCBO = new TF1("mybetaY",CBOFit,0, maxt, 4);

    int kick = GetKick();
    double CBO_freq = 0.477; // MHz
    CBO_freq = 0.466687;

    double Offset = 0.0;
    double Amp = 0.0;
    double Freq = CBO_freq;
    double Phase = 0.0;

    if ( kick == 100 ) { Offset=1.11176; Amp=8.50694; Freq=0.473353; Phase=1.78281; }
    if ( kick == 120 ) { Offset=1.47711; Amp=7.42592; Freq=0.47041; Phase=1.76787; }
    if ( kick == 130 ) { Offset=1.72311; Amp=7.40363; Freq=0.466544; Phase=1.72438; }
    if ( kick == 140 ) { Offset=2.00567; Amp=8.01692; Freq=0.47104; Phase=1.51206; }
    if ( kick == 150 ) { Offset=2.09952; Amp=9.7042; Freq=0.46826; Phase=1.27273; }
    if ( kick == 160 ) { Offset=1.87499; Amp=12.4741; Freq=0.466718; Phase=1.23057; }
    if ( kick == 170 ) { Offset=2.61533; Amp=11.8592; Freq=0.465442; Phase=1.14499; }
    if ( kick == 180 ) { Offset=1.65759; Amp=9.90671; Freq=0.466002; Phase=1.04178; }
    if ( kick == 185 ) { Offset=1.02875; Amp=8.81544; Freq=0.467352; Phase=0.954481; }
    if ( kick == 190 ) { Offset=0.687325; Amp=8.06496; Freq=0.46739; Phase=0.787329; }
    if ( kick == 195 ) { Offset=0.466271; Amp=6.79017; Freq=0.46554; Phase=0.700052; }
    if ( kick == 200 ) { Offset=-0.0873511; Amp=6.37412; Freq=0.466871; Phase=0.516457; }
    if ( kick == 205 ) { Offset=-0.202685; Amp=5.56362; Freq=0.468539; Phase=0.156158; }
    if ( kick == 210 ) { Offset=-0.658895; Amp=5.37464; Freq=0.467713; Phase=3.73375e-11; }
    if ( kick == 215 ) { Offset=-0.912757; Amp=5.65311; Freq=0.460459; Phase=1.74305e-15; }
    if ( kick == 220 ) { Offset=-1.70927; Amp=6.32267; Freq=0.471556; Phase=5.71243; }
    if ( kick == 230 ) { Offset=-2.1726; Amp=7.84359; Freq=0.467821; Phase=5.50156; }
    if ( kick == 240 ) { Offset=-2.87348; Amp=9.91837; Freq=0.469035; Phase=5.24968; }
    if ( kick == 250 ) { Offset=-3.56978; Amp=12.4882; Freq=0.469589; Phase=5.11673; }
    if ( kick == 260 ) { Offset=-4.31889; Amp=14.9789; Freq=0.467618; Phase=5.08511; }
    if ( kick == 270 ) { Offset=-5.41454; Amp=16.375; Freq=0.467928; Phase=5.04201; }
    if ( kick == 280 ) { Offset=-5.66648; Amp=18.535; Freq=0.466618; Phase=5.00099; }
    if ( kick == 300 ) { Offset=-6.57974; Amp=20.9758; Freq=0.467149; Phase=4.88573; }
    if ( kick == 325 ) { Offset=-7.47563; Amp=24.5244; Freq=0.46646; Phase=4.74599; }
    if ( kick == 50 ) { Offset=-0.273382; Amp=24.3115; Freq=0.472365; Phase=1.43152; }
    if ( kick == 75 ) { Offset=0.545986; Amp=11.2287; Freq=0.478565; Phase=1.65677; }
    

    
    if ( Amp <= 0 ) {
      G4cout << "Not applying any CBO modulation for kick = " << kick << G4endl;
    }
    
    fCBO->SetParameter(0, Offset);
    fCBO->SetParameter(1, Amp);
    fCBO->SetParameter(2, Freq);
    fCBO->SetParameter(3, Phase);  
  }

  return( fCBO->Eval(time) );
}

double gm2ringsim::G2BeamTransportSource::SingleTurnArcLength(double arclength)
{
  G4double maxangle = TMath::TwoPi() * R_magic();
  while ( arclength < 0 ) { arclength += maxangle; }
  G4double decayarclength = std::fmod(arclength, maxangle);

  return( decayarclength );
}

double gm2ringsim::G2BeamTransportSource::PhaseAdvance(int var, double arclength)
{
  double phase_advance = 0.0;
  if ( GetUseConstBeta() ) {
    double const_beta = Beta(var, arclength);
    phase_advance = arclength/const_beta;
  }
  else {    
    G4double maxangle = TMath::TwoPi() * R_magic();
    int nturns = (int)arclength/maxangle;
    double singleturn_arclength = SingleTurnArcLength(arclength);
    if ( var == kX ) {
      phase_advance = nturns*fSingleTurnPhaseAdvanceX->Eval(maxangle) + fSingleTurnPhaseAdvanceX->Eval(singleturn_arclength);
    }    
    if ( var == kY ) {
      phase_advance = nturns*fSingleTurnPhaseAdvanceY->Eval(maxangle) + fSingleTurnPhaseAdvanceY->Eval(singleturn_arclength);
    }
  }
  return( phase_advance );
}


void gm2ringsim::G2BeamTransportSource::MaxAmplitudes(double arclength, double *MaxX, double *MaxY, double *MaxXprime, double *MaxYprime, double *s0X, double *s0Y)
{
  //------------------------------------------------------  
  //
  // Compute betaX, betaY @ decay point
  //
  //------------------------------------------------------  
  G4double decaybetaX = Beta(kX, arclength);
  G4double decaybetaY = Beta(kY, arclength);
  G4double decayalphaX = Alpha(kX, arclength);
  G4double decayalphaY = Alpha(kY, arclength);
  
  if ( debug_ ) {
    G4cout << "  t*c*beta      = " << arclength << " m." << endl;
    G4cout << "  betaX      = " << decaybetaX/m << " m." << G4endl;
    G4cout << "  betaY      = " << decaybetaY/m << " m." << G4endl;
    G4cout << "  alphaX     = " << decayalphaX << G4endl;
    G4cout << "  alphaY     = " << decayalphaY << G4endl;
  }

  G4double epsilonX, betaX, alphaX, gammaX;
  G4double epsilonY, betaY, alphaY, gammaY;
  G4double emittanceX, emittanceY;
  emittanceX = GetEmittanceX();
  emittanceY = GetEmittanceY();
  if ( decaybetaX > 0 ) {
    betaX = decaybetaX;
    alphaX = decayalphaX;
    //nux = R_magic() / betaX;
  }
  else {
    betaX = GetBetaX();
    alphaX = GetAlphaX();
    //nux = R_magic() / betaX;
  }

  if ( decaybetaY > 0 ) {
    betaY = decaybetaY;
    alphaY = decayalphaY;
    //nuy = R_magic() / betaY;
  }
  else {
    betaY = GetBetaY();
    alphaY = GetAlphaY();
    //nuy = R_magic() / betaY;
  }

  epsilonX       = emittanceX;
  epsilonY       = emittanceY;
  
  if ( debug_ ) {
    G4cout << "  EmitX         = " << emittanceX/mrad << " mm-mrad." << G4endl;
    G4cout << "  EmitY         = " << emittanceY/mrad << " mm-mrad." << G4endl;
    G4cout << "  BetaX         = " << betaX/m << " m." << G4endl;
    G4cout << "  BetaY         = " << betaY/m << " m." << G4endl;
    G4cout << "  (BetaX)       = " << GetBetaX()/m << " m." << G4endl;
    G4cout << "  (BetaY)       = " << GetBetaY()/m << " m." << G4endl;
    G4cout << "  AlphaX        = " << alphaX << G4endl;
    G4cout << "  AlphaY        = " << alphaY << G4endl;
  }
      


  //  First, calculate gamma from the user's parameters using the fact that beta*gamma - alpha^2 = 1 is invariant
  gammaX = ( 1+alphaX*alphaX )/betaX;
  gammaY = ( 1+alphaY*alphaY )/betaY;

  //  Next, use the above parameters to determine the point s{X,Y} at which the phase-space ellipses have alpha{X,Y} = 0.
  //  This serves to define what I will call the "naught" positions, i.e. the positions at which the phase-space ellipses
  //  are standing "upright" in phase space (alpha{X,Y}=0).  It is very easy to generate random positions/momenta at the
  //  naught coordinates, which can then easily be propagted back to the injection point.
  *s0X = -alphaX/gammaX;
  *s0Y = -alphaY/gammaY;


  //  Calculate beta{X,Y}0
  G4double betaX0 = betaX + 2*alphaX*(*s0X) + gammaX*(*s0X)*(*s0X);
  G4double betaY0 = betaY + 2*alphaY*(*s0Y) + gammaY*(*s0Y)*(*s0Y);
  
  
  //  Calculate gamma{X,Y}0 given that alpha{X,Y}=0 using the invariant (see above).
  G4double gammaX0 = 1/betaX0;
  G4double gammaY0 = 1/betaY0;

  *MaxX = std::sqrt( epsilonX*betaX0 );
  *MaxY = std::sqrt( epsilonY*betaY0 );
  *MaxXprime = std::sqrt( epsilonX*gammaX0 );
  *MaxYprime = std::sqrt( epsilonY*gammaY0 );

  if ( debug_ ) {
    G4cout << "  sX, sY                  = " << *s0X << " , " << *s0Y << G4endl;
    G4cout << "  alpha, beta, gamma (x0) = " << alphaX << " , " << betaX0 << " , " << gammaX0 << G4endl;
    G4cout << "  alpha, beta, gamma (y0) = " << alphaY << " , " << betaY0 << " , " << gammaY0 << G4endl;
    G4cout << "  emit (X,Y)              = " << epsilonX << " , " << epsilonY << G4endl;
  }
}

double gm2ringsim::G2BeamTransportSource::Eta(int var, double arclength)
{
  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( GetUseConstBeta() ) {
    if ( var == kX ) {
      return( 8.4*m );
    }
    if ( var == kY ) {
      return( 0.0 );
    }
  }
  else {
    double s = singleturn_arclength / m; // put the arclength in meters
    double etaval = 0.0;
    if ( var == kX ) {
      etaval = fEtaX->Eval(s);
    }
    if ( var == kY ) {
      etaval = 0.0;
    }
    etaval *= m; // put beta back in mm units
    return( etaval );
  }

  G4cout << "Eta not defined..." << G4endl;
  return( 0.0 );
}

double gm2ringsim::G2BeamTransportSource::Etap(int var, double arclength)
{
  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( GetUseConstBeta() ) { return( 0.0 ); }
  else {
    double s = singleturn_arclength / m; // put the arclength in meters
    double etapval = 0.0;
    if ( var == kX ) {
      etapval = fEtaX->Derivative(s);
    }
    if ( var == kY ) {
      etapval = 0.0;
    }
    
    return( etapval );
  }
  
  G4cout << "Eta' not defined..." << G4endl;
  return( 0.0 );
}

double gm2ringsim::G2BeamTransportSource::Beta(int var, double arclength)
{
  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( GetUseConstBeta() ) {
    if ( var == kX ) {
      return( 7.6*m );
    }
    if ( var == kY ) {
      return( 19.1*m );
    }
  }
  else {
    double s = singleturn_arclength / m; // put the arclength in meters
    double betaval = 0.0;
    if ( var == kX ) {
      betaval = fBetaX->Eval(s);
    }
    if ( var == kY ) {
      betaval = fBetaY->Eval(s);
    }
    betaval *= m; // put beta back in mm units
    return( betaval );
  }

  G4cout << "Beta not defined..." << G4endl;
  return( 0.0 );
}

double gm2ringsim::G2BeamTransportSource::Alpha(int var, double arclength)
{
  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( GetUseConstBeta() ) { return( 0.0 ); }
  else {
    double s = singleturn_arclength / m; // put the arclength in meters
    double alphaval = 0.0;
    if ( var == kX ) {
      alphaval = -0.5*fBetaX->Derivative(s);
    }
    if ( var == kY ) {
      alphaval = -0.5*fBetaY->Derivative(s);
    }
    
    return( alphaval );
  }
  
  G4cout << "Alpha not defined..." << G4endl;
  return( 0.0 );
}

void gm2ringsim::G2BeamTransportSource::TransportMatrix(double *M, int var, double arclength, double s0)
{
  G4double beta, alpha;
  G4double beta0, alpha0;
  G4double PsiXs;

  beta = Beta(var, arclength);
  alpha = Alpha(var, arclength);
  beta0 = Beta(var, s0);
  alpha0 = Alpha(var, s0);
  PsiXs = PhaseAdvance(var, arclength);

  double M11, M12, M21, M22, M13, M23, M31, M32, M33;
  
  // Compute phase advance
  M11 = TMath::Sqrt(beta/beta0)*TMath::Cos(PsiXs) + alpha0*TMath::Sin(PsiXs);
  M12 = TMath::Sqrt(beta*beta0)*TMath::Sin(PsiXs);
  M21 = ((alpha0 - alpha)*TMath::Cos(PsiXs) - (1+alpha0*alpha)*TMath::Sin(PsiXs))/(TMath::Sqrt(beta*beta0));
  M22 = TMath::Sqrt(beta0/beta)*TMath::Cos(PsiXs) - alpha*TMath::Sin(PsiXs);

  if ( GetUseDispersion() ) {
    M13 = Eta(var, arclength);
    M23 = Etap(var, arclength);
    M33 = 1;
    M31 = 0;
    M32 = 0;
    M[0] = M11;
    M[1] = M12;
    M[2] = M13;
    M[3] = M21;
    M[4] = M22;
    M[5] = M23;
    M[6] = M31;
    M[7] = M32;
    M[8] = M33;
  }
  else {
    M[0] = M11;
    M[1] = M12;
    M[2] = M21;
    M[3] = M22;
  }
}

void gm2ringsim::G2BeamTransportSource::Transport(double *newVar, double *newVarprime, double *newdelta, int var, double arclength, double s0)
{
  if ( debug_ ) { G4cout << "Transport()" << G4endl; }
  
  G4double oldVar = *newVar;
  G4double oldVarprime = *newVarprime;
  G4double olddelta = *newdelta;

  double M[9]={0, 0, 0, 0, 0, 0, 0, 0, 0};
  TransportMatrix(M, var, arclength, s0);
  
  bool debug = debug_;
  if ( debug ) {
    G4cout << "=== Transport ===" << G4endl;
    if ( GetUseDispersion() ) {
      G4cout << "  [ " << M[0] << "\t" << M[1] << "\t" << M[2] << " ]" << G4endl;
      G4cout << "  [ " << M[3] << "\t" << M[4] << "\t" << M[5] << " ]" << G4endl;
      G4cout << "  [ " << M[6] << "\t" << M[7] << "\t" << M[8] << " ]" << G4endl;
    }
    else {
      G4cout << "  [ " << M[0] << "\t" << M[1] << " ]" << G4endl;
      G4cout << "  [ " << M[2] << "\t" << M[3] << " ]" << G4endl;
    }
  }
  
  if ( GetUseDispersion() ) {
    *newVar      = M[0]*oldVar + M[1]*oldVarprime + M[2]*olddelta;
    *newVarprime = M[3]*oldVar + M[4]*oldVarprime + M[5]*olddelta;
    *newdelta    = M[6]*oldVar + M[7]*oldVarprime + M[8]*olddelta;
  }
  else {
    *newVar      = M[0]*oldVar + M[1]*oldVarprime;
    *newVarprime = M[2]*oldVar + M[3]*oldVarprime;  
  }
  
  if ( debug ) {
    G4cout << "Calc: " << G4endl;
    G4cout << "  " << oldVar << "\t" << oldVarprime << G4endl;
    G4cout << "  " << *newVar << "\t" << *newVarprime << G4endl;    
  }
}

// EndOfPrimaryGeneratorAction
void gm2ringsim::G2BeamTransportSource::GeneratePrimaryVertex(G4Event* evt) {

  mf::LogInfo("G2BeamTransportSource") << "GeneratePrimaryVertex";
  static G4double particleLifetime=0; // initialize
  //static G4bool firstVertex = true;

  G4ParticleDefinition *def = G4ParticleTable::GetParticleTable()->FindParticle(Particle_);
  if ( def ) {
    if ( first_event ) {
      G4cout << "Found Particle Info for [" << Particle_ << "]" << G4endl;
      G4cout << " g-2/2 : " << def->CalculateAnomaly() << G4endl;
      particleLifetime = def->GetPDGLifeTime();
      def->DumpTable();
      def->SetPDGLifeTime(0.0);
      beamTransportGun_ ->SetParticleDefinition(def);
    }
  }
  else {
    G4ParticleTable::GetParticleTable()->DumpTable();
    G4cout << "Could not find Particle Info for [" << Particle_ << "]" << G4endl;
    exit(1);
  }
    


  
  
  //------------------------------------------------------  
  //
  // Generate random Xe or dP/P from predetermined distributions
  //
  //------------------------------------------------------  
  G4double xe, mom, delta;
  G4double n = 0.137;
  if ( GetUseDispersion() ) {    
    mom     = GenerateRandomdPoverP()*P_magic();
    delta   = (mom - P_magic())/(P_magic());
    xe      = R_magic()*(1/(1-n)*delta);
  }
  else {
    xe      = GenerateRandomXe()*mm;
    mom     = P_magic()*(1 + xe*(1-n)/R_magic());
    delta   = (mom - P_magic())/(P_magic());
  }

  G4double EqRad = R_magic() + xe;
  G4double mugamma = Gamma(mom);
  G4double randT = 0.;
  G4double t0 = 0.0;
  //G4double kicker_timeoffset = 39.9*ns;
  G4double kicker_phaseoffset = -TMath::Pi()/2;
  if ( StartPerfect_ ) {
    kicker_phaseoffset = 0.0;
  }
  

  gm2geom::inflectorGeometry const& ig = infGeom_.ig ;//inflectorGeometry::getInstance();
  G4ThreeVector upstream(0., 0., -ig.mandrel_length());  
  upstream.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream.rotateY(infGeom_.swingAngle);
  upstream += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream.rotateY(infGeom_.rotAngle);
  G4ThreeVector downstream(R_magic()+ig.aperture_off(), 0., 0.);
  downstream.rotateY(infGeom_.rotAngle);
  G4ThreeVector dir = (downstream - upstream).unit();
  G4ThreeVector storage_orbit_start(R_magic(), 0., 0.);
  G4ThreeVector beam_start = storage_orbit_start;




  //------------------------------------------------------  
  //
  // Generate t0
  //
  //------------------------------------------------------
  G4ThreeVector diff = beam_start-downstream;
  G4double sign = 1.;
  if( dir.cosTheta(diff) < 0 ) {
    sign = -1.;
  }
  randT = 100.0*ns + sign*diff.mag()/(2.*pi*R_magic()/150.)*ns;
  if ( 0 ) {
    G4cout << diff << "\t" << beam_start << "\t";
    G4cout << "RandT(2) = " << randT << G4endl;
  }
  h_t0off->Fill(randT);
  
  if( GetUseFNALt0() ) {
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E989";
    randT = randTFromE989() + 100*ns;    
  } 
  else {
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E821";
    do{
      randT = G4RandGauss::shoot( 0., 25. )*ns;
    } while( abs(randT)>50. );
    randT += -50.;
  }
  



  //------------------------------------------------------  
  //  Generate a random decay time by inverting Exp[ -t/gamma*tau ], and add it to the random tInitial from above.
  //------------------------------------------------------  
  G4double randNum = 0.0;
  G4double newTime = GetMaxDecayTime() + 0.0001*ns;
  G4double decaytime = 0.0;
  t0 = randT;
  
  if ( GetFlatDecayTime() ) {
    while ( newTime > GetMaxDecayTime() ) {
      newTime = randT + G4UniformRand()*GetMaxDecayTime();
    }
  }
  else {
    if ( GetMaxDecayTime() > 0 ) {
      while ( newTime > GetMaxDecayTime() ) {
	randNum = G4UniformRand();
	decaytime =  - mugamma * particleLifetime * log(randNum);
	newTime = decaytime;
      }
    }
    else {
      randNum = G4UniformRand();
      decaytime =  - mugamma * particleLifetime * log(randNum);
      newTime = decaytime;
    }
  }
  
  randT = newTime;
  
  G4double totaltime = decaytime + t0;
  if ( debug_ ) { 
    G4cout << "  t0         = " << t0 << " ns." << G4endl;
    G4cout << "  t_{decay}  = " << decaytime/microsecond << " us." << G4endl;
  }







  //------------------------------------------------------  
  //
  // Compute omega_{c} , pos/mom angle = omega_{c} * t - inflector offset
  //
  //------------------------------------------------------  
  G4double s0 = kicker_phaseoffset * EqRad - RotAngle_ * EqRad;
  G4double omegaC = ComputeOmegaCGivenMomRadius(mom, EqRad);
  G4double radianlength = omegaC * decaytime + kicker_phaseoffset;
  G4double arclength = radianlength * EqRad;
  G4double psi = radianlength;
  G4double theta = std::fmod(psi, TMath::TwoPi());


  if ( debug_ ) {
    G4cout << "   t*c*beta      = " << arclength << endl;
    G4cout << "   t_{C} = 1/fC(" << mom/P_magic() << " , " << EqRad <<" ) = " << 1/ComputeFrequencyCGivenMomRadius(mom, EqRad) << " ns." << G4endl;
    G4cout << "   t_{C} = 1/fC(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << 1/ComputeFrequencyCGivenMomRadius(P_magic(), R_magic()) << " ns." << G4endl;
  }


  //------------------------------------------------------
  //
  // Compute omega_{s} , spin angle = omega_{s} * t
  //
  //------------------------------------------------------
  G4double omegaS = ComputeOmegaSGivenMomRadius(mom, EqRad);
  G4double omegaA = ComputeOmegaAGivenMomRadius(mom, EqRad);
  G4double radianlength_spin = omegaS*decaytime + kicker_phaseoffset;
  G4double psi_spin = radianlength_spin;
  G4double radianlength_anom = omegaA*decaytime + kicker_phaseoffset;
  if ( debug_ ) {
    G4cout << "   t_{S} = 1/fS(" << mom/P_magic() << " , " << EqRad <<" ) = " << 1/ComputeFrequencySGivenMomRadius(mom, EqRad) << " ns." << G4endl;
    G4cout << "   t_{S} = 1/fS(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << 1/ComputeFrequencySGivenMomRadius(P_magic(), R_magic()) << " ns." << G4endl;
    G4cout << "   t_{a} = 1/fa(" << mom/P_magic() << " , " << EqRad <<" ) = " << 1/ComputeFrequencyAGivenMomRadius(mom, EqRad) << " ns." << G4endl;
    G4cout << "   t_{a} = 1/fa(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << 1/ComputeFrequencyAGivenMomRadius(P_magic(), R_magic()) << " ns." << G4endl;
  }
  




  //------------------------------------------------------
  //
  // Rotate x, y, z axis
  //
  //------------------------------------------------------
  TVector3 x_axis(1, 0, 0);
  TVector3 y_axis(0, 1, 0);
  TVector3 z_axis(0, 0, 1);
  G4ThreeVector xaxis0(x_axis.X(), x_axis.Y(), x_axis.Z());
  G4ThreeVector yaxis0(y_axis.X(), y_axis.Y(), y_axis.Z());
  G4ThreeVector zaxis0(z_axis.X(), z_axis.Y(), z_axis.Z());
  if ( debug_ ) {
    G4cout << "  x0 = ( " << x_axis.X() << "\t" << x_axis.Y() << "\t" << x_axis.Z() << " )" << G4endl;
    G4cout << "  y0 = ( " << y_axis.X() << "\t" << y_axis.Y() << "\t" << y_axis.Z() << " )" << G4endl;
    G4cout << "  z0 = ( " << z_axis.X() << "\t" << z_axis.Y() << "\t" << z_axis.Z() << " )" << G4endl;
  }
  RotateRingToBeamCoordinates(theta, &x_axis, &y_axis, &z_axis);
  G4ThreeVector xaxis(x_axis.X(), x_axis.Y(), x_axis.Z());
  G4ThreeVector yaxis(y_axis.X(), y_axis.Y(), y_axis.Z());
  G4ThreeVector zaxis(z_axis.X(), z_axis.Y(), z_axis.Z());
  if ( debug_ ) {
    G4cout << "  x' = ( " << x_axis.X() << "\t" << x_axis.Y() << "\t" << x_axis.Z() << " )" << G4endl;
    G4cout << "  y' = ( " << y_axis.X() << "\t" << y_axis.Y() << "\t" << y_axis.Z() << " )" << G4endl;
    G4cout << "  z' = ( " << z_axis.X() << "\t" << z_axis.Y() << "\t" << z_axis.Z() << " )" << G4endl;
  }


  
 
  //------------------------------------------------------  
  //
  // Compute maximum displacements in X, Y, and in X', Y'
  //
  //------------------------------------------------------  
  G4double x0, y0, x0Prime, y0Prime;
  G4double MaxX, MaxY, MaxXprime, MaxYprime;
  G4double s0X, s0Y;
  MaxAmplitudes(arclength, &MaxX, &MaxY, &MaxXprime, &MaxYprime, &s0X, &s0Y);





  
  //first_event = false;
  //return;

  if ( debug_ ) {
    G4cout.precision(5);
    G4cout << endl;
    G4cout << "  Xe            = " << xe << " mm." << G4endl;
    G4cout << "  <R>           = " << EqRad << " mm." << G4endl;
    G4cout << "  T             = " << decaytime << " ns." << G4endl;
    G4cout << "  Gamma         = " << mugamma << G4endl;
    G4cout << "  Pz            = " << mom << " MeV." << G4endl;
    G4cout << "  Mom(gamma)    = " << Mom(mugamma) << " MeV." << G4endl;
    G4cout << "  R(gamma, n)   = " << R(mugamma, n) << " mm." << G4endl;
    G4cout << "  R(gammaM, n)  = " << R(gammaMagic(), n) << " mm." << G4endl;
    G4cout << "  s/(2pi*<R>)   = " << arclength / (TMath::TwoPi()*EqRad) << " mm." << G4endl;
    G4cout << "  Psi           = " << psi << " rad." << G4endl;
    G4cout << "  Theta         = " << theta << " rad." << G4endl;
  }



  //------------------------------------------------------
  //
  //  Generate (x, x') at t=0
  //
  //------------------------------------------------------
  bool inside = false;
  if ( MaxX > 0 ) {
    while ( inside == false ) {
      x0 = MaxX*(1*G4UniformRand()-0);
      if ( PosOffset_ ) { x0 = MaxX*0.99; }
      x0Prime = MaxXprime*(2*G4UniformRand()-1);
      if ( NoTransMom_ ) { x0Prime = 0.0; }

      if ( debug_ ) {
	G4cout << "  MaxAx         = " << MaxX << " mm." << G4endl;
	G4cout << "  [x,x']        = " << x0 << " , " << x0Prime/mrad << " mm,mrad." << G4endl;
	G4cout << "  (x'/|x'|)^{2} = " <<  TMath::Power(x0Prime/MaxXprime, 2) << G4endl;
	G4cout << "  (x/|x|)^{2}   = " << TMath::Power(x0/MaxX, 2) << G4endl;
	G4cout << "  Sum           = " << TMath::Power(x0Prime/MaxXprime, 2) + TMath::Power(x0/MaxX, 2) << G4endl;
      }
      
      if ( TMath::Power(x0Prime/MaxXprime, 2) + TMath::Power(x0/MaxX, 2) < 1 ) { inside = true; }
    }
  }
  else {
    x0 = 0.0;
    x0Prime = 0.0;
  }

  if ( debug_ ) {
    G4cout << "Ax            = " << MaxX << "mm." << G4endl;
    G4cout << "x             = " << x0 << " mm." << G4endl;
    G4cout << "x'            = " << x0Prime/mrad << " mrad." << G4endl;
  }
  


  //------------------------------------------------------
  //
  //  Generate (y, y') at t=0
  //
  //------------------------------------------------------
  inside = false;
  if ( MaxY > 0 ) {
    while ( inside == false ) {
      y0 = MaxY*(2*G4UniformRand()-1);
      if ( PosOffset_ ) { y0 = MaxY*0.99; }
      y0Prime = MaxYprime*(2*G4UniformRand()-1);
      if ( NoTransMom_ ) { y0Prime = 0.0; }

      if ( debug_ ) {
	G4cout << "  MaxAy         = " << MaxY << " mm." << G4endl;
	G4cout << "  [y,y']        = " << y0 << " , " << y0Prime/mrad << " mm,mrad." << G4endl;
	G4cout << "  (y'/|y'|)^{2} = " <<  TMath::Power(y0Prime/MaxYprime, 2) << G4endl;
	G4cout << "  (y/|y|)^{2}   = " << TMath::Power(y0/MaxY, 2) << G4endl;
	G4cout << "  Sum           = " << TMath::Power(y0Prime/MaxYprime, 2) + TMath::Power(y0/MaxY, 2) << G4endl;
      }
      if ( TMath::Power(y0Prime/MaxYprime, 2) + TMath::Power(y0/MaxY, 2) < 1 ) { inside = true; }
    }
  }
  else {
    y0 = 0.0;
    y0Prime = 0.0;
  }

  if ( debug_ ) {
    G4cout << "Ay            = " << MaxY << "mm." << G4endl;
    G4cout << "y             = " << y0 << " mm." << G4endl;
    G4cout << "y'            = " << y0Prime/mrad << " mrad." << G4endl;
  }



  //------------------------------------------------------
  //
  //  Propagate the particle from the "naught" coordinates back to the injection point
  //
  //------------------------------------------------------
  G4double randX = x0 + s0X*x0Prime;
  G4double randY = y0 + s0Y*y0Prime;
  G4double randXPrime = x0Prime;
  G4double randYPrime = y0Prime;



  //------------------------------------------------------
  //
  // Create transport matrix
  //
  // (x )    = M x (x )
  // (x')_{t}      (x')_{0}
  //
  //------------------------------------------------------
  double newdelta = delta;
  double newX = randX;
  double newXprime = randXPrime;
  G4double x_t0 = newX;
  G4double xprime_t0 = newXprime;
  Transport(&newX, &newXprime, &newdelta, kX, arclength, s0);
  double newY = randY;
  double newYprime = randYPrime;
  G4double y_t0 = newY;
  G4double yprime_t0 = newYprime;
  Transport(&newY, &newYprime, &newdelta, kY, arclength, s0);
  G4double x_gen = newX;
  G4double y_gen = newY;
  G4double xprime_gen = newXprime;
  G4double yprime_gen = newYprime;
  
  

  //------------------------------------------------------
  //
  // Apply CBO Oscillations
  //
  // x  = x + CBO
  // x' = x' + dCBO/ds
  //
  //------------------------------------------------------
  double x_cbo = CBOValue(decaytime/microsecond);
  newX += x_cbo;
  double xp_cbo = fCBO->Derivative(decaytime/microsecond);
  // dx/dt = [mm/us]
  xp_cbo *= TMath::TwoPi()*0.4666/((omegaC*microsecond)*R_magic()); // omegaC [1/ns] * 1000 -> [1/us] * [1/mm];
  newXprime += xp_cbo;





  
  //------------------------------------------------------
  //
  // Compute Position Vector
  //
  // X(t=0) = X_{e} + M(s)*(x,x')_{t=0}
  // Y(t=0) = M(s)*(y,y')_{t=0}
  //
  //------------------------------------------------------  
  double x = newX + xe;
  double y = newY;
  G4double posX, posY, posZ;
  G4ThreeVector posV = G4ThreeVector(x+R_magic(),y,0);
  G4ThreeVector posV0 = G4ThreeVector(x+R_magic(),y,0);
  G4double posX0 = posV.x();
  G4double posY0 = posV.y();
  G4double posZ0 = posV.z();
  if ( debug_ ) {
    G4cout << "x0            = " << posX0 << " mm." << G4endl;
    G4cout << "y0            = " << posY0 << " mm." << G4endl;
    G4cout << "z0            = " << posZ0 << " mm." << G4endl;
  }
  posV.rotateY(-theta);
  posX = posV.x();
  posY = posV.y();
  posZ = posV.z();
  if ( debug_ ) {
    G4cout << "x             = " << posX << " mm." << G4endl;
    G4cout << "y             = " << posY << " mm." << G4endl;
    G4cout << "z             = " << posZ << " mm." << G4endl;
  }
  







  //------------------------------------------------------
  //
  // Compute Momentum Vector
  //
  // Pz(t=0) = mom
  // Px(t=0) = x' * Pz  (x' = Px/Pz)
  // Py(t=0) = y' * Pz  (y' = Py/Pz)
  //
  //------------------------------------------------------  
  G4double mom_azimuth   = mom;
  G4double mom_vertical  = newYprime*mom;
  G4double mom_radial    = newXprime*mom;
  G4double momX, momY, momZ;
  G4double mom_azimuth_t0   = mom_azimuth;
  //G4double mom_vertical_t0  = mom_vertical;
  //G4double mom_radial_t0    = mom_radial;
  G4ThreeVector momV0 = G4ThreeVector(mom_radial,mom_vertical,mom_azimuth);
  ComputeGEANTMomentum(&momX, &momY, &momZ, mom_radial, mom_vertical, mom_azimuth, theta); 
  G4ThreeVector momV = G4ThreeVector(mom_radial,mom_vertical,mom_azimuth);
  //momV.rotateY(RotAngle_);
  //momV0.rotateY(RotAngle_);
  G4double momX0 = momV.x();
  G4double momY0 = momV.y();
  G4double momZ0 = momV.z();
  momV.rotateY(-theta);
  momX = momV.x();
  momY = momV.y();
  momZ = momV.z();
  if ( debug_ ) {
    G4cout << "pz            = " << momZ << G4endl;
    G4cout << "px            = " << momX << G4endl;
    G4cout << "py            = " << momY << G4endl;
  }
  

   




  //------------------------------------------------------
  //
  // Compute Spin Vector
  //
  // S = p @ t=0
  // S(t) = rotate(omega_{S}*t)*S(t=0)
  //
  //------------------------------------------------------  
  G4ThreeVector polV = G4ThreeVector(momX0, momY0, momZ0);
  if ( GetPolarization() == "E821" || GetPolarization() == "E821Mixed" ) {
    G4double mom_GeV = mom*GeV;
    mom_GeV = TMath::Min(mom_GeV, 3.13);
    mom_GeV = TMath::Max(mom_GeV, 3.06);
    G4double sz_mean = fsz->Eval(mom_GeV);
    G4double sz = G4RandGauss::shoot( sz_mean, 0.017 );
    sz = TMath::Min(sz, 1.0);
    G4double st = TMath::Sqrt(1 - sz*sz);;
    G4double sphi = fsphi->GetRandom();
    G4double sr = st*TMath::Cos(sphi);
    G4double sv = st*TMath::Sin(sphi);
    polV.setX(sr);
    polV.setY(sv);
    polV.setZ(sz);
  }
  else if ( GetPolarization() == "100%" || GetPolarization() == "100" || GetPolarization() == "FullMixed" || GetPolarization() == "Full-Mixed" || GetPolarization() == "Full" ) {
    polV.setX(0.0);
    polV.setY(0.0);
    polV.setZ(1.0);
  }
  polV = polV.unit();


  // The spin rotation angle offset is defined as
  // phi_{s} = omega_{s} / omega_{c} x phi_{c}
  //G4double RotAngle_spin = RotAngle_ * ComputeOmegaSGivenMomRadius(mom, x + R_magic()) / ComputeOmegaCGivenMomRadius(mom, x + R_magic());
  
  
  G4double polX, polY, polZ;
  G4double polX0, polY0, polZ0;
  polX0 = polV.x();
  polY0 = polV.y();
  polZ0 = polV.z();
  G4ThreeVector polV0 = G4ThreeVector(polX0, polY0, polZ0);
  G4double spin_phase_offset = TMath::Pi()/2;
  spin_phase_offset = 0.0;
  polV.rotateY( -psi_spin - spin_phase_offset);
  polX = polV.x();
  polY = polV.y();
  polZ = polV.z();


  G4ThreeVector polVt = G4ThreeVector(momX, momY, momZ);
  polVt = polVt.unit();
  polX0 = polVt.x();
  polY0 = polVt.y();
  polZ0 = polVt.z();
  polV0.setX(polX0);
  polV0.setY(polY0);
  polV0.setZ(polZ0);
  G4double anom_phase = omegaA*decaytime;
  polVt.rotateY( -anom_phase );
  polX = polVt.x();
  polY = polVt.y();
  polZ = polVt.z();
  polV.setX(polX);
  polV.setY(polY);
  polV.setZ(polZ);

  //G4cout << polV.


    sum_xe += xe;
    sum_x += randX;
    sum_y += randY;
    sum_xprime += randXPrime;
    sum_yprime += randYPrime;

    rms_xe += xe*xe;
    rms_x += randX*randX;
    rms_y += randY*randY;
    rms_xprime += randXPrime*randXPrime;
    rms_yprime += randYPrime*randYPrime;

    ngen++;

    h_x_cbo->Fill(x_cbo);
    h_xp_cbo->Fill(xp_cbo/mrad);

    h_Ax->Fill(MaxX);
    h_Ay->Fill(MaxY);

    h_pol->Fill(polV.dot(momV)/momV.mag());
    h_time_pol->Fill(decaytime/microsecond, polV.dot(momV)/momV.mag());

    h_xe->Fill(xe);
    h_x->Fill(randX);
    h_xp->Fill(randXPrime/mrad);
    h_y->Fill(randY);
    h_yp->Fill(randYPrime/mrad);
    
    h_t0->Fill(t0);
    h_time->Fill(totaltime/microsecond);
    h_time_m_t0->Fill((decaytime-t0)/microsecond);

    int counter = (int)150*(radianlength/TMath::TwoPi());
    G4double tmod = 0.14915*((float)counter/150);
    h_time_tmod->Fill((totaltime-tmod)/microsecond);
    h_time_decay_tmod->Fill((decaytime-tmod)/microsecond);

    double arc0 = std::fmod(radianlength, TMath::TwoPi());
    if ( fabs(arc0) < TMath::TwoPi()/150 ) {
      h_time_VS0->Fill((totaltime)/microsecond);
    }

    h_turn->Fill(psi/TMath::TwoPi());
    

    G4double betaX_mm = Beta(kX, arclength)*(mm/m);
    G4double betaY_mm = Beta(kY, arclength)*(mm/m);
    G4double gammaX_mm = (1+Alpha(kX, arclength)*Alpha(kX, arclength)) / betaX_mm;
    G4double gammaY_mm = (1+Alpha(kX, arclength)*Alpha(kY, arclength)) / betaY_mm;
    G4double newXprime_mrad = newXprime/mrad;
    G4double newYprime_mrad = newYprime/mrad;
    G4double emitX = gammaX_mm*newX*newX + 2*Alpha(kX, arclength)*newX*newXprime_mrad + betaX_mm*(newXprime_mrad*newXprime_mrad);
    G4double emitY = gammaY_mm*newY*newY + 2*Alpha(kY, arclength)*newY*newYprime_mrad + betaY_mm*(newYprime_mrad*newYprime_mrad);
    //G4cout << emitX/emittanceX << "\t" << emitY/emittanceY << G4endl;
    
    h_emitX->Fill(emitX);
    h_emitY->Fill(emitY);

  G4strstreambuf* oldBuffer = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
  // Load the beamTransportGun
  beamTransportGun_->SetParticleTime( totaltime );
  //beamTransportGun_->SetParticleTime( decaytime );
  beamTransportGun_->SetParticlePosition( G4ThreeVector( posX, posY, posZ ) );
  beamTransportGun_->SetParticleMomentum( G4ThreeVector( momX, momY, momZ ) );
  beamTransportGun_->SetParticlePolarization( G4ThreeVector( polX, polY, polZ ) );
  G4cout.rdbuf(oldBuffer);





  if ( first_event ) {
    G4cout << G4endl;
    G4cout << "====================================================================" << G4endl;
    G4cout << "=======================  BEAMTRANSPORT GUN   =======================" << G4endl;
    G4cout << "====================================================================" << G4endl;

    G4cout << "  Particle   = " << Particle_ << G4endl;
    G4cout << "Generating beam from a Uniform Distribution." << G4endl;

    if ( GetStartPerfect() ) {
      G4cout << "Initial Rotation Angle: " << RotAngle_ << G4endl;
    }
    G4cout << "Kicker Phase Offset Angle: " << kicker_phaseoffset << G4endl;



    G4cout << "Beta Function/Beam Information: " << G4endl;
    G4cout.precision(3);
    double M[9]={0, 0, 0, 0, 0, 0, 0, 0, 0};
    TransportMatrix(M, kX, arclength, s0);
    G4cout.precision(5);
    if ( GetUseDispersion() ) {
      G4cout << "  Using 4-parameter fits for dispersion function." << G4endl;
    }
    G4cout << "  Transport Matrix for X" << G4endl;
    if ( GetUseDispersion() ) {
      G4cout << "  [ " << M[0] << "\t" << M[1] << "\t" << M[2] << " ]" << G4endl;
      G4cout << "  [ " << M[3] << "\t" << M[4] << "\t" << M[5] << " ]" << G4endl;
      G4cout << "  [ " << M[6] << "\t" << M[7] << "\t" << M[8] << " ]" << G4endl;
    }
    else {
      G4cout << "  [ " << M[0] << "\t" << M[1] << " ]" << G4endl;
      G4cout << "  [ " << M[2] << "\t" << M[3] << " ]" << G4endl;
    }

    TransportMatrix(M, kY, arclength, s0);
    G4cout << "  Transport Matrix for Y" << G4endl;
    if ( GetUseDispersion() ) {
      G4cout << "  [ " << M[0] << "\t" << M[1] << "\t" << M[2] << " ]" << G4endl;
      G4cout << "  [ " << M[3] << "\t" << M[4] << "\t" << M[5] << " ]" << G4endl;
      G4cout << "  [ " << M[6] << "\t" << M[7] << "\t" << M[8] << " ]" << G4endl;
    }
    else {
      G4cout << "  [ " << M[0] << "\t" << M[1] << " ]" << G4endl;
      G4cout << "  [ " << M[2] << "\t" << M[3] << " ]" << G4endl;
    }
    G4cout << "  emittanceX = " << GetEmittanceX()/mrad << " mm*mrad." << G4endl;
    G4cout << "  emittanceY = " << GetEmittanceY()/mrad << " mm*mrad." << G4endl;
    if ( GetUseConstBeta() ) {
      G4cout << "  betaX      = " << BetaX/m << " m." << G4endl;
      G4cout << "  betaY      = " << BetaY/m << " m." << G4endl;
      G4cout << "  alphaX     = " << AlphaX << G4endl;
      G4cout << "  alphaY     = " << AlphaY << G4endl;
    }
    else {
      G4cout << "  Using 4-parameter fits for BetaX and BetaY. Alpha = 1/2*dBeta/ds" << G4endl;
    }
    G4cout << "  Xmax       = " << MaxX*mm << " mm." << G4endl;
    G4cout << "  X'max      = " << MaxXprime/mrad << " mrad." << G4endl;
    G4cout << "  Ymax       = " << MaxY*mm << " mm." << G4endl;
    G4cout << "  Y'max      = " << MaxYprime/mrad << " mrad." << G4endl;
    if ( 1 ) {
      G4cout << "  Simulating CBO from a " << GetKick() << "G Square Pulse Kick." << G4endl;
      G4cout << "    Using CBO of Form: CBO = O + Asin(2pi*f*t + P)" << G4endl;
      G4cout << "    [O] = " << fCBO->GetParameter(0) << " mm." << G4endl;
      G4cout << "    [A] = " << fCBO->GetParameter(1) << " mm." << G4endl;
      G4cout << "    [f] = " << fCBO->GetParameter(2) << " MHz." << G4endl;
      G4cout << "    [P] = " << fCBO->GetParameter(3) << " rad." << G4endl;
    }
    if ( 0 ) {
      G4cout << "OmegaC*t   = " << -psi << G4endl;
      G4cout << "OmegaA*t   = " << -anom_phase << G4endl;
      G4ThreeVector test3v1(1, 0, 0);
      G4ThreeVector test3v2(1, 0, 0);
      G4cout << "90d phase  = " << test3v1.rotateY(-TMath::Pi()/2) << G4endl;
      G4cout << "-90d phase = " << test3v2.rotateY(TMath::Pi()/2) << G4endl;
      G4ThreeVector test3v3(0, 0, 1);
      G4ThreeVector test3v4(0, 0, 1);
      G4cout << "90d phase  = " << test3v3.rotateY(-TMath::Pi()/2) << G4endl;
      G4cout << "-90d phase = " << test3v4.rotateY(TMath::Pi()/2) << G4endl;
    }

    G4cout << "Time Information: " << G4endl;
    if ( GetFlatDecayTime() ) {
      G4double maxtau = GetMaxDecayTime();
      G4cout << "  Decay time is flat between [0, " << maxtau << "] microseconds." << G4endl;
    }
    else {
      G4cout << "  Decay time is exponential." << G4endl;
    }
        
    if ( GetUseFNALt0() ) {
      G4cout << "  Using FNAL t0 beam [0,100] ns" << G4endl;
    }
    else {
      G4cout << "  Using Gaussian w/ sigmaT = 25 ns." << G4endl;
    }
    G4cout << "  t0         = " << t0 << " ns." << G4endl;
    G4cout << "  t_{decay}  = " << decaytime/microsecond << " us." << G4endl;
    G4cout << "  s0/2piR    = " << s0 << " turns." << G4endl;
    G4cout << "  s/2piR     = " << arclength/(TMath::TwoPi()*R_magic()) << " turns." << G4endl;
    G4cout << "  psi        = " << psi << " rad." << G4endl;
    G4cout << "  theta      = " << theta << " rad." << G4endl;
    G4cout << G4endl;
    
    G4cout << "Position Information:" << G4endl;
    G4cout.precision(3);
    G4cout << "  x(0)       = " << x_t0 << " mm." << G4endl;
    G4cout << "  x(s)       = " << x_gen << " mm." << G4endl;
    G4cout << "  x_{cbo}(s) = " << x_cbo << " mm." << G4endl;
    G4cout << "  y(0)       = " << y_t0 << " mm." << G4endl;
    G4cout << "  y(s)       = " << y_gen << " mm." << G4endl;
    G4cout.precision(4);
    G4cout << "  pos(t)     = " << posX << " , " << posY << " , " << posZ << " mm." << G4endl;
    G4cout << "  r.z(r)(y)0 = " << posV0.dot(zaxis0) << " , " << posV0.dot(xaxis0) << " , " << posV0.dot(yaxis0) << " MeV." << G4endl;
    G4cout << "  r.z(r)(y)  = " << posV.dot(zaxis) << " , " << posV.dot(xaxis) << " , " << posV.dot(yaxis) << " MeV." << G4endl;
    G4cout << G4endl;

    G4cout << "Momentum Information:" << G4endl;    
    G4cout << "  p_{az}(0)  = " << mom_azimuth_t0 << " MeV." << G4endl;
    G4cout << "  p_{az}(t)  = " << mom_azimuth << " MeV." << G4endl;
    G4cout << "  x'(0)      = " << xprime_t0/mrad << " mrad." << G4endl;
    G4cout << "  x'(s)      = " << xprime_gen/mrad << " mrad." << G4endl;
    G4cout << "  y'(0)      = " << yprime_t0/mrad << " mrad." << G4endl;
    G4cout << "  y'(s)      = " << yprime_gen << " mrad." << G4endl;
    G4cout << "  mom(t)     = " << momX << " , " << momY << " , " << momZ << " MeV." << G4endl;
    G4cout << "  mom.pos0   = " << momV0.dot(posV0)/(posV0.mag()*momV0.mag()) << " rad." << G4endl;
    G4cout << "  mom.pos    = " << momV.dot(posV)/(posV.mag()*momV.mag()) << " rad." << G4endl;
    G4cout << "  p.z(r)(y)0 = " << momV0.dot(zaxis0) << " , " << momV0.dot(xaxis0) << " , " << momV0.dot(yaxis0) << " MeV." << G4endl;
    G4cout << "  p.z(r)(y)  = " << momV.dot(zaxis) << " , " << momV.dot(xaxis) << " , " << momV.dot(yaxis) << " MeV." << G4endl;
    G4cout << G4endl;
    
    G4cout << "Spin Information: " << G4endl;
    if ( GetPolarization() == "Full-Mixed" || GetPolarization() == "FullMixed" ) {
      G4cout << "  Using polarization of muons in tails is rotated by 2pi/3 from 100% in the core." << G4endl;
    }
    else if ( GetPolarization() == "Mixed" || GetPolarization() == "E821Mixed" ) {
      G4cout << "  Using polarization of muons in tails is rotated by 2pi/3 from what is in the core." << G4endl;
    }
    else if ( GetPolarization() == "E821" || GetPolarization() == "E821Mixed" ) {
      G4cout << "  Using E821 Hughon Polarization Fits." << G4endl;
    }
    else if ( GetPolarization() == "100%" || GetPolarization() == "100" || GetPolarization() == "Full" ) {
      G4cout << "  Assuming 100% polarization (s-dot-p=1)." << G4endl;
    }
    else {
      G4cout << "  Random Polarization." << G4endl;
    }
    
    double pol_azimuth, pol_radial, pol_vertical;
    ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polX0, polY0, polZ0, -RotAngle_);
    G4cout.precision(3);
    G4cout << "  s(0)       = " << polX0 << " , " << polY0 << " , " << polZ0 << G4endl;
    G4cout << "  s(t)       = " << polX << " , " << polY << " , " << polZ << G4endl;
    G4cout << "  s'(t)      = " << polVt.x() << " , " << polVt.y() << " , " << polVt.z() << G4endl;
    G4cout << "  s.z(r)(y)0 = " << polV0.dot(zaxis0) << " , " << polV0.dot(xaxis0) << " , " << polV0.dot(yaxis0) << G4endl;
    G4cout << "  s.z(r)(y)  = " << polV.dot(zaxis) << " , " << polV.dot(xaxis) << " , " << polV.dot(yaxis) << G4endl;
    G4cout << "  s(0).p(0)  = " << polV0.dot(momV0)/momV0.mag() << G4endl;
    G4cout << "  s(t).p(t)  = " << polV.dot(momV)/momV.mag() << G4endl;
    G4cout << "  |s|        = " << polV.mag() << G4endl;
    G4cout << "  A(s)/A(c)  = " << psi_spin/psi << G4endl;
    G4cout << "  P(s)/P(c)  = " << radianlength/radianlength_spin << G4endl;
    G4cout << "  P(c)/P(a)  = " << radianlength/radianlength_anom << G4endl;
    G4cout << "  t_{c}      = " << 1/ComputeFrequencyCGivenMomRadius(P_magic(), R_magic())/microsecond << " us." << G4endl;
    G4cout << "  t_{s}      = " << 1/ComputeFrequencySGivenMomRadius(P_magic(), R_magic())/microsecond << " us." << G4endl;
    G4cout << "  t_{a}      = " << 1/ComputeFrequencyAGivenMomRadius(P_magic(), R_magic())/microsecond << " us." << G4endl;
    G4cout << "  t_{a}/t_{c}= " << 1/ComputeFrequencyAGivenMomRadius(P_magic(), R_magic())/(1/ComputeFrequencyCGivenMomRadius(P_magic(), R_magic())) << G4endl;
    first_event = false;
  }




  
  //  FIRE    
  mf::LogInfo("G2BeamTransportSource") << "About to call beamTransportGun_->GeneratePrimaryVertex";  
  beamTransportGun_->GeneratePrimaryVertex( evt );
  mf::LogInfo("G2BeamTransportSource") << "Just got back from calling beamTransportGun_->GeneratePrimaryVertex";  
  
  if ( evt && 0 ) {
    G4cout << "Generated muon w/ t0 = " << t0 << " and total time = " << randT << G4endl;
    G4PrimaryVertex *primary_vtx = evt->GetPrimaryVertex();
    G4PrimaryParticle *primary = primary_vtx->GetPrimary();
    primary->SetWeight(t0/100.0); // *microsecond);
  }

} // generatePrimaries


G4double gm2ringsim::G2BeamTransportSource::randTFromE989()
{
  //  NSF: E989 Tri-Gaussian longitudinal profile.  The beam is 100 ns "long", and is 
  //  comprised of a large central Gaussian flanked on either side by two smaller,     
  //  skinnier Gaussians.  (I made an educated guess at the parameters here -- this 
  //  distribution will change once we hear more definitive characteristics from Fermilab.)
  
  static G4int const nBinsZ = 101;
  static G4double const longitudinalProfile[nBinsZ] =
    {  //  trailing edge                                                                                               
      0.196236,
      0.243809, 0.304403, 0.376835, 0.456942, 0.537576,
      0.609626, 0.663974, 0.693850, 0.696746, 0.675160,
      0.635854, 0.587933, 0.540497, 0.500681, 0.472630,
      0.457511, 0.454250, 0.460555, 0.473826, 0.491751,
      0.512566, 0.535069, 0.558507, 0.582433, 0.606584,
      0.630796, 0.654953, 0.678956, 0.702718, 0.726149,
      0.749162, 0.771668, 0.793580, 0.814810, 0.835270,
      0.854875, 0.873541, 0.891187, 0.907737, 0.923116,
      0.937254, 0.950088, 0.961558, 0.971610, 0.980198,
      0.987281, 0.992825, 0.996805, 0.999200, 1.000000,
      0.999200, 0.996805, 0.992825, 0.987281, 0.980198,
      0.971610, 0.961558, 0.950088, 0.937254, 0.923116,
      0.907737, 0.891187, 0.873541, 0.854875, 0.835270,
      0.814810, 0.793580, 0.771668, 0.749162, 0.726149,
      0.702718, 0.678956, 0.654953, 0.630796, 0.606584,
      0.582433, 0.558507, 0.535069, 0.512566, 0.491751,
      0.473826, 0.460555, 0.454250, 0.457511, 0.472630,
      0.500681, 0.540497, 0.587933, 0.635854, 0.675160,
      0.696746, 0.693850, 0.663974, 0.609626, 0.537576,
      0.456942, 0.376835, 0.304403, 0.243809, 0.196236
      // leading edge 
    }; //  Generates random numbers uniformly across bins of above distribution 
  
  randFromDistr scaledLongDistr( longitudinalProfile, nBinsZ, 0 );
  
  //  Generates random numbers as delta functions at lower edge of bins of above distribution 
  //  randFromDistr scaledLongDistr( longitudinalProfile, nBinsZ, 1 );
  
  //  RandGeneral always returns a variate on ]0,1[.  It is the user's responsibility to transform this into the domain native to the distribution (see below).                                                                        
  double randNum = scaledLongDistr.shoot();
  
  //  Convert from ]0,1[ to a bonafide longitudinal coordinate, or equivalently, an offset time.
  static G4double const tMin = -100.*ns;
  static G4double const tMax = 0.*ns;
  static G4double const DeltaT = tMax-tMin;
  static G4double const dt = DeltaT/( nBinsZ-1 );
  
  //  Here, "z" and "tOffset" may be regarded as the same 
  G4double myRandT = tMin + randNum*DeltaT - dt/2.;
  
  return myRandT;
}


double gm2ringsim::G2BeamTransportSource::GenerateRandomdPoverP()
{
  static G4int const nBins = 155;
  double cdf[nBins]={0.0 , 0.000129915 , 0.000301122 , 0.000509591 , 0.000743573 , 0.00103462 , 0.001372 , 0.00177685 , 0.0022747 , 0.00285109 , 0.00348724 , 0.00423383 , 0.00508685 , 0.00604997 , 0.00711883 , 0.00833809 , 0.00966947 , 0.0111784 , 0.0128741 , 0.0147285 , 0.0168215 , 0.0191053 , 0.0215797 , 0.024311 , 0.0272198 , 0.0304274 , 0.0338311 , 0.0375325 , 0.0414367 , 0.0457041 , 0.0502195 , 0.0550059 , 0.0601247 , 0.0655452 , 0.0712524 , 0.0771809 , 0.0835209 , 0.0900197 , 0.0969176 , 0.104047 , 0.111493 , 0.119264 , 0.127142 , 0.13533 , 0.143782 , 0.152465 , 0.161342 , 0.170432 , 0.179726 , 0.189215 , 0.198976 , 0.208898 , 0.21888 , 0.22912 , 0.239541 , 0.25021 , 0.260924 , 0.27179 , 0.282979 , 0.294106 , 0.305592 , 0.317095 , 0.328708 , 0.340428 , 0.352263 , 0.364333 , 0.376396 , 0.388421 , 0.400651 , 0.412801 , 0.425186 , 0.437578 , 0.449941 , 0.462429 , 0.474996 , 0.487626 , 0.500159 , 0.512618 , 0.525043 , 0.537537 , 0.549962 , 0.562255 , 0.574621 , 0.586961 , 0.599303 , 0.61142 , 0.623489 , 0.635614 , 0.647447 , 0.659217 , 0.670986 , 0.682569 , 0.694095 , 0.705365 , 0.716531 , 0.727596 , 0.738541 , 0.749257 , 0.759854 , 0.770301 , 0.780496 , 0.79058 , 0.800566 , 0.810375 , 0.819982 , 0.829254 , 0.83841 , 0.847217 , 0.855921 , 0.864361 , 0.872515 , 0.880417 , 0.888186 , 0.895579 , 0.902743 , 0.909667 , 0.916207 , 0.922479 , 0.928446 , 0.934119 , 0.939461 , 0.944602 , 0.949443 , 0.953955 , 0.958241 , 0.962226 , 0.965879 , 0.969266 , 0.972461 , 0.975374 , 0.978038 , 0.980489 , 0.982726 , 0.984764 , 0.98661 , 0.988251 , 0.989745 , 0.991091 , 0.992263 , 0.99332 , 0.994231 , 0.995063 , 0.995757 , 0.996384 , 0.996926 , 0.997372 , 0.997761 , 0.998092 , 0.998357 , 0.998605 , 0.998797 , 0.998955 , 0.999074 , 0.999178 , 1.0};
  double dP[nBins]={0.99694 , 0.99698 , 0.99702 , 0.99706 , 0.9971 , 0.99714 , 0.99718 , 0.99722 , 0.99726 , 0.9973 , 0.99734 , 0.99738 , 0.99742 , 0.99746 , 0.9975 , 0.99754 , 0.99758 , 0.99762 , 0.99766 , 0.9977 , 0.99774 , 0.99778 , 0.99782 , 0.99786 , 0.9979 , 0.99794 , 0.99798 , 0.99802 , 0.99806 , 0.9981 , 0.99814 , 0.99818 , 0.99822 , 0.99826 , 0.9983 , 0.99834 , 0.99838 , 0.99842 , 0.99846 , 0.9985 , 0.99854 , 0.99858 , 0.99862 , 0.99866 , 0.9987 , 0.99874 , 0.99878 , 0.99882 , 0.99886 , 0.9989 , 0.99894 , 0.99898 , 0.99902 , 0.99906 , 0.9991 , 0.99914 , 0.99918 , 0.99922 , 0.99926 , 0.9993 , 0.99934 , 0.99938 , 0.99942 , 0.99946 , 0.9995 , 0.99954 , 0.99958 , 0.99962 , 0.99966 , 0.9997 , 0.99974 , 0.99978 , 0.99982 , 0.99986 , 0.9999 , 0.99994 , 0.99998 , 1.00002 , 1.00006 , 1.0001 , 1.00014 , 1.00018 , 1.00022 , 1.00026 , 1.0003 , 1.00034 , 1.00038 , 1.00042 , 1.00046 , 1.0005 , 1.00054 , 1.00058 , 1.00062 , 1.00066 , 1.0007 , 1.00074 , 1.00078 , 1.00082 , 1.00086 , 1.0009 , 1.00094 , 1.00098 , 1.00102 , 1.00106 , 1.0011 , 1.00114 , 1.00118 , 1.00122 , 1.00126 , 1.0013 , 1.00134 , 1.00138 , 1.00142 , 1.00146 , 1.0015 , 1.00154 , 1.00158 , 1.00162 , 1.00166 , 1.0017 , 1.00174 , 1.00178 , 1.00182 , 1.00186 , 1.0019 , 1.00194 , 1.00198 , 1.00202 , 1.00206 , 1.0021 , 1.00214 , 1.00218 , 1.00222 , 1.00226 , 1.0023 , 1.00234 , 1.00238 , 1.00242 , 1.00246 , 1.0025 , 1.00254 , 1.00258 , 1.00262 , 1.00266 , 1.0027 , 1.00274 , 1.00278 , 1.00282 , 1.00286 , 1.0029 , 1.00294 , 1.00298 , 1.00302 , 1.00306 , 1.0031};




//   static G4int const nBins = 38;
//   double cdf[nBins]={0.0 , 0.0015626 , 0.00426483 , 0.00899726 , 0.0161335 , 0.0266487 , 0.0408601 , 0.0594256 , 0.0826554 , 0.110629 , 0.142586 , 0.17847 , 0.21729 , 0.259452 , 0.304861 , 0.351347 , 0.399728 , 0.44885 , 0.498562 , 0.548034 , 0.597903 , 0.646066 , 0.692533 , 0.736575 , 0.778673 , 0.818133 , 0.854371 , 0.886704 , 0.914809 , 0.938267 , 0.957138 , 0.971535 , 0.981782 , 0.988829 , 0.993449 , 0.996165 , 0.997601 , 1.0};
//   double dP[nBins]={0.99704 , 0.9972 , 0.99736 , 0.99752 , 0.99768 , 0.99784 , 0.998 , 0.99816 , 0.99832 , 0.99848 , 0.99864 , 0.9988 , 0.99896 , 0.99912 , 0.99928 , 0.99944 , 0.9996 , 0.99976 , 0.99992 , 1.00008 , 1.00024 , 1.0004 , 1.00056 , 1.00072 , 1.00088 , 1.00104 , 1.0012 , 1.00136 , 1.00152 , 1.00168 , 1.00184 , 1.002 , 1.00216 , 1.00232 , 1.00248 , 1.00264 , 1.0028 , 1.00296};
  
  G4double x = G4UniformRand();
  for ( int j = 0; j < nBins; j++ ) { 
    if ( x < cdf[j] ) { 
      h_mom_rand->Fill(dP[j], x);
      return( dP[j] );
    }
  }

  G4cout << "No random dP/P generated. Returning 0.0" << G4endl;
  return( -1.0 );
}

double gm2ringsim::G2BeamTransportSource::GenerateRandomXe()
{   
  if ( StartPerfect_ || 1 ) {
    static G4int const nBinsXe = 121;
    double cdf_xe[nBinsXe]={-59.5041, -58.5124, -57.5207, -56.5289, -55.5372, -54.5455, -53.5537, -52.562, -51.5702, -50.5785, -49.5868, -48.595, -47.6033, -46.6116, -45.6198, -44.6281, -43.6364, -42.6446, -41.6529, -40.6612, -39.6694, -38.6777, -37.686, -36.6942, -35.7025, -34.7107, -33.719, -32.7273, -31.7355, -30.7438, -29.7521, -28.7603, -27.7686, -26.7769, -25.7851, -24.7934, -23.8017, -22.8099, -21.8182, -20.8264, -19.8347, -18.843, -17.8512, -16.8595, -15.8678, -14.876, -13.8843, -12.8926, -11.9008, -10.9091, -9.91736, -8.92562, -7.93388, -6.94215, -5.95041, -4.95868, -3.96694, -2.97521, -1.98347, -0.991736, 7.77156e-16, 0.991736, 1.98347, 2.97521, 3.96694, 4.95868, 5.95041, 6.94215, 7.93388, 8.92562, 9.91736, 10.9091, 11.9008, 12.8926, 13.8843, 14.876, 15.8678, 16.8595, 17.8512, 18.843, 19.8347, 20.8264, 21.8182, 22.8099, 23.8017, 24.7934, 25.7851, 26.7769, 27.7686, 28.7603, 29.7521, 30.7438, 31.7355, 32.7273, 33.719, 34.7107, 35.7025, 36.6942, 37.686, 38.6777, 39.6694, 40.6612, 41.6529, 42.6446, 43.6364, 44.6281, 45.6198, 46.6116, 47.6033, 48.595, 49.5868, 50.5785, 51.5702, 52.562, 53.5537, 54.5455, 55.5372, 56.5289, 57.5207, 58.5124, 59.5041};
    double cdf[nBinsXe]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7.30146e-07, 7.30146e-07, 7.30146e-07, 7.30146e-07, 7.30146e-07, 1.46029e-06, 2.92058e-06, 2.92058e-06, 4.38088e-06, 1.24125e-05, 2.70154e-05, 3.79676e-05, 6.64433e-05, 0.00010149, 0.000153331, 0.00020225, 0.000243869, 0.000327836, 0.000514753, 0.000933127, 0.00171657, 0.00301039, 0.00509423, 0.00809951, 0.0122905, 0.018185, 0.0259202, 0.0358268, 0.0480714, 0.0630174, 0.0806987, 0.100697, 0.123422, 0.148261, 0.175247, 0.203909, 0.234527, 0.266645, 0.300005, 0.334682, 0.370465, 0.406999, 0.444027, 0.481591, 0.519049, 0.556434, 0.593457, 0.629985, 0.665817, 0.700588, 0.734268, 0.766471, 0.79691, 0.825808, 0.852822, 0.877582, 0.900338, 0.920582, 0.938153, 0.95301, 0.965197, 0.974973, 0.982513, 0.988278, 0.992439, 0.995394, 0.997357, 0.998585, 0.9993, 0.999711, 0.999895, 0.999965, 0.999984, 0.99999, 0.999993, 0.999996, 0.999997, 0.999998, 0.999999, 0.999999, 0.999999, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    
    G4double x = G4UniformRand();
    for ( int j = 0; j < nBinsXe; j++ ) { 
      if ( x < cdf[j] ) {
	return( cdf_xe[j] );
      }
    }
    
  }
  else {
    //return( 10*G4UniformRand() - 5 );
    static G4int const nBinsXe = 121;
    double cdf_xe[nBinsXe]={-59.5041, -58.5124, -57.5207, -56.5289, -55.5372, -54.5455, -53.5537, -52.562, -51.5702, -50.5785, -49.5868, -48.595, -47.6033, -46.6116, -45.6198, -44.6281, -43.6364, -42.6446, -41.6529, -40.6612, -39.6694, -38.6777, -37.686, -36.6942, -35.7025, -34.7107, -33.719, -32.7273, -31.7355, -30.7438, -29.7521, -28.7603, -27.7686, -26.7769, -25.7851, -24.7934, -23.8017, -22.8099, -21.8182, -20.8264, -19.8347, -18.843, -17.8512, -16.8595, -15.8678, -14.876, -13.8843, -12.8926, -11.9008, -10.9091, -9.91736, -8.92562, -7.93388, -6.94215, -5.95041, -4.95868, -3.96694, -2.97521, -1.98347, -0.991736, 7.77156e-16, 0.991736, 1.98347, 2.97521, 3.96694, 4.95868, 5.95041, 6.94215, 7.93388, 8.92562, 9.91736, 10.9091, 11.9008, 12.8926, 13.8843, 14.876, 15.8678, 16.8595, 17.8512, 18.843, 19.8347, 20.8264, 21.8182, 22.8099, 23.8017, 24.7934, 25.7851, 26.7769, 27.7686, 28.7603, 29.7521, 30.7438, 31.7355, 32.7273, 33.719, 34.7107, 35.7025, 36.6942, 37.686, 38.6777, 39.6694, 40.6612, 41.6529, 42.6446, 43.6364, 44.6281, 45.6198, 46.6116, 47.6033, 48.595, 49.5868, 50.5785, 51.5702, 52.562, 53.5537, 54.5455, 55.5372, 56.5289, 57.5207, 58.5124, 59.5041};
    double cdf[nBinsXe]={6.38515e-05, 6.38515e-05, 6.38515e-05, 6.38515e-05, 6.38515e-05, 6.38515e-05, 6.38515e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 8.51354e-05, 0.000106419, 0.000148987, 0.000255406, 0.00027669, 0.000383109, 0.000617231, 0.00129831, 0.00202196, 0.00338413, 0.0050017, 0.00734293, 0.010876, 0.0149625, 0.0205602, 0.0272646, 0.035991, 0.0459731, 0.0588924, 0.0746424, 0.0914992, 0.110442, 0.131364, 0.155585, 0.180019, 0.206709, 0.236655, 0.267389, 0.299485, 0.333582, 0.369402, 0.408841, 0.44728, 0.486549, 0.524647, 0.562234, 0.600651, 0.639111, 0.677103, 0.712838, 0.747552, 0.78233, 0.813617, 0.843691, 0.870679, 0.894879, 0.916269, 0.933424, 0.948429, 0.961029, 0.971437, 0.978759, 0.984995, 0.989486, 0.992849, 0.995381, 0.997105, 0.998233, 0.998915, 0.999383, 0.999745, 0.999957, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    
    G4double x = G4UniformRand();
    for ( int j = 0; j < nBinsXe; j++ ) { 
      if ( x < cdf[j] ) {
	return( cdf_xe[j] );
      }
    }
  }

  return( -99.9 );
}
