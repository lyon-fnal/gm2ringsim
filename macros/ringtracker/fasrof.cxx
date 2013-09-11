#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>

#include <TMinuit.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TF1.h>
#include <TAxis.h>
#include <TLine.h>
#include <TMath.h>

//#include "Math/GSLMinimizer.h"
#include "Math/Functor.h"
#include "Minuit2/Minuit2Minimizer.h"

using namespace std;


#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraph2D.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TMatrixD.h"
#include "TVector.h"

#define MAXREP 1
#define ICHMOM 0
#define PMUSD 3.0830563e-3 // Sigma momentum
#define PMUMEAN 3.0830563  // Mean momentum
#define TAUMU 2196.9803    // MuLan lifetime in ns
#define GAMMA 29.4         // Magic gamma
#define MAXRAD 202            // num of radial bins
#define NUM 20000          // Time in ns to collect data
#define MAXTIMES 20000

double lambscale;
string hist;
int NRAD = 51;
double a[MAXRAD][MAXRAD];
double b[MAXRAD];
int start_time = 500; // in ns
int end_time = 20000; // in ns
int first_time_bin = 250; //limits of analysis
int last_time_bin = 10000;
double um[MAXTIMES][MAXRAD]; // NUM is time, NRAD is radius
double sol[MAXRAD];
double qc[MAXTIMES]; // calorimeter hits for each ns up to 100 us.
double t[MAXTIMES]; // [time in ns]
bool first_pass = true;

ofstream out;

//-------------------------------------------------------------------------

// fcn passes back f = - 2*ln(L), the function to be minimized.

// end of fcn

//-------------------------------------------------------------------------



void fasrof_run(double pw, int t0binsize, string outputfilebase)
{
  cout << "fasrof_run(" << pw << " , " << t0binsize << " , " << outputfilebase << ")" << endl;

  double momspec[MAXRAD];                // momentum for radial bin
  double radw = 1.42*(double)NRAD/9.0; // this is the sigma of the radial bin or something...



  //----------------------------------------------------------------
  // Some sort of transformation matrix relating nearest radial bins
  //----------------------------------------------------------------
  double lambda = 0.1;
//   if ( outputfilebase.find("dP0.5") != string::npos ) { lambda = 1; }
//   if ( outputfilebase.find("dP0.25") != string::npos ) { lambda = 0.1; }
//   if ( outputfilebase.find("dP0.1") != string::npos ) { lambda = 0.01; }
//   if ( outputfilebase.find("dP0.05") != string::npos ) { lambda = 0.005; }
  
//   if ( outputfilebase.find("dP2") != string::npos ) { lambda = 1; }
//   if ( outputfilebase.find("dP05") != string::npos ) { lambda = 0.005; }

  lambda = 1;
  lambda = lambscale;
  cout << "Base Lambda = " << lambda << endl;

  string simplebase = outputfilebase;
  string removes[4];
  removes[0] = "NewSupports_200Turns_muplus_NoSpinTracking_CentralOrbit_";
  removes[1] = "NoKick_InflectorOpen_NoDelta_NoLaunch_";
  removes[2] = "UniformInflectorGun_";
  removes[3] = "CollimatorON_";
  for ( int i = 0; i < 0; i++ ) {
    cout << i << "\t" << simplebase << endl;
    cout << removes[i] << endl;
    unsigned int pos = simplebase.find(removes[i]);
    cout << "  POS = " << pos << endl;
    if ( pos != string::npos ) {
      string str1, str2;
      str1 = simplebase.substr(0, pos);
      //cout << "  STR1 = " << str1 << endl;
      str2 = simplebase.substr(pos + removes[i].size());
      //cout << "  STR2 = " << str2 << endl;
      simplebase = str1 + str2;
    }
    cout << i << "\t" << simplebase << endl;
  }

 
  stringstream outputfile;
  outputfile << "rootfiles/fastrotation/" << simplebase;
  outputfile << "_" << "Delta" << pw*1000 << "ns_t0" << t0binsize << "ns_" << "NRad" << NRAD << "_" << "Lamb" << lambda << ".root";
  
  stringstream datfile;
  datfile    << "rootfiles/fastrotation/" << simplebase;
  datfile    << "_" << "Delta" << pw*1000 << "ns_t0" << t0binsize << "ns_" << "NRad" << NRAD << "_" << "Lamb" << lambda << ".dat";
  //out.open(datfile.str().c_str());
  
  cout << "OUTPUT FILE = " << outputfile.str() << endl;


  double minlambda = lambda;
  double maxlambda = lambda;
  double bestdist = 0.0;
  double bestlambda = lambda;
  double diffdist = 0.0;
  if ( lambda <= 0.0 ) {
    minlambda = 1e-3;
    maxlambda = 10;
  }



  
  //double zn = 0.1;
  double tbin = 0.001;  // 1 ns
  if ( t0binsize > 1 ) { tbin = (double)t0binsize/1000; }
  double rcent = 711.2; // in cm
  int jim = NRAD;
  //double pw = 0.148;    // time in microseconds
  //pw = 0.002;
  // double pw = 0.02;  //
  double t0inc = 0.0;
  double tbin1 = 0.0;
  double tau = 0.0;
  double tbin11 = 0.0;
  double dtry = 0.05;
  double filfac = 1.0;
  int brep = 3;


  double sumNc = 0.0;
  //---------------------------------
  // Number of t0 bins
  //
  // pw = 0.148 us
  // tbin = 0.001 us
  // nt0bins ~= 148
  //---------------------------------
  int nt0bins = (int)(pw/tbin+0.0001);
  //   cout << "PW = " << pw << endl;
  //   cout << "tbin = " << tbin << endl;
  //   cout << "radw = " << radw << endl;
  //   cout << "nt0bins = " << nt0bins << endl;


  int rmlife = 0;
  //double muamp = 0.3;
  //double mufreq = 1.4253;
  double hpw = 0.5*pw;
  double hpwsq = hpw*hpw;
  double tmin = 0.050;  //Start of analysis in microseconds
  double tmax = 50.0; //End of analysis in microseconds
  int ndim = 2000;
  double width = 9.0;	// total radial width in cm

  //------------------------------------------------------------
  // Individual radial bin width ( 9.0 cm / N (N=50) = 0.18 cm )
  //------------------------------------------------------------
  double binwid = width/float(jim);
  cout << "Radial Bin Width: " << binwid << endl;
#if 1
  for(int i=0;i<MAXTIMES;i++) {
    for(int j=0;j<NRAD;j++) {
      um[i][j] = 0.0;
    }
  } 
#endif
  double rhat[MAXRAD];
  double r[MAXRAD]; // [NRAD]
  double tt[MAXRAD]; // [NRAD]
  double fi[MAXRAD]; // [NRAD]
  double tindex[MAXTIMES];
  double genspec[MAXTIMES];  // [time in ns]
  double bfld = 1.45;  // B magic
  double be = 0.99942;  // beta_{magic}
  double sigsum;
  double sig;
  double t0sd = 0.01;
  double t0mean = 0.0;
  double t0weight[1000];
  double tinc = -0.0;

  for(int i=0;i<200;i++) { t0weight[i] = 0.0; }
  //for ( int i = 0; i < 100000; i++ ) { tindex[i] = 0; }

#if 0
  for(int i=0;i<nt0bins;i++) {
     
    int t0binh = (int)(((float)nt0bins/2.0)+0.5); 
    double arg = ((double)i+tinc-(double)nt0bins/2.0)*tbin;
    t0weight[i] = exp(-(arg*arg)/(2.0*t0sd*t0sd));
    //printf("index %d weight %f \n",i,t0weight[i]);
  }
#endif

  //Fetch the data
  char hfname[20];
  int series = 94;
  printf("Get the t0 data \n");
  //sprintf(hfname,"g2gen%02d.root",series);

  string inputfilename = "g2gen_" + outputfilebase + ".root";
  inputfilename = "rootfiles/fastrotation/" + outputfilebase + ".root";
  TFile *f1 = TFile::Open(inputfilename.c_str());
  if ( f1 == NULL ) {
    cout << "Could not open " << inputfilename << endl;
    return;
  }
  //f1->ls();
  //get t0pulse
#if 1


  //
  //
  //
  TH1D *hEqRad = (TH1D*)f1->Get("EqRad");
  TH1D *hXe   = (TH1D*)f1->Get("Xe");

  //-------------------------------------
  // Get t0 pulse from previous run (in ns)
  //-------------------------------------
  TH1D *ht0 = (TH1D*)f1->Get("hT0Pulse");
  //if ( t0binsize > 1 ) { ht0->Rebin(t0binsize); }
  if ( ht0->GetBinWidth(1) != t0binsize ) {
    cout << "t0 bin no longer is correct...." << endl;
    return;
  }
  double t0pulsebin = ht0->GetBinWidth(1);


  TGraph *gr_t0 = new TGraph();
  double t0pulseval[MAXRAD];
  double t0val[MAXRAD];

  int middle = 0;
  int first = -1;
  int last = -1;
  double first_val, last_val;
  for(int i=0;i<200;i++) {
    t0pulseval[i] = ht0->GetBinContent(i+1);
    if ( t0pulseval[i] > 0 ) {
      if ( first < 0 ) { first = i; first_val = ht0->GetBinCenter(i+1); }
      last = i; last_val = ht0->GetBinCenter(i+1);
    }
    t0val[i] = ht0->GetBinCenter(i+1);
    //cout << "t0 Pulse: " << i << "\t" << t0val[i] << "\t" << t0pulseval[i] << endl;
    //printf("i %d %d t0pulseval %f \n",i,t0val[i],t0pulseval[i]);
    gr_t0->SetPoint(i, t0val[i], t0pulseval[i]);
  }
 
  // t0pulse data goes from 0 -> 200 based on data from -100 to 100 ns
  // t0 weight goes form 0 -> 150 w/ 50 empty bins


  //------------------------------------
  // First guess of beam is something centered at the middle of the ring...
  //------------------------------------
  double t0sum = 0.0;
  int t0avg = (int)(first + last)/2;
  double early_t0 = 99999.9;
  double late_t0 = -99999.9;
  for(int i=0;i<nt0bins;i++) {
    //cout << i << "\t" << t0avg-(int)(nt0bins/2)+i << endl;
    t0weight[i] = t0pulseval[t0avg-(int)(nt0bins/2)+i];
    if ( t0weight[i] > 0 && early_t0 > 99999 ) { early_t0 = t0avg-(int)(nt0bins/2)+i; }
    if ( t0weight[i] > 0 ) { late_t0 = t0avg-(int)(nt0bins/2)+i; }
    //printf("i %d t0weight %f \n",i,t0weight[i]);
    t0sum += t0weight[i];
    //cout << "t0 Weight: " << i << "\t" << 100-(int)(nt0bins/2)+i << "\t" << t0weight[i] << "\t" << t0sum << endl;
  }



  cout << "--------------- t0 Info ---------------" << endl;
  cout << "Num t0 Bins:     " << nt0bins << endl;
  cout << "t0 Bin Width:    " << t0pulsebin << endl;
  cout << "First Bin:       " << first_val << "\t" << first << endl;
  cout << "Last Bin:        " << last_val << "\t" << last << endl;
  cout << "First >0 t0 Bin: " << early_t0 << endl;
  cout << "Last >0 t0 Bin:  " << late_t0 << endl;
  cout << "---------------------------------------" << endl;


  //-------------------------------------
  // Get probability density of t0 counts
  //-------------------------------------
  TGraph *gr_t0weight = new TGraph();
  for(int i=0;i<nt0bins;i++) {
    t0weight[i] = t0weight[i]/t0sum;
    //printf("index %d weight %f \n",i,t0weight[i]);
  }

  for ( int i = 0; i < 200; i++ ) {
    gr_t0weight->SetPoint(i, i, t0weight[i]);
  }

#endif


  //----------------------------------
  // Get station 00 wiggle information
  //----------------------------------
  string histname = "";
  if ( hist == "Muon" ) { histname = "hSignalAllDet"; }
  else if ( hist == "Positron" )  { histname = "hSignalAllDet_BirthElectron_EgtEth"; }
  else {
    cout << "Could not find histogram for " << hist << endl;
    return;
  }

  if ( hist == "Muon" ) {
    cout << "Using Muon Truth Information" << endl;
  }

  if ( hist == "Positron" ) {
    cout << "Using Positron Truth Information" << endl;
  }

  TH1D *hfr = (TH1D*)f1->Get(histname.c_str());
  if ( hfr == NULL ) {
    cout << "Could not find " << histname << endl;
    return;
  }

  
  //if ( t0binsize > 1 ) { hfr->Rebin(t0binsize); }
  if ( hfr->GetBinWidth(1) != tbin*1000 ) {
    cout << "t0 bin no longer is correct...." << endl;
    cout << "Signal Bin Size = " << hfr->GetBinWidth(1) << endl;
    cout << "T0 Bin Size = " << tbin*1000 << endl;
    return;
  }

  if( outputfilebase.find("PerfectMatch_dP05") != string::npos ) {
    end_time = 12000;
  }
  //if( outputfilebase.find("GausstSigma25") != string::npos ) {
  //end_time = 11000;
  //}

  first_time_bin = hfr->FindBin(start_time);
  last_time_bin = hfr->FindBin(end_time);
  cout << "------------------- Start/Stop -------------------" << endl;
  cout << "Time Bin Size:  " << hfr->GetBinWidth(1) << " ns." << endl; 
  cout << "First Time Bin: " << first_time_bin << " (" << start_time << " ns)" << endl;
  cout << "Last Time Bin:  " << last_time_bin << " (" << end_time << " ns)" << endl;
  cout << "--------------------------------------------------" << endl;
  
  hfr->SetLineColor(3);
  //hfr->Draw();

  for ( int j = 0; j < MAXTIMES; j++ ) {
    qc[j] = 0.0;
    t[j] = 0.0;
  }
  for(int ib = 8;ib<hfr->GetNbinsX();ib++) { // 8 and 50000 seems arbitrary...
    if ( ib + 1 > MAXTIMES ) { break; }
    qc[ib-0] = hfr->GetBinContent(ib);
    //if(ib<320) printf("input bin %d value %f \n",ib-3,qc[ib-3]);
  }

  // f1->Close();
  // JIM <==> Number of radial bins
  for(int i=0;i<NRAD;i++) {
    b[i] = 0.0;
    r[i] = 1*rcent-width/2.0+binwid*((double)i+0.5); // radius at radial bin 'i'.
    rhat[i] = (r[i] - rcent)*10; // in mm
    fi[i] = bfld*10000; //field in gauss            // magnetic field in bin 'i' (in gauss?)
    //c        PRINT*,I,R(I),FI(I),BE
    // t(r) = (ns/s) * 2pi * R (in cm) / (beta * c (in cm/s?)) [unit=ns]
    // t(r)= 2pi/omega_{c}; omega_{c} = beta * c_light / R;
    tt[i] = 1.0e4*r[i]*2*3.141593/(2.9979246e8*0.99942); // time it takes to travel radial bin 'i'.
    //printf("Division %d Radius %f Field %f Period (microsec) %f \n",i,r[i],fi[i],tt[i]);
  }


  cout << "Created rhat,t,tt, and radii" << endl;


  TGraph *gr = new TGraph(jim, r, tt);
  gr->GetXaxis()->SetTitle("Radius [cm]");
  gr->GetYaxis()->SetTitle("Period [#mus]");
  gr->SetTitle("");

  TGraph *gr_inv = new TGraph(jim, tt, r);
  gr_inv->GetXaxis()->SetTitle("#tau_{c} [ns]");
  gr_inv->GetYaxis()->SetTitle("r [cm]");
  gr_inv->SetTitle("");

  cout << "Tbin = " << tbin << endl;

  TH1D *hdiff;
  TH1D *hmomspec = new TH1D("hmomspec","Spectrum",NRAD, -0.5*NRAD/9.0, +0.5*NRAD/9.0);
  hmomspec->SetXTitle("radius [cm]");
  TH1D *hum = new TH1D("hum","hum",50000/(tbin*1000),0.0,50000.0);
  hum->SetXTitle("time [ns]");
  TH2D *humspec = new TH2D("humspec","humspec",50000/(tbin*1000),0.0,50000.0, NRAD, -0.5*NRAD/9.0, +0.5*NRAD/9.0);
  humspec->SetXTitle("time [ns]");
  humspec->SetYTitle("radius [cm]");
  TH1D *hcheck = new TH1D("hcheck","hcheck",50000/(tbin*1000),0.0,50000.0);
  hcheck->SetXTitle("time [ns]");
  TH1D *hspread = new TH1D("hspread","hspread",50000/(tbin*1000),0.0,50000.0);
  hspread->SetXTitle("time bin (radial bin 20)");
  TH1D *hspread10 = new TH1D("hspread10","hspread10",50000/(tbin*1000),0.0,50000.0);
  hspread10->SetXTitle("time bin (radial bin 1)");
  TH1D *hspread30 = new TH1D("hspread30","hspread30",50000/(tbin*1000),0.0,50000.0);
  hspread30->SetXTitle("time bin (radial bin 39)");		       
  hum->SetLineColor(4);
  hcheck->SetLineColor(5);

  TGraph *g_HitDet = new TGraph();
  TGraph *g_del25 = new TGraph();
  TGraph *g_Ca25 = new TGraph();
  TGraph *g_CaIdec25 = new TGraph();
  TGraph *g_Cb25 = new TGraph();
  TGraph *g_X25 = new TGraph();
  TGraph *g_XQ25 = new TGraph();
  TGraph *g_Q25 = new TGraph();
  TGraph *g_tj25 = new TGraph();
  TGraph2D *g_NtjQ25 = new TGraph2D();
  TGraph *g_Ntj25 = new TGraph();
  TGraph *g_QCa25 = new TGraph();
  TGraph *g_DelPWg25 = new TGraph();
  TGraph *g_Region25 = new TGraph();
  TGraph *g_Betagk25 = new TGraph();
  //g_del25->GetXaxis()->SetXTitle("#delta #equiv #DeltaR/R_{i} #times t  [#mus]");
  int ndel25 = 0;
  int nreg25 = 0;

  TGraph2D *gr_um_init = new TGraph2D();
  TGraph2D *gr_um_final = new TGraph2D();

  TGraphErrors *gr_eqrad_passone = new TGraphErrors();
  TGraphErrors *gr_eqrad_passtwo = new TGraphErrors();
  TGraphErrors *gr_eqrad_passthree = new TGraphErrors();

//   TH1D  *hpulse[15];


//   char hnam[20];
//   for(int iturn=0;iturn<15;iturn++) { 
//     sprintf(hnam,"hpulse%02d",iturn);
//     hpulse[iturn] = new TH1D(hnam,"Pulse by turn",3000,-0.150,0.150);
//     hpulse[iturn]->SetLineColor(iturn+1);
//     hpulse[iturn]->SetXTitle("x [mm]");
//   }



  //--------------------------------------
  // Get an array of all times
  //--------------------------------------
  //int first_time_bin = (int)(tmin/tbin+1.0);
  //int last_time_bin = (int)(tmax/tbin+1.0);
  int idec;
  int iend = last_time_bin+1;
  cout << "IEND = " << iend << endl;
  //Write time bin values into t[j]
  for(int j=0;j<iend;j++) {
    tindex[j] = j;
    t[j] = ((double)(j)-1.0)*tbin;
    //  printf("time bin %d time %f \n",j,t[j]);
  }
  //int t0binh = (int)(((float)nt0bins/2.0)+0.5); 

  //   TH1D *hfr2 = (TH1D*)hfr->Clone("hfr2");
  //   hfr2->Sumw2();
  //   TH1D *hgen = (TH1D*)hfr->Clone("hgen");
  //   hgen->Sumw2();

  TH1D *hfr2 = new TH1D("hfr2", "", hfr->GetNbinsX(), hfr->GetXaxis()->GetXmin(), hfr->GetXaxis()->GetXmax());
  hfr2->Sumw2();
  TH1D *hgen = new TH1D("hgen", "", hfr->GetNbinsX(), hfr->GetXaxis()->GetXmin(), hfr->GetXaxis()->GetXmax());
  hgen->Sumw2();

  TGraph *gr_MC_Signal = new TGraph();
  TGraph *gr_Data_Signal = new TGraph();

  hfr2->SetXTitle("time [ns]");
  hgen->SetXTitle("time [ns]");

  //   for ( int i = 1; i <= hfr2->GetNbinsX(); i++ ) {
  //     hfr2->SetBinContent(i, 0.0);
  //     hgen->SetBinContent(i, 0.0);
  //   }

  TGraph *gr_t = new TGraph(iend, tindex, t);
  gr_t->GetXaxis()->SetTitle("Time Bin");
  gr_t->GetYaxis()->SetTitle("Time [#mus]");
  gr_t->SetTitle("");


  //--------------------------------------
  // Beta Values from CERNIII paper
  //--------------------------------------
  double betagsav[MAXRAD];
  double betagk[MAXRAD];
  double betag[MAXRAD];
  //Calculation for each time bin (signal from first_time_bin to last_time_bin)
  //======================================================
  

  //------------------------------
  // Number of degrees of freedom?
  //------------------------------
  int ndfr=1-jim; //???
  double chiqs;
  double bb[MAXRAD];

  bool stop = false;
  int Nreps = 0;
  double step = TMath::Sqrt(10);
  int Nlambs = 0;
  int MaxLambs = 4;
  double KSvals[10];
  double Distvals[10];
  double Lambs[10];
  double lamb = 0.0;
  cout << "Starting Lambda Scan..." << endl;
  while ( Nlambs < MaxLambs ) {    
    if ( Nlambs == 0 ) { lamb = minlambda; }
    if ( Nlambs == 1 ) { lamb = maxlambda; }
    if ( Nlambs == 2 ) { 
      if ( fabs(Distvals[0]/Distvals[1]-1) < 0.05 ) { stop = true; }
      lamb = (TMath::Log10(minlambda) + TMath::Log10(maxlambda))/2.0;
      cout << "ExpLamb = " << lamb << endl;
      lamb = TMath::Power(10, lamb);
    }

    double g1, g2;
    if ( Nlambs == 3 ) {
      stop = true;
      if ( fabs(Distvals[0]/Distvals[1]-1) < 0.05 && fabs(Distvals[1]/Distvals[2]-1) < 0.05 ) {
	lamb = (TMath::Log(minlambda) + TMath::Log(maxlambda))/2.0;
	lamb = TMath::Power(10, lamb);
      }
      else {
	double diff12 = fabs(Distvals[0]/Distvals[1]-1);
	double diff23 = fabs(Distvals[1]/Distvals[2]-1);
	double diff13 = fabs(Distvals[0]/Distvals[2]-1);
	
	
	// 1 is very different from 23
	if ( diff12 > 0.05 && diff23 < 0.05 ) {
	  // if 1 is better than 23
	  if ( Distvals[0] < Distvals[1] ) {
	    lamb = (TMath::Log(Lambs[0]) + TMath::Log(Lambs[1]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	  else {
	    // 23 is better than 1
	    lamb = (TMath::Log(Lambs[1]) + TMath::Log(Lambs[2]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	}

	// 12 is very different from 3
	if ( diff12 < 0.05 && diff23 > 0.05 ) {
	  // if 3 is better than 12
	  if ( Distvals[2] < Distvals[1] ) {
	    lamb = (TMath::Log(Lambs[1]) + TMath::Log(Lambs[2]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	  else {
	    // 12 is better than 3
	    lamb = (TMath::Log(Lambs[0]) + TMath::Log(Lambs[1]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	}

	// 1 is very different from 2 AND 2 is very different from 3
	if ( diff12 > 0.05 && diff23 > 0.05 ) {	  
	  // if 1 is better than 2 or 3
	  if ( Distvals[0] < Distvals[1] && Distvals[0] < Distvals[2] ) {
	    lamb = (TMath::Log(Lambs[0]) + TMath::Log(Lambs[1]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	  // if 2 is better than 1 or 3
	  else if ( Distvals[1] < Distvals[0] && Distvals[1] < Distvals[2] ) {
	    if ( Distvals[0] < Distvals[2] ) {
	      lamb = (TMath::Log(Lambs[0]) + TMath::Log(Lambs[1]))/2.0;
	      lamb = TMath::Power(10, lamb);	    
	    }
	    else {
	      lamb = (TMath::Log(Lambs[2]) + TMath::Log(Lambs[1]))/2.0;
	      lamb = TMath::Power(10, lamb);	    
	    }
	  }
	  // if 3 is better than 2 or 3
	  else {
	    lamb = (TMath::Log(Lambs[1]) + TMath::Log(Lambs[2]))/2.0;
	    lamb = TMath::Power(10, lamb);	    
	  }
	}
      }
    }

    Lambs[Nlambs] = lamb;

    //for ( double lamb = minlambda; lamb <= maxlambda; lamb *= step ) {
    
    Nreps++;

    cout << "=================== Lambda = " << lamb << " ===================" << endl;
    

    double sdw[MAXRAD][MAXRAD];
    TMatrixD lm(NRAD, NRAD);
    for(int n=0;n<NRAD;n++) {
      for(int k=0;k<NRAD;k++) {
	sdw[n][k] = 0.0;
	if(n==k) sdw[n][k] = lamb*2.0;
	if(n==k-1) sdw[n][k] = -1.0*lamb;
	if(n==k+1) sdw[n][k] = -1.0*lamb;

	//lm[n][k] = sdw[n][k];
      }
    }


    for(int irep=1;irep<=MAXREP;irep++) {

      //----------------------------------
      // Save and initialize a[][] and b[]
      //----------------------------------
      for(int n1=0; n1<jim; n1++) {
	bb[n1] = b[n1];
	b[n1] = 0;
	for(int i1=0;i1<jim;i1++) {
	  a[n1][i1] = 0.0;
	}
      }

      //printf("Calculate propagation factors \n");
      //printf("first time bin %d last %d \n",first_time_bin,last_time_bin);
      int ndatanp=0;
      for(int j = first_time_bin;j<=last_time_bin;j++) {
	if ( irep == 1 ) {
	  hfr2->SetBinContent(j, qc[j]);
	  gr_Data_Signal->SetPoint(ndatanp++, t[j], qc[j]);
	}
	//if ( j % 100 == 0 ) { cout << j << "/" << last_time_bin << endl; }
	//and for each radial bin:
	//========================
	for(int n = 0;n<jim;n++) {

	  //      Calculate parameters of the skew of the pulse:
	  double drr = binwid/r[n]; //relative binwidth
	  //double del = 0.0; //full stretching in time
	  double cb = 0.0; // half total pulse width
	  double ca = 0.0; //total width
	  //double cc = 0.0; //stretching compared to original half width
	
	  //       Calculation of BETAG(N), the BETA factors of the skew
	  //       pulse or Graziano pulse according to INPULS:=========
	  //       =====================================================
	  //    q locates the front of the pulse relative to [0,T]
	  //    the detector is at t=0, that is an even number of 
	  //    periods away




	  //-----------------------
	  // Hattersley Code
	  //-----------------------
	  double Delta = pw;
	  double del   = drr * t[j];
	  double FullPulse = Delta + 2*del;
	  double q_hat = fmod(t[j] + 0.5*FullPulse, tt[n]);
	  double x = fabs( q_hat - 0.5*FullPulse );

	  double betaval = 0.0;
	  //------------------------------
	  //             Delta
	  //         /------------/
	  //        /      x     /           
	  //       /------------/|
	  //
	  //------------------------------
	  if ( del < Delta/2 ) {
	    if ( x > del + Delta/2 ) { betaval = 0.0; } // x is beyond the 1st half of the pulse
	    if ( x < del + Delta/2 &&
		 x > Delta/2 ) {       betaval = 1.0/2 - (x/del) + (Delta/(2*del)) + 1.0/(2*del*del)*(x-Delta/2)*(x-Delta/2); }
	    if ( x > Delta/2 - del &&
		 x < Delta/2 ) {       betaval = 1.0/2 - (x/del) + (Delta/(2*del)) - 1.0/(2*del*del)*(x-Delta/2)*(x-Delta/2); }
	    if ( x < Delta/2 - del &&
		 x > 0 ) {             betaval = 1.0; }
	  }
	  if ( del > Delta/2 && del < Delta ) {
	    if ( x > del + Delta/2 ) { betaval = 0.0; } // x is beyond the 1st half of the pulse
	    if ( x < del + Delta/2 &&
		 x > Delta/2 ) {       betaval = 1.0/2 - (x/del) + (Delta/(2*del)) + 1.0/(2*del*del)*(x-Delta/2)*(x-Delta/2); }
	    if ( x > del - Delta/2 &&
		 x < Delta/2 ) {       betaval = 1.0/2 - (x/del) + (Delta/(2*del)) - 1.0/(2*del*del)*(x-Delta/2)*(x-Delta/2); }
	    if ( x < del - Delta/2 &&
		 x > 0 ) {             betaval = Delta/del - (x/del)*(x/del) - (Delta/(2*del))*(Delta/(2*del)); }
	  }
	  if ( del > Delta ) {
	    if ( x > del + Delta/2 ) { betaval = 0.0; } // x is beyond the 1st half of the pulse
	    if ( x > del - Delta/2 &&
		 x < del + Delta/2 ) { betaval = 1.0/2 - (x/del) + (Delta/(2*del)) + 1.0/(2*del*del)*(x-Delta/2)*(x-Delta/2); }
	    if ( x > Delta/2 &&
		 x > del - Delta/2 ) { betaval = Delta/del - (x/del)*(Delta/del); }
	    if ( x < Delta/2 &&
		 x > 0 ) {             betaval = Delta/del - (x/del)*(x/del) - (Delta/(2*del))*(Delta/(2*del)); }
	  }

	
	  bool do_carey = true;
	  if ( do_carey ) {
	    //Does the pulse straddle the detector at time j?
	    //      if(q-ca<0.0){
	    //x marks the end of the pulse relative to the middle
	    //Now look at what happens to parts of the pulse
	    for(int k = 0;k<nt0bins;k++) {


	      double pwg = pw/(float)nt0bins;
	      //double hpwg = hpw/(float)nt0bins;
	      del = drr*t[j];//stretching is the same for all parts of the pulse
	      ca = del+ pwg; //full width  // Shouldn't this be 2*del + pwg ??
	      cb = ca/2.0; //half width
	      //cc = del - hpwg;
	      //double cd = ca - 2*del;
	      double tj = t[j] - (k - (float)nt0bins/2.0)*tbin + cb;
	      double q = fmod(tj,tt[n]);
	      int nturns = (int)(tj/tt[n]);
	      if(q<0.0) q = q+tt[n]; //center it
	      double x = fabs(q-ca);
	      double cu= pwg/del;
	      double cv = x/del;

	      bool debug = false;
	      if ( n == jim/2 && j % 1 == 0 && irep == 1 ) { debug = true; }

	      if ( debug ) {
		if ( t[j] < 1000 && 0 ) {
		  cout.precision(4);
		  //cout << "ndel25=" << ndel25 << "\tj=" << j << "\tk=" << k << "/" << nt0bins << "\tPW=" << pw << "\tPWg=" << pwg << "\tdel=" << del << "\tCa=" << ca << "\ttj=" << tj << "\ttj-tFE=" << tj - t[j] << "\tq=" << q << "\tq-Ca=" << q-ca << "\tx=" << x << endl;
		  g_del25->SetPoint(ndel25, t[j], del);
		  g_Ca25->SetPoint(ndel25, t[j], ca);
		  g_Cb25->SetPoint(ndel25, t[j], cb);
		  g_tj25->SetPoint(ndel25, t[j], tj-t[j]);
		  g_Q25->SetPoint(ndel25, t[j], q);
		  g_X25->SetPoint(ndel25, t[j], x);
		  g_NtjQ25->SetPoint(ndel25, nturns, t[j], q);
		  g_Ntj25->SetPoint(ndel25, nturns, t[j]);
		  g_XQ25->SetPoint(ndel25, t[j], q-x);
		  g_QCa25->SetPoint(ndel25, t[j], q-ca);
		  g_DelPWg25->SetPoint(ndel25, t[j], del-pwg);
		  if ( q-ca < 0 ) { 
		    g_HitDet->SetPoint(ndel25, t[j], 1);
		  }
		  else {
		    g_HitDet->SetPoint(ndel25, t[j], 0);
		  }
		  ndel25++;
		}
	      }
	      debug = false;

	      //There are 2 shape cases. 
	      // a) Early in the evolution, the skew
	      //    is less than the pulse element width
	      // b) Late in the evolution, the skew is greater than
	      // 
	      //There are 4 cases 
	      // 1) The whole pulse is beyond detector
	      // 2) Tail end of pulse overlaps detector
	      // 3) Center of pulse overlaps detector
	      // 4) Leading edge of pulse overlaps detector
	      betagk[k] = 0.0;
	      if(q-ca>=0.0) {
		betagk[k] = 0.0;
		idec = 1;
		if ( debug ) { cout << " :: q-Ca > 0: Betag[k] = " << betag[k] << endl; }
		//            if(n==20) printf(" 1 betag = %f \n",betagkn]);        
	      }

	      else if(del<pwg) {
		if(q-ca>-del) {
		  betagk[k] = cv;
		  idec = 2;
		  if ( debug ) { cout << "del < PWg :: q-Ca > -del: Betag[k] = " << betag[k] << endl; }
		}
		else if(q-ca>-pwg) {
		  betagk[k] = 1.0;
		  idec = 3;
		  if ( debug ) { cout << "del < PWg :: q-Ca > -PWg: Betag[k] = " << betag[k] << endl; }
		}	  
		else if(q-ca>-(del+pwg)) {
		  betagk[k] = q/del;
		  idec = 4;
		  if ( debug ) { cout << "del < PWg :: q-Ca > -PWg-del: Betag[k] = " << betag[k] << endl; }
		}
	     
#if 0
		if(n==10 && k ==5) {
		  printf(" %d tj = %f elem %d betagk = %f \n",idec,t[j],k,betagk[k]); 
		  printf(" q = %f ca = %f cb = %f del %f cd %f \n",q,ca,cb,del,cd);
		  printf(" Intersection = %f \n",ca-q);
		  printf(" start %f b1 %f b2 %f \n",q-ca,q-ca+del,q-ca+pwg);
		}
#endif
	      }
	      else {
		if(q-ca>-pwg) {
		  betagk[k] = cv;
		  idec = 5;
		  if ( debug ) { cout << "del > PWg :: q-Ca > -PWg: Betag[k] = " << betag[k] << endl; }
		}
		else if(q-ca>-del) {
		  betagk[k] = cu;
		  idec = 6;
		  if ( debug ) { cout << "del > PWg :: q-Ca > -del: Betag[k] = " << betag[k] << endl; }
		} 
		else if(q-ca>-(del+pwg)) {
		  betagk[k] = q/del;
		  idec = 7;
		  if ( debug ) { cout << "del > PWg :: q-Ca > -PWg-del: Betag[k] = " << betag[k] << endl; }
		}
#if 0
		if(n==20 && k ==5) {
		  printf(" %d tj = %f elem %d betag = %f \n",idec,t[j],k,betagk[k]); 
		  printf(" q = %f ca = %f cb = %f del %f cd %f \n",q,ca,cb,del,cd);
		  printf(" Intersection = %f \n",ca-q);
		  printf(" start %f b1 %f b2 %f \n",q-ca,q-ca+pwg,q-ca+del);
		}
#endif
	      }

	      //           if(n==20)  hspread->Fill((float)j,betagk[k]*t0weight[k]);
	      //           if(n==1)  hspread10->Fill((float)j,betagk[k]*t0weight[k]);
	      //           if(n==39)  hspread30->Fill((float)j,betagk[k]*t0weight[k]);

	      // 	  if((k==20 || k==40 || k==60) && n==20) {
	      // 	    for(int itu=0;itu<15;itu++) {
	      // 	      if(nturns == (itu+1)*10) {
	      // 		double qfill;
	      // 		qfill = q;
	      // 		if(q>0.1) qfill=q-tt[n];
	      // 		hpulse[itu]->Fill(qfill,betagk[k]);
	      // 	      }
	      // 	    }
	      // 	  }


	      if ( n == jim/2 && irep == 1 ) {
		//cout << idec << endl;
		g_Region25->SetPoint(nreg25, t[j], idec);
		g_CaIdec25->SetPoint(nreg25, ca, idec);
		g_Betagk25->SetPoint(nreg25, t[j], betagk[k]);
		nreg25++;
	      }

	      if(betagk[k]<-0.0001) {
		printf("idec = %d betagk %f \n",idec,betagk[k]);
		exit(1);
	      }
	    } // loop over t0 bins (k), only time (j) and radial bins (n) remaining
	
	    //      } //end if over whether this radial bin registers
	    betag[n] = 0.0;
	    //      if(j==951) printf("n value %d nt0bins %d j %d\n",n,nt0bins,j);      



	    //-------------------------------------------------
	    // Beta_{g} = Sum_{k} [ Beta^{g}_{k} x t^{0}_{k} ]
	    //
	    // Sum over t0 bins to get Beta for each radial bin
	    //-------------------------------------------------
	    for(int k=0;k<nt0bins;k++) {
	      betag[n] += betagk[k]*t0weight[k];
            
	      if(betagk[k]>1000.0 || betagk[k]<-.0001) {
		printf("time bin %d pulse bin %d bad beta factor %f weight %f \n",j,k,betagk[k],t0weight[k]);
		exit(1); 
	      }
	    }
	  }

	  if ( betag[n] > 0 || betaval > 0 ) {
	    ;//cout << betag[n] << "\t" << betaval << endl;
	  }
	  //betag[n] = betaval;



	  //-------------------------------------------------------------
	  // Create matrix with beta values for all time/radial bin pairs
	  //-------------------------------------------------------------
#if 1
	  for(int k=0;k<jim;k++) {
	    if(j<NUM) {
	      um[j][k] = betag[k];
	    }
	    //if(k==NRAD/2 && j<NUM) hum->SetBinContent(j,um[j][k]);
	  }
#endif
	} //loop over radial bins (n), only time bins remaining (j)



	//put beta factor contribution into sums
	double gamma;
	double zerr = qc[j];
	sig = qc[j];
	//       if ( irep > 1 ) { 
	// 	zerr = 0.0;
	// 	for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	// 	  Zj += sol[nprime] * um[j][nprime]);
	// 	}
	if(zerr<=0.0) zerr = 1.0;
	//    zerr = 1.0;
	for(int n=0;n<jim;n++) {
	  gamma = betag[n]/zerr; //This is U_jn/sigma_j^2
	  b[n] += qc[j]*gamma;  //This increments (U_jn/sigma_j^2)*Sj

	  //if ( j > 10956 && j < 10958 && qc[j] > 0 ) { cout << j << "\t" << n << "\t" << qc[j] << "\t" << betag[n] << "\t" << gamma << "\t" << b[n] << endl; }

	  for (int i=0;i<jim;i++) {
	    a[n][i]+= gamma*betag[i];//This increments U^T_kj U_ji 
	  }
	}
      } //loop over time bins (j)





    

      //---------------------
      //Solve radial equation
      //---------------------
      double s[MAXRAD];
      for(int n = 0;n<jim;n++) {
	s[n] = b[n];
	for(int i=0;i<n;i++) {
	  if(fabs(a[i][n]-a[n][i])>.001) {
	    printf("a not symmetric %f %f \n",a[i][n], a[n][i]);
	  }
	}
      }



      //
      //
      // Make a guess for the lagrange multipler
      // lambda = -2(AA^{T})^{-1} x y
      //
      //
//       if ( 0 ) {
// 	TMatrixD amn(ndatanp, NRAD);
// 	for(int j = first_time_bin;j<=last_time_bin;j++) {
// 	  if ( j + 1 > NUM ) { break; }
// 	  for ( int n = 0; n < NRAD; n++ ) {
// 	    amn[j][n] = a[n][i];
// 	  }
// 	}
// 	TMatrixD Amn_T(TMatrixD::kTransposed,Amn);
// 	TMatrixD Ann = Amn_T * Amn;
// 	Ann.Invert();
// 	TVector Xn(NRAD);
// 	for ( int n = 0; n < NRAD; n++ ) {
// 	  Xn[n] = b[n];
// 	}
// 	Xn *= -2*Ann_passone; // Guess for lambda
//       }
    
    

      // Ax = b
      // --> x = (A^{-1}) x b
      //Solve equation by matrix inversion - OK not the best method
      const int nx = jim;
      const int ny = jim;
      TMatrixD aloc(nx,ny);
      TMatrixD beta(NUM, NRAD);
      TMatrixD axy(NRAD, NRAD);    
      TMatrixD bx(NRAD,1);
      TVector rhB_passone(NRAD);
      for(int n=0;n<jim;n++) {
	for(int i=0;i<jim;i++) {
	  aloc[n][i] = a[n][i]+sdw[n][i];
	  axy[n][i] = a[n][i];
	}
	rhB_passone(n) = b[n];
      }
      aloc.Invert();


      if ( irep == MAXREP && 0 ) {

	//----------------------------------------------------
	// Fill A(j,n) matrix with A(j,n) = Beta(j,n)/sqrt(Zj)
	//----------------------------------------------------
	TMatrixD Ajn_passone(NUM, NRAD);
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  if ( j + 1 > NUM ) { break; }
	  double Zj = qc[j];
	  if ( Zj < 1 ) { Zj = 1.0; }
	  for ( int n = 0; n < NRAD; n++ ) {
	    Ajn_passone[j][n] = um[j][n] / sqrt(Zj);
	  }
	}

	//------------------------------------------------------
	// Fill B(n) vector with B(n) = Sum_{j} [ Beta(j,n)/Zj ]
	//------------------------------------------------------
	TVector Bn_passone(NRAD);
	for ( int n = 0; n < NRAD; n++ ) {
	  double Bn_sum = 0.0;
	  for(int j = first_time_bin;j<=last_time_bin;j++) {
	    if ( j + 1 > NUM ) { break; }
	    double Zj = qc[j];
	    if ( Zj <= 0.0 ) { Zj = 1.0; }
	    Bn_sum += qc[j] * um[j][n] / Zj;
	    //Bn_sum += qc[j] * betag[n] / Zj;
	    double gamma = um[j][n] / Zj;
	    //if ( j > 10956 && j < 10958 && qc[j] > 0 ) { cout << "M: " << j << "\t" << n << "\t" << qc[j] << "\t" << um[j][n] << "\t" << gamma << "\t" << Bn_sum << endl; }
	  }
	  Bn_passone(n) = Bn_sum;
	}

	//----------------------------------------------------
	// Create A^{T}(j,n)
	//----------------------------------------------------
	TMatrixD Ajn_T_passone(TMatrixD::kTransposed,Ajn_passone);
	TMatrixD Ann_passone = Ajn_T_passone * Ajn_passone;

	//----------------------------------------------------
	// Add Lagrange Multiplier Matrix to A
	//----------------------------------------------------
	Ann_passone += lm;

	//----------------------------------------------------
	// Invert Ann ==> (A^{T}A)^{-1}
	//----------------------------------------------------
	TMatrixD Ann_passone_keep(Ann_passone);
	Ann_passone.Invert();


	//----------------------------------------------------
	// Xn = Ann^{-1} x Bn
	//----------------------------------------------------
	TVector Xn_passone(NRAD);
	Xn_passone = Bn_passone;
	Xn_passone *= Ann_passone;

      
	if ( 0 ) {
	  cout << "B(n) Pass 1" << endl;
	  cout << "[  ";
	  for ( int n = 0; n < NRAD; n++ ) { cout << b[n] << "  "; }
	  cout << "]" << endl;
      
	  Bn_passone.Print();

	  cout << "(A(n,n))^{-1} Pass 1" << endl;
	  for ( int n = 0; n < NRAD; n++ ) {
	    cout << "[ ";
	    for ( int m = 0; m < NRAD; m++ ) { cout << axy[n][m] << " "; }
	    cout << "]" << endl;
	  }
	  Ann_passone_keep.Print();
	}
	cout << "X(n) Pass 1" << endl;
	Xn_passone.Print();

      
	for ( int n = 0; n < NRAD; n++ ) { gr_eqrad_passone->SetPoint(n, rhat[n], Xn_passone(n)); }
      

	//-------------------------
	// Compute Chi2
	//-------------------------
	double chi2 = 0.0;
	int Ndof = 0;
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  Ndof++;
	  double exp = 0.0;
	  for ( int n = 0; n < NRAD; n++ ) {
	    exp += (Xn_passone(n) * um[j][n]);
	  }

	  double obs = qc[j];
	  double Zj = qc[j];
	  if ( Zj < 1 ) { Zj = 1.0; }

	  chi2 += (exp - obs)*(exp - obs)/Zj;
	}


	cout << "1st Pass Results" << endl;
	cout << "  Chi2 = " << chi2 << endl;
	cout << "  Ndof = " << Ndof - NRAD << endl;
	cout << "  Chi2/Ndof = " << chi2/Ndof << endl;      
	out << chi2/Ndof << endl;
	cout << endl;










	//-----------------------------
	// 2nd pass using first results
	//-----------------------------

	//----------------------------------------------------
	// Fill A(j,n) matrix with A(j,n) = Beta(j,n)/sqrt(Zj)
	//----------------------------------------------------
	TMatrixD Ajn_passtwo(NUM, NRAD);
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  if ( j + 1 > NUM ) { break; }
	  double Zj = 0.0;
	  for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	    Zj += (Xn_passone(nprime) * um[j][nprime]);
	  }
	  if ( Zj <= 0 ) { Zj = 1.0; } //cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	  if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	  for ( int n = 0; n < NRAD; n++ ) {
	    Ajn_passtwo[j][n] = um[j][n] / sqrt(Zj);
	  }
	}

	//------------------------------------------------------
	// Fill B(n) vector with B(n) = Sum_{j} [ Beta(j,n)/Zj ]
	//------------------------------------------------------
	TVector Bn_passtwo(NRAD);
	for ( int n = 0; n < NRAD; n++ ) {
	  double Bn_sum = 0.0;
	  for(int j = first_time_bin;j<=last_time_bin;j++) {
	    if ( j + 1 > NUM ) { break; }
	    double Zj = 0.0;
	    for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	      Zj += (Xn_passone(nprime) * um[j][nprime]);
	    }
	    //if ( Zj < 0 ) { cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	    if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	    Bn_sum += qc[j] * um[j][n] / Zj;
	  }
	  Bn_passtwo(n) = Bn_sum;
	}

	//----------------------------------------------------
	// Create A^{T}(j,n)
	//----------------------------------------------------
	TMatrixD Ajn_T_passtwo(TMatrixD::kTransposed,Ajn_passtwo);
	TMatrixD Ann_passtwo = Ajn_T_passtwo * Ajn_passtwo;


	//----------------------------------------------------
	// Add Lagrange Multiplier Matrix to A
	//----------------------------------------------------
	Ann_passtwo += lm;


	//----------------------------------------------------
	// Invert Ann ==> (A^{T}A)^{-1}
	//----------------------------------------------------
	Ann_passtwo.Invert();


	//----------------------------------------------------
	// Xn = Ann^{-1} x Bn
	//----------------------------------------------------
	TVector Xn_passtwo(NRAD);
	Xn_passtwo = Bn_passtwo;
	Xn_passtwo *= Ann_passtwo;

	if ( 0 ) {
	  cout << "B(n) Pass 2" << endl;
	  Bn_passtwo.Print();
	  cout << "(A(n,n))^{-1} Pass 2" << endl;
	  Ann_passtwo.Print();
	}
	cout << "X(n) Pass 2" << endl;
	Xn_passtwo.Print();
      
	for ( int n = 0; n < NRAD; n++ ) { gr_eqrad_passtwo->SetPoint(n, rhat[n], Xn_passtwo(n)); }


	//-------------------------
	// Compute Chi2
	//-------------------------
	chi2 = 0.0;
	Ndof = 0;
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  Ndof++;
	  double exp = 0.0;
	  for ( int n = 0; n < NRAD; n++ ) {
	    exp += (Xn_passtwo(n) * um[j][n]);
	  }

	  double obs = qc[j];
	  double Zj = 0.0;
	  for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	    Zj += (Xn_passtwo(nprime) * um[j][nprime]);
	  }
	  if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	  //if ( Zj < 0 ) { cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	
	  chi2 += (exp - obs)*(exp - obs)/Zj;
	}


	cout << "2nd Pass Results" << endl;
	cout << "  Chi2 = " << chi2 << endl;
	cout << "  Ndof = " << Ndof - NRAD << endl;
	cout << "  Chi2/Ndof = " << chi2/Ndof << endl;
	out << chi2/Ndof << endl;
	cout << endl;










	//-----------------------------
	// 3rd pass using first results
	//-----------------------------

	//----------------------------------------------------
	// Fill A(j,n) matrix with A(j,n) = Beta(j,n)/sqrt(Zj)
	//----------------------------------------------------
	TMatrixD Ajn_passthree(NUM, NRAD);
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  if ( j + 1 > NUM ) { break; }
	  double Zj = 0.0;
	  for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	    Zj += (Xn_passtwo(nprime) * um[j][nprime]);
	  }
	  if ( Zj <= 0 ) { Zj = 1.0; } //cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	  if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	  for ( int n = 0; n < NRAD; n++ ) {
	    Ajn_passthree[j][n] = um[j][n] / sqrt(Zj);
	  }
	}

	//------------------------------------------------------
	// Fill B(n) vector with B(n) = Sum_{j} [ Beta(j,n)/Zj ]
	//------------------------------------------------------
	TVector Bn_passthree(NRAD);
	for ( int n = 0; n < NRAD; n++ ) {
	  double Bn_sum = 0.0;
	  for(int j = first_time_bin;j<=last_time_bin;j++) {
	    if ( j + 1 > NUM ) { break; }
	    double Zj = 0.0;
	    for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	      Zj += (Xn_passtwo(nprime) * um[j][nprime]);
	    }
	    //if ( Zj < 0 ) { cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	    if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	    Bn_sum += qc[j] * um[j][n] / Zj;
	  }
	  Bn_passthree(n) = Bn_sum;
	}

	//----------------------------------------------------
	// Create A^{T}(j,n)
	//----------------------------------------------------
	TMatrixD Ajn_T_passthree(TMatrixD::kTransposed,Ajn_passthree);
	TMatrixD Ann_passthree = Ajn_T_passthree * Ajn_passthree;


	//----------------------------------------------------
	// Add Lagrange Multiplier Matrix to A
	//----------------------------------------------------
	Ann_passthree += lm;


	//----------------------------------------------------
	// Invert Ann ==> (A^{T}A)^{-1}
	//----------------------------------------------------
	Ann_passthree.Invert();


	//----------------------------------------------------
	// Xn = Ann^{-1} x Bn
	//----------------------------------------------------
	TVector Xn_passthree(NRAD);
	Xn_passthree = Bn_passthree;
	Xn_passthree *= Ann_passthree;

	if ( 0 ) {
	  cout << "B(n) Pass 3" << endl;
	  Bn_passthree.Print();
	  cout << "(A(n,n))^{-1} Pass 3" << endl;
	  Ann_passthree.Print();
	}
	cout << "X(n) Pass 3" << endl;
	Xn_passthree.Print();
      
	for ( int n = 0; n < NRAD; n++ ) { gr_eqrad_passthree->SetPoint(n, rhat[n], Xn_passthree(n)); }


	//-------------------------
	// Compute Chi2
	//-------------------------
	chi2 = 0.0;
	Ndof = 0;
	for(int j = first_time_bin;j<=last_time_bin;j++) {
	  Ndof++;
	  double exp = 0.0;
	  for ( int n = 0; n < NRAD; n++ ) {
	    exp += (Xn_passthree(n) * um[j][n]);
	  }

	  double obs = qc[j];
	  double Zj = 0.0;
	  for ( int nprime = 0; nprime < NRAD; nprime++ ) {
	    Zj += (Xn_passthree(nprime) * um[j][nprime]);
	  }
	  if ( TMath::Abs(Zj) < 1 ) { Zj = 1.0; }
	  //if ( Zj < 0 ) { cout << "Zj is less than zero!!!" << endl; Zj = 1.0; }
	
	  chi2 += (exp - obs)*(exp - obs)/Zj;
	}


	cout << "3rd Pass Results" << endl;
	cout << "  Chi2 = " << chi2 << endl;
	cout << "  Ndof = " << Ndof - NRAD << endl;
	cout << "  Chi2/Ndof = " << chi2/Ndof << endl;
	//out << chi2/Ndof << endl;
	cout << endl;



      


	//hgen->Scale(hfr2->Integral()/hgen->Integral());
	//chi2gen = hgen->Chi2Test(hfr2, "CHI2/NDF");
	//cout << "  Chi2/Ndof (gen) = " << chi2gen << endl;











	//return;

    
	//     //----------------------------------------
	//     // Run again with Zj = Sum(f_{n)*beta(n,j)
	//     //----------------------------------------
	//     TVector rhB_passtwo(NRAD);
	//     for(Int_t j = first_time_bin;j<=last_time_bin;j++) {
	//       if ( j + 1 > NUM ) { break; }
	//       double Zj = 0.0;
	//       for ( Int_t n = 0; n < NRAD; n++ ) { 
	// 	Zj += (X_passone(n) * um[j][n]);
	//       }
	//       if ( TMath::Abs(Zj) < 0.0001 ) { Zj = 1.0; }
	//       for ( int n = 0; n < NRAD; n++ ) {
	// 	beta[j][n] = um[j][n] / Zj;
	//       }
	//     }
    
    
	//     //--------------------------------
	//     // Fix right hand side with new Zj
	//     //--------------------------------
	//     double b_passtwo[NRAD];
	//     for ( Int_t n = 0; n < NRAD; n++ ) {  b_passtwo[n] = 0.0; }

	//     for(Int_t j = first_time_bin;j<=last_time_bin;j++) {
	//       if ( j + 1 > NUM ) { break; }
	//       double Zj = 0.0;
	//       for ( Int_t n = 0; n < NRAD; n++ ) { 
	// 	Zj += (X_passone(n) * um[j][n]);
	//       }
	//       if ( TMath::Abs(Zj) < 0.0001 ) { Zj = 1.0; }
	//       //cout << j << "\t" << Zj << "\t" << qc[j] << endl;
	//       for ( Int_t n = 0; n < NRAD; n++ ) { 
	// 	b_passtwo[n] += qc[j]*um[j][n]/Zj;
	// 	//cout << j << "\t" << n << "\tb=" << b_passtwo[n] << "\tq=" << qc[j] << "\tum=" << um[j][n] << "\tZj=" << Zj << endl;
	//       }
	//     }
	//     for ( Int_t n = 0; n < NRAD; n++ ) {  rhB_passtwo(n) = b_passtwo[n]; }

	//     TMatrixD betaT_passtwo(TMatrixD::kTransposed,beta);
	//     TMatrixD Amn_passtwo = betaT_passtwo*beta; 
	//     Amn_passtwo.Invert();
	//     TVector X_passtwo(rhB_passtwo);
	//     //X = rhB;
	//     X_passtwo *= Amn_passtwo;



	//     cout << "B(n) Pass 2" << endl;
	//     rhB_passtwo.Print();
	//     cout << "(A(n,n))^{-1} Pass 2" << endl;
	//     Amn_passtwo.Print();
	//     cout << "X(n) Pass 2" << endl;
	//     X_passtwo.Print();

	//     //TMatrixD Rn_passtwo = Amn_passtwo * bx;
    
    
	//     double chi2 = 0.0;
	//     int Ndof = 0.0;
	//     for( Int_t j = first_time_bin; j <= last_time_bin; j++) {
	//       if ( j + 1 > NUM ) { break; }
	//       Ndof++;
	//       double g = 0.0;
	//       for ( Int_t n = 0; n < NRAD; n++ ) {
	// 	g += (X_passtwo(n) * beta(j,n));
	//       }
	//       double chi = ( g - qc[j] )*( g - qc[j] );
	//       if ( qc[j] > 0 ) { chi/qc[j]; }
	//       chi2 += chi;
	//     }

	//     cout << "Chi2 = " << chi2 << endl;
	//     cout << "Ndof = " << Ndof - NRAD << endl;
	//     cout << "Chi2/Ndof = " << chi2/Ndof << endl;
      }
    
      double ai[MAXRAD][MAXRAD];
  
      //      if(irep==MAXREP){

      //       for(int im=0;im<NRAD;im++) {
      // // 	double arg = -((double)(im-NRAD/2)+0.5)*((double)(im-NRAD/2)+0.5);
      // // 	momspec[im] = exp(arg/(2.0*radw*radw));
      // 	momspec[im] = 
      // 	printf("Momentum Bin %d spectrum %f \n",im,momspec[im]);
      // 	hmomspec->SetBinContent(im+1, momspec[im]);
      //       }
      //       int np = 0;
      //       for(int j=2;j<NUM;j++) {
      // 	genspec[j] = 0.0;
      // 	for(int k=0;k<NRAD;k++) {  
      // 	  genspec[j] += um[j][k]*momspec[k];
      // 	  //humspec->SetBinContent(j, k+1, um[j][k]);
      // 	  gr_um_final->SetPoint(np++, t[j], r[k], um[j][k]);
      // 	}
      // 	//     if(j<1000) printf("%d genspec %f\n",j,genspec[j]);
      // 	hgen->SetBinContent(j,(float)genspec[j]);
      //       }
      //       hgen->SetLineColor(2);
      //       //    hgen->Draw("same");
      //       // hum->Draw("same");

      //     } // if ( irep == MAXREP )



      for(int n=0;n<jim;n++) {
	for(int i=0;i<jim;i++) {
	  ai[n][i] = 0.0;
	  for(int i2 = 0;i2<jim;i2++) {
	    ai[n][i] = ai[n][i] + a[n][i2]*aloc[i2][i];
	  }
	  // 	if(i<jim-1) {
	  // 	  printf("%4.1f ",ai[n][i]);
	  // 	} 
	  // 	else {
	  // 	  printf("%4.1f\n",ai[n][i]);
	  // 	}
	}
      }

  
  
      for(int n=0;n<jim;n++) {
	sol[n] = 0.0;
	for(int i=0;i<jim;i++) {
	  sol[n] += aloc[n][i]*b[i];
	}
	//printf("Momentum bin %d solution %f \n",n,sol[n]);
      }


      //cout << "Generating pseudodata..." << endl;
      int np = 0;
      int ngennp=0;
      for(int j = first_time_bin;j<=last_time_bin;j++) {
	genspec[j] = 0.0;
	for(int k=0;k<NRAD;k++) {  
	  //genspec[j] += um[j][k]*Xn_passtwo(k);
	  genspec[j] += um[j][k]*sol[k];
	  gr_um_final->SetPoint(np++, t[j], r[k], um[j][k]);
	
	  //if ( j > last_time_bin - 2 ) {
	  //cout << "  Rad[" << k << "] = " << um[j][k] << " , " << Xn_passtwo(k) << "\t" << genspec[j] << endl;
	  //}
	}
	hgen->SetBinContent(j,(float)genspec[j]);
	gr_MC_Signal->SetPoint(ngennp++, t[j], (float)genspec[j]);
	//cout << "Gen[" << j << "] = " << genspec[j] << endl;
      }
    

  
      TH1F *hist1_err, *hist2_err;
      hist1_err = (TH1F*)hgen->Clone("hist1_err");
      hist2_err = (TH1F*)hfr2->Clone("hist2_err");
      //hist1_err->Sumw2();
      //hist2_err->Sumw2();
      double mychi2 = 0.0;
      for ( int i = 1; i <= hist1_err->GetNbinsX(); i++ ) {
	double val1 = hist1_err->GetBinContent(i);
	double val2 = hist2_err->GetBinContent(i);
	double err1 = hist1_err->GetBinError(i);
	double err2 = hist2_err->GetBinError(i);
	double xval = hist1_err->GetBinCenter(i);
      
	double newerr1 = err1;
	if ( val1 > 0 ) {
	  if ( err1 <= 0.0 ) {
	    newerr1 = TMath::Sqrt(val1);
	  }
	}
	else {
	  newerr1 = 1.0;
	}
      
	double newerr2 = err2;
	if ( val2 > 0 ) {
	  if ( err2 <= 0.0 ) {
	    newerr2 = TMath::Sqrt(val2);
	  }
	}
	else {
	  newerr2 = 1.0;
	}

	if ( newerr2 > 0 ) {
	  mychi2 += ((val1-val2)*(val1-val2)/(newerr2*newerr2));
	}
	
	hist1_err->SetBinError(i, newerr1);
	hist2_err->SetBinError(i, newerr2);
      }


      Double_t chi2_val;
      Int_t Ndf_val, IsGood;
      double chi2gen = hist1_err->Chi2TestX(hist2_err, chi2_val, Ndf_val, IsGood, "WW, CHI2/NDF");
      double pval = hist1_err->Chi2Test(hist2_err, "WW");
      pval = chi2_val / Ndf_val;
      double ks = hist1_err->KolmogorovTest(hist2_err);

      mychi2 /= Ndf_val;

      double dist = fabs(mychi2 - 1.0);
      Distvals[Nlambs] = dist;
    
      diffdist = fabs(dist - bestdist);
      if ( bestdist == 0.0 ) { diffdist = 0.0; }
      if ( diffdist < 0.01 ) { bestdist = dist;  bestlambda = lamb; }
      else {
	if ( dist < bestdist ) { bestdist = dist; bestlambda = lamb; }
      }


      cout.precision(3);
      cout << "   -------> p-value  = " << ks   << "  \t Best results w/ lambda = " << bestlambda << endl;
      cout << "   -------> Chi2/Ndf = " << pval << "  \t Best results w/ lambda = " << bestlambda << endl;
      cout << "   -------> My Chi2  = " << mychi2 << "  \t Best results w/ lambda = " << bestlambda << endl;
    
      double ans = 0.0;
      np = 0;
      for(int j=2;j<NUM;j++) {
	//hfr2->SetBinContent(j,qc[j]);
	ans = 0.0;
	for(int k=0;k<NRAD;k++) {
	  ans += um[j][k]*sol[k];
	  if ( irep == 1 ) { gr_um_init->SetPoint(np++, t[j], r[k], um[j][k]); }
	}
	hcheck->SetBinContent(j,ans);
      }
      sumNc = hfr2->Integral();

    } // loop over times minimizing Ax = b

    Nlambs++;
    if ( stop ) { break; }

  } // loop over lambda checks

				 
  //hcheck->Draw("same");
  TFile *fout = new TFile(outputfile.str().c_str(),"RECREATE");
  
  //for(int idet=0;idet<15;idet++) {
  //hpulse[idet]->Write();
  //}

  cout << "Writing graphs" << endl;
  TGraph *geqrad = new TGraph();
  for(int n=0;n<NRAD;n++) {
    geqrad->SetPoint(n, rhat[n], sol[n]);
    //heqrad->SetBinContent(n+1,sol[n]);
  }

  //double sumGen = hgen->Integral();
  //hgen->Scale(sumNc/sumGen);

  gr_inv->Write("gPeriodRadius");
  gr->Write("gRadiusPeriod");
  gr_t->Write("gTime");

  gr_t0->GetXaxis()->SetTitle("t_{0} [ns]");
  gr_t0->GetYaxis()->SetTitle("Counts / ns");
  gr_t0->Write("gt0");
  gr_t0weight->GetYaxis()->SetTitle("t_{0} Weight [ns^{-1}]");
  gr_t0weight->GetXaxis()->SetTitle("t_{0} [ns]");
  gr_t0weight->Write("gt0weight");

  cout << "Writing solutions." << endl;
  gr_eqrad_passone->Write("gEqRad1");
  gr_eqrad_passtwo->Write("gEqRad2");
  gr_eqrad_passthree->Write("gEqRad3");

  if ( 0 ) {
    g_del25->Write("gDel25");
    g_Ca25->Write("gCa25");
    g_Cb25->Write("gCb25");
    g_Q25->Write("gQ25");
    g_X25->Write("gX25");
    g_tj25->Write("gtj25");
    g_NtjQ25->Write("gNtjQ25");
    g_Ntj25->Write("gNtj25");
    g_XQ25->Write("gXQ25");
    g_QCa25->Write("gQCa25");
    g_DelPWg25->Write("gDelPWg25");
    g_Region25->Write("gRegion25");
    g_CaIdec25->Write("gCaIdec25");
    g_Betagk25->Write("gBetagk25");
    g_HitDet->Write("gHitDet");
  }

  if ( hEqRad ) { hEqRad->Write("hEqRad"); }
  if ( hXe ) {   hXe->Write("hXe"); }


  hspread->Write();
  hspread10->Write();
  hspread30->Write();
  hcheck->Write();
  hgen->Write();
  hmomspec->Write();
  hum->Write();
  //humspec->Write();
  //gr_um_init->Write("Ujn_Init");
  //gr_um_final->Write("Ujn_Final");
  hfr2->Write();
  geqrad->Write("geqrad");

  if ( gr_Data_Signal ) { 
    TGraph *gr_Data_Signal_Norm = new TGraph();
    int n = gr_Data_Signal->GetN();
    Double_t *x = gr_Data_Signal->GetX();
    Double_t *y = gr_Data_Signal->GetY();
    int grint = gr_Data_Signal->Integral();
    for ( int i = 0; i < n; i++ ) {
      gr_Data_Signal_Norm->SetPoint(i, x[i], y[i]/grint);
    }
    gr_Data_Signal_Norm->Write("g_Data_Signal_Norm");
    cout << "Wrote " << "g_Data_Signal_Norm" << " w/ N=" << n << endl;
  }

  if ( gr_MC_Signal ) { 
    TGraph *gr_MC_Signal_Norm = new TGraph();
    int n = gr_MC_Signal->GetN();
    Double_t *x = gr_MC_Signal->GetX();
    Double_t *y = gr_MC_Signal->GetY();
    int grint = gr_MC_Signal->Integral();
    for ( int i = 0; i < n; i++ ) {
      //cout << i << "\t" << x[i] << "\t" << y[i] << "\t" << y[i]/grint << endl;
      gr_MC_Signal_Norm->SetPoint(i, x[i], y[i]/grint);
    }
    gr_MC_Signal_Norm->Write("g_MC_Signal_Norm");
    cout << "Wrote " << "g_MC_Signal_Norm" << " w/ N=" << n << endl;
  }


  if ( hEqRad ) {
    TGraph *gr_eqrad_GEANT = new TGraph();
    for ( int i = 1; i <= hEqRad->GetNbinsX(); i++ ) {
      gr_eqrad_GEANT->SetPoint(i-1, hEqRad->GetBinCenter(i), hEqRad->GetBinContent(i) / hEqRad->Integral());
    }
    gr_eqrad_GEANT->Write("g_EqRad_GEANT");
  }

  if ( hXe ) {
    TGraph *gr_eqrad_GEANT = new TGraph();
    for ( int i = 1; i <= hXe->GetNbinsX(); i++ ) {
      gr_eqrad_GEANT->SetPoint(i-1, hXe->GetBinCenter(i), hXe->GetBinContent(i) / hXe->Integral());
    }
    gr_eqrad_GEANT->Write("g_EqRad_THEORY");
  }
  
  TGraph *gr_eqrad_FastRotation = new TGraph();
  double binwidth = (rhat[1]-rhat[0]);
  gr_eqrad_FastRotation->SetPoint(0, rhat[0] - binwidth/2, 0.0);
  double eqradint = geqrad->Integral();
  for(int n=0;n<NRAD;n++) {
    gr_eqrad_FastRotation->SetPoint(n+1, rhat[n], sol[n]/eqradint);
    //cout << n+1 << "\t" << n << endl;
  }
  gr_eqrad_FastRotation->SetPoint(NRAD+1, rhat[NRAD-1] + binwidth/2, 0.0);
  gr_eqrad_FastRotation->Write("g_EqRad_FastRotation");

  if ( hEqRad ) {
    TH1F *hEqRadConv = (TH1F*)hEqRad->Clone("hEqRadConv");
    for ( int i = 1; i < hEqRadConv->GetNbinsX(); i++ ) {
      hEqRadConv->SetBinContent(i, 0.0);
    }
    for(int n=0;n<NRAD;n++) {
      hEqRadConv->Fill(rhat[n], sol[n]);
    }
    hEqRadConv->Write();
  }
  if ( 1 ) {
    hdiff = (TH1D*)hfr2->Clone("hData_MC");
    hdiff->Add(hgen, -1);
    hdiff->Write();
  }
  fout->Close();

  cout << "Writing " << outputfile.str() << endl;
  cout << "./plotfast.sh " << outputfile.str() << endl;
  out.close();
}

void fasrof()
{
  ifstream in;
  in.open("geninput.dat");
  //   double dP;
  //   in >> dP;
  //   string ptype_ss;
  //   in >> ptype_ss;
  //   double tbinh_init;
  //   in >> tbinh_init;
  //   string count;
  //   in >> count;
  double pw;
  in >> pw;
  int t0binsize;
  in >> t0binsize;
  int numrad;
  in >> numrad;
  in >> lambscale;
  string file;
  in >> file;
  in >> hist;
  in.close();


  if ( numrad > 0 && numrad <= MAXRAD ) { NRAD = numrad; }
  else { NRAD = MAXRAD; }

  cout << "------------ fasrof() ------------" << endl;
  cout << "PW   = " << pw << endl;
  cout << "t0   = " << t0binsize << endl;
  cout << "NRAD = " << NRAD << endl;
  cout << "FILE = " << file << endl;
  cout << "HIST = " << hist << endl;
  cout << "------------ fasrof() ------------" << endl;
  cout << endl;

  stringstream outfilename;

  if ( file.size() > 0 ) {
    int pos = file.find(".root");    
    string outname = file.substr(0, pos);
    outfilename << outname;
    cout << "Calling fasrof(): " << outfilename.str() << endl;
    fasrof_run(pw/1000, t0binsize, outfilename.str());
  }
}
