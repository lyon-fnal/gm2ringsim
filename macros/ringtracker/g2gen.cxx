#include <algorithm>
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TMath.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <iostream>
#include <sstream>
#include <fstream>


#define FBINT 0
#define ICHMOM 0
#define PMUSD 2*3.0830563e-3
#define PMUMEAN 3.0830563
#define TAUMU 2196.9803 //MuLan lifetime in ns
#define GAMMA 29.4

double	c_light = 299.792458; // in mm/ns
double	GeV     = 1000.0; // GeV
double	tesla	= 0.0010;
double	cm	= 10.0;
double	mm	= 1.0;
double  m       = 0.001;
double	um      = 0.0010;
double	nm      = 1.0000000000000002E-6;
double	MHz     = 1000.0;

double mMuon()
{
  static double const mMuon_ = 105.6583715;
  //  G4cout << "mMuon = " << mMuon_/MeV << " MeV/c^2" << G4endl;
  return mMuon_;
}

double aMuon()
{
  static double const aMuon_ = 11659208.9e-10; // avg. of mu+/-
  //  G4cout << "aMuon = " << aMuon_ << G4endl;
  return aMuon_;
}

double pMagic()
{
  static double const pMagic_ = mMuon()/std::sqrt( aMuon() );
  //  G4cout << "pMagic = " << pMagic_/GeV << " GeV/c" << G4endl;
  return pMagic_;
}

double B_magic() // NSF: B_magic = B_magic(R_magic(P_magic(aMuon()))) gotten from (used in) field map?
{
  static double const B_ = 1.451269*tesla; // magnitude of E821 field, used as default
  //  G4cout << "B_ " << B_ << '\n';
  return B_;
}

double R_magic()
{
  static double const R_magic_ = 1.000*pMagic()/( c_light*B_magic() );
  //  G4cout << "R_magic = " << R_magic_/m << " meters" << G4endl;  

  return R_magic_;
}

double gammaMagic()
{
  static double const gammaMagic_ = std::sqrt( 1.+1./aMuon() );
  //  G4cout << "gammaMagic = " << gammaMagic_ << G4endl;
  return gammaMagic_;
}

double betaMagic()
{
  static double const gammaMagic_2_ = gammaMagic()*gammaMagic();
  static double const betaMagic_ = std::sqrt( 1.-1./gammaMagic_2_ );
  //  G4cout << "betaMagic = " << betaMagic_ << G4endl;
  return betaMagic_;
}

double omegaCMagic()
{
  static double const omegaCMagic_ = betaMagic()*c_light/R_magic();
  //  G4cout << "omegaCMagic = " << omegaCMagic_ << " rad/ns" << G4endl;
  return omegaCMagic_;
}


void g2gen()
{
  ifstream in;
  in.open("geninput.dat");
  double dP;
  in >> dP;
  string ptype_ss;
  in >> ptype_ss;
  double tbinh_init;
  in >> tbinh_init;
  string count;
  in >> count;
  in.close();

  cout << "dP/P = " << dP << endl;
  cout << "t-bin = " << tbinh_init << endl;

  enum {kFlat, kGauss};
  int ptype = kGauss;
  if ( ptype_ss == "flat" ) { ptype = kFlat; }
  if ( ptype_ss == "gauss" ) { ptype = kGauss; }

  int num = 1;
  if ( count == "1k" ) { num = 1e3; }
  if ( count == "1M" ) { num = 1e6; }
  if ( count == "10M" ) { num = 1e7; }
  if ( count == "25M" ) { num = 25e7; }
  if ( count == "50M" ) { num = 50e7; }
  if ( count == "100M" ) { num = 1e8; }
  if ( count == "250M" ) { num = 2.5e9; }
  if ( count == "500M" ) { num = 5.0e9; }
  if ( count == "1B" ) { num = 1e9; }

  double fnal1half[23] = { 150, 750, 500, 450, 400, 400, 580, 620,1200,1300, 1600,2200,2400,2400,2400,2900,1750,3500,2500,2400,3200,2300,2750};
  TH1D *ht0 = new TH1D("ht0","ht0",46-FBINT*2,-23.0+FBINT,23.0-FBINT);
  ht0->SetXTitle("t0 bin");
  for(int j=0;j<23-FBINT;j++) {
    ht0->SetBinContent(j+1,fnal1half[j+FBINT]);
  }
  for(int j=FBINT;j<23;j++) {
    ht0->SetBinContent(46-j-FBINT,fnal1half[j]);
  }
  double tbinh = 1;
  if ( tbinh_init > 0 ) { tbinh = 3.636 * tbinh_init; }
  else { tbinh = tbinh_init; }
  //tbinh = 10;





  double signal[100000];
  int ichmom = ICHMOM;
  TRandom3 re;
  re.SetSeed(6280225);
  char   outfile[80];

  double pmusd = PMUSD;
  if ( dP > 0 ) { 
    pmusd = 0.01*dP*PMUMEAN;
  }
  double pmumean = PMUMEAN;

  cout << "<p>    = " << GeV*pmumean << " +/- " << GeV*pmusd << " GeV." << endl;

  // <t> = (ns/s) * 2pi * R (in cm) / (beta * c (in cm/s?)) [unit=ns]
  // <t> = 2pi/omega_{c}; omega_{c} = beta * c_light / R;
  //double average_revolution_time = 1.0e9 * TMath::TwoPi() * 711.2 / (betaMagic()*2.9979246e10);

  double average_revolution_time = TMath::TwoPi() * R_magic() / (betaMagic()*c_light);

  // <f> = [unit=1/ns];
  double average_revolution_frequency = 1.0/average_revolution_time;

  cout << "<t>   = " << average_revolution_time << endl;
  cout << "<R>   = " << R_magic() << endl;
  cout << "<v/c> = " << betaMagic() << endl;
  cout << "<f>   = " << average_revolution_frequency*MHz << endl;

  //return;


  // rang == range?
  double pmurang = pmumean + pmusd;
  double freqrang = 0.020794769/pmurang;  // 0.02079 ~ 1/48
  double freqsd = average_revolution_frequency - freqrang;
  double freqlo = average_revolution_frequency-freqsd*3.;
  double freqhi = average_revolution_frequency+freqsd*3.;
  freqsd *= MHz;
  freqlo *= MHz;
  freqhi *= MHz;
  double pmulo = pmumean-3*pmusd;
  double pmuhi = pmumean+3*pmusd;
  double taumu = TAUMU;
  double t0mean = 0.0;
  double t0sd = 1.0;
  double pmu;
  int ndet = 24;


  TH1D *ht0time = new TH1D("ht0time","ht0time",100, 5-5, 5+5);

  TH1D *hFreqSpec = new TH1D("hFreqSpec","Frequency Spectrum (MHz)",101,freqlo,freqhi);
  hFreqSpec->SetXTitle("Frequency Spectrum [MHz]");

  TH1D *hMomSpec = new TH1D("hMomSpec","Momentum Spectrum (GeV/c)",101,pmulo,pmuhi);
  hMomSpec->SetXTitle("Momentum Spectrum [GeV/c]");

  TH1D *hT0Pulse = new TH1D("hT0Pulse","T0Pulse (ns) ",200,-100.0,100.0);
  hT0Pulse->SetXTitle("t0 Pulse [ns]");

  TH1D  *hSignal[24];
  TH1D *hTimeDecay = new TH1D("hTimeDecay","Muon Decay Time ",50000,0.0,50000.0);
  hTimeDecay->SetXTitle("Muon Decay Time [ns]");

  TH1D *hDetector = new TH1D("hDetector","Detector Number ",24,-0.5,23.5);
  hDetector->SetXTitle("Detector Number");

  TH1D *hnTurns = new TH1D("hnTurns","Number of turns",10000,0.0,10000.0);
  hnTurns->SetXTitle("Number of Turns");

  char hnam[20], hnam2[30];
  for(int idet=0;idet<24;idet++) { 
    sprintf(hnam,"hSignal%02d",idet);
    hSignal[idet] = new TH1D(hnam,"Signal",50000,0.0,50000.0);
    sprintf(hnam2,"Signal %02d [ns]",idet);
    hSignal[idet]->SetXTitle(hnam2);
  }


  //Initialize the signal
  for(int i = 0;i<100000;i++) {
    signal[i] = 0.0;
  }

  TStopwatch w;
  w.Start();

  //The momentum distribution will be gaussian
  //  if(ichmom == 0) {
  int Nmuons = num;
  
  cout << "Will generate " << count << " muons." << endl;
  
  for(int iev=0;iev<Nmuons;iev++) {
  
    if( iev%(num/10) ==0 && iev > 0 ) {
      //printf("Muon number %d \n",iev);
      w.Stop();
      double time = w.RealTime();
      double rate = (double)iev/time;
      double exp_time = Nmuons/rate;
      double rem_time = exp_time - time;
      cout.precision(3);
      std::cout << "Number " << 100*(double)iev/Nmuons << "%. Time = " << time << ". Remaining time = " << rem_time << endl;
      w.Continue();
    }
    //   pmu = re.Uniform(pmumean-3*pmusd,pmumean+3*pmusd);

    //------------------------------------
    // Generate Muon Momentum w/ dP/P = 2%
    //------------------------------------
    if ( ptype == kGauss ) {
      pmu = re.Gaus(pmumean,pmusd);
    }
    if ( ptype == kFlat ) {
      pmu = pmumean + pmusd*(2.0*re.Uniform(1)-1);
    }
    //      double randb = re.Uniform(1.0)*1000;
    // int nran = (int)randb;
    // randb = (nran+re.Uniform(1.0))*1.0e-3;
   
#if 0
    double randb = re.Uniform(1.0);
    double rnmut = -log(1.0-randb);
    double taudil = GAMMA*taumu*pmu/pmumean;
    double muondecay_time = taudil*rnmut;
#endif
   
    //----------------------------------------
    // Generate a random decay time [0, 50] us
    //----------------------------------------
    double muondecay_time = 50000.0*re.Uniform(1.0);
   
    //muondecay_time is the time of decay for this muon
    hTimeDecay->Fill(muondecay_time);
   

    double t0 = 0.0;
    //---------------------------------------
    //Select a T0 for the muon (ns)
    if ( tbinh < 0 ) {
      t0 = re.Uniform(-1*tbinh);
      t0 = t0 - (-1*tbinh)/2.0;
      //t0 = re.Gaus(t0mean,-1*tbinh);
    }
    if ( tbinh > 0 ) {
      double tget = ht0->GetRandom();
      t0 = tget*tbinh;
    }
    //      double t0 = re.Uniform(10.0);
    //t0 correlated to momentum
    //---------------------------------------
    //
    // t0sd = uncertainty on t0
    // t0 is earlier if p > pmagic
    // t0 is later if p < pmagic
    //
    // t0 = C - (P-Pm/sig(P))sig{t}
    double dpP = (pmu-pmumean)/pmusd;
    //cout << pmu << "\t" << pmumean << "\t" << pmusd << "\t" << dpP << "\t" << t0sd << "\t";
    //double t0 = 0.0 - ((pmu-pmumean)/pmusd)*t0sd;
    //cout << t0 << endl;
    //double 
    ht0time->Fill(t0);

#if 0
    double t0 = re.Uniform(t0mean-t0sd,t0mean+t0sd);
#endif
    double revolution_time = average_revolution_time*pmu/pmumean; //This is incorrect!!

    double frev = 1./revolution_time;
    hMomSpec->Fill(pmu);
    hFreqSpec->Fill(MHz*frev);
      
#if 0
    //Maximal correlation of t0 and momentum
    if(pmu>pmumean) {
      t0 = -fabs(t0);
    }
    else {
      t0 = fabs(t0);
    }
#endif
    double t0fill = t0;
    if(t0<-148.0/2.0) t0fill = t0+148.0;
    if(t0>148.0/2.0) t0fill = t0-148.0;
    hT0Pulse->Fill(t0fill);
    
    //----------------------------------
    // nTurns = (t0 + t)/revolution_time
    //----------------------------------
    double nturns = (muondecay_time + t0)/revolution_time; //number of turns
    hnTurns->Fill(nturns);
    double time_since_last_revolution = nturns-(int)nturns;


    //-----------------------------
    // t = [nTurns - dT]*t_{c} + t0
    //-----------------------------
    double tfill = (nturns - time_since_last_revolution)*revolution_time+t0;
    int cdet = 0;
    //      int cdet = (int)(ndet*time_since_last_revolution); 
    //      double t = muondecay_time-average_revolution_time*(time_since_last_revolution-(time_since_last_revolution%(1.0/(double)ndet)));
    //Naive losses
    hSignal[cdet]->Fill(tfill);
#if 0
    if(tfill<4000.0 || (tfill>=4000.0 && pmu<pmumean+pmusd)) {
      hDetector->Fill((float)cdet);
      if(cdet>-1 && cdet<24) {
        hSignal[cdet]->Fill(tfill);
      }
    }
#endif
  }

  
  for ( int i = 0; i < 24; i++ ) {
    for ( int bin = 1; bin <= hSignal[i]->GetNbinsX(); bin++ ) {
      if ( hSignal[i]->GetBinContent(bin) <= 0 ) { hSignal[i]->SetBinContent(bin, 1.0/Nmuons); }
    }
  }


  int nn = 94;
  //sprintf(outfile,"g2gen%02d.root",nn);
  //printf("file is %s \n",outfile);
  stringstream outfilename;
  outfilename << "rootfiles/fastrotation/g2gen" << "_";

  if ( ptype == kFlat ) { outfilename << "Flat"; }
  if ( ptype == kGauss ) { outfilename << "Gauss"; }

  if ( dP > 0 ) { outfilename << "dP" << dP; }
  else { outfilename << "0.2dP"; }

  outfilename << "_";

  if ( tbinh_init > 0 ) { outfilename << "FNALx" << tbinh_init; }
  else if ( tbinh_init < 0 ) { outfilename << "Sqtbin" << -1*tbinh_init; }
  else { outfilename << "Deltat0"; }

  outfilename << ".root";

  cout << "Writing " << outfilename.str() << endl;
  cout << "./plotfast.sh " << outfilename.str() << endl;
  TFile *fout = new TFile(outfilename.str().c_str(),"RECREATE");
  hDetector->Write();
  hMomSpec->Write();
  hnTurns->Write();
  hFreqSpec->Write();
  hT0Pulse->Write();
  ht0->Write();
  ht0time->Write();
  hTimeDecay->Write();
  for(int idet=0;idet<24;idet++) {
    hSignal[idet]->Write();
  }
  fout->Write();
  fout->Close();
}
