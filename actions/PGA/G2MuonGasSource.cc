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

#include "gm2ringsim/actions/PGA/G2MuonGasSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"

#define randFromDistr CLHEP::RandGeneral

enum {kX, kY};

#include <iostream>
#include <cmath>

#include "TMath.h"
#include "TH1.h"


Double_t CBO(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*sin(TMath::TwoPi()*par[2]*xx + par[3]);
  return f;
}

Double_t betaFunction(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*sin(par[2]*xx + par[3]);
  return f;
}

Double_t SingleTurnPhaseAdvance(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Double_t f = par[0] + par[1]*xx;
  return f;
}

using std::endl;

// Constructor
gm2ringsim::G2MuonGasSource::G2MuonGasSource() :
  muonGasGun_(),
  g2GPS_(),
  infGeom_("inflector"),
  muonGasVerbosity_ (0),
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
  UseConstBeta(true),
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
  muonGasGun_ = new G4ParticleGun();
  g2GPS_ = new G2GeneralParticleSource();

  gRandom_ = new TRandom3();

  fsz   = new TF1("fsz", "-(3.68923)+(1.48959*x)", 3.06, 3.13);
  fsphi = new TF1("fsphi", "(((1788.72-(196.394*cos(x)))-(321.191*cos(2*x)))+(68.1887*cos(3*x)))+(((587.76*cos(4*x))-(94.1914*cos(5*x)))-(106.777*cos(6*x)))", -3.14159, 3.14159);

  
  // These are for +/-24 kV on the quads
  fBetaX = new TF1("mybetaX",betaFunction,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaX->SetParameter(0, 7.67067);
  fBetaX->SetParameter(1, 0.247307);
  fBetaX->SetParameter(2, 0.562465);
  fBetaX->SetParameter(3, 1.56706);
  
  fBetaY = new TF1("mybetaY",betaFunction,0,TMath::TwoPi()*R_magic()/1000,4);
  fBetaY->SetParameter(0, 18.8466);
  fBetaY->SetParameter(1, 0.496336);
  fBetaY->SetParameter(2, 0.562772);
  fBetaY->SetParameter(3, -1.58141);

  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceX = new TF1("myPhaseAdvanceX",SingleTurnPhaseAdvance,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceX->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceX->SetParameter(1, 0.130514);

  // Phase Advance = integral(1/beta)_{0}^{smax}
  fSingleTurnPhaseAdvanceY = new TF1("myPhaseAdvanceY",SingleTurnPhaseAdvance,0,TMath::TwoPi()*R_magic()/1000,2);
  fSingleTurnPhaseAdvanceY->SetParameter(0, 0.0);
  fSingleTurnPhaseAdvanceY->SetParameter(1, 0.0530519);


  fCBO = NULL;

  double timeperturn = 0.14915;
  int maxturns_ = 21;
  double maxt = maxturns_* timeperturn;
  int numhitsperturn = 150;


  if ( 1 ) {
    f = new TFile("muongas.root", "RECREATE");
  }
  h_Ax = new TH1F("hAx", "", 450, 0, 45.0*mm);
  h_Ay = new TH1F("hAy", "", 450, 0, 45.0*mm);
  h_pol = new TH1F("hpol", "", 100, -1, 1);
  h_time_pol = new TH2F("htime_pol", "", maxturns_*numhitsperturn, 0.0, maxt, 100, -1, 1);
  h_xe = new TH1F("hx", "", 900, -45.0*mm, 45.0*mm);
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

gm2ringsim::G2MuonGasSource::~G2MuonGasSource() 
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
  if ( 1 ) {
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

void gm2ringsim::G2MuonGasSource::SetVerbosity(bool verbose){
  muonGasVerbosity_ = verbose;
}

void gm2ringsim::G2MuonGasSource::SetDebug(bool debug){
  debug_ = debug;
}

double gm2ringsim::G2MuonGasSource::CBOValue(double time)
{
  if ( fCBO == NULL ) {
    double maxt = 10000;
    fCBO = new TF1("mybetaY",CBO,0, maxt, 4);

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

double gm2ringsim::G2MuonGasSource::SingleTurnArcLength(double arclength)
{
  G4double maxangle = TMath::TwoPi() * R_magic();
  G4double decayarclength = std::fmod(arclength, maxangle);

  return( decayarclength );
}

double gm2ringsim::G2MuonGasSource::PhaseAdvance(int var, double arclength)
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


double gm2ringsim::G2MuonGasSource::Beta(int var, double arclength)
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

double gm2ringsim::G2MuonGasSource::Alpha(int var, double arclength)
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

void gm2ringsim::G2MuonGasSource::Transport(double *newVar, double *newVarprime, int var, double arclength, double s0)
{
  G4double oldVar = *newVar;
  G4double oldVarprime = *newVarprime;

  G4double beta, alpha;
  G4double beta0, alpha0;
  G4double PsiXs;

  beta = Beta(var, arclength);
  alpha = Alpha(var, arclength);
  beta0 = Beta(var, s0);
  alpha0 = Alpha(var, s0);
  PsiXs = PhaseAdvance(var, arclength);


  // Compute phase advance
  G4double M00, M01, M10, M11;
  M00 = TMath::Sqrt(beta/beta0)*TMath::Cos(PsiXs) + alpha0*TMath::Sin(PsiXs);
  M01 = TMath::Sqrt(beta*beta0)*TMath::Sin(PsiXs);
  M10 = ((alpha0 - alpha)*TMath::Cos(PsiXs) - (1+alpha0*alpha)*TMath::Sin(PsiXs))/(TMath::Sqrt(beta*beta0));
  M11 = TMath::Sqrt(beta0/beta)*TMath::Cos(PsiXs) - alpha*TMath::Sin(PsiXs);
  
  bool debug = false;
  if ( debug ) {
    G4cout << "=== Transport ===" << G4endl;
    G4cout << "Betas\t" << beta << "\t" << alpha << "\t" << beta0 << "\t" << alpha0 << G4endl;
    G4cout << "Ms\t" << M00 << "\t" << M01 << "\t" << M10 << "\t" << M11 << G4endl;
  }

  *newVar      = M00*oldVar + M01*oldVarprime;
  *newVarprime = M10*oldVar + M11*oldVarprime;  

  if ( debug ) {
    G4cout << "Calc: " << G4endl;
    G4cout << "  " << oldVar << "\t" << oldVarprime << G4endl;
    G4cout << "  " << M00*oldVar << "\t" << M01*oldVarprime << G4endl;
  }
}

// EndOfPrimaryGeneratorAction
void gm2ringsim::G2MuonGasSource::GeneratePrimaryVertex(G4Event* evt) {

  mf::LogInfo("G2MuonGasSource") << "GeneratePrimaryVertex";
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
      muonGasGun_ ->SetParticleDefinition(def);
    }
  }
  else {
    G4ParticleTable::GetParticleTable()->DumpTable();
    G4cout << "Could not find Particle Info for [" << Particle_ << "]" << G4endl;
    exit(1);
  }
    


  
//   if( firstVertex ){
//     G4String defaultParticle = "mu+";
//     G4String particle = g2GPS_ ->GetParticleDefinition()->GetParticleName();
    
//     if( g2GPS_ ->GetParticleDefinition()!=NULL && particle!=defaultParticle ){
//       muonGasGun_ ->SetParticleDefinition( g2GPS_ ->GetParticleDefinition() );
//     } else {
//       muonGasGun_ ->SetParticleDefinition( G4ParticleTable::GetParticleTable()->FindParticle(defaultParticle) );
//     }
    
//     // Store PDGLifeTime and then define it to be 0 so we can decay the muons manually
//     particleLifetime = muonGasGun_ ->GetParticleDefinition()->GetPDGLifeTime();
//     muonGasGun_ ->GetParticleDefinition()->SetPDGLifeTime(0.);
    
//     firstVertex = false;
//   }
  
  //  First, generate an initial transverse position in local storage-region (SR) coordinates.     
  //  The coordinate system used here is the standard one found in accelerator physics texts,
  //  i.e. "z" = longitudinal, "x" = radial, and "y" = vertical.
  
  G4double randX_SR = 0.*mm; // radial
  G4double randY_SR = 0.*mm; // vertical     
  
  G4double xe = GenerateRandomXe()*mm;
  G4double EqRad = R_magic() + xe;
  G4double n = 0.137;
  G4double mom = xe*P_magic()*(1-n)/R_magic() + P_magic();
  G4double mugamma = Gamma(mom);

  
  //FIXME: Is using G4RandGuass::shoot in this manner problematic?
  // see: http://mu2e.fnal.gov/public/hep/computing/Random.shtml#fireShoot
  // BK, Nov 2012
  
  // see https://muon.npl.washington.edu/elog/g2/Simulation/85 for explanation
  double E821_SingleGaussianWidth = 9.18770675965942;
  
  if( g2GPS_->GetMuonGasGunTransverseDistr()=="e989" ){
    mf::LogInfo("PGA") << "TRANSVERSE PROFILE: E989";
    G4double randRho_SR;
    do{
      randX_SR = G4RandGauss::shoot( 0., E821_SingleGaussianWidth )*mm;
      randY_SR = G4RandGauss::shoot( 0., E821_SingleGaussianWidth )*mm;
      randRho_SR = std::sqrt( randX_SR*randX_SR + randY_SR*randY_SR );
    } while( randRho_SR>45. );
    
  } else if ( g2GPS_->GetMuonGasGunTransverseDistr()=="e821" ){
    mf::LogInfo("PGA") << "TRANSVERSE PROFILE: E821";
    G4double randRho_SR;
    do{
      randX_SR = randXFromE821();
      randY_SR = G4RandGauss::shoot( 0., E821_SingleGaussianWidth )*mm;
      randRho_SR = std::sqrt( randX_SR*randX_SR + randY_SR*randY_SR );
    } while( randRho_SR>45. );
    
  } else if ( g2GPS_->GetMuonGasGunTransverseDistr()=="uniform" ){
    mf::LogInfo("PGA") << "TRANSVERSE PROFILE: UNIFORM";
    G4double randRho_SR;
    do{
      randX_SR = ( G4UniformRand()-0.5 )*90.*mm;
      randY_SR = ( G4UniformRand()-0.5 )*90.*mm;
      randRho_SR = std::sqrt( randX_SR*randX_SR + randY_SR*randY_SR );
    } while( randRho_SR>45. );
    
  } else if ( g2GPS_->GetMuonGasGunTransverseDistr()=="point" ){
    mf::LogInfo("PGA") << "TRANSVERSE PROFILE: POINT";
    
    randX_SR = 0.*mm;
    randY_SR = 0.*mm;
	
  }
  else {
    mf::LogInfo("PGA") << "NO TRANSVERSE PROFILE SPECIFIED.\n"
  		       << "Using default values for randX_SR:"<<randX_SR
  		       << " and randY_SR:"<<randY_SR;
    
  }

  //  Initialize {position, momentum, spin} vectors at 12 o'clock in standard global Cartesian
  //  coordinates.  NB: In a muonGas the momentum only depends on the radial coordinate.


  
  G4double rX = 0.;
  G4double rY = R_magic()+randX_SR; // 12 o'clock 
  G4double rZ = randY_SR;  //0.;
  G4ThreeVector r = G4ThreeVector(rX,rY,rZ);
  
  G4double pX = eplus*c_light*B_magic()*( R_magic()+randX_SR ); // downstream
  G4double pY = 0.;
  G4double pZ = 0.;
  G4ThreeVector p = G4ThreeVector(pX,pY,pZ);
  
  
  G4double sX = 1.; // downstream
  G4double sY = 0.;
  G4double sZ = 0.;
  G4ThreeVector s = G4ThreeVector(sX,sY,sZ);
  
  
  //  Randomly generate an initial longitudinal position, which, for the muonGas, 
  //  is equivalent to generating an initial time offset. 
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

  // Direction of inflector axis
  G4ThreeVector dir = (downstream - upstream).unit();


  G4ThreeVector storage_orbit_start(R_magic(), 0., 0.);
  //storage_orbit_start.rotateY(infGeom_.rotAngle);
  G4ThreeVector beam_start = storage_orbit_start;
  //if ( GetStartPerfect() ) {
  //beam_start.rotateY(RotAngle_);
  //}
  //beam_start.rotateY(-kicker_phaseoffset);
  G4ThreeVector diff = beam_start-downstream;
  G4double sign = 1.;
  if( dir.cosTheta(diff) < 0 ) {
    sign = -1.;
  }
  randT = 100.0*ns + sign*diff.mag()/(2.*pi*R_magic()/150.)*ns;
  if ( 0 ) {
    G4cout << diff << "\t" << beam_start << "\t";
    G4cout << "RandT(1) = " << randT << "\t";
    //randT = kicker_timeoffset + 100*ns;
    G4cout << "RandT(2) = " << randT << G4endl;
  }
  h_t0off->Fill(randT);
  
  if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="e989" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E989";
    randT = randTFromE989() + 100*ns;
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="e821" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E821";
    do{
      randT = G4RandGauss::shoot( 0., 25. )*ns;
    } while( abs(randT)>50. );
    randT += -50.;
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="uniform" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: UNIFORM";
    //randT = ( G4UniformRand()-1. )*100.*ns;
    double sigmat = 1;
    randT += ((2*G4UniformRand()-1) * sigmat)*ns;
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="uniformRing" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: UNIFORMRING";
    randT = ( G4UniformRand()*CLHEP::twopi*R_magic()/(betaMagic()*c_light) );
    
  }
  else {
    mf::LogInfo("PGA") << "NO LOGITUDINAL PROFILE SPECIFIED.\n"
  		       << "Using default value for randT:"<<randT;
  }
  
  
  //  Generate a random decay time by inverting Exp[ -t/gamma*tau ], and add it to the random tInitial from above.
  G4double randNum = 0.0;
  G4double newTime = GetMaxDecayTime() + 0.0001*ns;
  G4double decaytime = 0.0;
  t0 = randT;
  if ( debug_ ) {
    G4cout << "MaxTime:        " << GetMaxDecayTime() << G4endl;
    G4cout << "Rand FNAL time: " << randT << G4endl;
  }
  
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
  if ( debug_ ) { 
    G4cout << "Final NewTime = " << newTime << G4endl;
  }
  
  G4double totaltime = decaytime + t0;





  
  
  //  The cyclotron angular frequency, omegaC, depends on the longitudinal momentum => fast rotation.
  G4double gamma  = std::sqrt( 1 + (pX/mMuon())*(pX/mMuon()) );
  G4double beta   = std::sqrt( 1 - 1/(gamma*gamma) );
  G4double omegaC = beta*c_light/rY; // rad/ns 
  
  
  //  Given mu+/+, and assuming betaCrossE vanishes, the angular precession frequency depends only on the strength of the magnetic field 
  //  Determine the clockwise angles by which to rotate the {position,momentum,spin}.
  G4double psiCRand = omegaC*randT;        // cyclotron 
  G4double psiARand = omegaAMagic()*randT; // anomaly                                                  
  G4double psiSRand = psiARand + psiCRand; // spin       
  
  //  Take into accound the position of the inflector exit in g2MIGTRACE (@Matthias Smith, 23 AUG '12)            
  G4double inflectorAngle = -5.33137595152413*deg;
  
  psiCRand += inflectorAngle;
  psiARand += inflectorAngle;
  psiSRand += inflectorAngle;
  
  // Rotate the {position, momentum, spin} vectors clockwise by the appropriate amount.
  r.rotateZ( -psiCRand );
  p.rotateZ( -psiCRand );
  s.rotateZ( -psiSRand );
  
  
  // Transform {position,momentum,spin} back to g2MIGTRACE coordinates.
  G4ThreeVector g2r = G4ThreeVector( r.y(), r.z(), r.x() );
  G4ThreeVector g2p = G4ThreeVector( p.y(), p.z(), p.x() );
  G4ThreeVector g2s = G4ThreeVector( s.y(), s.z(), s.x() );





  //-------------------------------------------
  // Compute omega_{c} , pos/mom angle = omega_{c} * t - inflector offset
  //-------------------------------------------
  G4double s0 = kicker_phaseoffset * EqRad;
  omegaC = ComputeOmegaCGivenMomRadius(mom, EqRad);
  G4double radianlength = omegaC * decaytime + kicker_phaseoffset;
  G4double arclength = radianlength * EqRad;
  G4double psi = radianlength;


  //-------------------------------------------
  // Compute betaX, betaY @ decay point
  //-------------------------------------------
  G4double decaybetaX = Beta(kX, arclength);
  G4double decaybetaY = Beta(kY, arclength);
  G4double decayalphaX = Alpha(kX, arclength);
  G4double decayalphaY = Alpha(kY, arclength);
  
  if ( debug_ ) {
    G4cout << "t*c*beta      = " << arclength << endl;
    G4cout << "BetaX         = " << decaybetaX << G4endl;
    G4cout << "BetaY         = " << decaybetaY << G4endl;
    G4cout << "AlphaX        = " << decayalphaX << G4endl;
    G4cout << "AlphaY        = " << decayalphaY << G4endl;
  }

  
  

  //====================================================================//
  //=========================   INFLECTOR GUN   ========================//
  //====================================================================//
  G4double epsilonX, betaX, alphaX, gammaX;
  G4double epsilonY, betaY, alphaY, gammaY;
  G4double x0, y0, x0Prime, y0Prime;
  G4double emittanceX, emittanceY;
  G4double nux = sqrt(1-n);
  G4double nuy = sqrt(n);
  emittanceX = GetEmittanceX();
  emittanceY = GetEmittanceY();
  if ( decaybetaX > 0 ) {
    betaX = decaybetaX;
    alphaX = decayalphaX;
    nux = R_magic() / betaX;
  }
  else {
    betaX = GetBetaX();
    alphaX = GetAlphaX();
    nux = R_magic() / betaX;
  }

  if ( decaybetaY > 0 ) {
    betaY = decaybetaY;
    alphaY = decayalphaY;
    nuy = R_magic() / betaY;
  }
  else {
    betaY = GetBetaY();
    alphaY = GetAlphaY();
    nuy = R_magic() / betaY;
  }

  epsilonX       = emittanceX;
  epsilonY       = emittanceY;
  
  if ( debug_ ) {
    G4cout << "EmitX         = " << emittanceX << G4endl;
    G4cout << "EmitY         = " << emittanceY << G4endl;
    G4cout << "EpsilonX      = " << epsilonX << G4endl;
    G4cout << "EpsilonY      = " << epsilonY << G4endl;
    G4cout << "BetaX         = " << betaX << G4endl;
    G4cout << "BetaY         = " << betaY << G4endl;
    G4cout << "(BetaX)       = " << GetBetaX() << G4endl;
    G4cout << "(BetaY)       = " << GetBetaY() << G4endl;
    G4cout << "AlphaX        = " << alphaX << G4endl;
    G4cout << "AlphaY        = " << alphaY << G4endl;
  }
      


  //  First, calculate gamma from the user's parameters using the fact that beta*gamma - alpha^2 = 1 is invariant
  gammaX = ( 1+alphaX*alphaX )/betaX;
  gammaY = ( 1+alphaY*alphaY )/betaY;

  //  Next, use the above parameters to determine the point s{X,Y} at which the phase-space ellipses have alpha{X,Y} = 0.
  //  This serves to define what I will call the "naught" positions, i.e. the positions at which the phase-space ellipses
  //  are standing "upright" in phase space (alpha{X,Y}=0).  It is very easy to generate random positions/momenta at the
  //  naught coordinates, which can then easily be propagted back to the injection point.
  G4double s0X = -alphaX/gammaX;
  G4double s0Y = -alphaY/gammaY;


  //  Calculate beta{X,Y}0
  G4double betaX0 = betaX + 2*alphaX*s0X + gammaX*s0X*s0X;
  G4double betaY0 = betaY + 2*alphaY*s0Y + gammaY*s0Y*s0Y;


  //  Calculate gamma{X,Y}0 given that alpha{X,Y}=0 using the invariant (see above).
  G4double gammaX0 = 1/betaX0;
  G4double gammaY0 = 1/betaY0;

  G4double MaxAx = std::sqrt( epsilonX*betaX0 );
  G4double MaxAy = std::sqrt( epsilonY*betaY0 );
  G4double MaxXprime = std::sqrt( epsilonX*gammaX0 );
  G4double MaxYprime = std::sqrt( epsilonY*gammaY0 );

  if ( debug_ ) {
    G4cout << "  sX, sY                  = " << s0X << " , " << s0Y << G4endl;
    G4cout << "  alpha, beta, gamma (x0) = " << alphaX << " , " << betaX0 << " , " << gammaX0 << G4endl;
    G4cout << "  alpha, beta, gamma (y0) = " << alphaY << " , " << betaY0 << " , " << gammaY0 << G4endl;
    G4cout << "  emit (X,Y)              = " << epsilonX << " , " << epsilonY << G4endl;
    G4cout << "  Max(Ax)                 = " << epsilonX * betaX0 << " , " << MaxAx << G4endl;
    G4cout << "  Max(Ay)                 = " << epsilonY * betaY0 << " , " << MaxAy << G4endl;
  }

  // Generate random equilibrium radius from expected distribution
  //
  // Xe = (p-p0)/p0 x (1/(1-n)) x Rmagic
  //
  G4double Ax = MaxAx;
  G4double Ay = MaxAy;

  if ( debug_ ) {
    G4cout << "t*c*beta      = " << arclength << endl;
    G4cout << "   OmegaC(" << mom/P_magic() << " , " << EqRad <<" ) = " << ComputeOmegaCGivenMomRadius(mom, EqRad) << G4endl;
    G4cout << "   OmegaC(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaCGivenMomRadius(P_magic(), R_magic()) << G4endl;
    G4cout << "   t*OmegaC(" << mom/P_magic() << " , " << EqRad <<" ) = " << ComputeOmegaCGivenMomRadius(mom, EqRad)*decaytime << G4endl;
    G4cout << "   t*OmegaC(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaCGivenMomRadius(P_magic(), R_magic())*decaytime << G4endl;
    G4cout << "   149ns*OmegaC(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaCGivenMomRadius(P_magic(), R_magic())*149.15*ns << G4endl;
    G4cout << "   14.5*149ns*OmegaC(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaCGivenMomRadius(P_magic(), R_magic())*14.5*149.15*ns << G4endl;
  }


  //-------------------------------------------
  // Compute omega_{s} , spin angle = omega_{s} * t
  //-------------------------------------------
  G4double omegaS = ComputeOmegaSGivenMomRadius(mom, EqRad);
  G4double radianlength_spin = omegaS*decaytime + kicker_phaseoffset;
  //G4double arclength_spin = radianlength * EqRad;
  G4double psi_spin = radianlength_spin;

  if ( debug_ ) {
    G4cout << "    t*OmegaS(" << mom/P_magic() << " , " << EqRad <<" ) = " << ComputeOmegaSGivenMomRadius(mom, EqRad)*decaytime << G4endl;
    G4cout << "    t*OmegaS(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaSGivenMomRadius(P_magic(), R_magic())*decaytime << G4endl;
    G4cout << "    149ns*OmegaS(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaSGivenMomRadius(P_magic(), R_magic())*149.15*ns << G4endl;
    G4cout << "    14*149ns*OmegaS(" << P_magic()/P_magic() << " , " << R_magic() <<" ) = " << ComputeOmegaSGivenMomRadius(P_magic(), R_magic())*14.5*149.15*ns << G4endl;
  }

  
  //first_event = false;
  //return;

  if ( debug_ ) {
    G4cout.precision(5);
    G4cout << endl;
    G4cout << "Xe            = " << xe << G4endl;
    G4cout << "<R>           = " << EqRad << G4endl;
    G4cout << "T             = " << decaytime << G4endl;
    G4cout << "Gamma         = " << mugamma << G4endl;
    G4cout << "Pz            = " << mom << G4endl;
    G4cout << "Mom(gamma)    = " << Mom(mugamma) << G4endl;
    G4cout << "R(gamma, n)   = " << R(mugamma, n) << G4endl;
    G4cout << "R(gammaM, n)  = " << R(gammaMagic(), n) << G4endl;
    G4cout << "s/(2pi*<R>)   = " << arclength / (TMath::TwoPi()*EqRad) << G4endl;
    G4cout << "Psi           = " << psi << G4endl;
  }

  G4double argX = nux * psi + TMath::Pi();
  G4double argY = nuy * psi;
  //G4double argX = nux * psi + alphaX;
  //G4double argY = nuy * psi + alphaY;

  G4double cosX = TMath::Cos(argX);
  G4double cosY = TMath::Cos(argY);

  G4double dxds = - nux/EqRad * TMath::Sin(argX);
  G4double dyds = - nuy/EqRad * TMath::Sin(argY);

  if ( debug_ ) {
    G4cout << "Time          = " << decaytime << G4endl;
    G4cout << "psi           = " << psi << G4endl;
    G4cout << "ArgX          = " << argX << G4endl;
    G4cout << "ArgY          = " << argY << G4endl;
    G4cout << "cosX          = " << cosX << G4endl;
    G4cout << "cosY          = " << cosY << G4endl;
    G4cout << "cosX'         = " << dxds << G4endl;
    G4cout << "cosY'         = " << dyds << G4endl;
    G4cout << "MaxAx         = " << MaxAx << G4endl;
    G4cout << "MaxAy         = " << MaxAy << G4endl;
  }
  
  bool inside = false;
  if ( epsilonX > 0.0 ) {
    while ( inside == false ) {
      x0 = MaxAx*(1*G4UniformRand()-0);
      if ( StartPerfect_ ) { x0 = MaxAx*(2*G4UniformRand()-1); }
      if ( PosOffset_ ) { x0 = MaxAx*0.99; }
      x0Prime = MaxXprime*(2*G4UniformRand()-1);
      if ( NoTransMom_ ) { x0Prime = 0.0; }

      if ( debug_ ) {
	G4cout << "  [x,x']        = " << x0 << " , " << x0Prime << G4endl;
	G4cout << "  (x'/|x'|)^{2} = " <<  TMath::Power(x0Prime/MaxXprime, 2) << G4endl;
	G4cout << "  (x/|x|)^{2}   = " << TMath::Power(x0/MaxAx, 2) << G4endl;
	G4cout << "  Sum           = " << TMath::Power(x0Prime/MaxXprime, 2) + TMath::Power(x0/MaxAx, 2) << G4endl;
      }
      
      if ( TMath::Power(x0Prime/MaxXprime, 2) + TMath::Power(x0/MaxAx, 2) < 1 ) { inside = true; }
    }
  }
  else {
    x0 = 0.0;
    x0Prime = 0.0;
  }

  if ( debug_ ) {
    G4cout << "Ax            = " << Ax << G4endl;
    G4cout << "x             = " << x0 << G4endl;
    G4cout << "x'            = " << x0Prime << G4endl;
  }
  

  inside = false;
  if ( epsilonY > 0.0 ) {
    while ( inside == false ) {
      y0 = MaxAy*(2*G4UniformRand()-1);
      if ( PosOffset_ ) { y0 = MaxAy*0.99; }
      y0Prime = MaxYprime*(2*G4UniformRand()-1);
      if ( NoTransMom_ ) { y0Prime = 0.0; }

      if ( debug_ ) {
	G4cout << "  MaxAy, Ay     = " << MaxAy << " , " << Ay << G4endl;
	G4cout << "  [y,y']        = " << y0 << " , " << y0Prime << G4endl;
	G4cout << "  (y'/|y'|)^{2} = " <<  TMath::Power(y0Prime/MaxYprime, 2) << G4endl;
	G4cout << "  (y/|y|)^{2}   = " << TMath::Power(y0/MaxAy, 2) << G4endl;
	G4cout << "  Sum           = " << TMath::Power(y0Prime/MaxYprime, 2) + TMath::Power(y0/MaxAy, 2) << G4endl;
      }

      //G4cout << "y'/max(y') = " << y0Prime << "\t" << MaxYprime << G4endl;
      if ( TMath::Power(y0Prime/MaxYprime, 2) + TMath::Power(y0/MaxAy, 2) < 1 ) { inside = true; }
    }
  }
  else {
    y0 = 0.0;
    y0Prime = 0.0;
  }

  if ( debug_ ) {
    G4cout << "Ay            = " << Ay << G4endl;
    G4cout << "y             = " << y0 << G4endl;
    G4cout << "y'            = " << y0Prime << G4endl;
  }

  //  Propagate the particle from the "naught" coordinates back to the injection point
  G4double randX = x0 + s0X*x0Prime;
  G4double randY = y0 + s0Y*y0Prime;
  G4double randXPrime = x0Prime;
  G4double randYPrime = y0Prime;



  //---------------------------
  // Create transport matrix
  //---------------------------
  double newX = randX;
  double newXprime = randXPrime;
  Transport(&newX, &newXprime, kX, arclength, s0);
  double newY = randY;
  double newYprime = randYPrime;
  Transport(&newY, &newYprime, kY, arclength, s0);


  //--------------------------
  // Apply CBO Oscillations
  //--------------------------
  double x_cbo = CBOValue(decaytime/microsecond);
  newX += x_cbo;
  double xp_cbo = fCBO->Derivative(decaytime/microsecond);
  // dx/dt = [mm/us]
  xp_cbo *= TMath::TwoPi()*0.4666/((omegaC*microsecond)*R_magic()); // omegaC [1/ns] * 1000 -> [1/us] * [1/mm];
  newXprime += xp_cbo;

//   double Mx00, Mx01, Mx10, Mx11;
//   double BetaXs0  = decaybetaX0;
//   double AlphaXs0 = decayalphaX0;
//   double BetaXs   = decaybetaX;
//   double AlphaXs  = decayalphaX;
//   double PsiXs    = arclength*1/7.9; // Use average beta here
//   Mx00 = TMath::Sqrt(BetaXs/BetaXs0)*TMath::Cos(PsiXs) + AlphaXs0*TMath::Sin(PsiXs);
//   Mx01 = TMath::Sqrt(BetaXs*BetaXs0)*TMath::Sin(PsiXs);
//   Mx10 = ((AlphaXs0 - AlphaXs)*TMath::Cos(PsiXs) - (1+AlphaXs0*AlphaXs)*TMath::Sin(PsiXs))/(TMath::Sqrt(BetaXs*BetaXs0));
//   Mx11 = TMath::Sqrt(BetaXs0/BetaXs)*TMath::Cos(PsiXs) - AlphaXs*TMath::Sin(PsiXs);
  
//   G4double newX2      = Mx00*randX + Mx01*randXPrime;
//   G4double newXprime2 = Mx10*randX + Mx11*randXPrime;

//   if ( fabs(newX2 - newX) > 0.01 ) {
//     G4cout << "It doesn't work!!!" << G4endl;
//     G4cout << "\t" << newX2 << G4endl;      
//     G4cout << "\t" << newX << G4endl;
//     G4cout << "Betas\t" << BetaXs << "\t" << AlphaXs << "\t" << BetaXs0 << "\t" << AlphaXs0 << G4endl;
//     G4cout << "Ms\t" << Mx00 << "\t" << Mx01 << "\t" << Mx10 << "\t" << Mx11 << G4endl;
//     G4cout << newX2 << "\t" << newX << "\t" << x0 << G4endl;
//     G4cout << newXprime2 << "\t" << newXprime << "\t" << x0Prime << G4endl;
//     G4cout << "Calc: " << G4endl;
//     G4cout << "  " << randX << "\t" << randXPrime << G4endl;
//     G4cout << "  " << Mx00*randX << "\t" << Mx01*randXPrime << G4endl;
//     exit(1);
//   }
//   return;

//   double My00, My01, My10, My11;
//   double BetaYs0  = decaybetaY0;
//   double AlphaYs0 = decayalphaY0;
//   double BetaYs   = decaybetaY;
//   double AlphaYs  = decayalphaY;
//   double PsiYs    = arclength*1/19.1; // Use average beta here
//   My00 = TMath::Sqrt(BetaYs/BetaYs0)*TMath::Cos(PsiYs) + AlphaYs0*TMath::Sin(PsiYs);
//   My01 = TMath::Sqrt(BetaYs*BetaYs0)*TMath::Sin(PsiYs);
//   My10 = ((AlphaYs0 - AlphaYs)*TMath::Cos(PsiYs) - (1+AlphaYs0*AlphaYs)*TMath::Sin(PsiYs))/(TMath::Sqrt(BetaYs*BetaYs0));
//   My11 = TMath::Sqrt(BetaYs0/BetaYs)*TMath::Cos(PsiYs) - AlphaYs*TMath::Sin(PsiYs);
  
//   if ( fabs(My00*randY + My01*randYPrime - newY) > 0.01 ) {
//     G4cout << "It doesn't work!!!" << G4endl;
//     G4cout << My00*randY + My01*randYPrime << G4endl;
//     G4cout << newY << G4endl;
//   }
//   newY      = My00*randY + My01*randYPrime;
//   newYprime = My10*randY + My11*randYPrime;




  
  //double x = xe + randX;
  double x = newX + xe;
  //double y = randY;
  double y = newY;
  double theta = psi;
  G4double posX, posY, posZ;
  //ComputeGEANTPosition(&posX, &posY, &posZ, x, y, theta);
  //G4cout << "My:  " << posX << " , " << posY << " , " << posZ << G4endl;  
  G4ThreeVector posV = G4ThreeVector(x+R_magic(),y,0);
  posV.rotateY(RotAngle_);
  G4double posX0 = posV.x();
  G4double posY0 = posV.y();
  G4double posZ0 = posV.z();
  posV.rotateY(-psi);
  posX = posV.x();
  posY = posV.y();
  posZ = posV.z();

  //G4cout << "Pos: " << posV.x() << " , " << posV.y() << " , " << posV.z() << G4endl;
  //G4cout << "     " << TMath::Sqrt(posV.x()*posV.x() + posV.z()*posV.z())-R_magic() << "\t" << x << G4endl;
  //G4cout << "     " << y << "\t" << posV.y() << G4endl;
  //first_event = false;
  //return;
  
  
  if ( debug_ ) {
    G4cout << "x             = " << posX << G4endl;
    G4cout << "y             = " << posY << G4endl;
    G4cout << "z             = " << posZ << G4endl;
  }
  
  
  G4double mom_azimuth   = mom;
  G4double mom_vertical  = newYprime*mom;
  G4double mom_radial    = newXprime*mom;
  //G4double mom_vertical  = randYPrime*mom;
  //G4double mom_radial    = randXPrime*mom;
  G4double momX, momY, momZ;
  ComputeGEANTMomentum(&momX, &momY, &momZ, mom_radial, mom_vertical, mom_azimuth, theta); 
  //G4cout << "My:  " << momX << " , " << momY << " , " << momZ << G4endl;  
  G4ThreeVector momV = G4ThreeVector(mom_radial,mom_vertical,mom_azimuth);
  momV.rotateY(RotAngle_);
  G4double momX0 = momV.x();
  G4double momY0 = momV.y();
  G4double momZ0 = momV.z();
  momV.rotateY(-psi);
  momX = momV.x();
  momY = momV.y();
  momZ = momV.z();
  //G4cout << "Rot: " << momV.x() << " , " << momV.y() << " , " << momV.z() << G4endl;
  //first_event = false;
  //return;
  
  if ( debug_ ) {
    G4cout << "pz            = " << momZ << G4endl;
    G4cout << "px            = " << momX << G4endl;
    G4cout << "py            = " << momY << G4endl;
  }
  

  //G4double spinphase = ComputeOmegaSGivenMomRadius(mom, x + R_magic())*decaytime + kicker_phaseoffset;
  
  // Assume 100% polarization to start with
  G4ThreeVector spin(0, 0, 1);
  G4double sx = 0;
  G4double sy = 0;
  G4double sz = 1;
  if ( GetPolarization() == "E821" || GetPolarization() == "E821Mixed" ) {
    //G4double sr = -0.9237 * (mom_start.mag()*GeV/MeV) + 2.8275;
    G4double mom_GeV = mom*GeV;
    mom_GeV = TMath::Min(mom_GeV, 3.13);
    mom_GeV = TMath::Max(mom_GeV, 3.06);
    G4double sz_mean = fsz->Eval(mom_GeV);
    sz = gRandom_->Gaus(sz_mean, 0.017);
    sz = TMath::Min(sz, 1.0);
    G4double st = TMath::Sqrt(1 - sz*sz);;
    G4double sphi = fsphi->GetRandom();
    G4double sr = st*TMath::Cos(sphi);
    G4double sv = st*TMath::Sin(sphi);
    sy = sv;
    sx = sr;

    spin.setX(sr);
    spin.setY(sv);
    spin.setZ(sz);
    spin = spin.unit();
  }
  else if ( GetPolarization() == "100%" || GetPolarization() == "100" || GetPolarization() == "FullMixed" || GetPolarization() == "Full-Mixed" || GetPolarization() == "Full" ) {
    sx = 0;
    sy = 0;
    sz = 1;
    spin.setX(sx);
    spin.setY(sy);
    spin.setZ(sz);
    spin = spin.unit();
  }
  else {
    s.setX(2*gRandom_->Uniform()-1);
    s.setY(2*gRandom_->Uniform()-1);
    s.setZ(2*gRandom_->Uniform()-1);
    s = s.unit();
  }


  //spin.rotateY( -spinphase );
  G4double polX, polY, polZ;
  polX = spin.x();
  polY = spin.y();
  polZ = spin.z();
  //G4cout << "My:  " << polX << " , " << polY << " , " << polZ << G4endl;  

  if ( 0 ) {
    G4cout << "OmegaC*t = " << psi << G4endl;
    G4cout << "OmegaS*t = " << psi_spin << G4endl;
    G4cout << "OmegaA*t = " << psi_spin - psi << G4endl;
    G4cout << "OmegaA   = " << (psi_spin - psi)/decaytime << G4endl;
  }

  //G4ThreeVector polV = G4ThreeVector(0, 0, 1);
  //G4ThreeVector polV; 
  //G4ThreeVector polV = G4ThreeVector(mom_radial,mom_vertical,mom_azimuth);
  G4ThreeVector polV = G4ThreeVector(momX0, momY0, momZ0);
//   polV.setX(mom_start.x()/mom_start.mag());
//   polV.setY(mom_start.y()/mom_start.mag());
//   polV.setZ(mom_start.z()/mom_start.mag());

  // The initial rotation angle offset for position and momentum.
  // The spin rotation angle offset is defined as
  // phi_{s} = omega_{s} / omega_{c} x phi_{c}
  //G4double RotAngle_spin = RotAngle_ * ComputeOmegaSGivenMomRadius(mom, x + R_magic()) / ComputeOmegaCGivenMomRadius(mom, x + R_magic());
  //polV.rotateY(RotAngle_spin);
  G4double polX0, polY0, polZ0;
  polV = polV.unit();
  polX0 = polV.x() / polV.mag();
  polY0 = polV.y() / polV.mag();
  polZ0 = polV.z() / polV.mag();
  G4double spin_phase_offset = TMath::Pi()/2;
  polV.rotateY( -psi_spin - spin_phase_offset);
  polX = polV.x() / polV.mag();
  polY = polV.y() / polV.mag();
  polZ = polV.z() / polV.mag();
  //G4cout << "Rot: " << polX << " , " << polY << " , " << polZ << G4endl; 
  //first_event = false;
  //return;

  G4ThreeVector polVt = G4ThreeVector(momX, momY, momZ);
  G4double omegaA = ComputeOmegaAGivenMomRadius(mom, EqRad);
  G4double anom_phase = omegaA*decaytime + TMath::Pi()/2;
  polVt.rotateY( -anom_phase );
  polVt = polVt.unit();

  //G4cout << polV.

  if ( 1 ) {
    
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

    h_pol->Fill(polV.dot(momV)/momV.mag());
    h_time_pol->Fill(decaytime/microsecond, polV.dot(momV)/momV.mag());

    h_Ax->Fill(MaxAx);
    h_Ay->Fill(MaxAy);

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
    
    G4double betaX_mm = betaX*(mm/m);
    G4double betaY_mm = betaY*(mm/m);
    G4double gammaX_mm = ( 1+alphaX*alphaX )/betaX_mm;
    G4double gammaY_mm = ( 1+alphaY*alphaY )/betaY_mm;
    G4double randXPrime_mrad = randXPrime/mrad;
    G4double randYPrime_mrad = randYPrime/mrad;
    G4double emitX = gammaX_mm*randX*randX + 2*alphaX*randX*randXPrime_mrad + betaX_mm*(randXPrime_mrad*randXPrime_mrad);
    G4double emitY = gammaY_mm*randY*randY + 2*alphaY*randY*randYPrime_mrad + betaY_mm*(randYPrime_mrad*randYPrime_mrad);
    //G4cout << emitX/emittanceX << "\t" << emitY/emittanceY << G4endl;
    
    h_emitX->Fill(emitX);
    h_emitY->Fill(emitY);

    G4strstreambuf* oldBuffer = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
    // Load the muonGasGun
    muonGasGun_->SetParticleTime( totaltime );
    //muonGasGun_->SetParticleTime( decaytime );
    muonGasGun_->SetParticlePosition( G4ThreeVector( posX, posY, posZ ) );
    muonGasGun_->SetParticleMomentum( G4ThreeVector( momX, momY, momZ ) );
    muonGasGun_->SetParticlePolarization( G4ThreeVector( polX, polY, polZ ) );
    G4cout.rdbuf(oldBuffer);
  }  
  else {
    G4strstreambuf* oldBuffer = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
    // Load the muonGasGun.                                                           
    muonGasGun_->SetParticleTime( randT );
    muonGasGun_->SetParticlePosition( G4ThreeVector( g2r.x(), g2r.y(), g2r.z() ) );
    muonGasGun_->SetParticleMomentum( G4ThreeVector( g2p.x(), g2p.y(), g2p.z() ) );
    muonGasGun_->SetParticlePolarization( G4ThreeVector( g2s.x(), g2s.y(), g2s.z() ) );
    G4cout.rdbuf(oldBuffer);
  }




  if ( first_event ) {
    G4cout << G4endl;
    G4cout << "====================================================================" << G4endl;
    G4cout << "===========================  MUONGAS GUN   =========================" << G4endl;
    G4cout << "====================================================================" << G4endl;

    G4cout << "Generating beam from a Uniform Distribution." << G4endl;
    G4cout << "FNAL t0 beam." << G4endl;

    if ( GetStartPerfect() ) {
      G4cout << "Initial Rotation Angle: " << RotAngle_ << G4endl;
    }
    G4cout << "Kicker Phase Offset Angle: " << kicker_phaseoffset << G4endl;

    if ( 1 ) {
      G4cout << "Simulating CBO from a " << GetKick() << "G Square Pulse Kick." << G4endl;
      G4cout << "  Using CBO of Form: CBO = O + Asin(2pi*f*t + P)" << G4endl;
      G4cout << "  [O] = " << fCBO->GetParameter(0) << " mm." << G4endl;
      G4cout << "  [A] = " << fCBO->GetParameter(1) << " mm." << G4endl;
      G4cout << "  [f] = " << fCBO->GetParameter(2) << " MHz." << G4endl;
      G4cout << "  [P] = " << fCBO->GetParameter(3) << " rad." << G4endl;
    }

    if ( GetFlatDecayTime() ) {
      G4double maxtau = GetMaxDecayTime();
      G4cout << "Decay time is flat between [0, " << maxtau << "] microseconds." << G4endl;
    }
    else {
      G4cout << "Decay time is exponential." << G4endl;
    }
    
    
    G4cout << "emittanceX = " << emittanceX/mrad << " mm*mrad." << G4endl;
    G4cout << "emittanceY = " << emittanceY/mrad << " mm*mrad." << G4endl;
    if ( GetUseConstBeta() ) {
      G4cout << "betaX      = " << betaX/m << " m." << G4endl;
      G4cout << "betaY      = " << betaY/m << " m." << G4endl;
      G4cout << "alphaX     = " << alphaX << G4endl;
      G4cout << "alphaY     = " << alphaY << G4endl;
    }
    else {
      G4cout << "Using 4-parameter fits for BetaX and BetaY. Alpha = 1/2*dBeta/ds" << G4endl;
    }
    G4cout << "Xmax       = " << std::sqrt( epsilonX*betaX0 )*mm << " mm." << G4endl;
    G4cout << "X'max      = " << std::sqrt( epsilonX*gammaX0 )/mrad << " mrad." << G4endl;
    G4cout << "Ymax       = " << std::sqrt( epsilonY*betaY0 )*mm << " mm." << G4endl;
    G4cout << "Y'max      = " << std::sqrt( epsilonY*gammaY0 )/mrad << " mrad." << G4endl;
    G4cout << "Particle   = " << Particle_ << G4endl;
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
    G4cout << "r(0)       = " << posX0 << " , " << posY0 << " , " << posZ0 << G4endl;
    G4cout << "p(0)       = " << momX0 << " , " << momY0 << " , " << momZ0 << G4endl;

    double pol_azimuth, pol_radial, pol_vertical;
    ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polX0, polY0, polZ0, -RotAngle_);
    G4cout << "s(0)-rot   = " << pol_radial << " , " << pol_vertical << " , " << pol_azimuth << endl;
    ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polV.x(), polV.y(), polV.z(), radianlength);
    G4cout << "s(t)-rot   = " << pol_radial << " , " << pol_vertical << " , " << pol_azimuth << endl;
    ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, polVt.x(), polVt.y(), polVt.z(), radianlength);
    G4cout << "s'(t)-rot  = " << pol_radial << " , " << pol_vertical << " , " << pol_azimuth << endl;
    G4cout << "|s|        = " << polV.mag() << G4endl;

    if ( GetPolarization() == "Full-Mixed" || GetPolarization() == "FullMixed" ) {
      G4cout << "Using polarization of muons in tails is rotated by 2pi/3 from 100% in the core." << G4endl;
    }
    else if ( GetPolarization() == "Mixed" || GetPolarization() == "E821Mixed" ) {
      G4cout << "Using polarization of muons in tails is rotated by 2pi/3 from what is in the core." << G4endl;
    }
    else if ( GetPolarization() == "E821" || GetPolarization() == "E821Mixed" ) {
      G4cout << "Using E821 Hughon Polarization Fits." << G4endl;
    }
    else if ( GetPolarization() == "100%" || GetPolarization() == "100" || GetPolarization() == "Full" ) {
      G4cout << "Assuming 100% polarization (s-dot-p=1)." << G4endl;
    }
    else {
      G4cout << "Random Polarization." << G4endl;
    }
    
    first_event = false;
  }





  if( muonGasVerbosity_ ) {
    // Print some stuff                                                                                       
    
    static int COUNT_PRIMARIES = 0;
    G4cout << endl;
    G4cout << "********************************************************************" << endl;
    printf("*  PRIMARY VERTEX: %-6d%43s\n", ++COUNT_PRIMARIES, "*");
    G4cout << "* ---------------------------------------------------------------- *" << endl;
    //    printf("*           RANDOM NUMBER:% 11.3f%11s%8s    [0..1] *\n"      , randNum , "", "");         
    printf("*       RANDOM DECAY TIME:% 11.3f%11s%8s     [ns]  *\n"      , randT   , "", "");
    printf("*         CYCLOTRON ANGLE:% 11.3f%11s%8s    [rad]  *\n"      , psiCRand, "", "");
    printf("*         ANOMALOUS ANGLE:% 11.3f%11s%8s    [rad]  *\n"      , psiARand, "", "");
    printf("*              SPIN ANGLE:% 11.3f%11s%8s    [rad]  *\n"      , psiSRand, "", "");
    G4cout << "* ---------------------------------------------------------------- *" << endl;
    printf("*  CARTESIAN   POSITION  :% 11.3f% 11.3f% 8.3f     [mm]  *\n", r.x(), r.y(), r.z());
    printf("*              MOMENTUM  :% 11.3f% 11.3f% 8.3f   [MeV/c] *\n", p.x(), p.y(), p.z());
    printf("*            POLARIZATION:% 11.3f% 11.3f% 8.3f      []   *\n", s.x(), s.y(), s.z());
    G4cout << "********************************************************************" << endl;
    G4cout << endl << endl << endl << endl;      
  }
  
  //  FIRE    
  mf::LogInfo("G2MuonGasSource") << "About to call muonGasGun_->GeneratePrimaryVertex";  
  muonGasGun_->GeneratePrimaryVertex( evt );
  mf::LogInfo("G2MuonGasSource") << "Just got back from calling muonGasGun_->GeneratePrimaryVertex";  
  
  if ( evt && 0 ) {
    G4cout << "Generated muon w/ t0 = " << t0 << " and total time = " << randT << G4endl;
    G4PrimaryVertex *primary_vtx = evt->GetPrimaryVertex();
    G4PrimaryParticle *primary = primary_vtx->GetPrimary();
    primary->SetWeight(t0/100.0); // *microsecond);
  }

} // generatePrimaries


G4double gm2ringsim::G2MuonGasSource::randXFromE821()
{
  // The following is taken from Fig. 20 of the E821 Final Report, G.W. Bennet et al, Phys. Rev. D 73, 072003 (2006 )
  static G4int const nBins = 36;
  static G4double const radialProfile[nBins] = // probability amplitude                                             
    {    0.000, 0.000, 0.000, 0.000, 0.005, // 7070 mm < r < 7080 mm 
	 0.010, 0.010, 0.015, 0.015,        // 7080 mm < r < 7090 mm
	 0.015, 0.025, 0.055, 0.115, 0.235, // 7090 mm < r < 7100 mm
	 0.400, 0.595, 0.775, 0.935,        // 7100 mm < r < 7110 mm 
	 1.000, 0.950, 0.890, 0.820, 0.735, // 7110 mm < r < 7120 mm
	 0.645, 0.550, 0.435, 0.320,        // 7120 mm < r < 7130 mm
	 0.205, 0.125, 0.075, 0.044, 0.023, // 7130 mm < r < 7140 mm
	 0.010, 0.005, 0.000, 0.000         // 7140 mm < r < 7150 mm
    };
  
  //  Generates random numbers uniformly across bins of above distribution
  
  randFromDistr scaledE821X( radialProfile, nBins, 0 );
  
  //  Generates random numbers as delta functions at lower edge of bins of above distribution 
  //  randFromDistr scaledE821X( radialProfile, nBins, 1 );
  
  //  RandGeneral always returns a variate on ]0,1[.  It is the user's responsibility to transform this into the domain native to the distribution (see below).                                                                        
  double randNum = scaledE821X.shoot();
  
  //  Convert from ]0,1[ to a bonafide radial coordinate in local storage region (SR) coordinates                   
  static G4double const rhoMin   = 7071*mm;
  static G4double const rhoMax   = 7149*mm;
  static G4double const DeltaRho = rhoMax-rhoMin;
  static G4double const dRho     = DeltaRho/( nBins-1 );
  //  "x" in traditional beamSpeak                                                                                  
  G4double myRandX_SR = rhoMin + randNum*DeltaRho - dRho/2. - R_magic();
  
  return myRandX_SR;
}

G4double gm2ringsim::G2MuonGasSource::randTFromE989()
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


double gm2ringsim::G2MuonGasSource::GenerateRandomXe()
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
