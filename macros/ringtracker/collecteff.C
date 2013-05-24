#include <map>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <iterator>

#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TMath.h"
#include "TStopwatch.h"
#include "TLorentzVector.h"

string nice_deltaname;
string nice_launchname;
string nice_fieldname;
string nice_inflectorname;
string nice_kickname;
string nice_beamstartname;
string nice_prhatname;
string nice_sigmapname;
string nice_tsigmaname;

bool is_lcrkick;
bool is_squarekick;
bool is_nokick;
bool is_scrapingoff;
bool is_uniform;
bool is_101turns;
bool is_fullhighmom;
bool is_fullhighermom;
bool is_fullhighestmom;
bool is_newsupports;
bool is_noquads;
bool is_noquadsupports;
bool is_mappedfield;
bool is_openinflector;
bool is_closedinflector;
bool is_partiallyopeninflector;
bool is_upstreamcryo;
bool is_upstreammandrel;
bool is_downstreamcryo;
bool is_downstreammandrel;
bool is_centralorbit_offset77;
bool is_centralorbit;
bool is_nosigmap;
bool is_perfectmatch;
bool is_perfectmatch_dP05;
bool is_perfectmatch_dP005;
bool is_perfectmatch_dP1;
bool is_perfectmatch_dP001;
bool is_perfectmatch_dP0001;
bool is_perfectmatch_dP2;
bool is_perfectmatch_dP5;
bool is_perfectmatch_dP10;
bool is_perfectmatch_dP0075;
bool is_perfectmatch_dP025;
bool is_perfectmatch_dP075;
bool is_perfectmatch_dP0025;
bool is_bnlmatch;
bool is_bnlmatch_dP05;
bool is_bnlmatch_dP005;
bool is_bnlmatch_dP1;
bool is_bnlmatch_dP001;
bool is_bnlmatch_dP0001;
bool is_bnlmatch_dP2;
bool is_bnlmatch_dP5;
bool is_bnlmatch_dP10;
bool is_bnlmatch_dP0075;
bool is_bnlmatch_dP025;
bool is_bnlmatch_dP075;
bool is_bnlmatch_dP0025;
bool is_sigmap02;
bool is_sigmap05;
bool is_noprhat;
bool is_prhat1mrad;
bool is_prhat2mrad;
bool is_prhat5mrad;
bool is_found;
bool is_perfect;
bool is_tsigma1;
bool is_tsigma25;
bool is_tsigma50;
int sigmatnum;
int offsetnum;
int kicknum;
int beamsizenum;
int deltanum;
int launchnum;
double prhatnum;
double sigmapnum;



void NiceName(string name)
{
  is_found = false;
  stringstream deltaname;
  stringstream launchname;
  stringstream fieldname;
  stringstream inflectorname;
  stringstream kickname;
  stringstream beamstartname;
  stringstream prhatname;
  stringstream sigmapname;
  stringstream tsigmaname;
  stringstream beamsizename;
  
  is_scrapingoff = false;
  is_uniform = false;
  is_101turns = false;
  is_fullhighmom = false;
  is_fullhighermom = false;
  is_fullhighestmom = false;
  is_newsupports = false;
  is_noquads = false;
  is_noquadsupports = false;
  is_mappedfield = false;
  is_openinflector = false;
  is_closedinflector = false;
  is_partiallyopeninflector = false;
  is_upstreamcryo = false;
  is_upstreammandrel = false;
  is_downstreammandrel = false;
  is_downstreamcryo = false;  
  is_noprhat = false;
  is_prhat1mrad = false;
  is_prhat2mrad = false;
  is_prhat5mrad = false;
  is_nosigmap = false;
  is_sigmap02 = false;
  is_sigmap05 = false;
  is_perfect = false;
  is_perfectmatch = false;
  is_perfectmatch_dP05 = false;
  is_perfectmatch_dP005 = false;
  is_perfectmatch_dP1 = false;
  is_perfectmatch_dP001 = false;
  is_perfectmatch_dP0001 = false;
  is_perfectmatch_dP2 = false;
  is_perfectmatch_dP5 = false;
  is_perfectmatch_dP10 = false;
  is_perfectmatch_dP0075 = false;
  is_perfectmatch_dP025 = false;
  is_perfectmatch_dP075 = false;
  is_perfectmatch_dP0025 = false;
  is_bnlmatch_dP05 = false;
  is_bnlmatch_dP005 = false;
  is_bnlmatch_dP1 = false;
  is_bnlmatch_dP001 = false;
  is_bnlmatch_dP0001 = false;
  is_bnlmatch_dP2 = false;
  is_bnlmatch_dP5 = false;
  is_bnlmatch_dP10 = false;
  is_bnlmatch_dP0075 = false;
  is_bnlmatch_dP025 = false;
  is_bnlmatch_dP075 = false;
  is_bnlmatch_dP0025 = false;
  is_tsigma1 = false;
  is_tsigma50 = false;
  offsetnum = -1;
  kicknum = -1;
  beamsizenum = -1;
  deltanum = -99;
  launchnum = -99;
  prhatnum = -1;
  sigmapnum = -1.0;
  sigmatnum = -1;
  
  
  stringstream testname;
  testname << "NoDelta";
  if ( name.find(testname.str()) != string::npos ) {
    deltaname.str("");
    deltaname << "d = 0 mrad";
    deltanum = 0;
  }
  testname.str("");
  testname << "Delta0mrad";
  if ( name.find(testname.str()) != string::npos ) {
    deltaname.str("");
    deltaname << "d = 0 mrad";
    deltanum = 0;
  }
  testname.str("");
  testname << "Delta0deg";
  if ( name.find(testname.str()) != string::npos ) {
    deltaname.str("");
    deltaname << "d = 0 deg";
    deltanum = 0;
  }
  testname.str("");
  for ( int i = 50; i > 0; i-- ) {
    testname << "Deltap" << i << "mrad";
    if ( name.find(testname.str()) != string::npos ) {
      deltaname.str("");
      deltaname << "d = +" << i << " mrad";
      deltanum = i;
      break;
    }
    testname.str("");
    testname << "Deltap" << i << "deg";
    if ( name.find(testname.str()) != string::npos ) {
      deltaname.str("");
      deltaname << "d = +" << i << " deg";
      deltanum = i;
      break;
    }
    testname.str("");
    testname << "Deltam" << i << "mrad";
    if ( name.find(testname.str()) != string::npos ) {
      deltaname.str("");
      deltaname << "d = -" << i << " mrad";
      deltanum = -1*i;
      break;
    }
    testname.str("");
  }
  if ( deltaname.str().size() == 0 ) {
    deltaname << "d = 0 deg";
  }
    
  testname.str("");
  for ( int i = 0; i < 10; i ++ ) {
    testname << "BeamSize" << i << "pi";
    if ( name.find(testname.str()) != string::npos ) {
      beamsizename.str("");
      beamsizename << "Beamsize = +" << i << " pi mm*mrad";
      beamsizenum = i;
      break;
    }
    testname.str("");
  }
    
  testname.str("");
  for ( int i = 10; i <= 100; i += 10 ) {
    testname << "BeamSize" << i << "pi";
    if ( name.find(testname.str()) != string::npos ) {
      beamsizename.str("");
      beamsizename << "Beamsize = +" << i << " pi mm*mrad";
      beamsizenum = i;
      break;
    }
    testname.str("");
  }

  testname.str("");
  testname << "NoKick";
  if ( name.find(testname.str()) != string::npos ) {
    kickname.str("");
    kickname << "Kick = 0 kV";
    kicknum = 0;
    is_nokick = true;
  }
  testname.str("");
  for ( int i = 120; i >= 0; i -= 5 ) {
    testname << i << "kVLCRKick";
    if ( name.find(testname.str()) != string::npos ) {
      kickname.str("");
      kickname << "Kick = +" << i << " kV";
      kicknum = i;
      is_lcrkick = true;
      break;
    }
    testname.str("");
  }
  testname.str("");
  for ( int i = 350; i >= 25; i -= 5 ) {
    testname << i << "GSquareKick";
    if ( name.find(testname.str()) != string::npos ) {
      kickname.str("");
      kickname << "Kick = +" << i << " G";
      kicknum = i;
      is_squarekick = true;
      break;
    }
    testname.str("");
  }

  //cout << "kickname = " << kickname.str() << endl;

  testname.str("");
  testname << "Launch0mrad";
  if ( name.find(testname.str()) != string::npos ) {
    launchname.str("");
    launchname << "Launch = 0 mrad ";
    launchnum = 0;
  }
  testname.str("");
  testname << "NoLaunch";
  if ( name.find(testname.str()) != string::npos ) {
    launchname.str("");
    launchname << "Launch = 0 mrad ";
    launchnum = 0;
  }
  testname.str("");
  for ( int i = 50; i > 0; i-- ) {
    testname << "Launchp" << i << "mrad";
    if ( name.find(testname.str()) != string::npos ) {
      launchname.str("");
      launchname << "Launch = +" << i << " mrad";
      launchnum = i;
    }
    testname.str("");
    testname << "Launchm" << i << "mrad";
    if ( name.find(testname.str()) != string::npos ) {
      launchname.str("");
      launchname << "Launch = -" << i << " mrad";
      launchnum = -1*i;
    }
    testname.str("");
  }

  if ( launchname.str().size() == 0 ) {
    launchname << "Launch = 0 mrad ";
  }

  testname.str("");
  testname << "PerfectStorage";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    is_perfect = true;
    
    testname.str("");
    for ( int i = 80; i >= 0; i -= 5  ) {
      testname << "Offset" << i << "mm";
      if ( name.find(testname.str()) != string::npos ) {
	beamstartname << "BeamStart = " << i << " mm";
	offsetnum = i;
	break;
      }
      testname.str("");
    }
    testname << "Offset77mm";
    if ( name.find(testname.str()) != string::npos ) {
      beamstartname << "BeamStart = 77 mm";
      offsetnum = 77;
    }
  }

  testname.str("");
  testname << "MappedField";
  if ( name.find(testname.str()) != string::npos ) {
    fieldname.str("");
    fieldname << "Field = Mapped";
    is_mappedfield = true;
  }
  else {
    fieldname.str("");
    fieldname << "Field = None";
  }

  testname.str("");
  testname << "ScrapingOFF";
  if ( name.find(testname.str()) != string::npos ) {
    is_scrapingoff = true;
  }
  else {
    is_scrapingoff = false;
  }

  testname.str("");
  testname << "UniformInflectorGun";
  if ( name.find(testname.str()) != string::npos ) {
    is_uniform = true;
  }
  else {
    is_uniform = false;
  }

  testname.str("");
  testname << "101Turns";
  if ( name.find(testname.str()) != string::npos ) {
    is_101turns = true;
  }
  else {
    is_101turns = false;
  }

  testname.str("");
  testname << "NoQuads";
  if ( name.find(testname.str()) != string::npos ) {
    is_noquads = true;
  }
  else {
    is_noquads = false;
  }

  testname.str("");
  testname << "NoQuadSupport";
  if ( name.find(testname.str()) != string::npos ) {
    is_noquadsupports = true;
  }
  else {
    is_noquadsupports = false;
  }

  testname.str("");
  testname << "NewSupport";
  if ( name.find(testname.str()) != string::npos ) {
    is_newsupports = true;
  }
  else {
    is_newsupports = false;
  }

  testname.str("");
  testname << "FullHighMom";
  if ( name.find(testname.str()) != string::npos ) {
    is_fullhighmom = true;
  }
  else {
    is_fullhighmom = false;
  }

  testname.str("");
  testname << "FullHigherMom";
  if ( name.find(testname.str()) != string::npos ) {
    is_fullhighermom = true;
  }
  else {
    is_fullhighermom = false;
  }

  testname.str("");
  testname << "FullHighestMom";
  if ( name.find(testname.str()) != string::npos ) {
    is_fullhighestmom = true;
  }
  else {
    is_fullhighestmom = false;
  }

  testname.str("");
  testname << "InflectorOpen";
  if ( name.find(testname.str()) != string::npos ) {
    inflectorname.str("");
    inflectorname << "Inflector = Open";
    is_openinflector = true;
  }
  testname.str("");
  testname << "InflectorClosed";
  if ( name.find(testname.str()) != string::npos ) {
    inflectorname.str("");
    inflectorname << "Inflector = Closed";
    is_closedinflector = true;
  }
  testname.str("");
  testname << "ClosedInflector";
  if ( name.find(testname.str()) != string::npos ) {
    inflectorname.str("");
    inflectorname << "Inflector = Closed";
    is_closedinflector = true;
  }
  testname.str("");
  testname << "PartiallyOpen";
  if ( name.find(testname.str()) != string::npos ) {
    inflectorname.str("");
    inflectorname << "Inflector = PartiallyOpen";
    is_partiallyopeninflector = true;
  }

  testname.str("");
  testname << "UpstreamCryo";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    beamstartname << "Beamstart = Upstream Cryo";
    is_upstreamcryo = true;
  }
  testname.str("");
  testname << "UpstreamMandrel";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    beamstartname << "Beamstart = Upstream Mandrel";
    is_upstreammandrel = true;
  }
  testname.str("");
  testname << "DownstreamCryo";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    beamstartname << "Beamstart = Downstream Cryo";
    is_downstreamcryo = true;
  }
  testname.str("");
  testname << "DownstreamMandrel";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    beamstartname << "Beamstart = Downstream Mandrel";
    is_downstreammandrel = true;
  }
  testname.str("");
  testname << "CentralOrbit_Offset77";
  if ( name.find(testname.str()) != string::npos ) {
    beamstartname.str("");
    beamstartname << "Beamstart = Central Orbit w/ Offset 77 mm";
    is_centralorbit_offset77 = true;
  }
  if ( is_centralorbit_offset77 == false ) {
    testname.str("");
    testname << "CentralOrbit";
    if ( name.find(testname.str()) != string::npos ) {
      beamstartname.str("");
      beamstartname << "Beamstart = Central Orbit";;
      is_centralorbit = true;
    }
  }


  testname.str("");
  testname << "Prhat1mrad";
  if ( name.find(testname.str()) != string::npos ) {
    prhatname.str("");
    prhatname << "Px/P = 1 mrad";
    is_prhat1mrad = true;
    prhatnum = 1;
  }
  testname.str("");
  testname << "Prhat2mrad";
  if ( name.find(testname.str()) != string::npos ) {
    prhatname.str("");
    prhatname << "Px/P = 2 mrad";
    is_prhat2mrad = true;
    prhatnum = 2;
  }
  testname.str("");
  testname << "Prhat5mrad";
  if ( name.find(testname.str()) != string::npos ) {
    prhatname.str("");
    prhatname << "Px/P = 5 mrad";
    is_prhat5mrad = true;
    prhatnum = 5;
  }
  testname.str("");
  testname << "NoPrhat";
  if ( name.find(testname.str()) != string::npos ) {
    prhatname.str("");
    prhatname << "Px/P = 0 mrad";
    is_noprhat = true;
    prhatnum = 0;
  }

  testname.str("");
  testname << "PerfectMatch";
  if ( name.find(testname.str()) != string::npos ) {
    bool foundit = false;
    for ( int dP = 0; dP < 12; dP++ ) {
      testname.str("");
      if ( dP == 0 ) { testname << "dP05"; }
      if ( dP == 1 ) { testname << "dP005"; }
      if ( dP == 2 ) { testname << "dP1"; }
      if ( dP == 3 ) { testname << "dP001"; }
      if ( dP == 4 ) { testname << "dP0001"; }
      if ( dP == 5 ) { testname << "dP2"; }
      if ( dP == 6 ) { testname << "dP5"; }
      if ( dP == 7 ) { testname << "dP10"; }
      if ( dP == 8 ) { testname << "dP0075"; }
      if ( dP == 9 ) { testname << "dP025"; }
      if ( dP == 10 ) { testname << "dP075"; }
      if ( dP == 11 ) { testname << "dP0025"; }
      if ( name.find(testname.str()) != string::npos ) {
	sigmapname.str("");
	sigmapname << "PerfectMatch_" << testname.str();
	if ( dP == 0 ) { is_perfectmatch_dP05 = true; sigmapnum = 8; foundit = true; }
	if ( dP == 1 ) { is_perfectmatch_dP005 = true; sigmapnum = 9; foundit = true; }
	if ( dP == 2 ) { is_perfectmatch_dP1 = true; sigmapnum = 10; foundit = true; }
	if ( dP == 3 ) { is_perfectmatch_dP001 = true; sigmapnum = 11; foundit = true; }
	if ( dP == 4 ) { is_perfectmatch_dP0001 = true; sigmapnum = 12; foundit = true; }
	if ( dP == 5 ) { is_perfectmatch_dP2 = true; sigmapnum = 13; foundit = true; }
	if ( dP == 6 ) { is_perfectmatch_dP5 = true; sigmapnum = 14; foundit = true; }
	if ( dP == 7 ) { is_perfectmatch_dP10 = true; sigmapnum = 15; foundit = true; is_perfectmatch_dP1 = false; }
	if ( dP == 8 ) { is_perfectmatch_dP0075 = true; sigmapnum = 16; foundit = true; }
	if ( dP == 9 ) { is_perfectmatch_dP025 = true; sigmapnum = 17; foundit = true; }
	if ( dP == 10 ) { is_perfectmatch_dP075 = true; sigmapnum = 18; foundit = true; }
	if ( dP == 11 ) { is_perfectmatch_dP0025 = true; sigmapnum = 19; foundit = true; }
      }
    }
    if ( !foundit ) {
      sigmapname.str("");
      sigmapname << "PerfectMatch";
      is_perfectmatch = true;
      sigmapnum = 5;
    }
  }

  testname.str("");
  testname << "E821Match";
  if ( name.find(testname.str()) != string::npos ) {
    bool foundit = false;
    for ( int dP = 0; dP < 12; dP++ ) {
      testname.str("");
      if ( dP == 0 ) { testname << "dP05"; }
      if ( dP == 1 ) { testname << "dP005"; }
      if ( dP == 2 ) { testname << "dP1"; }
      if ( dP == 3 ) { testname << "dP001"; }
      if ( dP == 4 ) { testname << "dP0001"; }
      if ( dP == 5 ) { testname << "dP2"; }
      if ( dP == 6 ) { testname << "dP5"; }
      if ( dP == 7 ) { testname << "dP10"; }
      if ( dP == 8 ) { testname << "dP0075"; }
      if ( dP == 9 ) { testname << "dP025"; }
      if ( dP == 10 ) { testname << "dP075"; }
      if ( dP == 11 ) { testname << "dP0025"; }
      if ( name.find(testname.str()) != string::npos ) {
	sigmapname.str("");
	sigmapname << "E821Match_" << testname.str();
	if ( dP == 0 ) { is_bnlmatch_dP05 = true; sigmapnum = 20; foundit = true; }
	if ( dP == 1 ) { is_bnlmatch_dP005 = true; sigmapnum = 21; foundit = true; }
	if ( dP == 2 ) { is_bnlmatch_dP1 = true; sigmapnum = 22; foundit = true; }
	if ( dP == 3 ) { is_bnlmatch_dP001 = true; sigmapnum = 23; foundit = true; }
	if ( dP == 4 ) { is_bnlmatch_dP0001 = true; sigmapnum = 24; foundit = true; }
	if ( dP == 5 ) { is_bnlmatch_dP2 = true; sigmapnum = 25; foundit = true; }
	if ( dP == 6 ) { is_bnlmatch_dP5 = true; sigmapnum = 26; foundit = true; }
	if ( dP == 7 ) { is_bnlmatch_dP10 = true; sigmapnum = 27; foundit = true; is_bnlmatch_dP1 = false; }
	if ( dP == 8 ) { is_bnlmatch_dP0075 = true; sigmapnum = 28; foundit = true; }
	if ( dP == 9 ) { is_bnlmatch_dP025 = true; sigmapnum = 29; foundit = true; }
	if ( dP == 10 ) { is_bnlmatch_dP075 = true; sigmapnum = 30; foundit = true; }
	if ( dP == 11 ) { is_bnlmatch_dP0025 = true; sigmapnum = 31; foundit = true; }
      }
    }
  }

  testname.str("");
  testname << "SigmaP02";
  if ( name.find(testname.str()) != string::npos ) {
    sigmapname.str("");
    sigmapname << "dP/P = 0.2%";
    is_sigmap02 = true;
    sigmapnum = 0.2;
  }
  testname.str("");
  testname << "SigmaP05";
  if ( name.find(testname.str()) != string::npos ) {
    sigmapname.str("");
    sigmapname << "dP/P = 0.5%";
    is_sigmap05 = true;
    sigmapnum = 0.5;
  }
  testname.str("");
  testname << "NoSigmaP";
  if ( name.find(testname.str()) != string::npos ) {
    sigmapname.str("");
    sigmapname << "dP/P = 0%";
    is_nosigmap = true;
    sigmapnum = 0.0;
  }

  testname.str("");
  testname << "tSigma50";
  if ( name.find(testname.str()) != string::npos ) {
    tsigmaname.str("");
    tsigmaname << "sigma(t) = 50 ns";
    is_tsigma50 = true;
    sigmatnum = 50;
  }
  else {
    testname.str("");
    testname << "tSigma25";
    if ( name.find(testname.str()) != string::npos ) {
      tsigmaname.str("");
      tsigmaname << "sigma(t) = 25 ns";
      is_tsigma25 = true;
      sigmatnum = 25;
    }
    else {
      tsigmaname.str("");
      tsigmaname << "sigma(t) = 1 ns";
      is_tsigma1 = true;
      sigmatnum = 1;
    }
  }

  //if ( deltaname.str().size() > 0 ) { is_found = true; }
  nice_deltaname = deltaname.str();
  nice_launchname = launchname.str();
  nice_fieldname = fieldname.str();
  nice_inflectorname = inflectorname.str();
  nice_kickname = kickname.str();
  nice_beamstartname = beamstartname.str();
  if ( beamstartname.str().size() > 0 ) { is_found = true; } 
  nice_prhatname = prhatname.str();
  nice_sigmapname = sigmapname.str(); 
  nice_tsigmaname = tsigmaname.str();
  if ( kickname.str().size() > 0 ) { is_found = true; } 

  //cout << kickname.str() << endl;
}

void geteff(int numer, int denom, double *eff, double *err)
{
  double loc_eff = 0.0;
  double loc_err = 0.0;
  if ( denom > 0 ) {
    loc_eff = (double)numer /  denom;
  }
  else { 
    loc_eff = 0.0;
  }
  
  if ( denom > 25 && loc_eff > 0.0 && loc_eff < 1.0 ) {
    loc_err = TMath::Sqrt(loc_eff * (1-loc_eff)/(double)denom);
  }
  else {
    if ( denom > 0 ) {
      loc_err = 1.0/TMath::Sqrt((double)denom);
    }
    else {
      loc_err = 0.0;
    }
  }
  *eff = loc_eff * 100.0; *err = loc_err * 100.0;
}

void collecteff()
{
  bool found_lcrkick = false;
  bool found_squarekick = false;
  bool found_nokick = false;
  bool found_perfect = false;
  bool found_downstreammandrel = false;
  bool found_upstreammandrel = false;
  bool found_upstreamcryo = false;
  bool found_centralorbit_offset77 = false;
  bool found_centralorbit = false;
  bool found_off0 = false;
  bool found_off10 = false;
  bool found_off20 = false;
  bool found_off30 = false;
  bool found_off40 = false;
  bool found_off50 = false;
  bool found_off60 = false;
  bool found_off70 = false;
  bool found_off75 = false;
  bool found_off77 = false;
  bool found_off80 = false;
  bool found_perfectmatch = false;
  bool found_perfectmatch_dP05 = false;
  bool found_perfectmatch_dP005 = false;
  bool found_perfectmatch_dP1 = false;
  bool found_perfectmatch_dP001 = false;
  bool found_perfectmatch_dP0001 = false;
  bool found_perfectmatch_dP2 = false;
  bool found_perfectmatch_dP5 = false;
  bool found_perfectmatch_dP10 = false;
  bool found_perfectmatch_dP0075 = false;
  bool found_perfectmatch_dP025 = false;
  bool found_perfectmatch_dP075 = false;
  bool found_perfectmatch_dP0025 = false;
  bool found_perfectmatch_beam = false;
  bool found_bnlmatch_dP05 = false;
  bool found_bnlmatch_dP005 = false;
  bool found_bnlmatch_dP1 = false;
  bool found_bnlmatch_dP001 = false;
  bool found_bnlmatch_dP0001 = false;
  bool found_bnlmatch_dP2 = false;
  bool found_bnlmatch_dP5 = false;
  bool found_bnlmatch_dP10 = false;
  bool found_bnlmatch_dP0075 = false;
  bool found_bnlmatch_dP025 = false;
  bool found_bnlmatch_dP075 = false;
  bool found_bnlmatch_dP0025 = false;
  bool found_bnlmatch_beam = false;
  bool found_noprhat = false;
  bool found_prhat1 = false;
  bool found_prhat2 = false;
  bool found_prhat5 = false;
  bool found_sigmat1 = false;
  bool found_sigmat25 = false;
  bool found_sigmat50 = false;  
  bool found_mapped = false;
  bool found_vanish = false;
  bool found_scrapingoff = false;
  bool found_uniform = false;
  bool found_101turns = false;
  bool found_fullhighmom = false;
  bool found_fullhighermom = false;
  bool found_fullhighestmom = false;
  bool found_newsupports = false;
  bool found_noquads = false;
  bool found_noquadsupports = false;
  bool found_inflectoropen = false;
  bool found_inflectorpartial = false;
  bool found_inflectorclosed = false;
  bool found_beam1pi = false;
  bool found_beam10pi = false;
  bool found_beam20pi = false;
  bool found_beam30pi = false;
  bool found_beam40pi = false;
  bool found_beam42pi = false;
  bool found_beam50pi = false;
  bool found_beam60pi = false;
  bool found_beam0pi = false;
  bool found_deltainf = false;


  ifstream in;
  in.open("files.dat");
  int nfiles = 0;

  vector<string> files;
  map<string, double> eff_inf;
  map<string, double> err_inf;
  map<string, double> eff_infentrance;
  map<string, double> err_infentrance;
  map<string, double> eff_inf_length;
  map<string, double> err_inf_length;
  map<string, double> eff_turn1;
  map<string, double> err_turn1;
  map<string, double> eff_turn100;
  map<string, double> err_turn100;
  map<string, double> eff_ring;
  map<string, double> err_ring;

  int kl_or_dl = -1;
  bool eff_final = true;
  bool eff_intoring = false;
  bool eff_inflen = false;
  bool eff_ringacc = false;
  bool eff_intoinf = false;

  string previousfile = "";
  while ( in.eof() == false ) {
    string file;
    in >> file;

    if ( kl_or_dl == -1 ) {
      if ( file == "0" ) { kl_or_dl = 0; }
      if ( file == "1" ) { kl_or_dl = 1; }
      if ( file == "2" ) { eff_final = false; eff_intoring = true; kl_or_dl = 1; }
      if ( file == "3" ) { eff_final = false; eff_inflen = true; kl_or_dl = 1; }
      if ( file == "4" ) { eff_final = false; eff_ringacc = true; kl_or_dl = 1; }
      if ( file == "5" ) { eff_final = false; eff_intoinf = true; kl_or_dl = 1; }
      continue;
    }
    
    if ( file == previousfile ) { break; }
    previousfile = file;
    if ( file.size() < 1 ) { break; }

    files.push_back(file);
  }

  for ( int i = 0; i < (int)files.size(); i++ ) {
    string file = files[i];    
    cout << files[i] << endl;
    //continue;
    
    ifstream infile;
    infile.open(files[i].c_str());
    if ( infile.is_open() == false ) {
      cout << "Could not open " << files[i] << endl;
      continue;
    }

    nfiles++;

    if ( found_perfect == false ) {
      if ( files[i].find("PerfectStorage") != string::npos ) {
	found_perfect = true;
      }
    }
    if ( found_sigmat1 == false ) {
      if ( files[i].find("Sigma1") != string::npos ) {
	found_sigmat1 = true;
      }
    }
    if ( found_sigmat25 == false ) {
      if ( files[i].find("Sigma25") != string::npos ) {
	found_sigmat25 = true;
      }
    }
    if ( found_sigmat50 == false ) {
      if ( files[i].find("Sigma50") != string::npos ) {
	found_sigmat50 = true;
      }
    }
    if ( found_deltainf == false ) {
      if ( kl_or_dl == 1 ) { found_deltainf = true; }
    }

    if ( found_noprhat == false ) {
      if ( files[i].find("NoPrhat") != string::npos ) {
	found_noprhat = true;
      }
    }
    if ( found_perfectmatch == false ) {
      if ( files[i].find("PerfectMatch_C") != string::npos ) {
	found_perfectmatch = true;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP05 == false ) {
      if ( files[i].find("PerfectMatch_dP05") != string::npos ) {
	found_perfectmatch_dP05 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP005 == false ) {
      if ( files[i].find("PerfectMatch_dP005") != string::npos ) {
	found_perfectmatch_dP005 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP1 == false ) {
      if ( files[i].find("PerfectMatch_dP1") != string::npos ) {
	found_perfectmatch_dP1 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP001 == false ) {
      if ( files[i].find("PerfectMatch_dP001") != string::npos ) {
	found_perfectmatch_dP001 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP0001 == false ) {
      if ( files[i].find("PerfectMatch_dP0001") != string::npos ) {
	found_perfectmatch_dP0001 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP2 == false ) {
      if ( files[i].find("PerfectMatch_dP2") != string::npos ) {
	found_perfectmatch_dP2 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP5 == false ) {
      if ( files[i].find("PerfectMatch_dP5") != string::npos ) {
	found_perfectmatch_dP5 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP10 == false ) {
      if ( files[i].find("PerfectMatch_dP10") != string::npos ) {
	found_perfectmatch_dP10 = true; found_perfectmatch = false; found_perfectmatch_dP1 = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP0075 == false ) {
      if ( files[i].find("PerfectMatch_dP0075") != string::npos ) {
	found_perfectmatch_dP0075 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP025 == false ) {
      if ( files[i].find("PerfectMatch_dP025") != string::npos ) {
	found_perfectmatch_dP025 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP075 == false ) {
      if ( files[i].find("PerfectMatch_dP075") != string::npos ) {
	found_perfectmatch_dP075 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_perfectmatch_dP0025 == false ) {
      if ( files[i].find("PerfectMatch_dP0025") != string::npos ) {
	found_perfectmatch_dP0025 = true; found_perfectmatch = false;
	found_perfectmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP05 == false ) {
      if ( files[i].find("E821Match_dP05") != string::npos ) {
	found_bnlmatch_dP05 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP005 == false ) {
      if ( files[i].find("E821Match_dP005") != string::npos ) {
	found_bnlmatch_dP005 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP1 == false ) {
      if ( files[i].find("E821Match_dP1") != string::npos ) {
	found_bnlmatch_dP1 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP001 == false ) {
      if ( files[i].find("E821Match_dP001") != string::npos ) {
	found_bnlmatch_dP001 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP0001 == false ) {
      if ( files[i].find("E821Match_dP0001") != string::npos ) {
	found_bnlmatch_dP0001 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP2 == false ) {
      if ( files[i].find("E821Match_dP2") != string::npos ) {
	found_bnlmatch_dP2 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP5 == false ) {
      if ( files[i].find("E821Match_dP5") != string::npos ) {
	found_bnlmatch_dP5 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP10 == false ) {
      if ( files[i].find("E821Match_dP10") != string::npos ) {
	found_bnlmatch_dP10 = true; found_bnlmatch_dP1 = false;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP0075 == false ) {
      if ( files[i].find("E821Match_dP0075") != string::npos ) {
	found_bnlmatch_dP0075 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP025 == false ) {
      if ( files[i].find("E821Match_dP025") != string::npos ) {
	found_bnlmatch_dP025 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP075 == false ) {
      if ( files[i].find("E821Match_dP075") != string::npos ) {
	found_bnlmatch_dP075 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_bnlmatch_dP0025 == false ) {
      if ( files[i].find("E821Match_dP0025") != string::npos ) {
	found_bnlmatch_dP0025 = true;
	found_bnlmatch_beam = true;
      }
    }
    if ( found_prhat1 == false ) {
      if ( files[i].find("Prhat1") != string::npos ) {
	found_prhat1 = true;
      }
    }
    if ( found_prhat2 == false ) {
      if ( files[i].find("Prhat2") != string::npos ) {
	found_prhat2 = true;
      }
    }
    if ( found_prhat5 == false ) {
      if ( files[i].find("Prhat5") != string::npos ) {
	found_prhat5 = true;
      }
    }
    if ( found_off0 == false ) {
      if ( files[i].find("Offset0mm") != string::npos ) {
	found_off0 = true;
      }
    }
    if ( found_off10 == false ) {
      if ( files[i].find("Offset10mm") != string::npos ) {
	found_off10 = true;
      }
    }
    if ( found_off20 == false ) {
      if ( files[i].find("Offset20mm") != string::npos ) {
	found_off20 = true;
      }
    }
    if ( found_off30 == false ) {
      if ( files[i].find("Offset30mm") != string::npos ) {
	found_off30 = true;
      }
    }
    if ( found_off40 == false ) {
      if ( files[i].find("Offset40mm") != string::npos ) {
	found_off40 = true;
      }
    }
    if ( found_off50 == false ) {
      if ( files[i].find("Offset50mm") != string::npos ) {
	found_off50 = true;
      }
    }
    if ( found_off60 == false ) {
      if ( files[i].find("Offset60mm") != string::npos ) {
	found_off60 = true;
      }
    }
    if ( found_off70 == false ) {
      if ( files[i].find("Offset70mm") != string::npos ) {
	found_off70 = true;
      }
    }
    if ( found_off75 == false ) {
      if ( files[i].find("Offset75mm") != string::npos ) {
	found_off75 = true;
      }
    }
    if ( found_off77 == false ) {
      if ( files[i].find("Offset77mm") != string::npos ) {
	found_off77 = true;
      }
    }
    if ( found_off80 == false ) {
      if ( files[i].find("Offset80mm") != string::npos ) {
	found_off80 = true;
      }
    }
    if ( found_downstreammandrel == false ) {
      if ( files[i].find("DownstreamMandrel") != string::npos ) {
	found_downstreammandrel = true;
      }
    }
    if ( found_centralorbit_offset77 == false ) {
      if ( files[i].find("CentralOrbit_Offset77") != string::npos ) {
	found_centralorbit_offset77 = true;
      }
    }
    if ( found_centralorbit_offset77 == false ) {
      if ( found_centralorbit == false ) {
	if ( files[i].find("CentralOrbit") != string::npos ) {
	  found_centralorbit = true;
	}
      }
    }
    if ( found_upstreammandrel == false ) {
      if ( files[i].find("UpstreamMandrel") != string::npos ) {
	found_upstreammandrel = true;
      }
    }
    if ( found_upstreamcryo == false ) {
      if ( files[i].find("UpstreamCryo") != string::npos ) {
	found_upstreamcryo = true;
      }
    }
    if ( found_mapped == false ) {
      if ( files[i].find("MappedField") != string::npos ) {
	found_mapped = true;
      }
    }
    if ( found_scrapingoff == false ) {
      if ( files[i].find("ScrapingOFF") != string::npos ) {
	found_scrapingoff = true;
      }
    }
    if ( found_uniform == false ) {
      if ( files[i].find("Uniform") != string::npos ) {
	found_uniform = true;
      }
    }
    if ( found_101turns == false ) {
      if ( files[i].find("101Turns") != string::npos ) {
	found_101turns = true;
      }
    }
    if ( found_noquads == false ) {
      if ( files[i].find("NoQuads") != string::npos ) {
	found_noquads = true;
      }
    }
    if ( found_noquadsupports == false ) {
      if ( files[i].find("NoQuadSupport") != string::npos ) {
	found_noquadsupports = true;
      }
    }
    if ( found_newsupports == false ) {
      if ( files[i].find("NewSupport") != string::npos ) {
	found_newsupports = true;
      }
    }
    if ( found_fullhighmom == false ) {
      if ( files[i].find("FullHighMom") != string::npos ) {
	found_fullhighmom = true;
      }
    }
    if ( found_fullhighermom == false ) {
      if ( files[i].find("FullHigherMom") != string::npos ) {
	found_fullhighermom = true;
      }
    }
    if ( found_fullhighestmom == false ) {
      if ( files[i].find("FullHighestMom") != string::npos ) {
	found_fullhighestmom = true;
      }
    }
    if ( found_vanish == false ) {
      if ( files[i].find("mrad_Trap") != string::npos ) {
	found_vanish = true;
      }
      if ( files[i].find("Launch_Trap") != string::npos ) {
	found_vanish = true;
      }
      if ( files[i].find("mrad_Beam") != string::npos ) {
	found_vanish = true;
      }
      if ( files[i].find("Launch_Beam") != string::npos ) {
	found_vanish = true;
      }
    }
    if ( found_inflectoropen == false ) {
      if ( files[i].find("InflectorOpen") != string::npos ) {
	found_inflectoropen = true;
      }
      if ( files[i].find("OpenInflector") != string::npos ) {
	found_inflectoropen = true;
      }
    }
    if ( found_inflectorclosed == false ) {
      if ( files[i].find("InflectorClosed") != string::npos ) {
	found_inflectorclosed = true;
      }
      if ( files[i].find("ClosedInflector") != string::npos ) {
	found_inflectorclosed = true;
      }
    }
    if ( found_inflectorpartial == false ) {
      if ( files[i].find("InflectorPartial") != string::npos ) {
	found_inflectorpartial = true;
      }
      if ( files[i].find("PartialInflector") != string::npos ) {
	found_inflectorpartial = true;
      }
      if ( files[i].find("PartiallyOpen") != string::npos ) {
	found_inflectorpartial = true;
      }
    }
    if ( found_beam1pi == false ) {
      if ( files[i].find("BeamSize1pi") != string::npos ) {
	found_beam1pi = true;
      }
    }
    if ( found_beam10pi == false ) {
      if ( files[i].find("BeamSize10pi") != string::npos ) {
	found_beam10pi = true;
      }
    }
    if ( found_beam20pi == false ) {
      if ( files[i].find("BeamSize20pi") != string::npos ) {
	found_beam20pi = true;
      }
    }
    if ( found_beam30pi == false ) {
      if ( files[i].find("BeamSize30pi") != string::npos ) {
	found_beam30pi = true;
      }
    }
    if ( found_beam40pi == false ) {
      if ( files[i].find("BeamSize40pi") != string::npos ) {
	found_beam40pi = true;
      }
    }
    if ( found_beam42pi == false ) {
      if ( files[i].find("BeamSize42pi") != string::npos ) {
	found_beam42pi = true;
      }
    }
    if ( found_beam0pi == false ) {
      if ( files[i].find("BeamSize0pi") != string::npos ) {
	found_beam0pi = true;
      }
    }
    if ( found_beam50pi == false ) {
      if ( files[i].find("BeamSize50pi") != string::npos ) {
	found_beam50pi = true;
      }
    }
    if ( found_beam60pi == false ) {
      if ( files[i].find("BeamSize60pi") != string::npos ) {
	found_beam60pi = true;
      }
    }
    if ( found_lcrkick == false ) {
      if ( files[i].find("LCRKick") != string::npos ) {
	found_lcrkick = true;
      }
    }
    if ( found_nokick == false ) {
      if ( files[i].find("NoKick") != string::npos ) {
	found_nokick = true;
      }
    }
    if ( found_squarekick == false ) {
      if ( files[i].find("SquareKick") != string::npos ) {
	found_squarekick = true;
      }
    }


    //NiceName(files[i]);
    //cout << nice_beamstartname << "\t" << nice_inflectorname << "\t" << nice_fieldname << "\t" << nice_kickname << "\t" << nice_sigmapname << endl;
    //continue;
    
    double eff = 0.0;
    double err = 0.0;
    string dummy;
    string name;
    //infile >> name;


    int Nstart_inflector = 0;
    int Nexit_inflector = 0;
    int Nstart_ring = 0;
    int Nstored1_ring = 0;
    int Nstored10_ring = 0;
    int Nstored100_ring = 0;
    int Ngen = 0;
    
    infile >> Nstart_inflector >> Nexit_inflector;
    infile >> Nstart_ring;
    infile >> Nstored1_ring >> Nstored10_ring >> Nstored100_ring;
    infile >> Ngen;

    if ( found_upstreamcryo || found_upstreammandrel ) {
      geteff(Nstart_ring, Nstart_inflector, &eff, &err);
      eff_inf_length[file] = eff;
      err_inf_length[file] = err;

      geteff(Nstart_ring, Ngen, &eff, &err);
      eff_inf[file] = eff;
      err_inf[file] = err;

      geteff(Nstart_inflector, Ngen, &eff, &err);
      eff_infentrance[file] = eff;
      err_infentrance[file] = err;
    }
    else if ( found_downstreammandrel ) {
      geteff(Nstart_ring, Ngen, &eff, &err);
      eff_inf_length[file] = eff;
      err_inf_length[file] = err;

      geteff(Nstart_ring, Ngen, &eff, &err);
      eff_inf[file] = eff;
      err_inf[file] = err;

      geteff(Ngen, Ngen, &eff, &err);
      eff_infentrance[file] = eff;
      err_infentrance[file] = err;
    }
    else {
      geteff(Ngen, Ngen, &eff, &err);
      eff_inf_length[file] = eff;
      err_inf_length[file] = err;
      eff_inf[file] = eff;
      err_inf[file] = err;
      eff_infentrance[file] = eff;
      err_infentrance[file] = err;
    }
    

    //if ( found_upstreamcryo ) { Nstart_inflector = 25000; }
    if ( Ngen == Nstored100_ring ) { Ngen = Nstart_inflector; }
    if ( Ngen == 0 ) { Ngen = Nstart_inflector; }
    if ( Ngen > 0 ) { Nstart_inflector = Ngen; }
    
    int Nring = Nstart_inflector;
    if ( Nstart_inflector <= 0 ) { Nring = Nstart_ring; }
    geteff(Nstored1_ring, Nring, &eff, &err);
    eff_turn1[file] = eff;
    err_turn1[file] = err;
    //geteff(Nstored10_ring, Nring, &eff, &err);
    //eff_turn10[file] = eff;
    //err_turn10[file] = err;
    geteff(Nstored100_ring, Nring, &eff, &err);
    eff_turn100[file] = eff;
    err_turn100[file] = err;
    geteff(Nstored100_ring, Nstart_ring, &eff, &err);
    eff_ring[file] = eff;
    err_ring[file] = err;

    infile.close();
  }

  //if ( nfiles == 0 ) { return; }
  //  return;

  ofstream out, out2, out3, out4, out5, out6;
  ofstream out_dm, out_dm2, out_dm3, out_dm4, out_dm5, out_dm6;
  out.open("storage.dat");
  out2.open("storage2.dat");
  out3.open("storage3.dat");
  out4.open("storage4.dat");
  out5.open("storage5.dat");
  out6.open("storage6.dat");

  out_dm2.open("storage_dm2.dat");
  out_dm3.open("storage_dm3.dat");
  out_dm4.open("storage_dm4.dat");
  out_dm5.open("storage_dm5.dat");
  out_dm6.open("storage_dm6.dat");

  if ( found_off0 ) {
    if ( found_sigmat1 ) {
      out2 << "void GetEffErr_sigmat1(double *eff, double *err, int *launch, int *kick, int offset, int prhat, double sigmap, int delta, int sigmat, int num) {" << endl;
      out2 << "  *eff = 0.0; *err = 0.0;" << endl;
      out2 << "  *launch = -99; *kick = -99;" << endl;
    }
    if ( found_sigmat50 ) {
      out2 << "void GetEffErr_sigmat50(double *eff, double *err, int *launch, int *kick, int offset, int prhat, double sigmap, int delta, int sigmat, int num) {" << endl;
      out2 << "  *eff = 0.0; *err = 0.0;" << endl;
      out2 << "  *launch = -99; *kick = -99;" << endl;
    }
  }
  out3 << "void GetEffErrKick(double *eff, double *err, int *launch, int *kick, int offset, int prhat, double sigmap, int delta, int sigmat, int num) {" << endl; 
  out3 << "  *eff = 0.0; *err = 0.0;" << endl;
  out5 << "void GetEffErrKick(double *eff, double *err, int *kick, int offset, int prhat, double sigmap, int delta, int launch, int sigmat, int num) {" << endl;
  out5 << "  *eff = 0.0; *err = 0.0;" << endl;
  out4 << "void GetEffErrLaunch(double *eff, double *err, int *launch, int *kick, int offset, int prhat, double sigmap, int delta, int sigmat, int num) {" << endl;
  out4 << "  *eff = 0.0; *err = 0.0;" << endl;
  out6 << "void GetEffErrLaunch(double *eff, double *err, int *launch, int offset, int prhat, double sigmap, int delta, int kick, int sigmat, int num) {" << endl;
  out6 << "  *eff = 0.0; *err = 0.0;" << endl;


  if ( nfiles == 1 ) {
    cout << "NFILES =  " << nfiles << endl;
    cout << "Start?    " << found_centralorbit << "\t" << found_centralorbit_offset77 << "\t" << found_downstreammandrel << "\t" << found_upstreammandrel << "\t" << found_upstreamcryo << endl;
    cout << "Size?     " << found_beam1pi << "\t" << found_beam10pi << "\t" << found_beam20pi << "\t" << found_beam30pi << "\t" << found_beam40pi << "\t" << found_beam42pi << "\t" << found_beam50pi << "\t" << found_beam60pi << "\t" << found_beam0pi << endl;
    cout << "SigmaT?   " << found_sigmat1 << "\t" << found_sigmat25 << "\t" << found_sigmat50 << endl; 
    cout << "Mapped?   " << found_mapped << "\t" << found_vanish << endl;
    cout << "Scraping? " << found_scrapingoff << endl;
    cout << "Uniform? " << found_uniform << endl;
    cout << "101Turns? " << found_101turns << endl;
    cout << "Quads?    " << found_fullhighmom << "\t" << found_fullhighermom << "\t" << found_fullhighestmom << "\t" << found_newsupports << "\t" << found_noquads << "\t" << found_noquadsupports << endl;
    cout << "Beam?     " << found_perfectmatch << "\t" << found_perfectmatch_dP05 << "\t" << found_perfectmatch_dP005 << "\t" << found_perfectmatch_dP1 << "\t" << found_perfectmatch_dP2 << "\t" << found_perfectmatch_dP5 << "\t" << found_perfectmatch_dP10 << "\t" << found_perfectmatch_dP0075 << "\t" << found_perfectmatch_dP025 << "\t" << found_perfectmatch_dP0025 << "\t" << found_perfectmatch_dP075 << "\t" << found_perfectmatch_dP001 << "\t" << found_perfectmatch_dP0001 << "\t" << found_bnlmatch_dP05 << "\t" << found_bnlmatch_dP005 << "\t" << found_bnlmatch_dP1 << "\t" << found_bnlmatch_dP2 << "\t" << found_bnlmatch_dP5 << "\t" << found_bnlmatch_dP10 << "\t" << found_bnlmatch_dP0075 << "\t" << found_bnlmatch_dP025 << "\t" << found_bnlmatch_dP0025 << "\t" << found_bnlmatch_dP075 << "\t" << found_bnlmatch_dP001 << "\t" << found_bnlmatch_dP0001 << endl;
    cout << "Inf?      " << found_inflectoropen << "\t" << found_inflectorpartial << "\t" << found_inflectorclosed << endl;
    cout << "Delta?    " << found_deltainf << endl;
    //return;
  }

  stringstream funcname;
  if ( eff_final ) {
    if ( found_deltainf ) {
      funcname << "void GetEffErr_DeltaLaunch_";
    }
    else {
      funcname << "void GetEffErr_KickLaunch_";
    }
  }
  else if ( eff_intoring ) {
    funcname << "void GetEffErr_Inflector_DeltaLaunch_";
  }
  else if ( eff_inflen ) {
    funcname << "void GetEffErr_InflectorInternal_DeltaLaunch_";
  }
  else if ( eff_ringacc ) {
    funcname << "void GetEffErr_RingAcceptance_DeltaLaunch_";
  }
  else if ( eff_intoinf ) {
    funcname << "void GetEffErr_InflectorAcceptance_DeltaLaunch_";
  }


  if ( found_downstreammandrel ) { funcname << "DM_"; }
  if ( found_upstreammandrel ) { funcname << "UM_"; }
  if ( found_upstreamcryo ) { funcname << "UC_"; }
  if ( found_centralorbit_offset77 ) { funcname << "CO77_"; }
  if ( found_centralorbit ) { funcname << "CO_"; }

  if ( found_sigmat1 ) { funcname << "sigmat1_"; }
  if ( found_sigmat25 ) { funcname << "sigmat25_"; }
  if ( found_sigmat50 ) { funcname << "sigmat50_"; }

  if ( found_lcrkick ) { funcname << "LCRKick_"; }
  else if ( found_squarekick ) { funcname << "SquareKick_"; }
  else if ( found_nokick ) { funcname << "NoKick_"; }

  if ( found_uniform ) { funcname << "UniformBeam_"; }
  else { funcname << "GaussianBeam_"; }

  if ( found_bnlmatch_beam ) { funcname << "BNLmatch_"; }
  if ( found_perfectmatch_beam ) { funcname << "Perfectmatch_"; }

  if ( found_beam0pi ) { funcname << "Beamsize0pi_"; }
  if ( found_beam1pi ) { funcname << "Beamsize1pi_"; }
  if ( found_beam10pi ) { funcname << "Beamsize10pi_"; }
  if ( found_beam20pi ) { funcname << "Beamsize20pi_"; }
  if ( found_beam30pi ) { funcname << "Beamsize30pi_"; }
  if ( found_beam40pi ) { funcname << "Beamsize40pi_"; }
  if ( found_beam42pi ) { funcname << "Beamsize42pi_"; }
  if ( found_beam50pi ) { funcname << "Beamsize50pi_"; }
  if ( found_beam60pi ) { funcname << "Beamsize60pi_"; }

  if ( found_scrapingoff ) { funcname << "ScrapingOFF_"; }
  else { funcname << "ScrapingON_"; }

  if ( found_fullhighmom ) { funcname << "FullHighMom_"; }
  if ( found_fullhighermom ) { funcname << "FullHigherMom_"; }
  if ( found_fullhighestmom ) { funcname << "FullHighestMom_"; }
  if ( found_newsupports ) { funcname << "NewSupports_"; }
  if ( found_noquads ) { funcname << "NoQuads_"; }
  if ( found_noquadsupports ) { funcname << "NoQuadSupports_"; }
  
  if ( found_mapped ) { funcname << "Mapped_"; }
  if ( found_vanish ) { funcname << "Vanish_"; }

  if ( found_inflectoropen ) { funcname << "Open"; }
  if ( found_inflectorclosed ) { funcname << "Closed"; }
  if ( found_inflectorpartial ) { funcname << "PartiallyOpen"; }

  //
  // Only write this for the first function
  //

  if ( found_deltainf ) {
    funcname << "(double *eff, double *err, int *launch, int *delta, int kick, int beamstart, int inf, int field, int prhat, double sigmap, int sigmat, int beamsize, int quad ,int beamtype, int num)";
  }
  else {
    funcname << "(double *eff, double *err, int *launch, int *kick, int beamstart, int inf, int field, int prhat, double sigmap, int sigmat, int beamsize, int quad, int beamtype, int num)";
  }


  if ( ( found_bnlmatch_beam && found_bnlmatch_dP0001 ) ||
       ( found_perfectmatch_beam && found_perfectmatch_dP0001 ) ) {
    out_dm2 << funcname.str() << " {";
    out_dm2 << "  bool debug = false;" << endl;
    out_dm2 << "  *eff = 0.0; *err = 0.0;" << endl;
    if ( found_deltainf ) {
      out_dm2 << "  *launch = -99; *delta = -99;" << endl;
    }
    else {
      out_dm2 << "  *launch = -99; *kick = -99;" << endl;
    }
    out_dm2 << "  int kPerfectMatch = 5;" << endl;
    out_dm2 << "  int kPerfectMatch_dP05 = 8;" << endl;
    out_dm2 << "  int kPerfectMatch_dP005 = 9;" << endl;
    out_dm2 << "  int kPerfectMatch_dP1 = 10;" << endl;
    out_dm2 << "  int kPerfectMatch_dP001 = 11;" << endl;
    out_dm2 << "  int kPerfectMatch_dP0001 = 12;" << endl;
    out_dm2 << "  int kPerfectMatch_dP2 = 13;" << endl;
    out_dm2 << "  int kPerfectMatch_dP5 = 14;" << endl;
    out_dm2 << "  int kPerfectMatch_dP10 = 15;" << endl;
    out_dm2 << "  int kPerfectMatch_dP0075 = 16;" << endl;
    out_dm2 << "  int kPerfectMatch_dP025 = 17;" << endl;
    out_dm2 << "  int kPerfectMatch_dP075 = 18;" << endl;
    out_dm2 << "  int kPerfectMatch_dP0025 = 19;" << endl;
    out_dm2 << "  int kE821Match_dP05 = 20;" << endl;
    out_dm2 << "  int kE821Match_dP005 = 21;" << endl;
    out_dm2 << "  int kE821Match_dP1 = 22;" << endl;
    out_dm2 << "  int kE821Match_dP001 = 23;" << endl;
    out_dm2 << "  int kE821Match_dP0001 = 24;" << endl;
    out_dm2 << "  int kE821Match_dP2 = 25;" << endl;
    out_dm2 << "  int kE821Match_dP5 = 26;" << endl;
    out_dm2 << "  int kE821Match_dP10 = 27;" << endl;
    out_dm2 << "  int kE821Match_dP0075 = 28;" << endl;
    out_dm2 << "  int kE821Match_dP025 = 29;" << endl;
    out_dm2 << "  int kE821Match_dP075 = 30;" << endl;
    out_dm2 << "  int kE821Match_dP0025 = 31;" << endl;
    out_dm2 << "  int kGaussian = 0;" << endl;
    out_dm2 << "  int kUniform = 1;" << endl;
    out_dm2 << "  int kClosed = 2;" << endl;
    out_dm2 << "  int kPartial = 1;" << endl;
    out_dm2 << "  int kOpen = 0;" << endl;
    out_dm2 << "  int kVanish = 0;" << endl;
    out_dm2 << "  int kMapped = 1;" << endl;
    out_dm2 << "  int kDownstreamMandrel = 0;" << endl;
    out_dm2 << "  int kUpstreamMandrel = 1;" << endl;
    out_dm2 << "  int kUpstreamCryo = 2;" << endl;
    out_dm2 << "  int kCentralOrbit_Offset77 = 3;" << endl;
    out_dm2 << "  int kCentralOrbit = 4;" << endl;
    out_dm2 << "  int kNewSupports = 0;" << endl;
    out_dm2 << "  int kNoQuads = 1;" << endl;
    out_dm2 << "  int kNoQuadSupports = 2;" << endl;
    out_dm2 << "  int kFullHighMom = 3;" << endl;
    out_dm2 << "  int kFullHigherMom = 4;" << endl;
    out_dm2 << "  int kFullHighestMom = 5;" << endl;
    out_dm2 << endl;
  }
  

  if ( nfiles > 0 ) {
    
    if ( found_centralorbit_offset77 || found_centralorbit || found_downstreammandrel || found_upstreammandrel || found_upstreamcryo ) {
      cout << endl;
      cout << endl;
      for ( int beamstart = 0; beamstart < 5; beamstart++ ) {

	if ( beamstart == 0 && found_downstreammandrel == false ) { continue; }
	if ( beamstart == 1 && found_upstreammandrel == false ) { continue; }
	if ( beamstart == 2 && found_upstreamcryo == false ) { continue; }
	if ( beamstart == 3 && found_centralorbit_offset77 == false ) { continue; }
	if ( beamstart == 4 && found_centralorbit == false ) { continue; }


	for ( int inf = 0; inf < 3; inf++ ) {
	  for ( int field = 0; field < 2; field++ ) {
	    for ( int mom = 8; mom < 32; mom++ ) {
	      if ( mom == 8 && (found_perfectmatch_dP05 == false) ){ continue; }
	      if ( mom == 9 && (found_perfectmatch_dP005 == false) ){ continue; }
	      if ( mom == 10 && (found_perfectmatch_dP1 == false) ){ continue; }
	      if ( mom == 11 && (found_perfectmatch_dP001 == false) ){ continue; }
	      if ( mom == 12 && (found_perfectmatch_dP0001 == false) ){ continue; }
	      if ( mom == 13 && (found_perfectmatch_dP2 == false) ){ continue; }
	      if ( mom == 14 && (found_perfectmatch_dP5 == false) ){ continue; }
	      if ( mom == 15 && (found_perfectmatch_dP10 == false) ){ continue; }
	      if ( mom == 16 && (found_perfectmatch_dP0075 == false) ){ continue; }
	      if ( mom == 17 && (found_perfectmatch_dP025 == false) ){ continue; }
	      if ( mom == 18 && (found_perfectmatch_dP075 == false) ){ continue; }
	      if ( mom == 19 && (found_perfectmatch_dP0025 == false) ){ continue; }	
	      if ( mom == 20 && (found_bnlmatch_dP05 == false) ){ continue; }
	      if ( mom == 21 && (found_bnlmatch_dP005 == false) ){ continue; }
	      if ( mom == 22 && (found_bnlmatch_dP1 == false) ){ continue; }
	      if ( mom == 23 && (found_bnlmatch_dP001 == false) ){ continue; }
	      if ( mom == 24 && (found_bnlmatch_dP0001 == false) ){ continue; }
	      if ( mom == 25 && (found_bnlmatch_dP2 == false) ){ continue; }
	      if ( mom == 26 && (found_bnlmatch_dP5 == false) ){ continue; }
	      if ( mom == 27 && (found_bnlmatch_dP10 == false) ){ continue; }
	      if ( mom == 28 && (found_bnlmatch_dP0075 == false) ){ continue; }
	      if ( mom == 29 && (found_bnlmatch_dP025 == false) ){ continue; }
	      if ( mom == 30 && (found_bnlmatch_dP075 == false) ){ continue; }
	      if ( mom == 31 && (found_bnlmatch_dP0025 == false) ){ continue; }	       
	    
	      for ( int ts = 0; ts < 3; ts++ ) {
	      
		if ( ts == 0 && found_sigmat1 == false ) { continue; }
		if ( ts == 1 && found_sigmat25 == false ) { continue; }
		if ( ts == 2 && found_sigmat50 == false ) { continue; }
	      
		for ( int bs = 1; bs < 10; bs++ ) {
		  int beamsize = bs;
		  if ( bs == 1 && found_beam1pi == false ) { continue; }
		  if ( bs == 2 && found_beam10pi == false ) { continue; }
		  if ( bs == 3 && found_beam20pi == false ) { continue; }
		  if ( bs == 4 && found_beam30pi == false ) { continue; }
		  if ( bs == 5 && found_beam40pi == false ) { continue; }
		  if ( bs == 9 && found_beam42pi == false ) { continue; }
		  if ( bs == 6 && found_beam50pi == false ) { continue; }
		  if ( bs == 7 && found_beam60pi == false ) { continue; }
		  if ( bs == 8 && found_beam0pi == false ) { continue; }

		  if ( found_beam0pi ) { beamsize = 0; }
		  if ( found_beam1pi ) { beamsize = 1; }
		  if ( found_beam10pi ) { beamsize = 10; }
		  if ( found_beam20pi ) { beamsize = 20; }
		  if ( found_beam30pi ) { beamsize = 30; }
		  if ( found_beam40pi ) { beamsize = 40; }
		  if ( found_beam42pi ) { beamsize = 42; }
		  if ( found_beam50pi ) { beamsize = 50; }
		  if ( found_beam60pi ) { beamsize = 60; }
		  
		  //cout << "HI" << endl;

		  //if ( beamsize == 1 || beamsize % 10 != 0 ) { continue; }
		  //for ( int kck = 0; kck < 5; kck++ ) {
		  
		  //cout << "### " << beamsize << "\t" << inf << "\t" << field << "\t" << mom << "\t" << ts << "\t" << endl;
		  int Nkeep = 600;
		  double min_inf_eff[600];
		  double min_inf_err[600];
		  string keep_inf_name[600];
		  double min_turn_eff[600];
		  double min_turn_err[600];
		  string keep_turn_name[600];
		  map<string, int> check_dupl;
		  for ( int m = 0; m < Nkeep; m++ ) {
		    min_inf_eff[m] = -1.0;
		    min_inf_err[m] = -1.0;
		    keep_inf_name[m] = "";
		    min_turn_eff[m] = -1.0;
		    min_turn_err[m] = -1.0;
		    keep_turn_name[m] = "";
		  }

		  for ( std::map<string, double>::iterator iter = eff_inf.begin(); iter != eff_inf.end(); iter++ ) {
		    string name = iter->first;
		    NiceName(name);


		    //cout << kck << "\t" << kicknum << endl;

		    //if ( kicknum <= 0 ) { continue; }

		    // 		if ( kck == 0 && kicknum != 150 ) { continue; }
		    // 		if ( kck == 1 && kicknum != 165 ) { continue; }
		    // 		if ( kck == 2 && kicknum != 180 ) { continue; }
		    // 		if ( kck == 3 && kicknum != 195 ) { continue; }
		    // 		if ( kck == 4 && kicknum != 210 ) { continue; }

		    if ( beamsize != beamsizenum ) { continue; }
	    
		    //cout << name << endl;


		
		    if ( is_downstreammandrel && beamstart != 0 ) { continue; }
		    if ( is_upstreammandrel && beamstart != 1 ) { continue; }
		    if ( is_upstreamcryo && beamstart != 2 ) { continue; }
		    if ( is_centralorbit_offset77 && beamstart != 3 ) { continue; }
		    if ( is_centralorbit && beamstart != 4 ) { continue; }


		    if ( inf == 0 && is_openinflector == false ) { continue; }
		    if ( inf == 1 && is_partiallyopeninflector == false ) { continue; }
		    if ( inf == 2 && is_closedinflector == false ) { continue; }

		    if ( field == 0 && is_mappedfield == true ) { continue; }
		    if ( field == 1 && is_mappedfield == false ) { continue; }

		    if ( mom == 8 && (is_perfectmatch_dP05 == false) ){ continue; }
		    if ( mom == 9 && (is_perfectmatch_dP005 == false) ){ continue; }
		    if ( mom == 10 && (is_perfectmatch_dP1 == false) ){ continue; }
		    if ( mom == 11 && (is_perfectmatch_dP001 == false) ){ continue; }
		    if ( mom == 12 && (is_perfectmatch_dP0001 == false) ){ continue; }
		    if ( mom == 13 && (is_perfectmatch_dP2 == false) ){ continue; }
		    if ( mom == 14 && (is_perfectmatch_dP5 == false) ){ continue; }
		    if ( mom == 15 && (is_perfectmatch_dP10 == false) ){ continue; }
		    if ( mom == 16 && (is_perfectmatch_dP0075 == false) ){ continue; }
		    if ( mom == 17 && (is_perfectmatch_dP025 == false) ){ continue; }
		    if ( mom == 18 && (is_perfectmatch_dP075 == false) ){ continue; }
		    if ( mom == 19 && (is_perfectmatch_dP0025 == false) ){ continue; }
		    if ( mom == 20 && (is_bnlmatch_dP05 == false) ){ continue; }
		    if ( mom == 21 && (is_bnlmatch_dP005 == false) ){ continue; }
		    if ( mom == 22 && (is_bnlmatch_dP1 == false) ){ continue; }
		    if ( mom == 23 && (is_bnlmatch_dP001 == false) ){ continue; }
		    if ( mom == 24 && (is_bnlmatch_dP0001 == false) ){ continue; }
		    if ( mom == 25 && (is_bnlmatch_dP2 == false) ){ continue; }
		    if ( mom == 26 && (is_bnlmatch_dP5 == false) ){ continue; }
		    if ( mom == 27 && (is_bnlmatch_dP10 == false) ){ continue; }
		    if ( mom == 28 && (is_bnlmatch_dP0075 == false) ){ continue; }
		    if ( mom == 29 && (is_bnlmatch_dP025 == false) ){ continue; }
		    if ( mom == 30 && (is_bnlmatch_dP075 == false) ){ continue; }
		    if ( mom == 31 && (is_bnlmatch_dP0025 == false) ){ continue; }
				  
				  
		
		    if ( ts == 0 && is_tsigma1 == false ) { continue; }
		    if ( ts == 1 && is_tsigma25 == false ) { continue; }
		    if ( ts == 2 && is_tsigma50 == false ) { continue; }

		    stringstream dupl;
		    dupl << launchnum << "_" << deltanum;
		    if ( check_dupl[dupl.str()] == 1 ) { 
		      cout << "================== DUPLICATE!!!! ====================" << endl;
		      cout << "Name = " << name << endl;
		      return;
		    }
		    check_dupl[dupl.str()] = 1;
	    
		    //cout << nice_beamstartname << "\t" << nice_inflectorname << "\t" << nice_fieldname << "\t" << nice_kickname << "\t" << nice_sigmapname << endl;
		    double eff = iter->second;
		    double err = err_inf[name];

	    	    
		    double eff_turn = eff_turn100[name];
		    double err_turn = err_turn100[name];

		    if ( eff_intoring ) {
		      eff_turn = eff_inf[name];
		      err_turn = err_inf[name];
		    }
		    if ( eff_inflen ) {
		      eff_turn = eff_inf_length[name];
		      err_turn = err_inf_length[name];
		    }
		    if ( eff_ringacc ) {
		      eff_turn = eff_ring[name];
		      err_turn = err_ring[name];
		    }
		    if ( eff_intoinf ) {
		      eff_turn = eff_infentrance[name];
		      err_turn = err_infentrance[name];
		    }
		
		    //cout.precision(3);
		    //cout << nice_beamstartname << " (" << is_upstreammandrel << "," << is_upstreamcryo << "," << is_downstreammandrel << ") \t" << nice_inflectorname << " (" << is_openinflector << ") \t" << nice_fieldname << " (" << is_mappedfield << ") \t" << nice_kickname << "\t" << nice_sigmapname << "\t" << nice_deltaname << "\t" << nice_launchname << "\t" << "eff = " << eff_turn100[name] << "+/-" << err_turn100[name] << "\t EffInf = " << err << "+/-" << err << endl;
		
		    //cout << "\t\t" << eff << "\t" << eff_turn << endl;
		    
		    if ( eff_turn < 0.0 ) { continue; }
	    
		    //cout.precision(2);
		    //cout << nice_beamstartname << " (" << is_upstreammandrel << "," << is_upstreamcryo << "," << is_downstreammandrel << ") \t" << nice_inflectorname << " (" << is_openinflector << ") \t" << nice_fieldname << " (" << is_mappedfield << ") \t" << nice_kickname << "\t" << nice_sigmapname << "\t" << nice_deltaname << "\t" << nice_launchname << "\t" << "Eff = " << eff_turn100[name] << "+/-" << err_turn100[name] << "\t EffInf = " << err << "+/-" << err << endl;

		    //cout << eff << "\t" << err << endl;

		    bool store = false;
		    if ( 0 ) {
		      for ( int t = 0; t < Nkeep; t++ ) {
			if ( eff > min_inf_eff[t] && store == false ) {
			  for ( int q = Nkeep-1; q > 0; q-- ) {
			    min_inf_eff[q] = min_inf_eff[q-1];
			    min_inf_err[q] = min_inf_err[q-1];
			    keep_inf_name[q] = keep_inf_name[q-1];
			  }
			  min_inf_eff[t] = eff;
			  min_inf_err[t] = err;
			  keep_inf_name[t] = name;
			  store = true;
			}
		      }
		    }

	    
		    store = false;
		    if ( 0 ) {
		      cout << "[-]\t" << eff_turn << " >? {";
		      for ( int tp = 0; tp < Nkeep; tp++ ) {
			cout <<  min_turn_eff[tp] << ",";
		      }
		      cout << "}" << endl;
		    }

		    for ( int t = 0; t < Nkeep; t++ ) {
		      //cout << "\tEff: " << eff_turn << "\tMinEff[" << t << "] = " << min_turn_eff[t] << endl;
		      if ( eff_turn > min_turn_eff[t] && store == false ) {
			//cout << "\t\t[" << t << "]\t" << eff_turn << " > " << min_turn_eff[t] << endl;
			for ( int q = Nkeep - 1; q > t; q-- ) {
			  //cout << "  " << q << "\t" << t << "\t" << min_turn_eff[q-1] << "\t" << min_turn_eff[q] << "\t";
			  min_turn_eff[q] = min_turn_eff[q-1];
			  min_turn_err[q] = min_turn_err[q-1];
			  keep_turn_name[q] = keep_turn_name[q-1];
			  //cout << "\t" << min_turn_eff[q] << endl;
			}
			min_turn_eff[t] = eff_turn;
			min_turn_err[t] = err_turn;
			keep_turn_name[t] = name;
			store = true;
			break;
		      }
		    }
		    if ( 0 ) {
		      cout << "[-]\t" << eff_turn << " >? {";
		      for ( int tp = 0; tp < Nkeep; tp++ ) {
			cout <<  min_turn_eff[tp] << ",";
		      }
		      cout << "}" << endl;
		      cout << endl;
		    }

		
		    if ( deltanum == 0 && launchnum == 0 && 0 ) {
		      int t = Nkeep - 1;
		      min_turn_eff[t] = eff_turn;
		      min_turn_err[t] = err_turn;
		      keep_turn_name[t] = name;
		    }
		  }

		  //cout << "Found = " << is_found << endl;


		  NiceName(keep_turn_name[0]);
		  //cout << "Name = " << keep_turn_name[0] << endl;
		  //cout << "Found = " << is_found << "\t" << Nkeep << "\t" << endl;

		  if ( is_found ) {

		    int kick_eff[5];
		    int kick_save[5] = {-1, -1, -1, -1, -1};
		    double eff_kick[5];
		    double err_kick[5];
		    kick_eff[0] = kicknum;
		    kick_save[0] = 0;
		    eff_kick[0] = min_turn_eff[0];
		    err_kick[0] = min_turn_err[0];
		    //out_dm2 << "  if ( beamstart == " << beamstart << " && prhat == " << prhatnum << " && sigmap == " << sigmapnum << " && field == " << field << " && inf == " << inf << " && sigmat == " << sigmatnum << " && num == 0 ) { *eff = " << eff_kick[0] << ";  " << "*err = " << err_kick[0] << ";" << " *launch = " << launchnum << ";  *delta = " << deltanum << "; *kick = " << kicknum << "; " << "}" << endl;




		
		    //cout.precision(3);
		    //cout << nice_beamstartname << "\t" << nice_inflectorname << "\t" << nice_fieldname << "\t" << nice_kickname << "\t" << nice_sigmapname << "\t" << nice_prhatname << endl;
		    //out << "//  " << nice_beamstartname << "\t" << nice_inflectorname << "\t" << nice_fieldname << "\t" << nice_kickname << "\t" << nice_sigmapname << "\t" << nice_prhatname << endl;
		    cout << "  Best Inflector Efficiencies" << endl;
		    for ( int t = 0; t < Nkeep; t++ ) {
		      NiceName(keep_inf_name[t]);
		      //if ( prhatnum < 0 ) { continue; }
		      if ( min_turn_eff[t] > 1e-5 || min_turn_eff[t] == 0.0 ) {
			if ( 0 ) {
			  cout.precision(3);		    
			  cout << "\t" << nice_deltaname << "\t" << nice_launchname << "\t" << min_inf_eff[t];
			  cout.precision(2);
			  cout << " +/- " << min_inf_err[t] << endl;
			}
		      }
		    }
		    //cout << "  Best Storage Efficiencies" << endl;
		    for ( int t = 0; t < Nkeep; t++ ) {
		      NiceName(keep_turn_name[t]);
		      //if ( prhatnum < 0 ) { continue; }
		      if ( min_turn_eff[t] > 1e-5 || min_turn_eff[t] == 0.0 ) {
			cout.precision(3);
			if ( found_deltainf ) {
			  cout << "\t[" << t << "]\t" << nice_deltaname << "\t" << nice_launchname << "\t" << min_turn_eff[t];
			}
			else {
			  cout << "\t[" << t << "]\t" << nice_kickname << "\t" << nice_launchname << "\t" << min_turn_eff[t];
			}
			cout.precision(2);
			cout << " +/- " << min_turn_err[t] << endl;
			if ( is_upstreamcryo ) {
			  out << "eff_uc[\"" << keep_turn_name[t] << "\"] = " << min_turn_eff[t] << ";" << endl;
			  out << "err_uc[\"" << keep_turn_name[t] << "\"] = " << min_turn_err[t] << ";" << endl;
			}
			if ( is_upstreammandrel ) {
			  out << "eff_um[\"" << keep_turn_name[t] << "\"] = " << min_turn_eff[t] << ";" << endl;
			  out << "err_um[\"" << keep_turn_name[t] << "\"] = " << min_turn_err[t] << ";" << endl;
			}
			if ( is_downstreammandrel ) {
			  out << "eff_dm[\"" << keep_turn_name[t] << "\"] = " << min_turn_eff[t] << ";" << endl;
			  out << "err_dm[\"" << keep_turn_name[t] << "\"] = " << min_turn_err[t] << ";" << endl;
			}

			if ( t >= 0 ) {
			  string momname;
			  if ( mom == 5 ) { momname = "kPerfectMatch"; }
			  else if ( mom == 8 ) { momname = "kPerfectMatch_dP05"; }
			  else if ( mom == 9 ) { momname = "kPerfectMatch_dP005"; }
			  else if ( mom == 10 ) { momname = "kPerfectMatch_dP1"; }
			  else if ( mom == 11 ) { momname = "kPerfectMatch_dP001"; }
			  else if ( mom == 12 ) { momname = "kPerfectMatch_dP0001"; }
			  else if ( mom == 13 ) { momname = "kPerfectMatch_dP2"; }
			  else if ( mom == 14 ) { momname = "kPerfectMatch_dP5"; }
			  else if ( mom == 15 ) { momname = "kPerfectMatch_dP10"; }
			  else if ( mom == 16 ) { momname = "kPerfectMatch_dP0075"; }
			  else if ( mom == 17 ) { momname = "kPerfectMatch_dP025"; }
			  else if ( mom == 18 ) { momname = "kPerfectMatch_dP075"; }
			  else if ( mom == 19 ) { momname = "kPerfectMatch_dP0025"; }
			  else if ( mom == 20 ) { momname = "kE821Match_dP05"; }
			  else if ( mom == 21 ) { momname = "kE821Match_dP005"; }
			  else if ( mom == 22 ) { momname = "kE821Match_dP1"; }
			  else if ( mom == 23 ) { momname = "kE821Match_dP001"; }
			  else if ( mom == 24 ) { momname = "kE821Match_dP0001"; }
			  else if ( mom == 25 ) { momname = "kE821Match_dP2"; }
			  else if ( mom == 26 ) { momname = "kE821Match_dP5"; }
			  else if ( mom == 27 ) { momname = "kE821Match_dP10"; }
			  else if ( mom == 28 ) { momname = "kE821Match_dP0075"; }
			  else if ( mom == 29 ) { momname = "kE821Match_dP025"; }
			  else if ( mom == 30 ) { momname = "kE821Match_dP075"; }
			  else if ( mom == 31 ) { momname = "kE821Match_dP0025"; }
			  else { 
			    cout << "Mom = " << mom << endl;
			    momname = "-1";
			    exit(1);
			  }	      
			  string infname;
			  if ( inf == 0 ) { infname = "kOpen"; }
			  if ( inf == 1 ) { infname = "kPartial"; }
			  if ( inf == 2 ) { infname = "kClosed"; }
			  string fieldname;
			  if ( field == 0 ) { fieldname = "kVanish"; }
			  if ( field == 1 ) { fieldname = "kMapped"; }
			  string startname;
			  if ( beamstart == 0 ) { startname = "kDownstreamMandrel"; }
			  if ( beamstart == 1 ) { startname = "kUpstreamMandrel"; }
			  if ( beamstart == 2 ) { startname = "kUpstreamCryo"; }
			  if ( beamstart == 3 ) { startname = "kCentralOrbit_Offset77"; }
			  if ( beamstart == 4 ) { startname = "kCentralOrbit"; }
			  string beamtypename;
			  if ( found_uniform ) { beamtypename = "kUniform"; }
			  else { beamtypename = "kGaussian"; }
			  string quadname;
			  if ( found_newsupports ) { quadname = "kNewSupports"; }
			  if ( found_noquads ) { quadname = "kNoQuads"; }
			  if ( found_noquadsupports ) { quadname = "kNoQuadSupports"; }
			  if ( found_fullhighmom ) { quadname = "kFullHighMom"; }
			  if ( found_fullhighermom ) { quadname = "kFullHigherMom"; }
			  if ( found_fullhighestmom ) { quadname = "kFullHighestMom"; }

			  if ( found_deltainf ) {
			    out_dm2 << "  if ( kick == " << kicknum << " && beamstart == " << startname << " && sigmap == " << momname << " && field == " << fieldname << " && inf == " << infname << " && sigmat == " << sigmatnum << " && beamsize == " << beamsize << " && beamtype == " << beamtypename << " && quad == " << quadname << " && num == " << t << " ) { *eff = " << min_turn_eff[t] << ";  " << "*err = " << min_turn_err[t] << ";" << " *launch = " << launchnum << ";  *delta = " << deltanum << "; " << " return; }" << endl;
			  }
			  else {
			    out_dm2 << "  if ( beamstart == " << startname << " && sigmap == " << momname << " && field == " << fieldname << " && inf == " << infname << " && sigmat == " << sigmatnum << " && beamsize == " << beamsize << " && beamtype == " << beamtypename << " && quad == " << quadname << " && num == " << t << " ) { *eff = " << min_turn_eff[t] << ";  " << "*err = " << min_turn_err[t] << ";" << " *launch = " << launchnum << ";  *kick = " << kicknum << "; " << " return; }" << endl;
			  }
			}
		      }
		    }
		    out_dm2 << endl;
	    
		    cout << endl;
		  }
		} // beamsize
	      } // ts
	    } // moms
	  } // field
	} // inflector
      } // beam start
    }
    cout << endl;

  } // if nfiles > 0

  if ( found_off80 ) {
    out2 << "}" << endl;
    out3 << "}" << endl;
    out4 << "}" << endl;
    out5 << "}" << endl;
    out6 << "}" << endl;
  }

  out.close();
  out2 << endl;
  out2.close();
  out3 << endl;
  out3.close();
  out4 << endl;
  out4.close();
  out5 << endl;
  out5.close();

  out6 << endl;
  out6.close();

  out_dm.close();
  if ( found_bnlmatch_beam && found_bnlmatch_dP10 ) {
    out_dm2 << "  return;" << endl;
    out_dm2 << "}" << endl;
  }
  if ( found_perfectmatch_beam && found_perfectmatch_dP10 ) {
    out_dm2 << "  return;" << endl;
    out_dm2 << "}" << endl;
  }
  out_dm2 << endl;
  out_dm2.close();
  out_dm3 << "}" << endl;
  out_dm3 << endl;
  out_dm3.close();
  out_dm4 << "}" << endl;
  out_dm4 << endl;
  out_dm4.close();
  out_dm5 << "}" << endl;
  out_dm5 << endl;
  out_dm5.close();
  out_dm6 << "}" << endl;
  out_dm6 << endl;
  out_dm6.close();
}
