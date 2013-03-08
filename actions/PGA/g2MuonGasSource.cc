// Get the PGA header
#include "gm2ringsim/actions/PGA/PrimaryGeneratorAction_service.hh"

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

// g2migtrace helpers

#include "gm2ringsim/actions/PGA/g2MuonGasSource.hh"
#include "gm2ringsim/common/g2PreciseValues.hh"

#define randFromDistr CLHEP::RandGeneral

#include <iostream>
#include <cmath>

using std::endl;

// Constructor
gm2ringsim::g2MuonGasSource::g2MuonGasSource() :
  muonGasGun_(),
  g2GPS_(),
  muonGasVerbosity_ (0)
{
  muonGasGun_ = new G4ParticleGun();
  g2GPS_ = new g2GeneralParticleSource();
}

gm2ringsim::g2MuonGasSource::~g2MuonGasSource() 
{ }

void gm2ringsim::g2MuonGasSource::SetVerbosity(bool verbose){
  muonGasVerbosity_ = verbose;
}

// EndOfPrimaryGeneratorAction
void gm2ringsim::g2MuonGasSource::GeneratePrimaryVertex(G4Event* evt) {

  mf::LogInfo("g2MuonGasSource") << "GeneratePrimaryVertex";
  static G4double particleLifetime=0; // initialize
  static G4bool firstVertex = true;
  
  if( firstVertex ){
    G4String defaultParticle = "mu-";
    G4String particle = g2GPS_ ->GetParticleDefinition()->GetParticleName();
    
    if( g2GPS_ ->GetParticleDefinition()!=NULL && particle!=defaultParticle ){
      muonGasGun_ ->SetParticleDefinition( g2GPS_ ->GetParticleDefinition() );
    } else {
      muonGasGun_ ->SetParticleDefinition( G4ParticleTable::GetParticleTable()->FindParticle(defaultParticle) );
    }
    
    // Store PDGLifeTime and then define it to be 0 so we can decay the muons manually
    particleLifetime = muonGasGun_ ->GetParticleDefinition()->GetPDGLifeTime();
    muonGasGun_ ->GetParticleDefinition()->SetPDGLifeTime(0.);
    
    firstVertex = false;
  }
  
  //  First, generate an initial transverse position in local storage-region (SR) coordinates.     
  //  The coordinate system used here is the standard one found in accelerator physics texts,
  //  i.e. "z" = longitudinal, "x" = radial, and "y" = vertical.
  
  G4double randX_SR = 0.*mm; // radial
  G4double randY_SR = 0.*mm; // vertical     
  
  
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
  
  
  if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="e989" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E989";
    randT = randTFromE989();
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="e821" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: E821";
    do{
      randT = G4RandGauss::shoot( 0., 25. )*ns;
    } while( abs(randT)>50. );
    randT += -50.;
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="uniform" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: UNIFORM";
    randT = ( G4UniformRand()-1. )*100.*ns;
    
  } else if( g2GPS_->GetMuonGasGunLongitudinalDistr()=="uniformRing" ){
    mf::LogInfo("PGA") << "LONGITUDINAL PROFILE: UNIFORMRING";
    randT = ( G4UniformRand()*CLHEP::twopi*R_magic()/(betaMagic()*c_light) );
    
  }
  else {
    mf::LogInfo("PGA") << "NO LOGITUDINAL PROFILE SPECIFIED.\n"
  		       << "Using default value for randT:"<<randT;
  }
  
  
  //  Generate a random decay time by inverting Exp[ -t/gamma*tau ], and add it to the random tInitial from above.
  G4double randNum = G4UniformRand();
  randT += -gammaMagic() * particleLifetime * log(randNum);
  
  
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
  
  // Load the muonGasGun.                                                           
  muonGasGun_->SetParticleTime( randT );
  muonGasGun_->SetParticlePosition( G4ThreeVector( g2r.x(), g2r.y(), g2r.z() ) );
  muonGasGun_->SetParticleMomentum( G4ThreeVector( g2p.x(), g2p.y(), g2p.z() ) );
  muonGasGun_->SetParticlePolarization( G4ThreeVector( g2s.x(), g2s.y(), g2s.z() ) );
  
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
  mf::LogInfo("g2MuonGasSource") << "About to call muonGasGun_->GeneratePrimaryVertex";  
  muonGasGun_->GeneratePrimaryVertex( evt );
    mf::LogInfo("g2MuonGasSource") << "Just got back from calling muonGasGun_->GeneratePrimaryVertex";  
} // generatePrimaries


G4double gm2ringsim::g2MuonGasSource::randXFromE821()
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

G4double gm2ringsim::g2MuonGasSource::randTFromE989()
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

