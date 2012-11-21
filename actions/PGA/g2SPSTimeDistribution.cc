/** @file g2SPSTimeDistribution.cc

    Implements the General Particle Source time distribution
    interface. 

    - Zach Hartwig 2005
*/

#include "Geant4/Randomize.hh"

#include "gm2ringsim/actions/PGA/g2SPSTimeDistribution.hh"

g2SPSTimeDistribution::g2SPSTimeDistribution()
{
  // Initialise all variables
  particle_time = 0.;
  
  timeDisType = "Mono";
  timeMono = 109.*ns;
  timeMin = 0.;
  timeMax = 0.*ns;
  timeSE = 0.*ns;
  timeGrad = 0.;
  timeCept = 0.;
  
  IPDFEnergyExist = false;
  IPDFArbExist = false;

  verbosityLevel = 0 ;
}

g2SPSTimeDistribution::~g2SPSTimeDistribution()
{}


void g2SPSTimeDistribution::SetTimeDisType(G4String DisType)
{
  timeDisType = DisType;
}


void g2SPSTimeDistribution::SetTimeMin(G4double tMin)
{
  timeMin = tMin;
}


void g2SPSTimeDistribution::SetTimeMax(G4double tMax)
{
  timeMax = tMax;
}


void g2SPSTimeDistribution::SetTimeMono(G4double tMono)
{
  timeMono = tMono;
}


void g2SPSTimeDistribution::SetBeamSigmaInT(G4double t)
{
  timeSE = t;
}


void g2SPSTimeDistribution::SetTimeZero(G4double zero)
{
  timeZero = zero;
}


void g2SPSTimeDistribution::SetTimeGradient(G4double grad)
{
  timeGrad = grad;
}


void g2SPSTimeDistribution::SetTimeInterCept(G4double cept)
{
  timeCept = cept;
}


void g2SPSTimeDistribution::GenerateMonoChronologic()
{
  // Method to generate MonoEnergetic particles.
  particle_time = timeMono;
}


void g2SPSTimeDistribution::GenerateGaussEnergies()
{
  // Method to generate Gaussian particles.
  particle_time = G4RandGauss::shoot(timeMono,timeSE);
  if (particle_time < 0) particle_time = 0.;
}


void g2SPSTimeDistribution::GenerateLinearEnergies(G4bool bArb = false)
{
  G4double rndm;
  G4double timeMaxsq = std::pow(timeMax,2.); //Emax squared
  G4double timeMinsq = std::pow(timeMin,2.); //Emin squared
  G4double intersq = std::pow(timeCept,2.); //cept squared
  
  if (bArb) rndm = G4UniformRand();
  else rndm = timeRndm->GenRandEnergy();
  
  G4double bracket = ((timeGrad/2.)*(timeMaxsq - timeMinsq) + timeCept*(timeMax-timeMin));
  bracket = bracket * rndm;
  bracket = bracket + (timeGrad/2.)*timeMinsq + timeCept*timeMin;
  // Now have a quad of form m/2 E**2 + cE - bracket = 0
  bracket = -bracket;
  //  G4cout << "BRACKET" << bracket << G4endl;
  if(timeGrad != 0.)
    {
      G4double sqbrack = (intersq - 4*(timeGrad/2.)*(bracket));
      //      G4cout << "SQBRACK" << sqbrack << G4endl;
      sqbrack = std::sqrt(sqbrack);
      G4double root1 = -timeCept + sqbrack; 
      root1 = root1/(2.*(timeGrad/2.));
      
      G4double root2 = -timeCept - sqbrack;
      root2 = root2/(2.*(timeGrad/2.));
      
      //      G4cout << root1 << " roots " << root2 << G4endl;

      if(root1 > timeMin && root1 < timeMax)
	particle_time = root1;
      if(root2 > timeMin && root2 < timeMax)
	particle_time = root2;
    }
  else if(timeGrad == 0.)
    // have equation of form cE - bracket =0
    particle_time = bracket/timeCept;

  if(particle_time < 0.)
    particle_time = -particle_time;
  
  if(verbosityLevel >= 1)
    G4cout << "Time is " << particle_time << G4endl;
}


G4double g2SPSTimeDistribution::GenerateOne(G4ParticleDefinition* a)
{
  particle_definition = a;
  particle_time = -1.;
  //  while ( (timeDisType == "Arb")?  : (particle_time < timeMin || particle_time > timeMax) ) { 
    if(timeDisType == "Mono")
      GenerateMonoChronologic();
    else if(timeDisType == "Lin")
      GenerateLinearEnergies();
    else if(timeDisType == "Gauss")
      GenerateGaussEnergies();
    else
      G4cout << "Error: TimeDisType has unusual value" << G4endl;
    //}
  return particle_time;
}









