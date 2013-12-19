// Get the PGA header
//#include "gm2ringsim/actions/PGA/obsoletePrimaryGeneratorAction_service.hh"

// ART includes
#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "TLorentzVector.h"

// Geant includes
#include "Geant4/G4Event.hh"
#include "Geant4/G4GeneralParticleSource.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4PrimaryVertex.hh"
#include "Geant4/G4PrimaryParticle.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/globals.hh"
#include "Geant4/Randomize.hh"
#include "Geant4/G4strstreambuf.hh"

// g2migtrace helpers

#include "gm2ringsim/actions/PGA/G2InflectorSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>

#include "TMath.h"

using std::endl;

// Constructor
gm2ringsim::G2InflectorSource::G2InflectorSource() :
  inflectorGun_(),
  g2GPS_(),
  infGeom_("inflector"),
  Verbosity_(false),
  first_event(true),
  avg_x(0.0), 
  avg_y(0.0), 
  avg_xprime(0.0), 
  avg_yprime(0.0),
  sum_x(0.0),
  sum_y(0.0),
  sum_xprime(0.0),
  sum_yprime(0.0),
  ngen(0),
  StartUpstream(false),
  StartUpstreamCryo(false),
  StartDownstream(true),
  StartPerfect(false),
  GenGaussian(false),
  LaunchAngle(0.0),
  StorageOffset(0.0),
  TurnCounter_(11),
  RotAngle_(0.0),
  EmittanceX(40.0),
  EmittanceY(40.0),
  BetaX(19.1),
  BetaY(7.6),
  AlphaX(0.0),
  AlphaY(0.0),
  Pmean(0.005),
  dPOverP(0.005),
  SigmaT(50),
  FlatDecayTime(false),
  MaxDecayTime(-1.0),
  Particle_("mu+"),
  NumParticles_(1),
  DecayScaleFactor_(1),
  Polarization_("E821")
{
  inflectorGun_ = new G4ParticleGun();
  //inflectorGun_->SetVerbosity(true);
  g2GPS_ = new G2GeneralParticleSource();

  gRandom_ = new TRandom3();

  fsz   = new TF1("fsz", "-(3.68923)+(1.48959*x)", 3.06, 3.13);
  fsphi = new TF1("fsphi", "(((1788.72-(196.394*cos(x)))-(321.191*cos(2*x)))+(68.1887*cos(3*x)))+(((587.76*cos(4*x))-(94.1914*cos(5*x)))-(106.777*cos(6*x)))", -3.14159, 3.14159);
  fill_ = false;
  //fill_ = true;

  double microsecond = 1e3;
  double timeperturn = 1/ComputeFrequencyCGivenMomRadius(P_magic(), R_magic())/microsecond;
  int maxturns_ = 101;
  double maxt = maxturns_* timeperturn;
  int numhitsperturn = 150;


  if ( fill_ ) {
    f = new TFile("inflectorgun.root", "RECREATE");
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
  h_xprimex = new TH2F("hxprimex", "", 90*4, -45.0*mm, 45.0*mm, 40*4, -20, 20);
  h_yprimey = new TH2F("hyprimey", "", 90*4, -45.0*mm, 45.0*mm, 40*4, -20, 20);
  h_xy = new TH2F("hxy", "", 90*4, -45.0*mm, 45.0*mm, 90*4, -45.0*mm, 45.0*mm);
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

gm2ringsim::G2InflectorSource::~G2InflectorSource() 
{
  if ( 1 ) {
    G4cout << G4endl;
    G4cout << "====================================================================" << G4endl;
    G4cout << "=====================  INFLECTOR GUN STATS  ========================" << G4endl;
    G4cout << "====================================================================" << G4endl;
    
    G4cout << "   N   = " << ngen << G4endl;
    G4cout << "  <x>  = " << sum_x / ngen << G4endl;;
    G4cout << "  <y>  = " << sum_y / ngen << G4endl;;
    G4cout << "  <x'> = " << sum_xprime / ngen << G4endl;;
    G4cout << "  <y'> = " << sum_yprime / ngen << G4endl;
  }

  if ( fill_ ) {
    h_xy->Write();
    h_xprimex->Write();
    h_yprimey->Write();
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

void gm2ringsim::G2InflectorSource::SetVerbosity(bool verbose){
  Verbosity_ = verbose;
}

void gm2ringsim::G2InflectorSource::TransportTwiss(double alpha, double beta, double gamma, double s,
						   double *new_alpha, double *new_beta, double *new_gamma)
{
  // Going upstream (downstream replace s -> -s)
  // ( b(s) )   ( 1  -2s  +s*s )   (b)
  // ( a(s) ) = ( 0   1     -s ) x (a)
  // ( g(s) )   ( 0   0      1 )   (g)
  
  *new_gamma = gamma;
  *new_alpha = alpha - s * gamma;
  *new_beta  = beta - (2*s*alpha) + gamma*s*s;  
}


// EndOfPrimaryGeneratorAction
void gm2ringsim::G2InflectorSource::GeneratePrimaryVertex(G4Event* evt)
{
  //bool debug = false;

  //G4cout << "G2InflectorSource::GeneratePrimaryVertex  --- GENERATING EVENT!" << G4endl;

  mf::LogInfo("G2InflectorSource") << "GeneratePrimaryVertex";

  G4ParticleDefinition *def = G4ParticleTable::GetParticleTable()->FindParticle(Particle_);
  if ( def ) {
//     if ( GetFlatTime() ) {;
//       double tau = def->GetPDGLifeTime();
//       double gamma = gammaMagic();
//       double mutau = tau*gamma;
//       double rand = G4UniformRand();
//     }
    if ( first_event ) {
      G4cout << "Found Particle Info for [" << Particle_ << "]" << G4endl;
      G4cout << " g-2/2 : " << def->CalculateAnomaly() << G4endl;
      def->SetPDGLifeTime(def->GetPDGLifeTime()/DecayScaleFactor_);
      def->DumpTable();
    }
    inflectorGun_ ->SetParticleDefinition(def);
    inflectorGun_ ->SetNumberOfParticles(NumParticles_);
  }
  else {
    G4ParticleTable::GetParticleTable()->DumpTable();
    G4cout << "Could not find Particle Info for [" << Particle_ << "]" << G4endl;
    exit(1);
  }
    

//   if ( first_event ) {
//     G4String defaultParticle = "mu-";
//     G4String particle = g2GPS_ ->GetParticleDefinition()->GetParticleName();
    
//     if( g2GPS_ ->GetParticleDefinition()!=NULL && particle!=defaultParticle ){
//       inflectorGun_ ->SetParticleDefinition( g2GPS_ ->GetParticleDefinition() );
//     } else {
//       inflectorGun_ ->SetParticleDefinition( G4ParticleTable::GetParticleTable()->FindParticle(defaultParticle) );
//     }    
//   }
  

  //====================================================================//
  //=========================   INFLECTOR GUN   ========================//
  //====================================================================//
  G4double epsilonX, betaX, alphaX, gammaX, numSigmaX, numSigmaXPrime;
  G4double epsilonY, betaY, alphaY, gammaY, numSigmaY, numSigmaYPrime;
  G4double x0, y0, x0Prime, y0Prime, randX, randY, randXPrime, randYPrime;
  G4double dP_over_P, emittanceX, emittanceY;

  //static G4double const inflectorLength = 1.7*m;

  emittanceX = GetEmittanceX();
  emittanceY = GetEmittanceY();


  //  DEFAULTS: The emittances below are Fermilab's spec., and seem to be fairly well cinched down.  
  //  Here, I will assume pi * epsilon{X,Y} is the AREA OF THE BOUNDING ELLIPSE IN PHASE SPACE, 
  //  which I will take to be 99% Gaussians.  NB: gamma{X,Y} isn't actually ever needed, because of 
  //  the invariant relation beta*gamma - alpha*alpha = 1.  Default is to "sneak" the beam through
  //  the E821 inflector (see below).
  epsilonX       = emittanceX*mm*mrad;
  alphaX         = -0.543951;
  betaX          =  2.025*m;
  numSigmaX      =  2.57583; // sqrt( epsilonX*betaX  ) = numSigmaX, i.e. 1, 2, 3..., in this case 99%
  numSigmaXPrime =  2.57583; // sqrt( epsilonX*gammaX ) = numSigmaXPrime, i.e. 1, 2, 3..., in this case 99%
  //numSigmaX      =  2.0; 
  //numSigmaXPrime =  2.0; 
  //
  epsilonY       = emittanceY*mm*mrad;
  alphaY         = -0.0434492;
  betaY          = 19.600*m;
  numSigmaY      =  2.57583; // sqrt( epsilonY*betaY  ) = numSigmaY, i.e. 1, 2, 3..., in this case 99%
  numSigmaYPrime =  2.57583; // sqrt( epsilonY*gammaY ) = numSigmaYPrime, i.e. 1, 2, 3..., in this case 99%
  //numSigmaY      =  2.0; 
  //numSigmaYPrime =  2.0; 

  //G4double test = sqrt(epsilonY * betaY);
  //G4cout << test << "\t" << numSigmaY << G4endl;
    
  G4bool perfect = true;
  G4bool carol = false;
  if ( perfect ) {
    //  [Near-]Perfect match to ring (@Rubin).
    if ( 0 ) {
      alphaX =  0.0000000;
      betaX  =  7.66477*m;
      alphaY =  0.0000000;
      betaY  =  19.0763*m;
    }
  }
    
  if ( carol ) {
    //  Carol Johnstone's parameters from her 13 SEP 12 talk (see the DocDB).
    alphaX = -8.5;
    alphaY =  0.0;
    betaX  =  7.9*m;
    betaY  = 19.0763*m;
  }


  betaX = GetBetaX()*m;
  betaY = GetBetaY()*m;
  alphaX = GetAlphaX();
  alphaY = GetAlphaY();




  G4ThreeVector storage_orbit(R_magic(), 0., 0.);
  G4double storage_offset = GetStorageOffset();
  if ( storage_offset > 0 ) { storage_orbit += G4ThreeVector(storage_offset,0.,0.); }
  G4double RotAngle = 0.0;
  if ( GetRotAngle() > 0 || GetRotAngle() < 0 ) {
    RotAngle = GetRotAngle();
  }
  else {
    if ( GetTurnCounter() == 11 ) {
      //RotAngle = 2 * 3.14159 - 5.762;
      RotAngle = 2 * 3.14159 - (GetTurnCounter())/12.0 * 2 * 3.14159;    
    }
    else {
      RotAngle = 2 * 3.14159 - GetTurnCounter()/12.0 * 2 * 3.14159;
    }
  }
  if ( GetStartPerfect() ) {
    storage_orbit.rotateY(RotAngle);
  }


  // CALCULATE THE DOWNSTREAM MANDREL VERTEX POSITION
    
  
  gm2geom::inflectorGeometry const& ig = infGeom_.ig ;//inflectorGeometry::getInstance();

  //double delta_angle = ig.swingAngle;


  G4ThreeVector downstream(R_magic()+ig.aperture_off(), 0., 0.);
  downstream.rotateY(infGeom_.rotAngle);
    
    
  // CALCULATE THE UPSTREAM MANDREL VERTEX POSITION
    
  G4ThreeVector upstream(0., 0., -ig.mandrel_length());  
  upstream.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream.rotateY(infGeom_.swingAngle);
  upstream += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream.rotateY(infGeom_.rotAngle);
    
  
  G4ThreeVector upstream_cryo(0., 0., -ig.length()+ig.window_thickness()+ig.conductor_thickness() );
  upstream_cryo.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream_cryo.rotateY(infGeom_.swingAngle);  
  upstream_cryo += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_cryo.rotateY(infGeom_.rotAngle);


  // Direction of inflector axis
  G4ThreeVector dir = (downstream - upstream).unit();


  //G4cout << "BetaX: " << betaX << G4endl;
  //G4cout << "BetaY: " << betaY << G4endl;

  //  First, calculate gamma from the user's parameters using the fact that beta*gamma - alpha^2 = 1 is invariant
  gammaX = ( 1+alphaX*alphaX )/betaX;
  gammaY = ( 1+alphaY*alphaY )/betaY;

  if ( 0 ) {
  G4cout << "BetaX: " << betaX << G4endl;
  G4cout << "BetaY: " << betaY << G4endl;
  G4cout << "alphaX: " << alphaX << G4endl;
  G4cout << "alphaY: " << alphaY << G4endl;
  G4cout << "gammaX: " << gammaX << G4endl;
  G4cout << "gammaY: " << gammaY << G4endl;
  }
    
  double new_alphaX, new_betaX, new_gammaX;
  double new_alphaY, new_betaY, new_gammaY;
  if ( GetStartUpstream() || GetStartUpstreamCryo() || GetStartDownstream() ) {
    G4ThreeVector up, down;
    down = downstream;
    up = downstream;
    if ( GetStartUpstream() ) { up = upstream; }
    if ( GetStartUpstreamCryo() ) { up = upstream_cryo; }
    TransportTwiss(alphaX, betaX, gammaX, -1*(down - up).mag(), 
		   &new_alphaX, &new_betaX, &new_gammaX);
    TransportTwiss(alphaY, betaY, gammaY, -1*(down - up).mag(), 
		   &new_alphaY, &new_betaY, &new_gammaY);
    
    if ( first_event ) {
      G4cout << "[ Beta_{x}(DM)  ]   [\t" << betaX <<  "\t]" << "     " << "[ Beta_{x}(UM)  ]   [\t" << new_betaX <<  "\t]" << G4endl;
      G4cout << "[ Alpha_{x}(DM) ] = [\t" << alphaX << "\t]" << " ==> " << "[ Alpha_{x}(DM) ] = [\t" << new_alphaX << "\t]" << G4endl;
      G4cout << "[ Gamma_{x}(DM) ]   [\t" << gammaX << "\t]" << "     " << "[ Gamma_{x}(DM) ]   [\t" << new_gammaX << "\t]" << G4endl;
      G4cout << G4endl;
      G4cout << "[ Beta_{y}(DM)  ]   [\t" << betaY <<  "\t]" << "     " << "[ Beta_{y}(UM)  ]   [\t" << new_betaY <<  "\t]" << G4endl;
      G4cout << "[ Alpha_{y}(DM) ] = [\t" << alphaY << "\t]" << " ==> " << "[ Alpha_{y}(DM) ] = [\t" << new_alphaY << "\t]" << G4endl;
      G4cout << "[ Gamma_{y}(DM) ]   [\t" << gammaY << "\t]" << "     " << "[ Gamma_{y}(DM) ]   [\t" << new_gammaY << "\t]" << G4endl;
    }
      
    alphaX = new_alphaX;
    betaX  = new_betaX;
    gammaX = new_gammaX;
      
    alphaY = new_alphaY;
    betaY  = new_betaY;
    gammaY = new_gammaY;
  }

  if ( 0 ) {
  G4cout << "alphaX: " << alphaX << G4endl;
  G4cout << "alphaY: " << alphaY << G4endl;
  G4cout << "gammaX: " << gammaX << G4endl;
  G4cout << "gammaY: " << gammaY << G4endl;
  G4cout << "BetaX: " << betaX << G4endl;
  G4cout << "BetaY: " << betaY << G4endl;
  }

  //  Next, use the above parameters to determine the point s{X,Y} at which the phase-space ellipses have alpha{X,Y} = 0.
  //  This serves to define what I will call the "naught" positions, i.e. the positions at which the phase-space ellipses
  //  are standing "upright" in phase space (alpha{X,Y}=0).  It is very easy to generate random positions/momenta at the
  //  naught coordinates, which can then easily be propagted back to the injection point.
  G4double sX = -alphaX/gammaX;
  G4double sY = -alphaY/gammaY;

  if ( 0 ) {
    G4cout << "sX: " << sX << G4endl;
    G4cout << "sY: " << sY << G4endl;
  }

  //  Calculate beta{X,Y}0
  G4double betaX0 = betaX + 2*alphaX*sX + gammaX*sX*sX;
  G4double betaY0 = betaY + 2*alphaY*sY + gammaY*sY*sY;

  if ( 0 ) {
  G4cout << "BetaX0: " << betaX0 << G4endl;
  G4cout << "BetaY0: " << betaY0 << G4endl;

  G4cout << "BetaX: " << betaX << G4endl;
  G4cout << "BetaY: " << betaY << G4endl;

  G4cout << "BetaX0: " << betaX0 << G4endl;
  G4cout << "BetaY0: " << betaY0 << G4endl;
  }

  //  Calculate gamma{X,Y}0 given that alpha{X,Y}=0 using the invariant (see above).
  G4double gammaX0 = 1/betaX0;
  G4double gammaY0 = 1/betaY0;


  G4double sigmaX0 = 0.0;
  G4double sigmaY0 = 0.0;
  G4double sigmaX0Prime = 0.0;
  G4double sigmaY0Prime = 0.0;   
  
  G4double MaxX = std::sqrt( epsilonX*betaX0 );
  G4double MaxY = std::sqrt( epsilonY*betaY0 );

  if ( 0 ) {
  G4cout << "BetaX: " << betaX << G4endl;
  G4cout << "BetaY: " << betaY << G4endl;
  G4cout << "emitX0: " << epsilonX << G4endl;
  G4cout << "emitY0: " << epsilonY << G4endl;
  G4cout << "BetaX0: " << betaX0 << G4endl;
  G4cout << "BetaY0: " << betaY0 << G4endl;
  G4cout << "GammaX0: " << gammaX0 << G4endl;
  G4cout << "GammaY0: " << gammaY0 << G4endl;
  G4cout << "MaxX:  " << std::sqrt( epsilonX*betaX0 ) << G4endl;
  G4cout << "MaxY:  " << std::sqrt( epsilonY*betaY0 ) << G4endl;
  G4cout << "MaxX': " << std::sqrt( epsilonX*gammaX0 ) << G4endl;
  G4cout << "MaxY': " << std::sqrt( epsilonY*gammaY0 ) << G4endl;
  }

  if ( GetGenGaussian() ) {
    //  Again, we'll generate random positions/momenta by shooting 99% Gaussians, whice I've
    //  assumed represent the bounding ellipse in phase space.  The user is free to change
    //  the "containment" by specifying different values of numSigmaX, numSigmaY, numSigmaXPrime, 
    //  numSigmaYPrime.
    sigmaX0 = std::sqrt( epsilonX*betaX0 )/numSigmaX;
    sigmaY0 = std::sqrt( epsilonY*betaY0 )/numSigmaY;
    sigmaX0Prime = std::sqrt( epsilonX*gammaX0 )/numSigmaXPrime;
    sigmaY0Prime = std::sqrt( epsilonY*gammaY0 )/numSigmaYPrime;
    
    x0 = G4RandGauss::shoot(0.,sigmaX0);
    y0 = G4RandGauss::shoot(0.,sigmaY0);
    x0Prime = G4RandGauss::shoot(0.,sigmaX0Prime);
    y0Prime = G4RandGauss::shoot(0.,sigmaY0Prime);
  }
  else {
    bool inside = false;
    if ( epsilonX > 0.0 ) {
      while ( inside == false ) {	
	x0 = (2*G4UniformRand()-1) * std::sqrt( epsilonX*betaX0 );
	x0Prime = (2*G4UniformRand()-1) * std::sqrt( epsilonX*gammaX0 );
	
	if ( TMath::Power(x0Prime/TMath::Sqrt(epsilonX*gammaX0), 2) + TMath::Power(x0/TMath::Sqrt(epsilonX*betaX0), 2) < 1 ) { inside = true; }
      }
    }
    else {
      x0 = 0.0;
      x0Prime = 0.0;
    }

    inside = false;
    if ( epsilonY > 0.0 ) {
      while ( inside == false ) {
	y0 = (2*G4UniformRand()-1) * std::sqrt( epsilonY*betaY0 );
	y0Prime = (2*G4UniformRand()-1) * std::sqrt( epsilonY*gammaY0 );
	
	if ( TMath::Power(y0Prime/TMath::Sqrt(epsilonY*gammaY0), 2) + TMath::Power(y0/TMath::Sqrt(epsilonY*betaY0), 2) < 1 ) { inside = true; }      
      }
    }
    else {
      y0 = 0.0;
      y0Prime = 0.0;
    }
  }

  //  Propagate the particle from the "naught" coordinates back to the injection point
  randX = x0 + sX*x0Prime;
  randY = y0 + sY*y0Prime;
  randXPrime = x0Prime;
  randYPrime = y0Prime;
    
  G4ThreeVector xAxis, yAxis, zAxis;
  
  zAxis = dir;
  yAxis = G4ThreeVector(0, 1, 0);
  xAxis = yAxis.cross(zAxis);
  if ( GetStartPerfect() ) {
    xAxis = G4ThreeVector(1, 0, 0);
    xAxis.rotateY(RotAngle);
    yAxis = G4ThreeVector(0, 1, 0);
    yAxis.rotateY(RotAngle);
    zAxis = G4ThreeVector(0, 0, 1);
    zAxis.rotateY(RotAngle);
  }



  G4ThreeVector xy_gen(randX, randY, 0.0);
  if ( GetStartPerfect() ) {
    xy_gen.rotateY(RotAngle);
  }
  

  G4ThreeVector storage_orbit_start(R_magic(), 0., 0.);
  if ( storage_offset > 0 ) {
    storage_orbit_start += G4ThreeVector(storage_offset,0.,0.);
  }
  storage_orbit_start += xy_gen;
  if ( GetStartPerfect() ) {
    storage_orbit_start.rotateY(RotAngle);
  }
  
  G4ThreeVector downstream_start(R_magic()+ig.aperture_off(), 0.0, 0.0);
  downstream_start += xy_gen;
  downstream_start.rotateY(infGeom_.rotAngle);
  
  G4ThreeVector upstream_start(0., 0., -ig.mandrel_length());  
  upstream_start.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream_start.rotateY(infGeom_.swingAngle);
  upstream_start += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_start += xy_gen;
  upstream_start.rotateY(infGeom_.rotAngle);
      
  G4ThreeVector upstream_cryo_start(0., 0., -ig.length()+ig.window_thickness()+ig.conductor_thickness() );
  upstream_cryo_start.rotateX(-infGeom_.tiltAngle); // why negative?  WTF?
  upstream_cryo_start.rotateY(infGeom_.swingAngle);
  upstream_cryo_start += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_cryo_start += xy_gen;
  upstream_cryo_start.rotateY(infGeom_.rotAngle);


  //----------------------------------------
  // STARTING SPOT
  //----------------------------------------
  G4ThreeVector beam_start_offset = upstream;
  G4ThreeVector beam_start = upstream_start;
  if ( GetStartDownstream() ) {
    beam_start_offset = downstream;
    beam_start = downstream_start;
  }
  else if ( GetStartUpstream() ) {
    beam_start_offset = upstream;
    beam_start = upstream_start;
  }
  else if ( GetStartUpstreamCryo() ) {
    beam_start_offset = upstream_cryo;
    beam_start = upstream_cryo_start;
  }
  else if ( GetStartPerfect() ) {
    beam_start_offset = storage_orbit;
    beam_start = storage_orbit_start;
  }
  else {
    G4cout << "STARTING POINT NOT SPECIFIED!!!!" << G4endl;
    G4cout << "Assuming upstream." << G4endl;
  }      

  dP_over_P = GetdPOverP();
  Pmean = GetPmean();
  if ( Pmean < 0 ) { Pmean = pMagic(); }
    
  //  Fermilab tells us the beam will have a |deltaP/P| of 0.15% (in the downstream direction)

  G4double pX   = pMagic();
  if ( dP_over_P >= 0 ) {
    pX *= G4RandGauss::shoot( 1, dP_over_P ); // "downstream" @12 o'clock
  }
  else {
    double sigmapx = -pMagic()*dP_over_P;
    pX += ((2*G4UniformRand()-1) * sigmapx);
  }


  //G4double pX   = pMagic()*G4RandGauss::shoot( 1, 0.0015 ); // "downstream" @12 o'clock
  //G4double pX   = pMagic()*G4RandGauss::shoot( 1, 0.000015 ); // "downstream" @12 o'clock
  G4double pMag = pX/std::sqrt( 1-randXPrime*randXPrime-randYPrime*randYPrime );
  G4double pY   = randXPrime*pMag; // radial   @12 o'clock
  G4double pZ   = randYPrime*pMag; // vertical @lways
  G4ThreeVector p = G4ThreeVector(pX,pY,pZ);
  TLorentzVector momvec;
  G4double Emu = std::sqrt(pMag*pMag + mMuon()*mMuon());
  momvec.SetPxPyPzE(pX,pY,pZ, Emu);
  G4double mu_beta = momvec.Beta();
  G4double mu_gamma = momvec.Gamma();


  G4ThreeVector momX = pY * xAxis;
  G4ThreeVector momY = pZ * yAxis;
  G4ThreeVector momZ = pX * zAxis;

  G4ThreeVector mom_start = momX + momY + momZ;


  //-------------------------------
  // Relative Launch Angle
  //-------------------------------
  G4double launch_angle = GetLaunchAngle();
  if ( launch_angle > 0 || launch_angle < 0 ) {
    mom_start.rotateY(launch_angle*mrad);
  }

  // relative injection angle?
  //mom_start.rotateY(-3*mrad);
    
  if ( 0 ) {
    G4cout << "Px = " << momX << G4endl;
    G4cout << "Py = " << momY << G4endl;
    G4cout << "Pz = " << momZ << G4endl;
    G4cout << "p' = " << mom_start << G4endl;
    G4cout << "px.dot(py) = " << momX.dot(momY) << G4endl;
    G4cout << "py.dot(pz) = " << momY.dot(momZ) << G4endl;
    G4cout << "py.dot(pz) = " << momY.dot(momZ) << G4endl;
    G4cout << "p.dot(x-axis) = " << mom_start.dot(xAxis) << "  \t" << momX.mag() << G4endl;
    G4cout << "p.dot(y-axis) = " << mom_start.dot(yAxis) << "  \t" << momY.mag() << G4endl;
    G4cout << "p.dot(z-axis) = " << mom_start.dot(zAxis) << "  \t" << momZ.mag() << G4endl;


    G4cout.precision(3);
    G4cout << "gen[x,x'] = " << randX << " , " << randXPrime << G4endl;
    G4cout << "gen[y,y'] = " << randY << " , " << randYPrime << G4endl;

    G4cout << "me[x,x'] = " << (beam_start - beam_start_offset).x() << " , " << mom_start.dot(xAxis)/mom_start.mag() << G4endl;
    G4cout << "me[y,y'] = " << (beam_start - beam_start_offset).y() << " , " << mom_start.dot(yAxis)/mom_start.mag() << G4endl;
  }

  //  Assume the spin polarization vector is perfectly aligned with the momentum vector at injection.
  G4ThreeVector s = mom_start/mom_start.mag();
  G4double mom_GeV =  mom_start.mag()/(GeV/MeV);

  
  if ( GetPolarization() == "E821" || GetPolarization() == "E821Mixed" ) {
    //G4double sr = -0.9237 * (mom_start.mag()*GeV/MeV) + 2.8275;
    mom_GeV = TMath::Min(mom_GeV, 3.13);
    mom_GeV = TMath::Max(mom_GeV, 3.06);
    G4double sz_mean = fsz->Eval(mom_GeV);
    G4double sz = gRandom_->Gaus(sz_mean, 0.017);
    sz = TMath::Min(sz, 1.0);
    G4double st = TMath::Sqrt(1 - sz*sz);;
    G4double sphi = fsphi->GetRandom();
    G4double sr = st*TMath::Cos(sphi);
    G4double sv = st*TMath::Sin(sphi);

    s.setX(sr);
    s.setY(sv);
    s.setZ(sz);
    s = s.unit();
  }
  else if ( GetPolarization() == "100%" || GetPolarization() == "100" || GetPolarization() == "FullMixed" || GetPolarization() == "Full-Mixed" || GetPolarization() == "Full" ) {
    s.setX(mom_start.x()/mom_start.mag());
    s.setY(mom_start.y()/mom_start.mag());
    s.setZ(mom_start.z()/mom_start.mag());
  }
  else {
    s.setX(2*gRandom_->Uniform()-1);
    s.setY(2*gRandom_->Uniform()-1);
    s.setZ(2*gRandom_->Uniform()-1);
    s = s.unit();
  }

  if ( 0 ) {
    G4cout << "|p| = " << mom_start << "\ts-dot-r = " << s << "  \t" << p.mag() << "\t" << mom_GeV << "\t" << s.z() << G4endl;
  }

  G4double randT;
  do{
    randT = G4RandGauss::shoot(0.,25.)*ns;
  } while( abs(randT)>50.*ns );
  randT += -50.*ns;
  //inflectorGun->SetParticleTime( randT );

  // beta c = 2 pi 7.112m / 150ns
  // length/time = beta c    
  G4ThreeVector diff = beam_start-downstream;
  G4double sign = 1.;
  if( dir.cosTheta(diff) < 0 ) {
    sign = -1.;
  }
  randT = 100.0*ns + sign*diff.mag()/(2.*pi*R_magic()/150.)*ns;

  //G4cout << "RandT = " << randT << G4endl;

  double sigmat = GetSigmaT();
  //randT += G4RandGauss::shoot(0.,sigmat)*ns;
  if ( GetStartPerfect() ) {
    if ( sigmat < 0 ) {
      randT = G4RandGauss::shoot(100.,sigmat)*ns;
      if ( randT <= 0 ) { randT = 1*ns; }
    }
    else {
      randT = (G4UniformRand() * sigmat)*ns;
    }
  }
  else {
    randT += ((2*G4UniformRand()-1) * sigmat)*ns;
  }

  if ( sigmat == 0.0 ) {
    double randtime = randTFromE989();
    if ( GetStartPerfect() ) {
      randT = 100*ns + randtime;
    }
    else {
      randT += randtime;
    }

    if ( GetPolarization() == "Mixed" || GetPolarization() == "E821Mixed" || GetPolarization() == "FullMixed" || GetPolarization() == "Full-Mixed" ) {
      //G4cout << "Randtime: " << randtime << "\t" << 800*ns << "\t" << -20*ns << G4endl;
      if ( fabs(randtime) >= 80*ns ) { s.rotateY(2*TMath::Pi()/3); }
      else if ( fabs(randtime) <= 20*ns ) { s.rotateY(-2*TMath::Pi()/3); }
    }
  }

  inflectorGun_->SetParticleTime( randT );
    

  //G4cout << "me[x,x'] = " << (beam_start - beam_start_offset).x() << " , " << mom_start.dot(xAxis)/mom_start.mag() << G4endl;
  //G4cout << "me[y,y'] = " << (beam_start - beam_start_offset).y() << " , " << mom_start.dot(yAxis)/mom_start.mag() << G4endl;
    
  G4double me_x, me_y, me_xprime, me_yprime;
  me_x = (beam_start - beam_start_offset).x();
  me_y = (beam_start - beam_start_offset).y();
  me_xprime = mom_start.dot(xAxis)/mom_start.mag();
  me_yprime = mom_start.dot(yAxis)/mom_start.mag();

  sum_x += me_x;
  sum_y += me_y;
  sum_xprime += me_xprime;
  sum_yprime += me_yprime;
  ngen++;
    
  //  Load the gun
  inflectorGun_->SetParticlePosition( beam_start );

  //G4cout << "P-START: " << inflectorGun->GetParticlePosition() << G4endl;

  G4strstreambuf* oldBuffer = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
  inflectorGun_->SetParticleMomentum( mom_start );    

  G4cout.rdbuf(oldBuffer);
  //G4cout << "M-START: " << inflectorGun->GetParticleMomentumDirection() << G4endl;
  inflectorGun_->SetParticlePolarization( s );


  double mrad = 0.001;
  double x_cbo = 0.0;
  double xp_cbo = 0.0;
  h_xy->Fill(randX, randY);
  h_xprimex->Fill(randX, randXPrime/mrad);
  h_yprimey->Fill(randY, randYPrime/mrad);
  
  h_x_cbo->Fill(x_cbo);
  h_xp_cbo->Fill(xp_cbo/mrad);
  
  h_Ax->Fill(MaxX);
  h_Ay->Fill(MaxY);
  
  //h_pol->Fill(polV.dot(momV)/momV.mag());
  //h_time_pol->Fill(decaytime/microsecond, polV.dot(momV)/momV.mag());
  
  double xe = 0.0;
  h_xe->Fill(xe);
  h_x->Fill(randX);
  h_xp->Fill(randXPrime/mrad);
  h_y->Fill(randY);
  h_yp->Fill(randYPrime/mrad);

  double t0 = randT;
  double decaytime = 0.0;
  double totaltime = randT;
  double microsecond = 1e3;
  h_t0->Fill(t0);
  h_time->Fill(totaltime/microsecond);
  h_time_m_t0->Fill((decaytime-t0)/microsecond);
  
  double radianlength = 0.0;
  int counter = (int)150*(radianlength/TMath::TwoPi());
  G4double tmod = 0.14915*((float)counter/150);
  h_time_tmod->Fill((totaltime-tmod)/microsecond);
  h_time_decay_tmod->Fill((decaytime-tmod)/microsecond);
  
  double arc0 = std::fmod(radianlength, TMath::TwoPi());
  if ( fabs(arc0) < TMath::TwoPi()/150 ) {
    h_time_VS0->Fill((totaltime)/microsecond);
  }

  //G4cout << "x,x' = " << 

  //  FIRE

  //G4cout.precision(4);
  //G4cout << "Position: " << beam_start << G4endl;
  //G4cout << "Momentum: " << mom_start << G4endl;

  if ( first_event ) {
    G4cout << G4endl;
    G4cout << "====================================================================" << G4endl;
    G4cout << "=========================   INFLECTOR GUN   ========================" << G4endl;
    G4cout << "====================================================================" << G4endl;

    G4cout << "Pmagic = " << pMagic() << G4endl;
    G4cout << "Rmagic = " << R_magic() << G4endl;
    if ( GetGenGaussian() ) {
      G4cout << "Generating beam from a Gaussian Distribution." << G4endl;
    }
    else {
      G4cout << "Generating beam from a Uniform Distribution." << G4endl;
    }

    G4cout << "TurnCounter    = " << GetTurnCounter() << G4endl;
    if ( GetStartDownstream() ) {
      G4cout << "STARTING Downstream Of The Inflector Mandrel." << G4endl;
    }
    else if ( GetStartUpstream() ) {
      G4cout << "STARTING Upstream Of The Inflector Mandrel." << G4endl;
    }
    else if ( GetStartUpstreamCryo() ) {
      G4cout << "STARTING Upstream Of The Inflector Cryostat." << G4endl;
    }
    else if ( GetStartPerfect() ) {
      G4cout << "Rotation Angle = " << RotAngle << G4endl;
      if ( RotAngle > 0 || RotAngle < 0 ) {
	if ( GetStorageOffset() > 0 ) {
	  G4cout << "STARTING at " << R_magic()+GetStorageOffset() << "[cos(" << RotAngle << "), 0, sin(" << RotAngle << ")]" << G4endl;	
	}
	else {
	  G4cout << "STARTING at " << R_magic() << "[cos(" << RotAngle << "), 0, sin(" << RotAngle << ")]" << G4endl;	
	}
      }
      else {
	if ( GetStorageOffset() > 0 ) {
	  G4cout << "STARTING at " << R_magic()+GetStorageOffset() << "[1, 0, 0]" << G4endl;
	}
	else {
	  G4cout << "STARTING at " << R_magic() << "[1, 0, 0]" << G4endl;
	}
      }
    }
    else {
      G4cout << "STARTING POINT NOT SPECIFIED!!!!" << G4endl;
    }

    if ( GetSigmaT() < 0 ) {
      G4cout << "Gaussian t0 with sigma = " << GetSigmaT() << " ns." << G4endl;
    }
    else if ( GetSigmaT() > 0 ) {
      G4cout << "Flat t0  between [0, " << GetSigmaT() << "] ns." << G4endl;
    }
    else {
      G4cout << "FNAL t0 beam." << G4endl;
    }

    if ( GetFlatDecayTime() ) {
      G4double maxtau = GetMaxDecayTime();
      if ( maxtau <= 15000 ) {
	// assume this is actually the number of turns
	maxtau *= (0.16  * 190.0/231.0); // tau in microseconds
      }
      G4cout.precision(4);
      G4cout << "Decay time is flat between [0, " << maxtau << "] microseconds." << G4endl;
    }
    else {
      G4cout << "Decay time is exponential." << G4endl;
    }
    
    
    G4cout << "Inflector swing angle = " << infGeom_.swingAngle << " mrad." << G4endl;
    G4cout << "Launching muon beam with a " << launch_angle << " mrad kick." << G4endl;


      
    G4cout << "emittanceX = " << emittanceX << G4endl;
    G4cout << "emittanceY = " << emittanceY << G4endl;
    G4cout << "betaX      = " << betaX << G4endl;
    G4cout << "betaY      = " << betaY << G4endl;
    G4cout << "alphaX     = " << alphaX << G4endl;
    G4cout << "alphaY     = " << alphaY << G4endl;

    if ( GetGenGaussian() ) {
      G4cout << "sigma(x)  = " << sigmaX0 << G4endl;
      G4cout << "sigma(y)  = " << sigmaY0 << G4endl;
      G4cout << "sigma(x') = " << sigmaX0Prime << G4endl;
      G4cout << "sigma(y') = " << sigmaY0Prime << G4endl;
    }
    else {
      G4cout.precision(3);
      G4cout << "sqrt(emit*betaX) = " << std::sqrt( epsilonX*betaX0 ) << " mm." << G4endl;
      G4cout << "sqrt(emit*gamX)  = " << std::sqrt( epsilonX*gammaX0 )*1000 << " mrad." << G4endl;
      G4cout << "sqrt(emit*betaY) = " << std::sqrt( epsilonY*betaY0 ) << " mm." << G4endl;
      G4cout << "sqrt(emit*gamY)  = " << std::sqrt( epsilonY*gammaY0 )*1000 << " mrad." << G4endl;
    }
    G4cout << "Particle  = " << Particle_ << G4endl;
    G4cout << "<P>       = " << Pmean << " +/- " << Pmean * dP_over_P << " MeV" << G4endl;
    G4cout << "dP/P      = " << dP_over_P << G4endl;
    G4cout << "Spin:" << G4endl;
    G4cout << "x         = " << beam_start.x() << " , " << beam_start.y() << " , " << beam_start.z() << G4endl;
    G4cout << "p         = " << mom_start.x() << " , " << mom_start.y() << " , " << mom_start.z() << G4endl;
    G4cout << "|p|       = " << mom_start.mag() << G4endl;
    G4cout << "s         = " << s.x() << " , " << s.y() << " , " << s.z() << G4endl;
    G4cout << "|s|       = " << s.mag() << G4endl;
    G4cout << "s-dot-p   = " << s.dot(p)/(s.mag()*mom_start.mag()) << G4endl;

    G4double theta = ComputeTheta(beam_start.x(), beam_start.z());
    double pol_azimuth, pol_radial, pol_vertical;
    ComputePolarization(&pol_azimuth, &pol_radial, &pol_vertical, s.x(), s.y(), s.z(), theta);
    G4cout << "s(r,v,z)  = " << pol_radial << " , " << pol_vertical << " , " << pol_azimuth << G4endl;
   

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
    

    G4cout << "World Coordinates:" << G4endl;
    G4cout << "  Main Inflector Axis: " << dir << G4endl;
    G4cout << "  p-dot-inf   : " << dir.dot(mom_start.unit()) << G4endl;
    G4cout << "  xAxis.SetXYZ(" << xAxis.x() << " , " << xAxis.y() << " , " << xAxis.z() << ");" << G4endl;
    G4cout << "  yAxis.SetXYZ(" << yAxis.x() << " , " << yAxis.y() << " , " << yAxis.z() << ");" << G4endl;
    G4cout << "  zAxis.SetXYZ(" << zAxis.x() << " , " << zAxis.y() << " , " << zAxis.z() << ");" << G4endl;
    G4cout << "  p-dot-inf : " << dir.dot(mom_start.unit()) << G4endl;
    G4cout << "  p-dot-x   : " << xAxis.dot(mom_start.unit()) << G4endl;
    G4cout << "  p-dot-y   : " << yAxis.dot(mom_start.unit()) << G4endl;
    G4cout << "  p-dot-z   : " << zAxis.dot(mom_start.unit()) << G4endl;
    G4cout << "  beamStart.SetXYZ(" << beam_start_offset.x() << " , " << beam_start_offset.y() << " , " << beam_start_offset.z() << ");" << G4endl;
    G4cout << "  rhat_offset = " << sqrt(beam_start_offset.x()*beam_start_offset.x() + beam_start_offset.z()*beam_start_offset.z()) - R_magic() << ";" << G4endl;
    G4cout << "  yhat_offset = " << beam_start_offset.y() - 0.0 << ";" << G4endl;
    G4cout << G4endl;
    first_event = false;
  }



  //  FIRE
  if ( 0 ) {
  G4cout << "ID: " << evt->GetEventID() << G4endl;
  G4cout << "To be Kept: " << evt->ToBeKept() << G4endl;
  G4cout << "NumPV: " << evt->GetNumberOfPrimaryVertex() << G4endl;
  G4cout << "IsAborted: " << evt->IsAborted() << G4endl;
  G4cout << "G2InflectorSource: About to call inflectorGun_->GeneratePrimaryVertex" << G4endl;
  }
  inflectorGun_->GeneratePrimaryVertex( evt );

  if ( GetFlatDecayTime() ) {

    G4PrimaryVertex *primary_vtx = evt->GetPrimaryVertex();
    G4PrimaryParticle *primary = primary_vtx->GetPrimary();
    if ( primary ) {
      //primary->Print();
      //G4double tau = primary->GetProperTime();
      //G4cout << "Proper Time = " << tau << G4endl;
      G4double maxtau = GetMaxDecayTime();
//       G4double gamma_mu = mu_gamma;
//       G4double beta_mu  = mu_beta;
//       G4double LdecayMagic = gamma_mu * beta * 
// 	maxtau * Rmagic *TMath::TwoPi();
//       G4double Ldecay = LdecayMax + 1;
//       G4double Maxturns = maxtau;
//       G4double 

//       while ( Ldecay > LdecayMax ) {
// 	G4double newtau = G4UniformRand(); 
// 	Ldecay = gamma_mu * beta_mu * c_light * newtau
      
      // Decay length of magic momentum muon for N turns is Rmagic*N = 

      // Lab time is gamma * tau (tau = 2.2 microseconds)
      // Decay Length = gamma * beta * c * tau

      if ( maxtau <= 15000 ) {
	// assume this is actually the number of turns
	maxtau /= (gm2ringsim::omegaCMagic()/TMath::TwoPi());
	//G4cout << maxtau << G4endl;
	//maxtau *= (0.160   * 190.0/231.0); // tau in microseconds
	maxtau /= mu_gamma; // decay time in the lab is gamma*tau so we remove that feature here
      }
      maxtau *= G4UniformRand();      

      //double ratio = ( mu_gamma * mu_beta ) / ( gammaMagic() * betaMagic() );
      //double ratio = ( mu_gamma ) / ( gammaMagic() );
      //maxtau /= ratio;

      primary->SetProperTime(maxtau); // *microsecond);

      G4double Ldecay = maxtau * c_light * mu_gamma * mu_beta;
      G4double N2piR  = TMath::TwoPi() * R_magic();

      //G4cout << mu_gamma << "\t" << gammaMagic() << "\t" << ratio << G4endl;
      //G4cout << "Ldecay/N2piR = " << Ldecay << "\t" << N2piR << "\t";
      if ( 0 ) {
	if ( Ldecay > N2piR ) {
	  G4cout << "\tBIGGER" << G4endl;
	}
	else {
	  G4cout << G4endl;
	}
      }

//       G4cout << G4endl;
//       G4cout.precision(6);
//       G4cout << "Proper Time in GenEvt = " << primary->GetProperTime() << G4endl;
    }
  }
  if ( 0 ) {
    G4cout << "G2InflectorSource: About to call inflectorGun_->GeneratePrimaryVertex" << G4endl;
    G4cout << "ID: " << evt->GetEventID() << G4endl;
    G4cout << "To be Kept: " << evt->ToBeKept() << G4endl;
    G4cout << "NumPV: " << evt->GetNumberOfPrimaryVertex() << G4endl;
    G4cout << "IsAborted: " << evt->IsAborted() << G4endl;
  }
} // generatePrimaries


G4double gm2ringsim::G2InflectorSource::randTFromE989()
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

