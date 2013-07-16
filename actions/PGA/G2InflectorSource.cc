// Get the PGA header
//#include "gm2ringsim/actions/PGA/obsoletePrimaryGeneratorAction_service.hh"

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

#include "gm2ringsim/actions/PGA/G2InflectorSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"

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
  EmittanceX(40.0),
  EmittanceY(40.0),
  BetaX(19.1),
  BetaY(7.6),
  AlphaX(0.0),
  AlphaY(0.0),
  Pmean(0.005),
  dPOverP(0.005),
  SigmaT(50),
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
  alphaX = GetAlphaX()*m;
  alphaY = GetAlphaY()*m;




  G4ThreeVector storage_orbit(R_magic(), 0., 0.);
  G4double storage_offset = GetStorageOffset();
  if ( storage_offset > 0 ) { storage_orbit += G4ThreeVector(storage_offset,0.,0.); }
    

  // CALCULATE THE DOWNSTREAM MANDREL VERTEX POSITION
    

  inflectorGeometry const& ig = infGeom_.ig ;//inflectorGeometry::getInstance();
  G4ThreeVector downstream(R_magic()+ig.aperture_off(), 0., 0.);
  downstream.rotateY(ig.delta());
    
    
  // CALCULATE THE UPSTREAM MANDREL VERTEX POSITION
    
  G4ThreeVector upstream(0., 0., -ig.mandrel_length());  
  upstream.rotateX(-ig.zeta()); // why negative?  WTF?
  upstream.rotateY(ig.gamma());
  upstream += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream.rotateY(ig.delta());
    
  
  G4ThreeVector upstream_cryo(0., 0., -ig.length()+ig.window_thickness()+ig.conductor_thickness() );
  upstream_cryo.rotateX(-ig.zeta()); // why negative?  WTF?
  upstream_cryo.rotateY(ig.gamma());
  upstream_cryo += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_cryo.rotateY(ig.delta());


  // Direction of inflector axis
  G4ThreeVector dir = (downstream - upstream).unit();



  //  First, calculate gamma from the user's parameters using the fact that beta*gamma - alpha^2 = 1 is invariant
  gammaX = ( 1+alphaX*alphaX )/betaX;
  gammaY = ( 1+alphaY*alphaY )/betaY;

    
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



  //  Next, use the above parameters to determine the point s{X,Y} at which the phase-space ellipses have alpha{X,Y} = 0.
  //  This serves to define what I will call the "naught" positions, i.e. the positions at which the phase-space ellipses
  //  are standing "upright" in phase space (alpha{X,Y}=0).  It is very easy to generate random positions/momenta at the
  //  naught coordinates, which can then easily be propagted back to the injection point.
  G4double sX = -alphaX/gammaX;
  G4double sY = -alphaY/gammaY;


  //  Calculate beta{X,Y}0
  G4double betaX0 = betaX + 2*alphaX*sX + gammaX*sX*sX;
  G4double betaY0 = betaY + 2*alphaY*sY + gammaY*sY*sY;


  //  Calculate gamma{X,Y}0 given that alpha{X,Y}=0 using the invariant (see above).
  G4double gammaX0 = 1/betaX0;
  G4double gammaY0 = 1/betaY0;


  G4double sigmaX0 = 0.0;
  G4double sigmaY0 = 0.0;
  G4double sigmaX0Prime = 0.0;
  G4double sigmaY0Prime = 0.0;   
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
    yAxis = G4ThreeVector(0, 1, 0);
    zAxis = G4ThreeVector(0, 0, 1);
  }



  G4ThreeVector xy_gen(randX, randY, 0.0);

  G4ThreeVector storage_orbit_start(R_magic(), 0.0, 0.0);
  if ( storage_offset > 0 ) { storage_orbit_start += G4ThreeVector(storage_offset,0.,0.); }
  storage_orbit_start += xy_gen;
  
  G4ThreeVector downstream_start(R_magic()+ig.aperture_off(), 0.0, 0.0);
  downstream_start += xy_gen;
  downstream_start.rotateY(ig.delta());
  
  G4ThreeVector upstream_start(0., 0., -ig.mandrel_length());  
  upstream_start.rotateX(-ig.zeta()); // why negative?  WTF?
  upstream_start.rotateY(ig.gamma());
  upstream_start += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_start += xy_gen;
  upstream_start.rotateY(ig.delta());
      
  G4ThreeVector upstream_cryo_start(0., 0., -ig.length()+ig.window_thickness()+ig.conductor_thickness() );
  upstream_cryo_start.rotateX(-ig.zeta()); // why negative?  WTF?
  upstream_cryo_start.rotateY(ig.gamma());
  upstream_cryo_start += G4ThreeVector(R_magic()+ig.aperture_off(),0.,0.);
  upstream_cryo_start += xy_gen;
  upstream_cryo_start.rotateY(ig.delta());


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

  G4double pX   = pMagic()*G4RandGauss::shoot( 1, dP_over_P ); // "downstream" @12 o'clock
  //G4double pX   = pMagic()*G4RandGauss::shoot( 1, 0.0015 ); // "downstream" @12 o'clock
  //G4double pX   = pMagic()*G4RandGauss::shoot( 1, 0.000015 ); // "downstream" @12 o'clock
  G4double pMag = pX/std::sqrt( 1-randXPrime*randXPrime-randYPrime*randYPrime );
  G4double pY   = randXPrime*pMag; // radial   @12 o'clock
  G4double pZ   = randYPrime*pMag; // vertical @lways
  G4ThreeVector p = G4ThreeVector(pX,pY,pZ);

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

  
  if ( GetPolarization() == "E821" ) {
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
  else if (  GetPolarization() == "100%" ) {
    s = mom_start.unit();
  }
  else {
    s.setX(gRandom_->Uniform());
    s.setY(gRandom_->Uniform());
    s.setZ(gRandom_->Uniform());
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
  randT += G4RandGauss::shoot(0.,sigmat)*ns;
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
      if ( GetStorageOffset() > 0 ) {
	G4cout << "STARTING at [R_magic() + " << GetStorageOffset() << ", 0, 0]" << G4endl;	
      }
      else {
	G4cout << "STARTING at [R_magic(), 0, 0]" << G4endl;	
      }
    }
    else {
      G4cout << "STARTING POINT NOT SPECIFIED!!!!" << G4endl;
    }

    G4cout << "sigma(t0) = " << GetSigmaT() << " ns." << G4endl;
      
    if ( launch_angle > 0 || launch_angle < 0 ) {
      G4cout << "Launching muon beam with a " << 1000*launch_angle << " mrad kick." << G4endl;
    }

      
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
      G4cout << "sqrt(emit*betaX) = " << std::sqrt( epsilonX*betaX0 ) << G4endl;
      G4cout << "sqrt(emit*gamX)  = " << std::sqrt( epsilonX*gammaX0 ) << G4endl;
      G4cout << "sqrt(emit*betaY) = " << std::sqrt( epsilonY*betaY0 ) << G4endl;
      G4cout << "sqrt(emit*gamY)  = " << std::sqrt( epsilonY*gammaY0 ) << G4endl;
    }
    G4cout << "<P>       = " << Pmean << " +/- " << Pmean * dP_over_P << " MeV" << G4endl;
    G4cout << "dP/P      = " << dP_over_P << G4endl;
    G4cout << "s-dot-p   = " << s.x() << " , " << s.y() << " , " << s.z() << endl;
    G4cout << "Particle  = " << Particle_ << G4endl;

    if ( GetPolarization() == "E821" ) {
      G4cout << "Using E821 Hughon Polarization Fits." << G4endl;
    }
    else if ( GetPolarization() == "100%" ) {
      G4cout << "Assuming 100 polarization (s-dot-p=1)." << G4endl;
    }
    else {
      G4cout << "Random Polarization." << G4endl;
    }
    

    G4cout << "Inflector Coordinates:" << G4endl;
    G4cout << "  xAxis.SetXYZ(" << xAxis.x() << " , " << xAxis.y() << " , " << xAxis.z() << ");" << G4endl;
    G4cout << "  yAxis.SetXYZ(" << yAxis.x() << " , " << yAxis.y() << " , " << yAxis.z() << ");" << G4endl;
    G4cout << "  zAxis.SetXYZ(" << zAxis.x() << " , " << zAxis.y() << " , " << zAxis.z() << ");" << G4endl;
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
  if ( 0 ) {
    G4cout << "G2InflectorSource: About to call inflectorGun_->GeneratePrimaryVertex" << G4endl;
    G4cout << "ID: " << evt->GetEventID() << G4endl;
    G4cout << "To be Kept: " << evt->ToBeKept() << G4endl;
    G4cout << "NumPV: " << evt->GetNumberOfPrimaryVertex() << G4endl;
    G4cout << "IsAborted: " << evt->IsAborted() << G4endl;
  }
} // generatePrimaries
