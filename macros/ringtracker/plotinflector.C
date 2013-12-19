void style() {

  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameBorderMode(0);

  gStyle->SetTitleColor(1);   // 0
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleBorderSize(1);

  /*
    gStyle->SetTitleX(0.10);
    gStyle->SetTitleY(0.94);
    gStyle->SetTitleW(0.5);
    gStyle->SetTitleH(0.06);

    gStyle->SetLabelOffset(1e-04);
    gStyle->SetLabelSize(0.2);
    gStyle->SetTitleSize(0.2);
  */
  gStyle->SetStatColor(0);
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatX(0.90);
  gStyle->SetStatY(0.90);
  gStyle->SetStatW(0.30);
  gStyle->SetStatH(0.10);

  gStyle->SetErrorX(0.0);   // Horizontal error bar size
  gStyle->SetOptStat(0);
  //   gStyle->SetPaperSize(10.,12.);   // Printout size
}

double mintime_for_plot = -1.0;
double maxtime_for_plot = -1.0;

double eff_inf, err_inf;
double eff_turn1, err_turn1;
double eff_turn10, err_turn10;
double eff_turn100, err_turn100;
double eff_turn, err_turn;

double dPoverP = 0.01;
bool isE821 = false;

int Nstart_inflector = 0;
int Nexit_inflector = 0;
int Nstart_ring = 0;
int Nstored1_ring = 0;
int Nstored10_ring = 0;
int Nstored100_ring = 0;
int Ngen = 0;

bool plotinf = true;
bool plotringhits = false;
bool start_inflector = false;
int maxturns_real = 2000;
double maxtime;
bool plotringeff = false;

bool saveC = false;

bool makeeps = false;
string dir = "";
string basedir = "";
string rootdir = "";
double upstream_inflector_yield = -1.0;
bool zoom;
bool zoom2;
bool zoom3;
int update = 0;
string update_plotname1 = "";
string update_plotname2 = "";
string update_plotname3 = "";
string base;
int maxturnsreal = 2000;
stringstream ss_eff_turn;
stringstream ss_eff_inf;
double eff_turn_val[3];
double err_turn_val[3];
double eff_inf_val;
double err_inf_val;
int number_start_in_inflector = 1;
int number_start_in_ring = 1;
TH2D *HitsXZ[21];
TH2D *HitsRhatY[21];
TF1 *fBetaX = NULL;
TF1 *fBetaY = NULL;
double m = 1000.0;
double cm = m / 100;
int kick;
int beamsize;
bool is_muongas = false;

enum {kGeneratedDist, kRemainingDist};
enum {kBegin, kFinal};

enum {kEarly, kMiddle, kLate, kAll};
enum {k1tracker, k2trackers};

enum {kX, kY};
enum {k11, k12};

bool only_cbo = false;
bool test_gen = false;
bool test_evolve = true;

bool IsCaloElectron(string particle)
{
  if ( particle == "DecayElectron" ) { return( true ); }
  if ( particle == "DecayElectronQuadSection" ) { return( true ); }
  if ( particle == "DecayElectronOtherSection" ) { return( true ); }
  if ( particle == "DecayElectronKickerSection" ) { return( true ); }
  if ( particle == "DecayElectronEmptySection" ) { return( true ); }
  if ( particle == "DecayElectronEltEth" ) { return( true ); }
  if ( particle == "DecayElectronEgtEth" ) { return( true ); }
  if ( particle == "DecayElectronEgtHghEth" ) { return( true ); }
  if ( particle == "DecayElectronLongDriftTime" ) { return( true ); }
  if ( particle == "DecayElectronShortDriftTime" ) { return( true ); }

  return( false );
}

bool IsStrawElectron(string particle)
{
  if ( particle == "StrawElectron" ) { return( true ); }
  if ( particle == "StrawCaloElectron" ) { return( true ); }
  if ( particle == "GoodStrawElectron" ) { return( true ); }
  if ( particle == "GoodStrawCaloElectron" ) { return( true ); }
  if ( particle == "GoodOneStrawCaloElectron" ) { return( true ); }
  if ( particle == "GoodTwoStrawCaloElectron" ) { return( true ); }

  return( false );
}



Double_t CBO(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  //Float_t freq = par[2];
  //Float_t corr = xx-par[3];
  Double_t f = par[0] + par[1]*sin(TMath::TwoPi()*par[2]*xx + par[3]);
  return f;
}



Double_t CBODecay(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  //Float_t freq = par[2];
  //Float_t corr = xx-par[3];
  Double_t f = par[0] + par[1]*TMath::Exp(-xx/(par[4]*100))*sin(TMath::TwoPi()*par[2]*xx + par[3]);
  return f;
}

Double_t betaFunction(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  //Float_t freq = par[2];
  //Float_t corr = xx-par[3];
  Double_t f = par[0] + par[1]*sin(par[2]*xx + par[3]);
  return f;
}

void PrintXprimeX(TH2D *hist2d, string name)
{  
  TH2D *hist2d_tmp = (TH2D*)hist2d->Clone(name.c_str());
  double sum = hist2d_tmp->Integral();
  cout << "X'X - Integral() = " << sum << endl;

  TH1D *xprof = (TH1D*)hist2d_tmp->ProjectionX();
  double expXbin = xprof->GetBinWidth(1);
  double newXbin = 5.0;
  xprof->Rebin(newXbin/expXbin);
  double sumX = xprof->Integral();
  
  int maxXbin = xprof->GetMaximumBin();
  double runsumX = 0.0;
  double lowX = 0.0;
  double highX = 0.0;
  for ( int bin = 0; bin < xprof->GetNbinsX(); bin++ ) {
    double low = maxXbin - bin;
    double high = maxXbin + bin;
    if ( low < 1 ) { low = 1; }
    if ( high > xprof->GetNbinsX() ) { high = xprof->GetNbinsX(); }
    runsumX = xprof->Integral(low, high);
    //cout << runsumX << "/" << sumX << endl;
    if ( runsumX > 0.95*sumX ) { lowX = xprof->GetBinCenter(low); highX = xprof->GetBinCenter(high); break; }
  }
  
  TH1D *yprof = (TH1D*)hist2d_tmp->ProjectionY();
  double expYbin = yprof->GetBinWidth(1);
  double newYbin = 0.5;
 //yprof->Rebin(newYbin/expYbin);
  double sumY = yprof->Integral();

  int maxYbin = yprof->GetMaximumBin();
  double runsumY = 0.0;
  double lowY = 0.0;
  double highY = 0.0;
  //cout << "bins = " << yprof->GetNbinsX() << endl;
  for ( int bin = 0; bin < yprof->GetNbinsX(); bin++ ) {
    double low = maxYbin - bin;
    double high = maxYbin + bin;
    if ( low < 1 ) { low = 1; }
    if ( high > yprof->GetNbinsX() ) { high = yprof->GetNbinsX(); }
    runsumY = yprof->Integral(low, high);
    //cout << runsumY << "/" << sumY << "\t" <<  yprof->GetBinCenter(low) << "\t" << yprof->GetBinCenter(high) << endl;
    if ( runsumY > 0.95*sumY ) { lowY = yprof->GetBinCenter(low); highY = yprof->GetBinCenter(high); break; }
  }

  cout.precision(3);
  cout << "95% of X area between  " << lowX << " < " << xprof->GetBinCenter(maxXbin) << " < " << highX << endl;
  cout << "95% of X' area between " << lowY << " < " << yprof->GetBinCenter(maxYbin) << " < " << highY << endl;
  cout.precision(3);
  cout << "Area of X'-X is " << (highX-lowX)/2 << " x " << (highY-lowY)/2 << " x PI = " << (highX-lowX)*(highY-lowY)/4 << "xPI" << endl;
}

void PrintRhatT0(TH2D *hist2d, string suffix)
{
  stringstream cboname;
  cboname << "CBO_" << suffix;
  
  TH1D *hist1d = hist2d->ProfileX(cboname.str().c_str(), 1, 1000000, "i");

  double fCBO = 0.477; // MHz
  fCBO = 0.466687;

  int firstbin = hist1d->FindBin(1);
  int lastbin = hist1d->FindLastBinAbove(-99.9)-1;

  if ( suffix == "BeamScan" ) { firstbin = hist1d->FindBin(1); }
  else { firstbin = hist1d->FindBin(1); }
  
  if ( kick == 0 ) { 
    firstbin = hist1d->FindBin(50);
    lastbin = hist1d->FindBin(160);
  }

  
  cout << "Fit goes from [" << hist1d->GetBinLowEdge(firstbin) <<" -> " << hist1d->GetBinLowEdge(lastbin) << "]" << endl;

  //TF1 *frhat = new TF1("Frhat",CBO, hist1d->GetBinLowEdge(firstbin), hist1d->GetBinLowEdge(lastbin), 4);
  TF1 *frhat = new TF1("Frhat",CBODecay, hist1d->GetBinLowEdge(firstbin), hist1d->GetBinLowEdge(lastbin), 5);

  bool dofit = false;
  
  double Offset = 0.0;
  double Amp = 0.0;
  double Freq = fCBO;
  double Phase = 0.0;
  double Tau = 3.0;

  Double_t chi2 = 0.0;
  Int_t Ndof    = 1;
  
  ifstream in;
  stringstream inname;
  string dummy;
  string cbodir = "CBO";
  if ( beamsize == 0 ) { cbodir += "_0pi"; }
  if ( isE821 ) { cbodir += "_E821"; }
  if ( kick < 0 ) { cbodir += "_lcr"; }

  if ( kick < 0 ) {
    inname << cbodir << "/CBO_" << -1*kick << "kV.dat";
  }
  else {
    inname << cbodir << "/CBO_" << kick << "G.dat";
  }
  in.open(inname.str().c_str());
  if ( in.is_open() ) {
    in >> chi2 >> dummy >> Ndof;       
    in >> Offset >> Amp >> Freq >> Phase;
    in.close();  
  }
  else {
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
  }

  Freq = fCBO;
  if ( suffix == "BeamScan" ) {
    Offset = 0.0;
    Amp = 6.0;
    Phase = 1.57;
    frhat->SetParameter(0, Offset);
    frhat->SetParameter(1, Amp);
    frhat->SetParameter(2, Freq);
    frhat->SetParameter(3, Phase);
    frhat->SetParameter(4, Tau);
    frhat->SetParLimits(0, Offset-1, Offset+1);
    frhat->SetParLimits(1, Amp/1.5, Amp *1.5);
    frhat->SetParLimits(2, Freq*0.99, Freq*1.01);
    frhat->SetParLimits(3, -3.14, 3.14);
  }
  else {
    frhat->SetParameter(0, Offset);
    frhat->SetParameter(1, Amp);
    frhat->SetParameter(2, Freq);
    frhat->SetParameter(3, Phase);
    frhat->SetParameter(4, Tau);
    frhat->SetParLimits(0, Offset-3, Offset+3);
    frhat->SetParLimits(1, Amp / 2, Amp * 2);
    frhat->SetParLimits(2, Freq*0.99, Freq*1.01);
    frhat->SetParLimits(3, -3.14, 3.14);
;
  }
  frhat->SetParLimits(4, Tau/2, Tau*2);

  if ( dofit ) {
    frhat->SetLineColor(kOrange+6);
    frhat->SetLineWidth(2);
    TFitResultPtr s = hist1d->Fit("Frhat", "RS");
  }


  chi2 = frhat->GetChisquare();
  Ndof    = frhat->GetNDF();

  if ( chi2/Ndof > 3 ) {
    Offset = frhat->GetParameter(0);
    Amp = frhat->GetParameter(1);
    Freq = frhat->GetParameter(2);
    Phase = frhat->GetParameter(3);
    Tau = frhat->GetParameter(4);
    
    frhat->SetParameter(0, Offset);
    frhat->SetParameter(1, Amp);
    frhat->SetParameter(2, Freq);
    frhat->SetParameter(3, Phase);
    frhat->SetParameter(4, Tau);
    frhat->SetParLimits(0, Offset-10, Offset+10);
    frhat->SetParLimits(1, 0, Amp * 3);
    frhat->SetParLimits(2, Freq*0.99, Freq*1.01);
    frhat->SetParLimits(3, -3.14, 3.14);
    frhat->SetParLimits(4, Tau / 2, Tau * 2);
    
    if ( dofit ) {
      frhat->SetLineColor(kOrange+6);
      frhat->SetLineWidth(2);
      TFitResultPtr s = hist1d->Fit("Frhat", "RS");
    }
    
    
    chi2 = frhat->GetChisquare();
    Ndof    = frhat->GetNDF();


    Offset = frhat->GetParameter(0);
    Amp = frhat->GetParameter(1);
    Freq = frhat->GetParameter(2);
    Phase = frhat->GetParameter(3);
    Tau = frhat->GetParameter(4);
    
    frhat->SetParameter(0, Offset);
    frhat->SetParameter(1, Amp);
    frhat->SetParameter(2, Freq);
    frhat->SetParameter(3, Phase);
    frhat->SetParameter(4, Tau);
    frhat->SetParLimits(0, Offset-3, Offset+3);
    frhat->SetParLimits(1, Amp / 1.5, Amp * 1.5);
    frhat->SetParLimits(2, Freq*0.99, Freq*1.01);
    frhat->SetParLimits(3, -3.14, 3.14);
    frhat->SetParLimits(4, Tau / 2, Tau * 2);
    
    if ( dofit ) {
      frhat->SetLineColor(kOrange+6);
      frhat->SetLineWidth(2);
      TFitResultPtr s = hist1d->Fit("Frhat", "RS");
    }
    
    chi2 = frhat->GetChisquare();
    Ndof    = frhat->GetNDF();
  }
  
  hist1d->SetLineColor(kAzure+6);
  hist1d->SetLineWidth(2);
  hist1d->SetMarkerSize(0.5);
  hist1d->SetMarkerColor(kAzure+6);


  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();

  double xmin, xmax, ymin, ymax;
  for ( int n = 0; n < 10; n++ ) {
    if ( n == 0 ) {
      xmin = 0.0;
      xmax = 10.0;
    }
    if ( n == 1 ) {
      xmin = 50.0;
      xmax = 60.0;
    }
    if ( n == 2 ) {
      xmin = 150.0;
      xmax = 160.0;
    }
    if ( n == 3 ) {
      xmin = 250.0;
      xmax = 260.0;
    }
    if ( n == 4 ) {
      xmin = 350.0;
      xmax = 360.0;
    }
    if ( n == 8 ) {
      xmin = 0.0;
      xmax = 150.0;
    }
    if ( n == 9 ) {
      xmin = 0.0;
      xmax = 400.0;
    }
    if ( n == 5 ) {
      xmin = 450.0;
      xmax = 460.0;
    }
    if ( n == 6 ) {
      xmin = 650.0;
      xmax = 660.0;
    }
    if ( n == 7 ) {
      xmin = 850.0;
      xmax = 860.0;
    }

    
    if ( xmax > maxtime ) { xmax = maxtime; }
    if ( xmin > maxtime ) { continue; }
    if ( xmax <= xmin ) { continue; }
	  
    
    int b1 = hist1d->FindBin(xmin);
    int bN = hist1d->FindBin(xmax);
    ymin = 99.9;
    ymax = -9.9;

    for ( int b = b1; b <= bN; b++ ) {
      if ( hist1d->GetBinContent(b) > ymax ) { ymax = hist1d->GetBinContent(b); }
      if ( hist1d->GetBinContent(b) < ymin ) { ymin = hist1d->GetBinContent(b); }
    }

    ymax = TMath::Max(fabs(ymax), fabs(ymin));   
    if ( ymax < 1 ) { ymax = 2; }
    else if ( ymax < 2 ) { ymax = 3; }
    else if ( ymax < 3 ) { ymax = 4; }
    else if ( ymax < 4 ) { ymax = 6; }
    else if ( ymax < 6 ) { ymax = 8; }
    else if ( ymax < 8 ) { ymax = 10; }
    else if ( ymax < 10 ) { ymax = 13; }
    else if ( ymax < 13 ) { ymax = 16; }
    else if ( ymax < 16 ) { ymax = 20; }
    else if ( ymax < 20 ) { ymax = 25; }
    else if ( ymax < 25 ) { ymax = 30; }
    else if ( ymax < 30 ) { ymax = 37; }
    else if ( ymax < 37 ) { ymax = 45; } 
    ymin = ymax * -1;

    TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleSize(0.06);
    h->SetXTitle("time [#mus]");
    h->SetYTitle("x [mm]");
    h->GetYaxis()->SetTitleOffset(1.1);
    h->GetXaxis()->SetTitleOffset(1.1);
    h->Draw();

    if ( dofit ) {
    Offset = frhat->GetParameter(0);
    Amp = frhat->GetParameter(1);
    Freq = frhat->GetParameter(2);
    Phase = frhat->GetParameter(3);
    Tau = frhat->GetParameter(4);
    
    stringstream fitval;
    fitval.precision(3);
    fitval << "#chi^{2}/N_{dof} = " << chi2 << " / " << Ndof;
    myText(0.67, 0.87, 1, fitval.str().c_str(), 0.04);
  
    stringstream fitvals;
    fitvals.precision(2);
    fitvals << "[0]=" << Offset << " , [f]=" << Freq << " , [#phi]=" << Phase;
    myText(0.18, 0.87, 1, fitvals.str().c_str(), 0.04);
  
    stringstream fitvals;
    fitvals.precision(3);
    fitvals << "[A]=" << Amp << " , [#tau]=" << Tau;
    myText(0.60, 0.20, 1, fitvals.str().c_str(), 0.04);
    }
    hist1d->Draw("HIST,SAME");
    stringstream c1name;
    c1name << "~/public_html/plots/" << dir << "/" << "CBO_" << xmin << "_" << xmax << "_" << suffix << ".png";
    c1->SaveAs(c1name.str().c_str());

    bool saveCBO = false;

    if ( saveCBO && dofit ) {
      if ( n == 0 && is_muongas == false ) {
	ofstream out;
	stringstream outname;
	string cbodir = "CBO";
	if ( beamsize == 0 ) { cbodir += "_0pi"; }
	if ( isE821 ) { cbodir += "_E821"; }
	if ( kick < 0 ) { cbodir += "_lcr"; }
      
	if ( kick < 0 ) {
	  outname << cbodir << "/CBO_" << -1*kick << "kV.dat";
	}
	else {
	  outname << cbodir << "/CBO_" << kick << "G.dat";
	}
	out.open(outname.str().c_str());
	out << chi2 << " / " << Ndof << endl;
	out << frhat->GetParameter(0) << "\t" << frhat->GetParameter(1) << "\t" << frhat->GetParameter(2) << "\t" << frhat->GetParameter(3) << endl;
	//out << hist1d->GetBinCenter(bin) << "\t" << hist1d->GetBinWidth(bin) << "\t" << hist1d->GetBinContent(bin) << "\t" << hist1d->GetBinError(bin) << endl;
	out.close();  
      }
    }
  }
}

void PrintXe(TH1D *hist1d)
{
  TH1F *htmp = (TH1F*)hist1d->Clone("xe");
  htmp->Scale(1.0/htmp->Integral());
  ofstream out;
  out.open("xe.dat");
  out.precision(3);
  out << "  double cdf[nBinsXe] = {";
  for ( int i = 1; i <= hist1d->GetNbinsX(); i++ ) {
    out << htmp->Integral(1, i);
    if ( i < hist1d->GetNbinsX() ) { out << ", "; }
  }
  out << "};" << endl;
  out.close();
}

double R_magic()
{
  return( 7.112 * m );
}

void CreateBetaFunctions()
{
  if ( fBetaX == NULL ) {
    // These are for +/-24 kV on the quads
    fBetaX = new TF1("mybetaX",betaFunction,0,TMath::TwoPi()*R_magic()/1000,4);
    fBetaX->SetParameter(0, 7.67067);
    fBetaX->SetParameter(1, 0.247307);
    fBetaX->SetParameter(2, 0.562465);
    fBetaX->SetParameter(3, 1.56706);
  }

  if ( fBetaY == NULL ) {
    fBetaY = new TF1("mybetaY",betaFunction,0,TMath::TwoPi()*R_magic()/1000,4);
    fBetaY->SetParameter(0, 18.8466);
    fBetaY->SetParameter(1, 0.496336);
    fBetaY->SetParameter(2, 0.562772);
    fBetaY->SetParameter(3, -1.58141);
  }
}

double SingleTurnArcLength(double arclength)
{
  double maxangle = TMath::TwoPi() * R_magic();
  double decayarclength = std::fmod(arclength, maxangle);

  return( decayarclength );
}

double PhaseAdvance(int var, double arclength)
{
  double const_beta = Beta(true, var, arclength);
  double phase_advance = arclength/const_beta;

  return( phase_advance );
}


double Beta(bool use_const_beta, int var, double arclength)
{
  CreateBetaFunctions();

  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( use_const_beta ) {
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

  cout << "Beta not defined..." << endl;
  return( 0.0 );
}

double Alpha(bool use_const_beta, int var, double arclength)
{
  CreateBetaFunctions();

  double singleturn_arclength = SingleTurnArcLength(arclength);

  if ( use_const_beta ) {
    if ( var == kX ) {
      return( 0 );
    }
    if ( var == kY ) {
      return( 0 );
    }
  }
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

  cout << "Alpha not defined..." << endl;
  return( 0.0 );
}

void Transport(double *newVar, double *newVarprime, bool use_const_beta, int var, double arclength, double s0)
{
  double oldVar = *newVar;
  double oldVarprime = *newVarprime;

  double beta, alpha;
  double beta0, alpha0;
  double PsiXs;

  beta = Beta(use_const_beta, var, arclength);
  alpha = Alpha(use_const_beta, var, arclength);
  beta0 = Beta(use_const_beta, var, s0);
  alpha0 = Alpha(use_const_beta, var, s0);
  PsiXs = PhaseAdvance(var, arclength);
  
  
  // Compute phase advance
  double M00, M01, M10, M11;
  M00 = TMath::Sqrt(beta/beta0)*TMath::Cos(PsiXs) + alpha0*TMath::Sin(PsiXs);
  M01 = TMath::Sqrt(beta*beta0)*TMath::Sin(PsiXs);
  M10 = ((alpha0 - alpha)*TMath::Cos(PsiXs) - (1+alpha0*alpha)*TMath::Sin(PsiXs))/(TMath::Sqrt(beta*beta0));
  M11 = TMath::Sqrt(beta0/beta)*TMath::Cos(PsiXs) - alpha*TMath::Sin(PsiXs);
  
  bool debug = false;
  if ( debug ) {
    cout << "=== Transport ===" << endl;
    cout << "Betas\t" << beta << "\t" << alpha << "\t" << beta0 << "\t" << alpha0 << endl;
    cout << "Ms\t" << M00 << "\t" << M01 << "\t" << M10 << "\t" << M11 << endl;
  }

  *newVar      = M00*oldVar + M01*oldVarprime;
  *newVarprime = M10*oldVar + M11*oldVarprime;  

  if ( debug ) {
    cout << "Calc: " << endl;
    cout << "  " << oldVar << "\t" << oldVarprime << endl;
    cout << "  " << M00*oldVar << "\t" << M01*oldVarprime << endl;
  }
}

double Transport(int var, int element, double time)
{
  double retval = 0.0;

  double beta, alpha;
  double beta0, alpha0;
  double PsiXs;

  bool use_const_beta = false;
  double s0 = 0.0;
  double arclength = s0 + TMath::TwoPi()*R_magic();  
  double phi = (arclength-s0) / R_magic();
  
  beta = Beta(use_const_beta, var, arclength);
  alpha = Alpha(use_const_beta, var, arclength);
  beta0 = Beta(use_const_beta, var, s0);
  alpha0 = Alpha(use_const_beta, var, s0);
  PsiXs = PhaseAdvance(var, arclength);

  double n0 = 0.314;
  double lq = 4.804*m;
  double lin = 6.368*m;
  double R = R_magic();

  double cosPsi = TMath::Cos(sqrt(n0)*lq/R) - sqrt(n0)*lin/(2*R)*TMath::Sin(sqrt(n0)*lq/R);
  double Psi    = 4*TMath::ACos(cosPsi);
  double beta0min = TMath::Sin(Psi/4)/(sqrt(n0)*TMath::Sin(sqrt(n0)*lq/R));

  cout << Psi << "\t" << phi << "\t" << beta0min << "\t";

  if ( var == kY ) {
    if ( element == k11 ) {
      retval = TMath::Cos(Psi) - (phi/beta0min)*TMath::Sin(Psi);      
      cout << "11\t" << retval << endl;
    }
    if ( element == k12 ) {
      retval = beta0min * (1 + (phi/beta0min)*(phi/beta0min)) * TMath::Sin(Psi);     
      cout << "12\t" << retval << endl;
    }
  }
  
  return( retval );
}

bool PlotVariable(string varname, string timeval)
{
  if ( only_cbo ) { return( false ); }

  if ( timeval == "Turn" ) { return( false ); }
  //if ( timeval == "Time" ) { return( false ); }

  if ( timeval.find("deg") != string::npos ) { return( true ); }
  
  if ( varname == "Rhat" ) { return( true ); }
  if ( varname == "Y" ) { return( true ); }
  if ( varname == "Vhat" ) { return( true ); }
  if ( varname == "Mom" ) { return( true ); }
  if ( varname == "Pol" ) { return( true ); }
  if ( varname == "Num" ) { return( true ); }
  if ( varname == "EqRad" ) { return( true ); }

  if ( varname == "PolXY" ) { return( true ); }
  if ( varname == "Xprime" ) { return( true ); }
  if ( varname == "Yprime" ) { return( true ); }
  if ( varname == "RhatY" ) { return( true ); }
  if ( varname == "XprimeX" ) { return( true ); }
  if ( varname == "YprimeY" ) { return( true ); }

  return( false );
}

bool PlotVariable(string varname, string particle, string timeval)
{
  if ( only_cbo ) {
    if ( varname == "Rhat" && particle == "DecayMuon" && timeval == "TimeOncePerTurn" ) {
      return( true );
    }
    if ( varname == "XprimeX" && particle == "DecayMuon" ) {
      return( true );
    }
    return( false );
  }

  if ( particle == "BirthMuon" ) {
    if ( timeval == "TimeOncePerTurn" ) { return( false ); }

    if ( varname == "RhatY" ) { return( true ); }
    if ( varname == "XprimeX" ) { return( true ); }
    if ( varname == "YprimeY" ) { return( true ); }
    if ( varname == "Xe" ) { return( true ); }
    if ( varname == "Mom" ) { return( true ); }
    if ( varname == "t0" ) { return( true ); }
    if ( varname == "Polt0" ) { return( true ); }
    if ( varname == "PolXY" ) { return( true ); }
    if ( varname == "Pol" ) { return( true ); }
    //if ( varname == "SpinPhase" ) { return( true ); }

    return( false );
  }

  if ( particle == "DecayMuon" ) {

    if ( timeval == "TimeOncePerTurn" || timeval == "TimeSingleHit" ) {
      if ( varname == "NumAllStations" ) { return( true ); }
      if ( varname == "NumAllStationsNoFastRotation" ) { return( true ); }
      if ( varname == "NudAllStations" ) { return( true ); }
      if ( varname == "Mom" ) { return( true ); }
      if ( varname == "Xe" ) { return( true ); }
      if ( varname == "Y" ) { return( true ); }
      if ( varname == "Y2" ) { return( true ); }
      if ( varname == "Pol" ) { return( true ); }
      if ( varname == "Rhat" ) { return( true ); }
      if ( varname == "SpinPhase" ) { return( true ); }
      return( false );
    }
    
    if ( timeval == "GeneratedDist" ) {
      if ( varname == "PolXY" ) { return( true ); }
      if ( varname == "Pol" ) { return( true ); }
      if ( varname == "RhatY" ) { return( true ); }
      if ( varname == "Rhat" ) { return( true ); }
      if ( varname == "Vhat" ) { return( true ); }
      if ( varname == "XprimeX" ) { return( true ); }
      if ( varname == "YprimeY" ) { return( true ); }
      //if ( varname == "XZ" ) { return( true ); }
      if ( varname == "Xe" ) { return( true ); }
      //if ( varname == "Time" ) { return( true ); }
      if ( varname == "SpinPhase" ) { return( true ); }
      return( false );
    }

    return( false );
  }

  if ( particle == "LostMuon" ) {

    if ( timeval == "TimeOncePerTurn" ) {
      if ( varname == "NumAllStations" ) { return( true ); }
      if ( varname == "NudAllStations" ) { return( true ); }
      if ( varname == "Pol" ) { return( true ); }
      if ( varname == "Rhat" ) { return( true ); }
      if ( varname == "SpinPhase" ) { return( true ); }
      return( false );
    }

    if ( varname == "PolXY" ) { return( true ); }
    if ( varname == "Pol" ) { return( true ); }
    if ( varname == "RhatY" ) { return( true ); }
    if ( varname == "R" ) { return( true ); }
    if ( varname == "XprimeX" ) { return( true ); }
    if ( varname == "YprimeY" ) { return( true ); }
    if ( varname == "XZ" ) { return( true ); }
    if ( varname == "Mom" ) { return( true ); }
    if ( varname == "Time" ) { return( true ); }
    if ( varname == "SystemHits" ) { return( true ); }
    if ( varname == "SpinPhase" ) { return( true ); }
  }

  if ( particle == "LostDecayMuon" ) {

    if ( timeval == "TimeOncePerTurn" ) {
      if ( varname == "NumAllStations" ) { return( true ); }
      if ( varname == "NudAllStations" ) { return( true ); }
      if ( varname == "Pol" ) { return( true ); }
      if ( varname == "Rhat" ) { return( true ); }
      if ( varname == "SpinPhase" ) { return( true ); }
      return( false );
    }

    if ( varname == "PolXY" ) { return( true ); }
    if ( varname == "Pol" ) { return( true ); }
    if ( varname == "RhatY" ) { return( true ); }
    if ( varname == "R" ) { return( true ); }
    if ( varname == "XprimeX" ) { return( true ); }
    if ( varname == "YprimeY" ) { return( true ); }
    if ( varname == "XZ" ) { return( true ); }
    if ( varname == "Mom" ) { return( true ); }
    if ( varname == "Time" ) { return( true ); }
    if ( varname == "SystemHits" ) { return( true ); }
    if ( varname == "SpinPhase" ) { return( true ); }
  }

  if ( particle == "StoredMuon" ) {
    if ( timeval == "RemainingDist" ) {
      if ( varname == "PolXY" ) { return( true ); }
      if ( varname == "Pol" ) { return( true ); }
      if ( varname == "RhatY" ) { return( true ); }
      if ( varname == "XprimeX" ) { return( true ); }
      if ( varname == "YprimeY" ) { return( true ); }
      if ( varname == "Mom" ) { return( true ); }
      if ( varname == "Xe" ) { return( true ); }
    }
  }

  if ( particle == "BirthElectron" || particle == "BirthElectronEgtEth" ) {

    if ( varname == "NumAllStations" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumAllStationsNoFastRotation" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NudAllStations" && timeval == "TimeOncePerTurn" ) { return( true ); }

    if ( varname == "RhatY" ) { return( true ); }
    //return( false );
    //if ( varname == "ThetaRhat" ) { return( true ); }
    //     if ( varname == "ThetaR" ) { return( true ); }
    if ( varname == "Theta" ) { return( true ); }
    if ( varname == "Rhat" ) { return( true ); }
    if ( varname == "XprimeX" ) { return( true ); }
    if ( varname == "YprimeY" ) { return( true ); }
    //if ( varname == "XZ" ) { return( true ); }
    if ( varname == "Energy" ) { return( true ); }
    if ( varname == "SpinPhase" ) { return( true ); }
  }

  if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) {
    if ( varname == "NumAllStations" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumAllStationsNoFastRotation" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "Energy" ) { return( true ); }
    if ( varname == "XZ" ) { return( true ); }
    
    if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) {
      if ( timeval == "RemainingDist" ) {
	if ( varname == "RhatY" ) { return( true ); }
	if ( varname == "XprimeX" ) { return( true ); }
	if ( varname == "YprimeY" ) { return( true ); }
	if ( varname == "Theta" ) { return( true ); }
	if ( varname == "SpinPhase" ) { return( true ); }
	return( false );
      }
    }
    
    
    
    if ( IsStrawElectron(particle) ) {
      if ( timeval == "RemainingDist" ) {
	if ( varname == "RhatY" ) { return( true ); }
	if ( varname == "XprimeX" ) { return( true ); }
	if ( varname == "YprimeY" ) { return( true ); }
	if ( varname == "Theta" ) { return( true ); }
	if ( varname == "SpinPhase" ) { return( true ); }
	return( false );
      }

      if ( timeval == "GeneratedDist" ) {
	if ( varname == "SystemHits" ) { return( true ); }
	if ( varname == "NumStraw" ) { return( true ); }
	//if ( varname == "NumStraw_with_SystemHits" ) { return( true ); }
	//if ( varname == "NumStraw_without_SystemHits" ) { return( true ); }
	//if ( varname == "DriftTime" ) { return( true ); }
	//if ( varname == "DeltaTheta" ) { return( true ); }
	//if ( varname == "DeltaP" ) { return( true ); }
	//if ( varname == "DeltaR" ) { return( true ); }
	//if ( varname == "Rhat_vs_DeltaP" ) { return( true ); }
	//if ( varname == "DeltaR_vs_DeltaP" ) { return( true ); }
	if ( varname == "StrawNum_vs_Energy" ) { return( true ); }
	if ( varname == "NumStrawStations_vs_Energy" ) { return( true ); }
	if ( varname == "StrawNum_vs_Theta" ) { return( true ); }
	//if ( varname == "StrawNum_vs_DeltaP" ) { return( true ); }
	//if ( varname == "DriftTime_vs_DeltaR" ) { return( true ); }
	if ( varname == "Theta" ) { return( true ); }
	return( false );
      }
      
      return( false );
    }
  }
  
  
  if ( IsStrawElectron(particle) || IsCaloElectron(particle) ) {
    //if ( varname == "RhatY" ) { return( true ); }
    //     if ( varname == "ThetaR" ) { return( true ); }
    if ( varname == "Theta" ) { return( true ); }
    //if ( varname == "Rhat" ) { return( true ); }
    //if ( varname == "ThetaRhat" ) { return( true ); }
    //if ( varname == "XprimeX" ) { return( true ); }
    if ( varname == "YprimeY" ) { return( true ); }
    if ( varname == "XZ" ) { return( true ); }
    if ( varname == "Energy" ) { return( true ); }

    if ( 0 ) {
    if ( varname == "NumCalo" ) { return( true ); }
    if ( varname == "NumCalo_with_SystemHits" ) { return( true ); }
    if ( varname == "NumCalo_without_SystemHits" ) { return( true ); }
    if ( varname == "DriftTime" ) { return( true ); }
    if ( varname == "DeltaTheta" ) { return( true ); }
    if ( varname == "DeltaP" ) { return( true ); }
    if ( varname == "DeltaR" ) { return( true ); }
    if ( varname == "Rhat_vs_DeltaP" ) { return( true ); }
    if ( varname == "DeltaR_vs_DeltaP" ) { return( true ); }
    if ( varname == "CaloNum_vs_Energy" ) { return( true ); }
    if ( varname == "CaloNum_vs_Theta" ) { return( true ); }
    if ( varname == "CaloNum_vs_DeltaP" ) { return( true ); }
    if ( varname == "DriftTime_vs_DeltaR" ) { return( true ); }
    }

    if ( varname == "NumAllStations" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumAllStationsNoFastRotation" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NudAllStations" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumCaloStation2" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumCaloStation8" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumCaloStation14" && timeval == "TimeOncePerTurn" ) { return( true ); }
    if ( varname == "NumCaloStation20" && timeval == "TimeOncePerTurn" ) { return( true ); }
    //if ( varname == "Y2" && timeval == "TimeOncePerTurn" ) { return( true ); }
  }

  return( false );
}

double GetLineWidth(TString name)
{
  double size = 3;
  if ( name.Contains("_Ratio") ) { size = 1; }
  if ( name.Contains("Num") ) { size = 1; }
  if ( name.Contains("Profile") ) { size = 1; }
  if ( name.Contains("FFT") ) { size = 1; }

  return( size );
}

double GetMarkerSize(TString name)
{
  double size = 0.5;
  if ( name.Contains("_Ratio") ) { size = 0.25; }
  if ( name.Contains("Num") ) { size = 0.1; }
  if ( name.Contains("Profile") ) { size = 0.1; }

  return( size );
}

void GetXminmax(TH2D *hist, double *xmin, double *xmax, double *ymin, double *ymax, double histxmin, double histxmax, double histymin, double histymax)
{
  bool isRatio = false;
  bool isTurn = false;
  bool isRhatY = false;
  bool isVhat = false;
  bool isRhat = false;
  bool isR = false;
  bool isTheta = false;
  bool isThetaR = false;
  bool isThetaRhat = false;
  bool isDriftTimeDeltaR = false;
  bool isRhatDeltaP = false;
  bool isDeltaRDeltaP = false;
  bool isCaloNumMom = false;
  bool isCaloNumEnergy = false;
  bool isCaloNumTheta = false;
  bool isCaloNumDeltaP = false;
  bool isStrawNumMom = false;
  bool isNumStrawMom = false;  
  bool isStrawNumEnergy = false;
  bool isStrawNumTheta = false;
  bool isStrawNumDeltaP = false;
  bool isXZ = false;
  bool isXprimeX = false;
  bool isYprimeY = false;
  bool isXprime = false;
  bool isYprime = false;
  bool isN = false;
  bool isNumCalo = false;
  bool isNumStraw = false;
  bool isNavg = false;
  bool isPol = false;
  bool isPolt0 = false;
  bool isXe = false;
  bool isEqRad = false;
  bool isMom = false;
  bool isZhat = false;
  bool isNum = false;
  bool isEqRadMom = false;
  bool isEnergy = false;

  TString name = hist->GetName();

  //cout << "2D: " << name << "\t" << hist->GetNbinsX() << endl;
  
  if ( name.Contains("BeamScan_") ) { isTurn = true; }
  if ( name.Contains("MuonDecay") ) { isTurn = true; }
  if ( name.Contains("_vs_Turn") ) { isTurn = true; }
  if ( name.Contains("_vs_OncePerTurn") ) { isTurn = true; }
  if ( name.Contains("_vs_SingleHit") ) { isTurn = true; }
  if ( name.Contains("_vs_Time") ) { isTurn = true; }
  if ( name.Contains("_vs_TimeOncePerTurn") ) { isTurn = true; }
  if ( name.Contains("_vs_TimeSingleHit") ) { isTurn = true; }
  if ( isTurn ) {
    if ( name.Contains("Track_Theta") || name.Contains("BeamScan_Theta") || name.Contains("Tracker_Theta") ) { isTheta = true; }
    if ( name.Contains("Track_R") || name.Contains("BeamScan_R") || name.Contains("Tracker_R") ) { isR = true; }
    if ( name.Contains("Track_Rhat") || name.Contains("BeamScan_Rhat") || name.Contains("MuonDecay_Rhat") || name.Contains("Tracker_Rhat") ) { isRhat = true; isR = false; }
    if ( name.Contains("Track_Y") || name.Contains("BeamScan_Y") || name.Contains("MuonDecay_Y") || name.Contains("Tracker_Y") ) { isVhat = true; }
    if ( name.Contains("Track_Vhat") || name.Contains("BeamScan_Vhat") || name.Contains("Tracker_Vhat") ) { isVhat = true; }
    if ( name.Contains("Track_Mom") ||  name.Contains("BeamScan_Mom") ||name.Contains("Tracker_Mom") ) { isMom = true; }
    if ( name.Contains("Track_Energy") || name.Contains("Tracker_Energy") ) { isEnergy = true; }
    if ( name.Contains("Track_Prhat") || name.Contains("BeamScan_Xprime") || name.Contains("MuonDecay_Xprime") || name.Contains("Tracker_Prhat") ) { isXprime = true; }
    if ( name.Contains("Track_Pvhat") || name.Contains("BeamScan_Yprime") || name.Contains("MuonDecay_Yprime") || name.Contains("Tracker_Pvhat") ) { isYprime = true; }
    if ( name.Contains("Track_Xe") || name.Contains("Tracker_Xe") ) { isXe = true; }
    if ( name.Contains("Track_EqRad") || name.Contains("Tracker_EqRad") ) { isXe = true; }
    if ( name.Contains("Track_Pol") || name.Contains("Tracker_Pol") ) { isPol = true; }
    if ( name.Contains("Track_Zhat") || name.Contains("Tracker_Zhat") ) { isZhat = true; }
    if ( name.Contains("Track_NumCounter") || name.Contains("Tracker_NumCounter") ) { isNum = true; }
  }
  else {
    if ( name.Contains("PolXY") ) { isPol = true; }
    if ( name.Contains("Polt0") ) { isPolt0 = true; }
    if ( name.Contains("RhatY") ) { isRhatY = true; }
    if ( name.Contains("EqRadMom") ) { isEqRadMom = true; }
    if ( name.Contains("XZ") ) { isXZ = true; }
    if ( name.Contains("XprimeX") ) { isXprimeX = true; }
    if ( name.Contains("YprimeY") ) { isYprimeY = true; }
    if ( name.Contains("ThetaR") ) { isThetaR = true; }
    if ( name.Contains("ThetaRhat") ) { isThetaRhat = true; isThetaR = false; }
    if ( name.Contains("DriftTime") && name.Contains("DeltaR") ) { isDriftTimeDeltaR = true; }
    if ( name.Contains("Rhat") && name.Contains("DeltaP") ) { isRhatDeltaP = true; }
    if ( name.Contains("DeltaR") && name.Contains("DeltaP") ) { isDeltaRDeltaP = true; }
    if ( name.Contains("CaloNum") && name.Contains("Mom") ) { isCaloNumMom = true; }
    if ( name.Contains("CaloNum") && name.Contains("Energy") ) { isCaloNumEnergy = true; }
    if ( name.Contains("CaloNum") && name.Contains("Theta") ) { isCaloNumTheta = true; }
    if ( name.Contains("CaloNum") && name.Contains("DeltaP") ) { isCaloNumDeltaP = true; }
    if ( name.Contains("StrawNum") && name.Contains("Mom") ) { isStrawNumMom = true; }
    if ( name.Contains("StrawNum") && name.Contains("Energy") ) { isStrawNumEnergy = true; }
    if ( name.Contains("NumStrawStation") && name.Contains("Energy") ) { isNumStrawMom = true; }
    if ( name.Contains("StrawNum") && name.Contains("Theta") ) { isStrawNumTheta = true; }
    if ( name.Contains("StrawNum") && name.Contains("DeltaP") ) { isStrawNumDeltaP = true; }
  }

  
  if ( name.Contains("_Ratio") ) { isRatio = true; }
  
  //if ( isRatio ) { hist->SetMinimum(-1e-5); }
  
  if ( isTurn ) {
    *xmin = hist->GetXaxis()->GetXmin();
    *xmax = hist->GetXaxis()->GetXmax();

    if ( maxtime_for_plot > 0 ) { 
      *xmax = maxtime_for_plot;
      // since I scale by 1.25 down the road I need to by 1.25 now to undo that.
      //*xmax /= 1.25;
    }

    if ( mintime_for_plot > 0 ) { 
      *xmin = mintime_for_plot;
      // since I scale by 1.25 down the road I need to by 1.25 now to undo that.
      //*xmax /= 1.25;
    }

    if ( isRhat || isVhat ) {
      *ymax = 300; *ymin = -300;
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 45; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }

    if ( isNum ) {
      *ymax = histymax; *ymin = histymin;
      return;
    }

    if ( isTheta ) {
      *ymax = TMath::TwoPi();
      *ymin = 0.0;
      return;
    }

    if ( isR ) {
      *ymax = 300; *ymin = -300;
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 10; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }


    if ( isZhat ) {
      *ymax = TMath::Pi(); *ymin = -TMath::Pi();
      //       for ( double mm = TMath::Pi(); mm >= 0.1; mm -= TMath::Pi()/8.0 ) {
      // 	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      // 	cout << mm << "\t" << *ymin << "\t" << *ymax << endl;
      //       }
      return;
    }

    if ( isEnergy ) {
      *ymax = 3.5; *ymin = 0;
      for ( double mmx = 3.5; mmx >= 1; mmx -= 0.1 ) {
	if ( histymax < mmx ) { *ymax = mmx; }
      }
      return;
    }
    
    if ( isMom ) {
      *ymax = 2; *ymin = 0;
      if ( name.Contains("Electron") ) {
	for ( double mmx = 3.5; mmx >= 1; mmx -= 0.1 ) {
	  if ( histymax < mmx ) { *ymax = mmx; }
	}
      }
      else {
	if ( name.Contains("Muon") && name.Contains("GeneratedDist") ) {
	  *ymin = 1.0*(1-3*dPoverP);
	  *ymax = 1.0*(1+3*dPoverP);
	  return;
	}
	else if ( name.Contains("BeamScan") || name.Contains("MuonDecay") ) {
	  *ymin = 1.0*(1-3*dPoverP);
	  *ymax = 1.0*(1+3*dPoverP);
	  return;
	}
	else if ( name.Contains("Muon") ) {
	  if ( name.Contains("LostMuon") ) {
	    *ymin = 1.0*(1-20*dPoverP);
	    *ymax = 1.0*(1+20*dPoverP);	    
	    return;
	  }
	  else {
	    *ymin = 1.0*(1-3*dPoverP);
	    *ymax = 1.0*(1+3*dPoverP);
	    return;
	  }
	}
	else if ( name.Contains("Electron") ) {
	  *ymin = 0.0; *ymax = 1.0;
	}
	else {
	  *ymin = 0.0; *ymax = 1.0;
	}
      }

      return;
    }


    if ( isPol ) {
      *ymax = 1.0; *ymin = -1.0;
      return;
    }
    
    if ( isXe || isEqRad ) { 
      *ymax = 50; *ymin = -50;
      for ( int mm = 50; mm >= 10; mm -= 10 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }

    if ( isXprime || isYprime ) {
      *ymax = 100; *ymin = -100;
      for ( int mm = 100; mm >= 20; mm -= 20 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 20; mm >= 10; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }
  }
  else {

    if ( isNumStrawMom ) {
      *xmin = 0; *xmax = 10;
      *ymin = 0.0;
      *ymax = 3.5;
      return;
    }

    if ( isCaloNumMom || isStrawNumMom ) {
      *xmin = 0; *xmax = 24;
      *ymin = 0.0;
      *ymax = 3.5;
      //for ( int mm = 3.2; mm >= 1; mm -= 0.5 ) {
      //if ( histymax < mm ) { *ymax = mm; }
      //}
      return;
    }

    if ( isCaloNumEnergy || isStrawNumEnergy ) {
      *xmin = 0; *xmax = 24;
      *ymin = 0.0;
      *ymax = 3.5;
      //for ( int mm = 3.2; mm >= 1; mm -= 0.5 ) {
      //if ( histymax < mm ) { *ymax = mm; }
      //}
      return;
    }

    if ( isCaloNumDeltaP || isStrawNumDeltaP ) {
      *xmin = 0; *xmax = 24;
      *ymin = -500;
      *ymax = 0;
      for ( int mm = -500; mm >= 10; mm += 100 ) {
	if ( histymin > mm ) { *ymin = mm; }
      }
      return;
    }

    if ( isCaloNumTheta || isStrawNumTheta ) {
      *xmin = 0; *xmax = 24;
      *ymin = 0.0;
      *ymax = TMath::TwoPi();
      return;
    }

    if ( isDriftTimeDeltaR ) {
      *xmin = 0;
      *xmax = 100;
      for ( int mm = 100; mm >= 10; mm -= 10 ) {
	if ( histxmax < mm ) { *xmax = mm; }
      }
      *ymin = 0;
      *ymax = 7000;
      for ( int mm = 7000; mm >= 1000; mm -= 500 ) {
	if ( histymax < mm ) { *ymax = mm; }
      }
      return;
    }

    if ( isRhatDeltaP ) {
      *xmax = 0; *xmin = -300;
      *ymin = -500;
      *ymax = 0;
      for ( int mm = -500; mm >= 10; mm += 100 ) {
	if ( histymin > mm ) { *ymin = mm; }
      }
      for ( int mm = -300; mm <= 100; mm += 50 ) {
	if ( histxmin > mm ) { *xmin = mm; }
      }
      return;
    }

    if ( isDeltaRDeltaP ) {
      *xmin = 0;
      *xmax = 7000;
      for ( int mm = 7000; mm >= 1000; mm -= 500 ) {
	if ( histxmax < mm ) { *xmax = mm; }
      }
      *ymin = -500;
      *ymax = 0;
      for ( int mm = -500; mm >= 10; mm += 100 ) {
	if ( histymin > mm ) { *ymin = mm; }
      }
      return;
    }
    
    if ( isThetaRhat ) {
      *ymax = 300; *ymin = -300;
      *xmin = 0.0;
      *xmax = TMath::TwoPi();
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 30; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }
    if ( isThetaR ) {
      *ymax = 300; *ymin = 0;
      *xmin = 0.0;
      *xmax = TMath::TwoPi();
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm ) { *ymax = mm; }
      }
      for ( int mm = 50; mm >= 30; mm -= 5 ) {
	if ( histymax < mm ) { *ymax = mm; }
      }
      return;
    }
    if ( isRhatY ) {
      *ymax = 300; *ymin = -300;
      *xmax = 300; *xmin = -300;
      for ( int mm = 300; mm >=50; mm -= 50 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 50; mm >= 30; mm -= 5 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 30; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }

    if ( isEqRadMom ) {
      *xmax = 50; *xmin = -50;
      for ( int mm = 50; mm >= 10; mm -= 10 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      *ymax = 2; *ymin = 0;
      if ( name.Contains("Electron") ) {
	*ymin = 0.0; *ymax = 1.0;
      }
      else {
	if ( name.Contains("BirthMuon") || name.Contains("DecayMuon") || name.Contains("StoredMuon") || name.Contains("RingTracker") || name.Contains("Tracker_Mom") ) {
	  *ymin = 1.0*(1-3*dPoverP);
	  *ymax = 1.0*(1+3*dPoverP);
	}
	else {
	  *ymin = 0.0; *ymax = 1.0;
	}
      }
      return;
    }


    if ( isXZ ) {
      *ymax = 8000.0; *ymin = -8000.0;
      *xmax = 8000.0; *xmin = -8000.0;
      return;
    }

    if ( isPol ) {
      *ymax = 1; *ymin = -1;
      *xmax = 1; *xmin = -1;
      //       for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
      // 	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      //       }
      //       for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
      // 	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      //       }
      return;
    }

    if ( isPolt0 ) {
      *ymax = 150; *ymin = 0;
      *xmax = 1; *xmin = -1;
      //       for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
      // 	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      //       }
      //       for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
      // 	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      //       }
      return;
    }
    
    if ( isXprimeX || isYprimeY ) {
      *ymax = 100; *ymin = -100;
      *xmax = 300; *xmin = -300;
      for ( int mm = 300; mm >=50; mm -= 50 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 50; mm >= 30; mm -= 5 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 100; mm >= 20; mm -= 20 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 20; mm >= 5; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }
  }
  
  cout << name << " has no limits from Hist2d." << endl;
}

void RebinHist(TH1D *hist1d)
{
  TString hname = hist1d->GetName();

  if ( hname.Contains("Profile") ) { return; }
  if ( hname.Contains("FFT") ) { return; }
  if ( hname.Contains("NumAllStations") ) { return; }
  if ( hname.Contains("NudAllStations") ) { return; }
  if ( hname.Contains("NumCalo") ) { return; }
  if ( hname.Contains("NumStraw") ) { return; }
  if ( hname.Contains("CaloNum") ) { return; }
  if ( hname.Contains("StrawNum") ) { return; }

  //if ( hname.Contains("_Ratio") ) { hist1d->Rebin(100); return; }

  //if ( hname.Contains("_Num") ) { hist1d->Rebin(7*5); return; }
  if ( hname.Contains("_Num") ) { hist1d->Rebin(5); return; }
  
  if ( hname.Contains("NgtEth") || hname.Contains("NwghtE") || hname.Contains("Nud") || hname.Contains("Nup") || hname.Contains("Ndown") || hname.Contains("Num") ) { ; }
  else {
    if ( hname.Contains("TimeOncePerTurn") ) { hist1d->Rebin(24); }

  }
  if ( maxtime_for_plot > 0 ) { return; }
  
  if ( hname.Contains("TrackYprime") || hname.Contains("TrackVhat") ) {
    //if ( !hname.Contains("OncePerTurn") ) { hist1d->Rebin(12); }
    int nturns_per_bin = 4;
    //while ( hist1d->GetMaximum() > 100 || hist1d->GetMinimum() < -100 ) { hist1d->Rebin(2); }
    hist1d->Rebin(nturns_per_bin);
  }
  
  if ( hname.Contains("NgtEth") || hname.Contains("NwghtE") || hname.Contains("Nud") || hname.Contains("Nup") || hname.Contains("Ndown") || hname.Contains("Num") ) {
    if ( !hname.Contains("FFT") && !hname.Contains("Fit") ) {
      int nturns_per_bin = 1;
      int wiggle_sf = 12;
      if ( hname.Contains("G4") == false ) {
	if ( !hname.Contains("OncePerTurn") ) { hist1d->Rebin(12); }
      }
      
      //cout << "NAME: " << hname << "\t" << hist1d->Integral() << endl;
      
      if ( maxturnsreal % 6 == 0 ) {
	if ( hist1d->Integral() < 200 ) {  nturns_per_bin = 6; }
	if ( hist1d->Integral() < 1000 ) { nturns_per_bin = 3; }
	else if ( hist1d->Integral() < 2000 ) { nturns_per_bin = 2; }	
      }
      else if ( maxturnsreal % 10 == 0 ) {
	if ( hist1d->Integral() < 100 ) {  nturns_per_bin = 10; }
	if ( hist1d->Integral() < 500 ) { nturns_per_bin = 5; }
	if ( hist1d->Integral() < 2000 ) { nturns_per_bin = 2; }
      }
      else if ( maxturnsreal % 4 == 0 ) {
	if ( hist1d->Integral() < 200 ) {  nturns_per_bin = 4; }
	if ( hist1d->Integral() < 1000 ) { nturns_per_bin = 2; }
      }
      
      hist1d->Rebin(nturns_per_bin);

      // Count # of empty bins
      int nempty = 0;
      int nbins = hist1d->GetNbinsX();
      for ( int i = 1; i <= hist1d->GetNbinsX(); i++ ) {
	if ( hist1d->GetBinContent(i) <= 0.0 ) { nempty++; }
      }
      cout << "EMPTY = " << nempty << " / " << nbins << endl;
      while ( (double)nempty/nbins > 0.25 && (double)nempty/nbins < 0.75 ) {
	hist1d->Rebin(2);
	nempty = 0;
	nbins = hist1d->GetNbinsX();
	for ( int i = 1; i <= hist1d->GetNbinsX(); i++ ) {
	  if ( hist1d->GetBinContent(i) <= 0.0 ) { nempty++; }
	}
	cout << "EMPTY = " << nempty << " / " << nbins << endl;
	if ( (double)nempty/nbins < 0.25 ) { break; }
      }
    }
  }
}

void GetXminmax(TH1D *hist, double *xmin, double *xmax, double *ymin, double *ymax, double histxmin, double histxmax, double histymin, double histymax, bool dolog)
{
  bool isProfile = false;
  bool isRatio = false;
  bool isTurn = false;
  bool isTime = false;
  bool isRhat = false;
  bool isVhat = false;
  bool isTheta = false;
  bool isY2 = false;
  bool isDeltaTheta = false;
  bool isDeltaP = false;
  bool isDeltaR = false;
  bool isDriftTime = false;
  bool isTheta = false;
  bool isTheta = false;
  bool isXprime = false;
  bool isYprime = false;
  bool isN = false;
  bool isNpart = false;
  bool isNavg = false;
  bool isNumCalo = false;
  bool isNumStraw = false;
  bool isSystemHits = false;
  bool isPol = false;
  bool isXe = false;
  bool isMom = false;
  bool isEnergy = false;
  bool isFFT = false;
  bool isZhat = false;
  bool isR = false;
  bool ist0 = false;
  bool isSnapshot = false;
  bool isMS = false;

  TString name = hist->GetName();

  if ( name.Contains("_MeanSquared") ) { isMS = true; }
  if ( name.Contains("_Profile") ) { isTime = true; isProfile = true; }
  if ( name.Contains("_Ratio") ) { isRatio = true; }
  if ( name.Contains("_vs_Turn") ) { isTurn = true; }
  if ( name.Contains("_vs_Time") ) { isTime = true; }
  if ( name.Contains("_Vhat_") ) { isRhat = true; }
  if ( name.Contains("_Y_") ) { isRhat = true; }
  if ( name.Contains("_Y2_") ) { isY2 = true; }
  if ( name.Contains("_Xprime_") ) { isXprime = true; }
  if ( name.Contains("_Yprime_") ) { isYprime = true; }
  if ( name.Contains("_NumStation") ) { isN = true; }
  if ( name.Contains("_NumCaloStation") ) { isN = true; }
  if ( name.Contains("_NumStrawStation") ) { isN = true; }
  if ( name.Contains("_NudStation") ) { isNavg = true; }
  if ( name.Contains("_Pol_") ) { isPol = true; }
  if ( name.Contains("_NumCalo_") ) { isNumCalo = true; }
  if ( name.Contains("_NumStraw_") ) { isNumStraw = true; }
  if ( name.Contains("_Mom_") ) { isMom = true; }
  if ( name.Contains("_Energy_") ) { isEnergy = true; }
  if ( name.Contains("_Theta_") ) { isTheta = true; }
  if ( name.Contains("_SystemHits_") ) { isSystemHits = true; }
  if ( name.Contains("_DeltaP_") ) { isDeltaP = true; }
  if ( name.Contains("_DeltaR_") ) { isDeltaR = true; }
  if ( name.Contains("_DriftTime_") ) { isDriftTime = true; }
  if ( name.Contains("_Xe_") ) { isXe = true; }
  if ( name.Contains("_EqRad_") ) { isXe = true; }
  if ( name.Contains("FFT") ) { isFFT = true; }
  if ( name.Contains("_Zhat_") ) { isZhat = true; }
  if ( name.Contains("_R_") ) { isR = true; }
  if ( name.Contains("Nparticles") ) { isNpart = true; }
  if ( name.Contains("_t0_") ) { ist0 = true; }
  if ( name.Contains("_Time_") ) { ist0 = true; }
  if ( name.Contains("_Rhat_") ) { isRhat = true; }
  if ( name.Contains("Snapshot") ) {
    isSnapshot = true;
    if ( name.Contains("_Rhat") ) { isRhat = true; }
    if ( name.Contains("_Time") ) { isTime = true; }
  }

  bool debug = true;

  double xmin95, xmax95;
  double xmin1, xmax1;
  double sumall = hist->Integral();
  xmin95 = hist->FindFirstBinAbove(0.02*sumall);
  xmax95 = hist->FindLastBinAbove(0.02*sumall);
  xmin1 = hist->FindFirstBinAbove(1);
  xmax1 = hist->FindLastBinAbove(1);

  if ( isFFT && 0 ) {
    cout << "isFFT" << endl;
    cout << "[X]  = " << histxmin << " , " << histxmax << endl;
    cout << "[X'] = " << xmin95 << " , " << xmax95 << endl;
    cout << "[X'] = " << xmin1 << " , " << xmax1 << endl;
    cout << "[Y]  = " << histymin << " , " << histymax << endl;
  }
  
  *xmin = histxmin;
  *xmax = histxmax;
  *ymax = 1.25*histymax;
  *ymin = 0.0;
  if ( dolog ) { 
    if ( *ymax > 10000 && histymin == 0 ) { *ymin = 1.0; }
    else if ( histymin > 0 ) { *ymin = histymin / 2; }
    else if ( *ymax < 10 ) { *ymin = 0.5; }
    else { *ymin = *ymax / 10000.0; }
  }

  if ( histymin < 0 ) {
    if ( TMath::Abs(histymax) > TMath::Abs(histymin) ) {
      *ymin = -1.25*histymax;
      *ymax = 1.25*histymax;
    }
    else {
      *ymin = 1.25*histymin;
      *ymax = -1.25*histymin;
    }
  }

  if ( isFFT ) {
    *xmin = 0.0; *xmax = histxmax/2;       
    if ( 0 ) {
      if ( *xmax < 12 ) { *xmax = 12.0; }
      if ( *xmax > 20 ) { *xmax = 20.0; }
      if ( *xmin < 0.1 ) { *xmin = 0.1; }
    }
    return;
  }


  if ( isRatio ) {    
    //cout << "YMAX = " << histymax << endl;
    for ( double mmx = 1.0; mmx >= 0.01; mmx -= 0.2 ) {
      if ( histymax < mmx ) { *ymax = mmx; }
    }
    *ymax = *ymax * 1.1;
    *ymin = 0.0;
    if ( isEnergy ) { *xmin = 0.0; *xmax = 3.5; }
    if ( isRhat || isVhat ) { *xmin = -45; *xmax = 45; }
    if ( isMom ) { 
      *xmin = 1.0*(1-3*dPoverP);
      *xmax = 1.0*(1+3*dPoverP);
    }
    return;
  }
  
  if ( isMS ) {
    *xmin = histxmin;
    *xmax = histxmax;
    *ymin = 0.0;
    *ymax = histymax;
    return;
  }
 
  if ( isProfile ) {
    cout << "isProfile()" << endl;
    *xmin = histxmin;
    *xmax = histxmax;
    *ymin = histymin*1.1;
    *ymax = histymax*1.1;

    if ( isRhat ) {
      *ymax = 300; *ymin = -300;
      for ( int mm = 300; mm > 0; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 20; mm -= 10 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 20; mm >= 6; mm -= 2 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
    }

    if ( isXprime || isYprime ) {
      *ymax = 20; *ymin = -20;
      for ( int mm = 20; mm > 0; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 10; mm >= 2; mm-- ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
    }

    if ( isMom ) {
      *ymin = 1.0*(1-3*dPoverP);
      *ymax = 1.0*(1+3*dPoverP);
      return;
    } 
   return;
  }

  if ( isSnapshot ) {
    if ( isRhat ) {
      *xmin = -90; *xmax = 90;
      return;
    }
    if ( isVhat ) {
      *xmin = -60; *xmax = 60;
      return;
    }

    if ( isTime ) {
      *xmin = 0.0;
      *xmax = 250.0;
      return;
    }
  }

  // If vs time plot
  if ( isTurn || isTime ) {
    if ( mintime_for_plot > 0 ) {
      *xmin = mintime_for_plot;
    }
    if ( maxtime_for_plot > 0 ) {
      *xmax = maxtime_for_plot;
    }
    
    if ( isRhat ) {
      *ymax = 300; *ymin = -300;
      for ( int mm = 300; mm > 0; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 35; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }

      if ( isProfile ) { 
	*ymax = *ymax * 1.5; 
	*ymin = *ymin * 1.5;
      }
      else {
	*ymax = *ymax * 1.2; 
	*ymin = *ymin * 1.2;
      }
      return;
    }
  
    if ( isMom ) {
      *ymin = 1.0*(1-3*dPoverP);
      *ymax = 1.0*(1+3*dPoverP);
      return;
    }

    if ( isXprime || isYprime ) {
      if ( debug ) { cout << "GetXminmax: IsX' or IsY'" << endl; }
      *ymax = 25; *ymin = -25;
      for ( int mm = 25; mm >= 5; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 25; mm >= 5; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }  

    if ( isPol ) {
      if ( debug ) { cout << "GetXminmax: IsPol" << endl; }
      *ymax = 1; *ymin = -1;
      for ( double mmy = 1; mmy >= 0.1; mmy -= 0.1 ) {
	if ( histymax < mmy && histymin > -mmy ) { *ymax = mmy; *ymin = -mmy; }
      }
      return;
    }
    
    if ( isY2 ) {
      *ymax = 300; *ymin = 0;
      for ( int mm = 300; mm > 0; mm -= 50 ) {
	if ( histymax < mm ) { *ymax = mm; }
      }
      for ( int mm = 50; mm >= 10; mm -= 5 ) {
	if ( histymax < mm ) { *ymax = mm; }
      }
      
      if ( isProfile ) { 
	*ymax = *ymax * 1.2; 
	*ymin = *ymin * 1.2;
      }
      return;
    }
  }
  else
    if ( isRhat || isVhat ) {
      if ( debug ) { cout << "GetXminmax: IsRhat or IsVhat" << endl; }
      *xmax = 300; *xmin = -300;
      for ( int mm = 300; mm > 0; mm -= 50 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 50; mm >= 10; mm -= 5 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      return;
    }
  
  
  if ( isR ) {
    if ( debug ) { cout << "GetXminmax: IsR" << endl; }
    *xmax = 300; *xmin = 0;
    for ( int mm = 300; mm > 0; mm -= 50 ) {
      if ( histxmax < mm ) { *xmax = mm; }
    }
    for ( int mm = 50; mm >= 10; mm -= 5 ) {
      if ( histxmax < mm ) { *xmax = mm; }
    }
    return;
  }


  if ( isZhat ) {
    if ( debug ) { cout << "GetXminmax: IsZhat" << endl; }
    *xmax = TMath::Pi(); *xmin = -TMath::Pi();
    for ( double mmx = TMath::Pi(); mmx >= 0.1; mmx -= TMath::Pi()/8.0 ) {
      if ( histxmax < mmx && histxmin > -mmx ) { *xmax = mmx; *xmin = -mmx; }
    }
    return;
  }
  
  if ( isPol ) {
    if ( debug ) { cout << "GetXminmax: IsPol" << endl; }
    *xmax = 1; *xmin = -1;
    for ( double mmx = 1; mmx >= 0.1; mmx -= 0.1 ) {
      if ( histxmax < mmx && histxmin > -mmx ) { *xmax = mmx; *xmin = -mmx; }
    }
    return;
  }

  if ( isXe ) {
    if ( debug ) { cout << "GetXminmax: IsXe" << endl; }
    *xmax = 50; *xmin = -50;
    for ( int mm = 50; mm >= 30; mm -= 20 ) {
      if ( xmax95 < mm && xmin95 > -mm ) { *xmax = mm; *xmin = -mm; }
      //if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    for ( int mm = 30; mm >= 15; mm -= 15 ) {
      if ( xmax95 < mm && xmin95 > -mm ) { *xmax = mm; *xmin = -mm; }
      //if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    for ( int mm = 15; mm >= 10; mm -= 5 ) {
      if ( xmax95 < mm && xmin95 > -mm ) { *xmax = mm; *xmin = -mm; }
      //if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }

  if ( ist0 ) {
    if ( debug ) { cout << "GetXminmax: Ist0" << endl; }
    *xmax = TMath::Min(histxmax * 1.25, histxmax + 5);
    *xmin = TMath::Max(histxmin * 0.75, histxmin - 5) ;
    return;
    //HERE
  }

  if ( isN ) {
    *ymax = histymax; *ymin = histymin;
    if ( isRatio ) {
      for ( double mmx = 1.0; mmx >= 0.01; mmx -= 0.2 ) {
	if ( histymax > mmx ) { *ymax = mmx; }
      }
      *ymax = *ymax * 1.1;
      *ymin = 0.0;
    }
    return;
  }
  if ( isNpart ) {
    *ymax = histymax; *ymin = histymin;
    *xmin = 0.0; *xmax = 10;
    return;
  }

  if ( isSystemHits ) {
    *ymax = histymax*1.2; *ymin = histymin;
    *xmin = 0.0; *xmax = 10;
    return;
  }

  if ( isNumCalo || isNumStraw ) {
    *ymax = histymax*1.2; *ymin = histymin;
    *xmin = 0.0; *xmax = 24;
    return;
  }

  if ( isNavg ) {
    if ( debug ) { cout << "GetXminmax: IsNavg" << endl; }
    double sum_num = 0;
    double sum_den = 0;
    for ( int i = 1; i < hist->GetNbinsX(); i++ ) {
      sum_num += hist->GetBinContent(i) * hist->GetBinWidth(i);
      sum_den += hist->GetBinWidth(i);
    }
    double yavg = sum_num / sum_den;
    double yrms = 0.0;
    for ( int i = 1; i < hist->GetNbinsX(); i++ ) {
      yrms += (hist->GetBinContent(i) - yavg)*(hist->GetBinContent(i) - yavg);
    }
    yrms /= hist->GetNbinsX();
    yrms = TMath::Sqrt(yrms);
    // hist->Integral() / (hist->GetNbinsX()*(histxmax - histxmin));
    //double yrms = TMath::Abs(yavg - 1);
    //cout << yavg << "\t" << hist->Integral() << "\t" << histxmin << "\t" << histxmax << endl;
    if( yavg > 0.95 && yavg < 1.05 ) {
      *ymin = 1 - 3*yrms; *ymax = 1 + 3*yrms;
    }
    else {
      *ymin = yavg - 3*yrms; *ymax = yavg + 3*yrms;
    }
    if ( isRatio ) {
      for ( double mmx = 1.0; mmx >= 0.01; mmx -= 0.2 ) {
	if ( histymax > mmx ) { *ymax = mmx; }
      }
      *ymax = *ymax * 1.1;
      *ymin = 0.0;
    }
    return;
  }

  if ( isXprime || isYprime ) {
    if ( debug ) { cout << "GetXminmax: IsX' or IsY'" << endl; }
    *xmax = 100; *xmin = -100;
    for ( int mm = 100; mm >= 20; mm -= 20 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    for ( int mm = 20; mm >= 5; mm -= 5 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }  

  if ( isTheta ) {
    if ( debug ) { cout << "GetXminmax: IsTheta" << endl; }
    *xmax = TMath::TwoPi(); *xmin = 0;
    return;
  }

  if ( isDeltaR ) {
    if ( debug ) { cout << "GetXminmax: IsDeltaR" << endl; }
    *xmin = 0;
    *xmax = 7000;
    for ( int mm = 7000; mm >= 1000; mm -= 500 ) {
      if ( histxmax < mm ) { *xmax = mm; }
    }
    return;
  }

  if ( isDeltaP ) {
    if ( debug ) { cout << "GetXminmax: IsDeltaP" << endl; }
    *xmin = -500;
    *xmax = 0;
    for ( int mm = -500; mm >= 10; mm += 100 ) {
      if ( histxmin > mm ) { *xmin = mm; }
    }
    return;
  }

  if ( isDriftTime ) {
    if ( debug ) { cout << "GetXminmax: IsDeltaP" << endl; }
    *xmin = 0;
    *xmax = 100;
    for ( int mm = 100; mm >= 10; mm -= 10 ) {
      if ( histxmax < mm ) { *xmax = mm; }
    }
    return;
  }

  if ( isEnergy ) {
    if ( debug ) { cout << "GetXminmax: IsEnergy" << endl; }
    *xmax = 3.5; *xmin = 0;
    for ( double mmx = 3.5; mmx >= 1; mmx -= 0.2 ) {
      if ( histxmax < mmx ) { *xmax = mmx; }
    }
    return;
  }

  if ( isMom ) {
    if ( debug ) { cout << "GetXminmax: IsMom" << endl; }
    *xmax = 2; *xmin = 0;
    if ( name.Contains("Electron") ) {
      for ( double mmx = 3.5; mmx >= 1; mmx -= 0.2 ) {
	if ( histxmax < mmx ) { *xmax = mmx; }
      }
      cout << *xmin << "\t" << *xmax << endl;
      return;
    }
    else {
      if ( name.Contains("Muon") && name.Contains("GeneratedDist") ) {
	*xmin = 1.0*(1-3*dPoverP);
	*xmax = 1.0*(1+3*dPoverP);
	return;
      }
      else if ( name.Contains("Muon") ) {
	if ( name.Contains("LostMuon") ) {
	  *xmin = 1.0*(1-20*dPoverP);
	  *xmax = 1.0*(1+20*dPoverP);	    
	  return;
	}
	else {
	  *xmin = 1.0*(1-3*dPoverP);
	  *xmax = 1.0*(1+3*dPoverP);
	  return;
	}
      }
    }
    return;
  }
  
  cout << name << " has no limits from Hist1D." << endl;
}

string SurviveTextRaw(int numer, int denom, string sup)
{
  double abseff, abserr;
  geteff(numer, denom, &abseff, &abserr);
  return( SurviveText(abseff, abserr, sup) );
}

string SurviveText(double abseff, double abserr, string sup)
{
  stringstream ss_abseff;
  if ( abseff >= 100 ) { ss_abseff.precision(3); }
  else if ( abseff >= 10 ) { ss_abseff.precision(2); }
  else { ss_abseff.precision(2); }
  
  ss_abseff.str("");
  if ( sup.size() == 0 ) {
    ss_abseff << "f_{sur} = " << abseff;
  }
  else {
    ss_abseff << "f^{" << sup << "}_{sur} = " << abseff;
  }
  if ( abseff >= 100 ) { ss_abseff.precision(1); }
  else if ( abseff >= 10 ) { 
    if ( err_turn100 >=1 ) { ss_abseff.precision(2); }
    else { ss_abseff.precision(1); }
  }
  else { ss_abseff.precision(1); }
  ss_abseff << "#pm" << abserr;
  
  return( ss_abseff.str() );
}

Double_t OscFit(Double_t *x, Double_t *par)
{
  double t = x[0];
  double N = par[0];
  //   double A = par[1];
  //   double Omega_a = par[2];
  //   double Phi = par[3];

  double gamma_tau = 64.0;
  gamma_tau /= 0.149;
  
  double Count = par[0]*TMath::Exp(-x[0]*par[1]/(gamma_tau)); //*(1 - A*TMath::Cos(Omega_a*t + Phi));
  
  //cout << "N = " << N << "\tA = " << A << "\tOmega_{a} = " << Omega_a << "\tPhi = " << Phi << "\tFunc = " << Count << endl;
  //cout << par[0] << "\t" << par[1] << "\t" << x[0] << "\tFunc = " << Count << endl;
  
  return( Count );
}

void FitOscillation(TH1D *hist1d, string Fitname)
{
  if ( hist1d == NULL ) { return; }
  cout << "=====================================" << endl;
  cout << "Running Fit for " << hist1d->GetName() << endl;
  cout << "=====================================" << endl;

  double int_prev;
  double int_curr;
  double int_start;

  cout << "Nbins = " << hist1d->GetNbinsX() << "\t" << hist1d->GetBinCenter(hist1d->GetNbinsX()-1) << "\t" << hist1d->GetBinWidth(1) << endl;
  hist1d->Rebin(24);
  cout << "Nbins = " << hist1d->GetNbinsX() << "\t" << hist1d->GetBinCenter(hist1d->GetNbinsX()-1) << "\t" << hist1d->GetBinWidth(1) << endl;

  cout << "Min: " << hist1d->GetMinimum() << "\tMax: " << hist1d->GetMaximum() << endl;

  //TF1 *fCount = new TF1("fCount", OscFit, 2, 1, 500);
  TF1 *fCount = new TF1("fCount", "[0]*exp(-[1]*x)*(1-[2]*cos([3]*x+[4]))", 1, 500);
  fCount->SetParName(0, "N");
  fCount->SetParName(1, "#gamma#tau");
  fCount->SetParName(2, "A");
  fCount->SetParName(3, "#omega_{a}");
  fCount->SetParName(4, "#phi");
  if ( Fitname.find("Nup") != string::npos ) {
    fCount->SetParameter(0, hist1d->GetMaximum());
  }
  if ( Fitname.find("Ndown") != string::npos ) {
    fCount->SetParameter(0, hist1d->GetMinimum());
  }
  fCount->SetParameter(1, 1/300.0);
  fCount->SetParameter(2, 0.5);
  fCount->SetParameter(3, 1/29.0);
  fCount->SetParameter(4, 0.0);
  
  //   if ( Fitname.find("Nup") != string::npos ) {
  //     fCount->SetParameters(hist1d->GetMaximum(), 1.0);
  //   }
  //   if ( Fitname.find("Ndown") != string::npos ) {
  //     ;//fCount->SetParLimits(0, hist1d->GetMinimum(), -1);
  //   }
  //   Double_t pmin, pmax;
  //cout << fCount->GetParLimits(0, pmin, pm
  //   if ( Fitname.find("Nup") != string::npos ) {ax);
  //cout << "Min: " << pmin << "\tMax: " << pmax << endl;
  //   fCount->SetParLimits(1, 0, 1);
  //   fCount->SetParLimits(2, 1/40, 1/40);
  //   fCount->SetParLimits(3, 0, TMath::TwoPi());
  //   if ( Fitname.find("Nup") != string::npos ) {
  //     fCount->SetParameters(hist1d->GetMaximum()/2.0, 0.5, 1/29, TMath::Pi());
  //   }
  //   if ( Fitname.find("Ndown") != string::npos ) {
  //     fCount->SetParameters(hist1d->GetMinimum()/2.0, 0.5, 1/29, TMath::Pi());
  //   }
  cout << "Fitting..." << endl;
  TFitResultPtr r = hist1d->Fit("fCount","SRQ");
  cout << "Done." << endl;
  TMatrixDSym cov = r->GetCovarianceMatrix();  //  to access the covariance matrix
  Double_t chi2   = r->Chi2(); // to retrieve the fit chi2
  //Double_t par0   = r->Parameter(0); // retrieve the value for the parameter 0
  //Double_t err0   = r->ParError(0); // retrieve the error for the parameter 0
  //r->Print("V");     // print full information of fit including covariance matrix
  cout << "N         = " << r->Parameter(0) << " +/- " << r->ParError(0) << endl;
  cout << "gt        = " << 1/r->Parameter(1) << " +/- " << 1/r->ParError(1) << endl;
  cout << "A         = " << r->Parameter(2) << " +/- " << r->ParError(2) << endl;
  cout << "Omega_{a} = " << 1/r->Parameter(3) << " +/- " << 1/r->ParError(3) << endl;
  cout << "Phi       = " << r->Parameter(4) << " +/- " << r->ParError(4) << endl;
  cout << endl;

  TH1D *hist1d_x_tmp = (TH1D*)fCount->GetHistogram();
  TH1D *hist1d_x = (TH1D*)(hist1d_x_tmp->Clone(Fitname.c_str()));
  //Fitname += "_Fit";
  //TH1D *hist1d_x = (TH1D*)hist1d->Clone(Fitname.c_str());
  MakePlot1D(hist1d_x, -1, -1, &int_prev, &int_curr, &int_start);
}


void ComputeFFT(TH1D *hist1d, string FFTname)
{
  //return;
  if ( hist1d == NULL ) { return; }
  cout << "=====================================" << endl;
  cout << "Running FFT for " << hist1d->GetName() << endl;
  cout << "=====================================" << endl;


  double int_prev;
  double int_curr;
  double int_start;
  double entries = hist1d->GetEntries();

  TString name = hist1d->GetName();
  if ( name.Contains("Num") ) {
    double binwidth = hist1d->GetBinWidth(1);
    double expbinwidth = 0.025;
    double ratio = expbinwidth / binwidth;
    if ( ratio > 1 ) {
      hist1d->Rebin(ratio);
    }
  }


  
  if ( 1 ) {
    cout << "Bins = " << hist1d->GetNbinsX() << endl;
  }
  double max = -9.9;
  double maxbin = -1;
  double range = hist1d->GetBinLowEdge(hist1d->GetNbinsX()-1) + hist1d->GetBinWidth(1) - hist1d->GetBinLowEdge(1);
  TH1D *h_wavelength = 0;
  TH1D *h_mag = 0;
  TH1D *h_ph = 0;
  cout << "Range = " << range << endl;

  FFTname += "_FFT";
  int nSamples = hist1d->GetNbinsX();
  // get the magnitude (i.e., power) of the transform of f(x)
  TH1* h1dMagRaw = NULL;
  h1dMagRaw = hist1d->FFT(h1dMagRaw, "MAG"); // this has units of 1/f_max
  cout << "Int  = " << h1dMagRaw->Integral() << endl;
  TH1D *h1dMag = new TH1D(FFTname.c_str(), "Magnitude (i.e., Power Spectrum)", h1dMagRaw->GetNbinsX(), 0, h1dMagRaw->GetXaxis()->GetXmax()/hist1d->GetXaxis()->GetXmax()); 
  if ( name.Contains("Turn") && !name.Contains("TimeOncePerTurn")) {
    h1dMag->SetXTitle("Frequency [MHz]");
  }
  else {
    h1dMag->SetXTitle("Frequency [MHz]");
  }



  // rescale axis to get real units
  double freq_min = 0.0;
  double freq_max = 0.0;
  for (Int_t bin = 1; bin <= nSamples; bin++){
    h1dMag->SetBinContent(bin, h1dMagRaw->GetBinContent(bin));
  }
  
  cout << "Bins = " << h1dMag->GetNbinsX() << endl;
  cout << "Xmax = " << h1dMag->GetBinCenter(h1dMag->GetNbinsX()) << endl;
  cout << "Int  = " << h1dMag->Integral() << endl;


  int xmin1 = h1dMag->FindFirstBinAbove(1);
  int xmax1 = h1dMag->FindLastBinAbove(1);

  
  h1dMag->Scale(1/TMath::Sqrt(entries));
  MakePlot1D(h1dMag, -1, -1, &int_prev, &int_curr, &int_start);
  

  
  if ( 0 ) {
    h_wavelength = new TH1D(FFTname.c_str(), "", hist1d->GetNbinsX(), 0.0, hist1d->GetNbinsX()/range);
    //h_wavelength->SetXTitle("Radial Betatron Frequency [1/#lambda_{x}]");
    string magname = FFTname + "_MagX";
    h_mag = (TH1D*)h_FFT_Mag->Clone(magname.c_str());
    string phname = FFTname + "_PhX";
    h_ph = (TH1D*)h_FFT_Ph->Clone(phname.c_str());

    double freq_min = hist1d->GetBinCenter(1);
    double freq_max = hist1d->GetBinCenter(hist1d->GetNbinsX()) + freq_min;
  
    freq_min = 1.0 / freq_max;
    freq_max = 1.0 / freq_min;
  
    for ( int bin = 1; bin < h_FFT_Mag->GetNbinsX(); bin++ ) {
      h_wavelength->SetBinContent(bin, h_FFT_Mag->GetBinContent(bin)/range);
      if ( h_FFT_Mag->GetBinContent(bin) > max ) {
	max = h_FFT_Mag->GetBinContent(bin);
	maxbin = bin;
      }

      //double binval = h_FFT_Mag->GetBinCenter(bin);
      //cout << binval / h_FFT_Mag->GetNbinsX() << "\t" << h_FFT_Mag->GetBinContent(bin) << endl;
    }
  
    string freqname;
    freqname = FFTname + "_Freq";
    TH1D *h_frequency = new TH1D(freqname.c_str(), "", hist1d->GetNbinsX(), freq_min, freq_max);
    for ( int bin = 1; bin < h_wavelength->GetNbinsX(); bin++ ) {
      double bincenter = h_wavelength->GetBinCenter(bin);
      double binval    = h_wavelength->GetBinContent(bin);
      int binfreq = h_frequency->FindBin(1/bincenter);
      h_frequency->SetBinContent(binfreq, binval);
    }

    // 	if ( wavelength ) {
    // 	  wavelength->AddBinContent(1, wavelength->GetBinContent(0));
    // 	  wavelength->AddBinContent(wavelength->GetNbinsX()-1, wavelength->GetBinContent(wavelength->GetNbinsX()));
    // 	}
  
    cout << "Max Freq: " << h_wavelength->GetMaximum() << endl;
    cout << "Max Frequency @ " << h_FFT_Mag->GetBinCenter(maxbin)/range << endl;
    cout << "1/Max Freq is " << 1.0/(h_FFT_Mag->GetBinCenter(maxbin)/range) << endl;
    cout << "Wavelength is " << TMath::TwoPi() / (h_FFT_Mag->GetBinCenter(maxbin)/range) << endl;  

    MakePlot1D(h_wavelength, -1, -1, &int_prev, &int_curr, &int_start);      
    MakePlot1D(h_frequency, -1, -1, &int_prev, &int_curr, &int_start);       
    MakePlot1D(h_mag, -1, -1, &int_prev, &int_curr, &int_start); 
    MakePlot1D(h_ph, -1, -1, &int_prev, &int_curr, &int_start);
  }
  cout << "=====================================" << endl;
  cout << endl;
}

void MakePlot(TH2D *hist, int r, int i, double *int_prev, double *int_curr, double *int_start)
{
  if ( update && hist == NULL ) { return; }
  if ( hist == NULL ) { return; }
  if ( hist->GetEntries() <= 0 ) { cout << "Histogram [" << hist->GetName() << "] is empty" << endl; return; }
  if ( hist->Integral() <= 0.0 ) { cout << "Histogram [" << hist->GetName() << "] is empty" << endl; return; }
  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  
  double xmin, xmax;
  double ymin, ymax;

  double histymax = 0.0;
  double histymin = 0.0;
  double histxmax = 0.0;
  double histxmin = 0.0;


  TString hname = hist->GetName();

  double thresh = 0.02;

  cout << "Getting xmin/max" << endl;

  //
  // Get HistXmin/max
  //
  if ( !hname.Contains("_vs_Time") ) {
    TH1D *xprof = (TH1D*)hist->ProjectionX();
    double intx = xprof->Integral();
    int nbinsx = xprof->GetNbinsX();
    for ( int bin = 1; bin < nbinsx; bin++ ) {
      double intx_tmp = (double)xprof->Integral(bin, nbinsx);
      //cout.precision(4);
      //cout << bin << "\t" <<  xprof->GetNbinsX() << "\t" << intx << "\t" << intx_tmp << "\t" << (double)intx_tmp / intx << endl;
      if ( (double)intx_tmp / intx < 1 - thresh ) {
	histxmin = xprof->GetBinCenter(bin);
	break;
      }
    }
    //cout << histxmin << endl;
    for ( int bin = xprof->GetNbinsX()-1; bin >= 1; bin-- ) {
      if ( xprof->Integral(bin, xprof->GetNbinsX()) / intx > thresh ) {
	histxmax = xprof->GetBinCenter(bin);
	break;
      }
    }
  }
  else {
    TH1D *xprof = (TH1D*)hist->ProjectionX();
    histxmin = xprof->GetBinLowEdge(1);
    histxmax = xprof->GetBinLowEdge(xprof->GetNbinsX())+xprof->GetBinWidth(1);
  }

  TH1D *yprof = (TH1D*)hist->ProjectionY();
  double inty = yprof->Integral();
  cout << yprof->GetNbinsX() << endl;
  histymin = yprof->GetBinLowEdge(yprof->FindFirstBinAbove(thresh*inty));
  histymax = yprof->GetBinLowEdge(yprof->FindLastBinAbove(thresh*inty)+1);

  cout << "Getting xmin/max - Done" << endl;


  if ( hname.Contains("NumCounter") == false ) {
    histymax *= 1.0;
    histymin *= 1.0;
  }
  if ( hname.Contains("Num") == false ) {
    histxmax *= 1.0;
    histxmin *= 1.0;
  }

  GetXminmax(hist, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax);



  TString xtitle = hist->GetXaxis()->GetTitle();
  TString ytitle = hist->GetYaxis()->GetTitle();
  
  cout << "==========" << endl;
  cout << "Hist2D Name: " << hname << "\t\tX/Y" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\tX/Y" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
  cout << "==========" << endl;
  
  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.1);
  h->GetXaxis()->SetTitleOffset(1.1);
  h->Draw();

  double binstep = (double)h->GetNbinsX() / hist->GetNbinsX();


  //if ( r != -1 ) { hist->Rebin2D(2,2); }

  
  //hist->Draw("SAME,CONT4Z");
  hist->Draw("SAME,COLZ");

  stringstream ss;


  double eff = 1.0;
  double abseff = 1.0;

  eff = *int_curr / *int_prev;
  abseff = *int_curr / upstream_inflector_yield;
  
  double m = *int_curr;
  double N = upstream_inflector_yield;
  double abserr = TMath::Sqrt(m*(1 - m/N)/N);
  abserr = TMath::Sqrt(abseff*(1-abseff)/N);
  abseff *= 100;
  abserr *= 100;


  
  stringstream ss_eff;
  //ss_eff.precision(2);
  ss_eff << "N_{sel} = " << m;
  stringstream ss_abseff;
  if ( abseff < 1 ) {
    ss_abseff.precision(1);
  }
  else {
    ss_abseff.precision(2);
  }


  //cout << hist->GetEntries() << "\t" << Ngen << endl;
  geteff(hist->GetEntries(), upstream_inflector_yield, &abseff, &abserr);
  ss_abseff << SurviveText(abseff, abserr, "");
  
  


  TString hname = hist->GetName();
  if ( hname.Contains("G4") == false ) {
    if ( hname.Contains("Tracker_Rhat_vs_Time") ||
	 hname.Contains("Tracker_Y_vs_Time")  ||
	 hname.Contains("Tracker_Prhat_vs_Time") ||
	 hname.Contains("Tracker_Pvhat_vs_Time")  ||
	 hname.Contains("Tracker_Mom_vs_Time") ||
	 hname.Contains("Tracker_Rhat_vs_Turn") ||
	 hname.Contains("Tracker_Y_vs_Turn")  ||
	 hname.Contains("Tracker_Prhat_vs_Turn") ||
	 hname.Contains("Tracker_Pvhat_vs_Turn")  ||
	 hname.Contains("Tracker_Mom_vs_Turn") ) {
      TH1D *xdist = (TH1D*)hist->ProjectionX();
      int bin0, bin05, bin1, bin5, bin10, bin50, bin100, bin200, bin500, bin1000;
      if ( hname.Contains("Turn") ) {
	bin0 = xdist->FindBin(0);
	bin05 = xdist->FindBin(0.5);
	bin1 = xdist->FindBin(1);
	bin5 = xdist->FindBin(5);
	bin10 = xdist->FindBin(10-1);
	bin50 = xdist->FindBin(50);
	bin100 = xdist->FindBin(100);
	bin200 = xdist->FindBin(200-1);
	bin500 = xdist->FindBin(500-1);
	bin1000 = xdist->FindBin(1000-1);
      }
      else {
	bin0 = xdist->FindBin(0);
	bin05 = xdist->FindBin(0.5);
	bin1 = xdist->FindBin(1);
	bin5 = xdist->FindBin(5);
	bin10 = xdist->FindBin(10-1);
	bin50 = xdist->FindBin(50);
	bin100 = xdist->FindBin(100);
	bin200 = xdist->FindBin(200-1);
	bin500 = xdist->FindBin(500-1);
	bin1000 = xdist->FindBin(1000-1);
      }
      double binval0, binval05, binval1, binval5, binval10, binval50, binval100, binval200, binval500, binval1000;
      binval0 = xdist->GetBinContent(bin0);
      binval05 = xdist->GetBinContent(bin05);
      binval1 = xdist->GetBinContent(bin1);
      binval5 = xdist->GetBinContent(bin5);
      binval10 = xdist->GetBinContent(bin10);
      binval50 = xdist->GetBinContent(bin50);
      binval100 = xdist->GetBinContent(bin100);
      binval200 = xdist->GetBinContent(bin200);
      binval500 = xdist->GetBinContent(bin500);
      binval1000 = xdist->GetBinContent(bin1000);

      if ( maxturnsreal < 50 ) { binval50 = 0.0; binval100 = 0.0; binval200 = 0.0; binval500 = 0.0; binval1000 = 0.0; }
      if ( maxturnsreal < 100 ) { binval100 = 0.0; binval200 = 0.0; binval500 = 0.0; binval1000 = 0.0; }
      if ( maxturnsreal < 200 ) { binval200 = 0.0; binval500 = 0.0; binval1000 = 0.0; }
      if ( maxturnsreal < 500 ) { binval500 = 0.0; binval1000 = 0.0; }
      if ( maxturnsreal < 1000 ) { binval1000 = 0.0; }

      //cout << "[US,0,0.5,1,5,10,50,100,200,500,1000] = [" << upstream_inflector_yield << " , " << binval0 << " , " << binval05 << " , " << binval1 << " , " << binval5 << " , " << binval10 << " , " << binval50 << " , " << binval100 << " , " << binval200 << " , " << binval500 << " , " << binval1000 << "]" << endl;
    
      //upstream_inflector_yield = binval0;

      ss.str("");
      double abseff0 = binval0 /  upstream_inflector_yield;
      double abserr0 = TMath::Sqrt(abseff0*(1-abseff0)/upstream_inflector_yield);
      abseff0 *= 100.0; abserr0 *= 100.0;

      double abseff05 = binval05 /  upstream_inflector_yield;
      double abserr05 = TMath::Sqrt(abseff05*(1-abseff05)/upstream_inflector_yield);
      abseff05 *= 100.0; abserr05 *= 100.0;

      double abseff1 = binval1 /  upstream_inflector_yield;
      double abserr1 = TMath::Sqrt(abseff1*(1-abseff1)/upstream_inflector_yield);
      abseff1 *= 100.0; abserr1 *= 100.0;

      double abseff5 = binval5 /  upstream_inflector_yield;
      double abserr5 = TMath::Sqrt(abseff5*(1-abseff5)/upstream_inflector_yield);
      abseff5 *= 100.0; abserr5 *= 100.0;

      double abseff10 = binval10 /  upstream_inflector_yield;
      double abserr10 = TMath::Sqrt(abseff10*(1-abseff10)/upstream_inflector_yield);
      abseff10 *= 100.0; abserr10 *= 100.0;

      double abseff100 = binval100 /  upstream_inflector_yield;
      double abserr100 = TMath::Sqrt(abseff100*(1-abseff100)/upstream_inflector_yield);
      abseff100 *= 100.0; abserr100 *= 100.0;

      double abseff200 = binval200 /  upstream_inflector_yield;
      double abserr200 = TMath::Sqrt(abseff200*(1-abseff200)/upstream_inflector_yield);
      abseff200 *= 100.0; abserr200 *= 100.0;

      double abseff500 = binval500 /  upstream_inflector_yield;
      double abserr500 = TMath::Sqrt(abseff500*(1-abseff500)/upstream_inflector_yield);
      abseff500 *= 100.0; abserr500 *= 100.0;

      double abseff1000 = binval1000 /  upstream_inflector_yield;
      double abserr1000 = TMath::Sqrt(abseff1000*(1-abseff1000)/upstream_inflector_yield);
      abseff1000 *= 100.0; abserr1000 *= 100.0;

      abseff1 = eff_turn1;
      abserr1 = err_turn1;
      abseff10 = eff_turn10;
      abserr10 = err_turn10;
      abseff100 = eff_turn100;
      abserr100 = err_turn100;

      eff_turn_val[0] = abseff1;
      err_turn_val[0] = abserr1;
      eff_turn_val[1] = abseff10;
      err_turn_val[1] = abserr10;
      eff_turn_val[2] = abseff100;
      err_turn_val[2] = abserr100;

      eff_turn_val[0] = eff_turn1;
      eff_turn_val[1] = eff_turn10;
      eff_turn_val[2] = eff_turn100;
      err_turn_val[0] = err_turn1;
      err_turn_val[1] = err_turn10;
      err_turn_val[2] = err_turn100;
    
      stringstream ss_storage;
      if ( maxturnsreal >= 1000 ) {
	ss_storage.precision(2);
	ss_storage << "f^{sur}_{100,500,1000} = [";
	if ( abseff100 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff100 << "#pm";
	  if ( abserr100/abseff100 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr100;
	  }
	}

	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff500 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff500 << "#pm";
	  if ( abserr500/abseff500 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr500;
	  }
	}
	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff1000 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff1000 << "#pm";
	  if ( abserr1000/abseff1000 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr1000;
	  }
	}
	ss_storage << " ](%)";
      }
      else if ( maxturnsreal > 100 ) {
	stringstream ss_storage;
	ss_storage.precision(2);
	ss_storage << "f^{sur}_{1,10,100} = [";
	if ( abseff1 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff1 << "#pm";
	  if ( abserr1/abseff1 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr1;
	  }
	}

	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff10 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff10 << "#pm";
	  if ( abserr10/abseff10 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr10;
	  }
	}
	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff100 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff100 << "#pm";
	  if ( abserr100/abseff100 < 0.05 ) { ss_storage << "#approx0"; }
	  else {
	    ss_storage.precision(1);
	    ss_storage << abserr100;
	  }
	}
	ss_storage << " ](%)";
	if ( (ss_eff_turn.str()).size() == 0 ) {
	  ss_eff_turn << ss_storage.str();
	  //cout << "INFLECTOR!!! : " << number_start_in_inflector << "\t" << number_start_in_ring << endl;
	  if ( number_start_in_inflector > 1 &&
	       number_start_in_ring > 1 ) {
	    double abs_eff_inf = (double)number_start_in_ring / number_start_in_inflector;
	    double abs_err_inf = TMath::Sqrt(abs_eff_inf*(1-abs_eff_inf)/number_start_in_inflector);
	    abs_eff_inf *= 100.0; abs_err_inf *= 100.0;

	    eff_inf_val = abs_eff_inf;
	    err_inf_val = abs_err_inf;

	    //cout << "\t" << abs_eff_inf << "\t" << abs_err_inf << endl;
	    if ( (ss_eff_inf.str()).size() == 0 ) {
	      ss_eff_inf.precision(2);
	      ss_eff_inf << "f^{sur}_{inf} = ";
	      if ( abs_eff_inf < 0.01 ) { ss_eff_inf << "<0.01"; }
	      else { 
		ss_eff_inf << abseff10 << "#pm";
		if ( abs_err_inf/abs_eff_inf < 0.05 ) { ss_eff_inf << "#approx0"; }
		else {
		  ss_eff_inf.precision(1);
		  ss_eff_inf << abs_err_inf;
		}
	      }
	      ss_eff_inf << " (%)";
	    }
	  }
	}
      }
      else if ( maxturnsreal >= 10 ) {

	stringstream ss_storage;
	ss_storage.precision(2);
	ss_storage << "f^{sur}_{1/2,1,5,10} = [";
	if ( abseff05 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff05 << "#pm";
	  ss_storage.precision(1);
	  ss_storage << abserr05; }
	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff1 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff1 << "#pm";
	  ss_storage.precision(1);
	  ss_storage << abserr1; }
	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff5 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff5 << "#pm";
	  ss_storage.precision(1);
	  ss_storage << abserr5; }
	ss_storage << " , ";
	ss_storage.precision(2);
	if ( abseff10 < 0.01 ) { ss_storage << "<0.01"; }
	else { 
	  ss_storage << abseff10 << "#pm";
	  ss_storage.precision(1);
	  ss_storage << abserr10; }

	ss_storage << " ](%)";
      }
    

    
      myText(0.3, 0.88, 1, ss_storage.str().c_str(), 0.035);
    }
    //cout << "R=" << r << "\tI=" << i << "\tEntries=" << hist->GetEntries() << "\t" << *int_curr << "\t" << *int_prev << "\t" << *int_start << endl;
    //if ( *int_prev > 0 ) { eff = *int_curr / *int_prev; }
    //if ( *int_start > 0 ) { abseff = *int_curr / *int_start; }
    //*int_prev = *int_curr;
  
    stringstream ss_eff;
    //ss_eff.precision(2);
    ss_eff << "N_{sel} = " << m;
    stringstream ss_abseff;
    if ( abseff < 1 ) {
      ss_abseff.precision(1);
    }
    else {
      ss_abseff.precision(2);
    }


    geteff(hist->GetEntries(), upstream_inflector_yield, &abseff, &abserr);
    ss_abseff << SurviveText(abseff, abserr, "");
 

    c1->RedrawAxis();
    if ( ss.str().size() > 0 ) {
      if ( r == -1 ) {
	if ( i != -1 ) {
	  //cout << "UP" << endl;
	  myText(0.2, 0.88, 1, ss.str().c_str(), 0.05);
	  //myText(0.60, 0.88, 1, ss_eff.str().c_str(), 0.05);
	  myText(0.60, 0.88, 1, ss_abseff.str().c_str(), 0.05);
	}
      }
      else {
	//cout << "DWN" << endl;
	myText(0.3, 0.85, 1, ss.str().c_str(), 0.04);
	//myText(0.5, 0.85, 1, ss_eff.str().c_str(), 0.04);
	myText(0.5, 0.85, 1, ss_abseff.str().c_str(), 0.04);
      }
    }
    else if ( plotringeff ) {
      myText(0.2, 0.88, 1, ss_eff_turn.str().c_str(), 0.04);    
      myText(0.2, 0.80, 1, ss_eff_inf.str().c_str(), 0.04);    
    }
    else {
      cout << "Not name for hist name: " << hname << endl;
    }
  }
    
  gPad->SetRightMargin( 0.15 );
  TPaletteAxis* palette
    = dynamic_cast<TPaletteAxis*>( hist->GetListOfFunctions()->FindObject( "palette" ) );
  if( palette ) {
    palette->SetX1NDC( 0.86 ); // Start the palette 86 % of the way across the image
    palette->SetX1NDC( 0.91 ); // End the palette 91% of the way across the image
    gPad->Modified(); // Update with the new position
  }
  
  if ( hname.Contains("_XZ") ) {
    double pi = TMath::Pi();
    double rad = pi/180.0;

    // Draw Thetas
    double R = 750;
    TLine *l = new TLine(0.0, 0.0, R*TMath::Sin(45*rad), R*TMath::Cos(45*rad));
    l->SetLineColor(kOrange+6);
    l->SetLineWidth(2);
    l->Draw("SAME");
    myText(0.57, 0.59, kOrange+6, "#theta=#frac{#pi}{4}", 0.035);
    TLine *l = new TLine(0.0, 0.0, R*TMath::Sin(-45*rad), R*TMath::Cos(-45*rad));
    l->SetLineColor(kOrange+6);
    l->SetLineWidth(2);
    l->Draw("SAME");
    myText(0.40, 0.59, kOrange+6, "#theta=-#frac{#pi}{4}", 0.035);


    
    //
    // Draw Kicker Plates
    double deg_Kicker[3][2];
    deg_Kicker[0][0] = 60.3;
    deg_Kicker[0][1] = 74.4;
    deg_Kicker[1][0] = 75.2;
    deg_Kicker[1][1] = 89.3;
    deg_Kicker[2][0] = 90.2;
    deg_Kicker[2][1] = 104.3;
    double R[2];
    R[0] = 7112 - 4*45.0;
    R[1] = 7112 + 8*45.0;
    double pi = TMath::Pi();
    double rad = pi/180.0;

    for ( int j = 0; j < 2; j++ ) {
      for ( int i = 0; i < 3; i++ ) {
	double X1 = R[j] * TMath::Sin(deg_Kicker[i][0]*rad);
	double Y1 = R[j] * TMath::Cos(deg_Kicker[i][0]*rad);
	double X2 = R[j] * TMath::Sin(deg_Kicker[i][1]*rad);
	double Y2 = R[j] * TMath::Cos(deg_Kicker[i][1]*rad);
	TLine *lKicker = new TLine(X1, Y1, X2, Y2);
	lKicker->SetLineColor(kBlack);
	lKicker->SetLineWidth(2);
	lKicker->Draw("SAME");
      }
    }

    //
    // Draw Quad Plates
    double deg_Quad[8][2];
    deg_Quad[0][0] = 16.38;
    deg_Quad[0][1] = 28.73;
    deg_Quad[1][0] = 33.5;
    deg_Quad[1][1] = 60.2;
    for ( int i = 2; i < 8; i++ ) {
      for ( int j = 0; j < 2; j++ ) {
	deg_Quad[i][j] = deg_Quad[i-2][j] + 90.0;
      }
    }


    for ( int j = 0; j < 2; j++ ) {
      for ( int i = 0; i < 8; i++ ) {
	double X1 = R[j] * TMath::Sin(deg_Quad[i][0]*rad);
	double Y1 = R[j] * TMath::Cos(deg_Quad[i][0]*rad);
	double X2 = R[j] * TMath::Sin(deg_Quad[i][1]*rad);
	double Y2 = R[j] * TMath::Cos(deg_Quad[i][1]*rad);
	TLine *lQuad = new TLine(X1, Y1, X2, Y2);
	lQuad->SetLineColor(kMagenta);
	lQuad->SetLineWidth(2);
	lQuad->Draw("SAME");
      }
    }

    if ( 0 ) {
      TVector2 axis(0.0, ymax*0.5);
      TLine *l = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l->SetLineColor(kGray+2);
      l->SetLineWidth(2);
      l->Draw("SAME");
    
      axis.Set(xmax*0.5, 0.0);
      TLine *l2 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l2->SetLineColor(kGray+2);
      l2->SetLineWidth(2);
      l2->Draw("SAME");    

      axis.Set(0.0, ymin*0.5);
      TLine *l3 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l3->SetLineColor(kGray+2);
      l3->SetLineWidth(2);
      l3->Draw("SAME");    

      axis.Set(xmin*0.5, 0.0);
      TLine *l4 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l4->SetLineColor(kGray+2);
      l4->SetLineWidth(2);
      l4->Draw("SAME");

      axis.Set(xmax*0.5/TMath::Sqrt(2), ymax*0.5/TMath::Sqrt(2));
      TLine *l5 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l5->SetLineColor(kGray+2);
      l5->SetLineWidth(2);
      l5->Draw("SAME");   

      axis.Set(xmax*0.5/TMath::Sqrt(2), ymin*0.5/TMath::Sqrt(2));
      TLine *l6 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l6->SetLineColor(kGray+2);
      l6->SetLineWidth(2);
      l6->Draw("SAME");  

      axis.Set(xmin*0.5/TMath::Sqrt(2), ymin*0.5/TMath::Sqrt(2));
      TLine *l7 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l7->SetLineColor(kGray+2);
      l7->SetLineWidth(2);
      l7->Draw("SAME");

      axis.Set(xmin*0.5/TMath::Sqrt(2), ymax*0.5/TMath::Sqrt(2));
      TLine *l8 = new TLine(0.0, 0.0, axis.X(), axis.Y());
      l8->SetLineColor(kGray+2);
      l8->SetLineWidth(2);
      l8->Draw("SAME");
    }
  }

  //int result = hist->Fit("[0]*sin([1]*x)", "R");
  //cout << result << endl;
  
  stringstream c1name, pngname, epsname;
  if ( zoom ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName() << "_Zoom";
  }
  else if ( zoom2 ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName() << "_Zoom2";
  }
  else if ( zoom3 ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName() << "_Zoom3";
  }
  else {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName();
  }


  stringstream effname;
  effname << "/afs/fnal.gov/files/home/room3/tgadfort/public_html/plots/" << dir << "/" << "StorageEfficiency.dat";


  if ( ( hname.Contains("20us") == false ) && ( hname.Contains("50us") == false ) && hname.Contains("XprimeYprime") ) {
    //cout << "MAKING " << effname.str() << endl;
    ofstream effout;
    effout.open(effname.str().c_str());
    if ( effout.is_open() ) {
      effout << dir << endl;
      effout << 1 << "\t" << eff_turn_val[0] << " +/- " << err_turn_val[0] << endl;
      effout << 10 << "\t" << eff_turn_val[1] << " +/- " << err_turn_val[1] << endl;
      effout << 100 << "\t" << eff_turn_val[2] << " +/- " << err_turn_val[2] << endl;
      effout << -1 << "\t" << eff_inf_val << " +/- " << err_inf_val << endl;
      effout.close();
    }
    else {
      cout << "COULD NOT OPEN IT!!!" << endl;
    }
  }
  
  cout << "Saving." << endl;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  cout << "Done" << endl;
  delete c1;
}


void MakePlot1D(TProfile *profile, int r, int i, double *int_prev, double *int_curr, double *int_start)
{
  MakePlot1D((TH1D*)profile, r, i, int_prev, int_curr, int_start);
}

void MakePlot1D(TH1D *hist1d, int r, int i, double *int_prev, double *int_curr, double *int_start)
{
  if ( update && hist1d == NULL ) { return; }
  if ( hist1d == NULL ) { return; }

  TString hname = hist1d->GetName();
  if ( hist1d->GetEntries() <= 0 ) { cout << "Histogram [" << hist1d->GetName() << "] is empty" << endl; return; }
  if ( fabs(hist1d->Integral()) <= 0.0 ) { cout << "Histogram [" << hist1d->GetName() << "] is empty" << endl; return; }

  if ( hname.Contains("_Nhits") ) {
    if ( hist1d->Integral(hist1d->FindBin(1), hist1d->FindBin(100)) <= 0 ) { return; }
  }
  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  
  double xmin, xmax;
  double ymin, ymax;
  double histxmin, histxmax;
  double histymin, histymax;
  histxmin = 99999.9;
  histxmax = -99999.9;
  double histx90min, histx90max;
  histx90min = 99999.9;
  histx90max = -99999.9;
  
  
  bool dologx = false;
  bool dolog = false;
  bool dofft = false;


  RebinHist(hist1d);
  if ( hname.Contains("Num") && hname.Contains("LostMuon") ) { dolog = true; }
  if ( hname.Contains("FFT") ) { dofft = true; dolog = true; } // dologx = true; }
  
  //
  // Get HistXmin/max
  //
  histymax = hist1d->GetMaximum();
  histymin = hist1d->GetMinimum();  
  histxmin = hist1d->GetBinLowEdge(hist1d->FindFirstBinAbove(0.1));
  histxmax = hist1d->GetBinLowEdge(hist1d->FindLastBinAbove(0.1)+1);
  if ( hname.Contains("FFT") ) {
    histxmin = hist1d->GetBinLowEdge(hist1d->FindFirstBinAbove(0));
    histxmax = hist1d->GetBinLowEdge(hist1d->FindLastBinAbove(0)+1);

    if ( histxmax > 10 ) { histxmax = 10.0; }
  }


  if ( hname.Contains("Profile") ) {
    histymin = 999.9;
    histymax = -99.9;
    for ( int bin = hist1d->FindBin(histxmin); bin < hist1d->FindBin(histxmax); bin++ ) {
      double cont = hist1d->GetBinContent(bin) + hist1d->GetBinError(bin);
      if ( cont > histymax ) { histymax = cont; }
      if ( cont < histymin ) { histymin = cont; }
    }
  }
  
  
  if ( ( maxtime_for_plot > 0 || mintime_for_plot > 0) && dofft == false ) {
    histymax = 0.0;
    histymin = 9999.0;
    histxmin = mintime_for_plot;
    histxmax = maxtime_for_plot;

    int sbin = hist1d->FindBin(mintime_for_plot)-1;
    int ebin = hist1d->FindBin(maxtime_for_plot);
    if ( fabs(hist1d->Integral(sbin, ebin)) <= 0.0 ) { return; }
    for ( int bin = hist1d->FindBin(mintime_for_plot); bin < hist1d->FindBin(maxtime_for_plot); bin++ ) {
      double cont = hist1d->GetBinContent(bin) + hist1d->GetBinError(bin);
      if ( cont > histymax ) { histymax = cont; }
      if ( cont < histymin ) { histymin = cont; }
    }
    if ( histymax == histymin ) { return; }
//     cout << "HIST1D(x): " << histxmin << " < x < " << histxmax << endl;
//     cout << "HIST1D(y): " << histymin << " < x < " << histymax << endl;
  }

  

//   if ( hname.Contains("_Num") == false ) {
//     histxmax *= 1.25;
//     histxmin *= 1.25;
//   }
  
  if ( 0 ) {
    cout << "::PreGetXminmax::" << endl;
    cout << "HIST1D(x): " << histxmin << " < x < " << histxmax << endl;
    cout << "HIST1D(y): " << histymin << " < x < " << histymax << endl;
  }
  GetXminmax(hist1d, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax, dolog);
  if ( 0 ) {
    cout << "::PostGetXminmax::" << endl;
    cout << "HIST1D(x)': " << xmin << " < x < " << xmax << endl;
    cout << "HIST1D(y)': " << ymin << " < x < " << ymax << endl;
  }

//   if ( hname.Contains("FFT") ) { 
//     int histxmin = hist1d->GetBinLowEdge(hist1d->FindFirstBinAbove(0.1));
//     histxmax = hist1d->GetBinLowEdge(hist1d->FindLastBinAbove(0.1)+1);
//     int sbin = hist1d->FindBin(mintime_for_plot);
//     int ebin = hist1d->FindBin(maxtime_for_plot);
//   }

    
  
  TString xtitle = hist1d->GetXaxis()->GetTitle();
  TString ytitle = hist1d->GetYaxis()->GetTitle();
  
  
  cout << "==========" << endl;
  cout << "Hist1D Name: " << hname << "\t\tX/Y [[ " << xmin << "/" << xmax << " ]]\t\t[[ " << ymin << "/" << ymax << " ]]\t<------ Using These." << endl;
  cout << "             " << hname << "\t\tX/Y " << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << "." << endl; 
  cout << "==========" << endl;



  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.1);
  h->GetXaxis()->SetTitleOffset(1.1);
  h->Draw();
  double binstep = (double)h->GetNbinsX() / hist1d->GetNbinsX();
  //cout << binstep << endl;




  double yields[5];
  string ynames[5];
  for ( int y = 0; y < 5; y++ ) { yields[y] = 0.0; }

  hist1d->SetLineColor(kBlue);
  hist1d->SetLineWidth(GetLineWidth(hname));
  hist1d->SetMarkerSize(GetMarkerSize(hname));
  hist1d->SetMarkerColor(kBlue);
  if ( hname.Contains("out_") || hname.Contains("1D") || hname.Contains("Wavelength") ) {
    if ( 0 ) {
      hist1d->SetMarkerSize(0.75);
      hist1d->SetLineWidth(1); 
      hist1d->SetLineColor(kAzure+6);
      hist1d->SetMarkerColor(kAzure+6);
    }
  }

  double binw = hist1d->GetBinWidth(1);
  stringstream yt;
  yt.precision(2);
  if ( binw >= 1 ) {
    yt << "Events / " << binw;
  }
  else {
    if ( binw == 0.5 ) { yt << "Events / #frac{1}{2}"; }
    else if ( binw == 0.1 ) { yt << "Events / 10^{-1}"; }
    else if ( binw == 0.01 ) { yt << "Events / 10^{-2}"; }
    else if ( binw == 0.001 ) { yt << "Events / 10^{-3}"; }
    else if ( binw == 0.2 ) { yt << "Events / #frac{1}{5}"; }
    else if ( binw == 0.25 ) { yt << "Events / #frac{1}{4}"; }
    else { yt << "Events / " << binw; }
  }

  if ( hname.Contains("_Mom") ) { yt << " P/P_{m}"; }
  else if ( hname.Contains("Wavelength") ) { yt << " #lambda^{-1}"; }
  else if ( hname.Contains("FFT_Freq") ) { yt << " Turns"; }
  else if ( hname.Contains("FFT") ) { yt << " Turns^{-1}"; }
  else if ( hname.Contains("NgtEth") ||
	    hname.Contains("NwghtE") ||
	    hname.Contains("Nud") ||
	    hname.Contains("Nup") ||
	    hname.Contains("Ndown") ||
	    hname.Contains("Num") ) { 
    if ( hname.Contains("_vs_Time") ) { yt << " #mus"; }
    else { yt << " Turns"; }
  }
  else if ( hname.Contains("TrackVhat") ) { yt << " mm"; }
  else if ( hname.Contains("TrackYprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Rhat") || hname.Contains("Vhat") ) { yt << " mm"; }
  else if ( hname.Contains("Y2") ) { yt << " mm^{2}"; }
  else if ( hname.Contains("Turn") ) { yt << " 30 degrees"; }
  else if ( hname.Contains("Kick") ) { yt << " mrad"; }
  else if ( hname.Contains("Px") || hname.Contains("Py") ) { yt << " MeV"; }
  else if ( hname.Contains("Xprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Yprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Degree") ) { yt << " ^{#circ}"; }
  else if ( hname.Contains("Xe") || hname.Contains("EqRad") || hname.Contains("Vhat") ) { yt << " mm"; }
  else if ( hname.Contains("Pol") || hname.Contains("PolX")|| hname.Contains("PolY") ) { yt << ""; }  
  else if ( hname.Contains("Time") ) { yt << " #mus"; }
  else { yt << " [Unit]"; }


  if ( hname.Contains("_Ratio") ) {
    yt.str("");
    yt << "Calo Acceptance/Efficiency";
  }
	
	
  if ( hname.Contains("Y2") ) { 
    yt.str("");
    yt << " <y^{2}> / " << binw << " mm^{2}";
  }
  if ( hname.Contains("TrackVhat") ) { 
    yt.str("");
    yt << " <y> / " << binw << " Turns";
  }
  if ( hname.Contains("TrackYprime") ) { 
    yt.str("");
    yt << "<y'> / " << binw << " Turns";
  }
  if ( hname.Contains("Nud") ) { 
    yt.str("");
    yt << "#sum[N_{u}-N_{d}] / " << binw << " Turns";
  }
  if ( hname.Contains("FFT") ) {
    yt.str("");
    yt << "Arbitrary Units";
  }
  
  h->SetYTitle(yt.str().c_str());


  if ( hname.Contains("Wavelength") || hname.Contains("FFT") || hname.Contains("Num") ) {
    hist1d->Draw("HIST,SAME");
  }
  else {
    hist1d->Draw("E1,HIST,SAME");
  }

  stringstream ss, ss_eff, ss_abseff;

  if ( hname.Contains("FFT") ) {
    double fa = 0.228; // MHz
    double fC = 6.7; // MHz
    double fx = 6.23; // Mhz
    double fy = 2.48; // Mhz
    double fCBO = 0.477; // MHz
    fCBO = 0.466;
    double fVW = 1.74; // Mhz

    int linewidth = 2;
    double ysf = 0.1;

    if ( hname.Contains("Time") || hname.Contains("BeamScan") || hname.Contains("MuonDecay") ) {
      TLine *l_a = new TLine(fa, ymin, fa, ysf*hist1d->GetBinContent(hist1d->FindBin(fa)));
      l_a->SetLineColor(kRed);
      l_a->SetLineWidth(linewidth);
      l_a->Draw("SAME");

      stringstream ss_a;    
      ss_a.precision(3);
      ss_a << "f_{g-2}"; // =" << fa;
      
      TLatex tl_a;
      //tl_a.SetNDC();
      tl_a.SetTextColor(kRed);
      tl_a.DrawLatex(1.05*fa, ymin*1.5, ss_a.str().c_str());


      if ( hname.Contains("Num") || hname.Contains("Rhat") || hname.Contains("Xprime") ) {
	TLine *l_C = new TLine(fC, ymin, fC, ysf*hist1d->GetBinContent(hist1d->FindBin(fC)));
	l_C->SetLineColor(kMagenta);
	l_C->SetLineWidth(linewidth);
	l_C->Draw("SAME");
	
	stringstream ss_C;    
	ss_C.precision(3);
	ss_C << "f_{C}"; // =" << fa;
	
	TLatex tl_C;
	//tl_C.SetNDC();
	tl_C.SetTextColor(kMagenta);
	tl_C.DrawLatex(1.05*fC, ymin*1.5, ss_C.str().c_str());
      }


      if ( hname.Contains("Num") || hname.Contains("Y2") || hname.Contains("_Y_") || hname.Contains("_Yprime_") ) {
	TLine *l_y = new TLine(fVW, ymin, fVW, ysf*hist1d->GetBinContent(hist1d->FindBin(fVW)));
	l_y->SetLineColor(kMagenta);
	l_y->SetLineWidth(linewidth);
	l_y->Draw("SAME");
	
	stringstream ss_y;    
	ss_y.precision(3);
	ss_y << "f_{VW}"; // =" << fa;
	
	TLatex tl_y;
	//tl_y.SetNDC();
	tl_y.SetTextColor(kMagenta);
	tl_y.DrawLatex(1.05*fVW, ymin*1.5, ss_y.str().c_str());

	TLine *l_C = new TLine(fy, ymin, fy, ysf*hist1d->GetBinContent(hist1d->FindBin(fy)));
	l_C->SetLineColor(kMagenta);
	l_C->SetLineWidth(linewidth);
	l_C->Draw("SAME");
	
	stringstream ss_C;    
	ss_C.precision(3);
	ss_C << "f_{y}"; // =" << fa;
	
	TLatex tl_C;
	//tl_C.SetNDC();
	tl_C.SetTextColor(kMagenta);
	tl_C.DrawLatex(1.05*fy, ymin*1.5, ss_C.str().c_str());
      }


      if ( hname.Contains("Num") || hname.Contains("Rhat") || hname.Contains("Xprime") ) {
	TLine *l_CBO = new TLine(fCBO, ymin, fCBO, ysf*hist1d->GetBinContent(hist1d->FindBin(fCBO)));
	l_CBO->SetLineColor(kOrange+6);
	l_CBO->SetLineWidth(2);
	l_CBO->Draw("SAME");
	
	stringstream ss_CBO;    
	ss_CBO.precision(3);
	ss_CBO << "f_{CBO}"; // =" << fa;
	
	TLatex tl_CBO;
	//tl_CBO.SetNDC();
	tl_CBO.SetTextColor(kRed);
	tl_CBO.DrawLatex(1.03*fCBO, ymin*1.5, ss_CBO.str().c_str());
	
	

	TLine *l_CBO2 = new TLine(fCBO*2, ymin, fCBO*2, ysf*hist1d->GetBinContent(hist1d->FindBin(fCBO*2)));
	l_CBO2->SetLineColor(kOrange+6);
	l_CBO2->SetLineWidth(2);
	l_CBO2->Draw("SAME");
	
	stringstream ss_CBO2;    
	ss_CBO2.precision(3);
	ss_CBO2 << "2f_{CBO}"; // =" << fa;
	
	TLatex tl_CBO2;
	//tl_CBO2.SetNDC();
	tl_CBO2.SetTextColor(kRed);
	tl_CBO2.DrawLatex(1.03*fCBO*2, ymin*1.5, ss_CBO2.str().c_str());
      }



      if ( 0 ) {
      double lmax = 0.0;
      for ( int i = 3; i < hist1d->GetNbinsX(); i++ ) {
	if ( hist1d->GetBinContent(i) > lmax ) { lmax = hist1d->GetBinContent(i); }
      }
      
      for ( int i = 3; i < hist1d->GetNbinsX(); i++ ) {
	if ( hist1d->GetBinContent(i) / lmax > 0.1 ) {
	  double f = hist1d->GetBinCenter(i);
	  TLine *l_new = new TLine(f, 0.0, f, ymax*0.25);
	  l_new->SetLineColor(kRed);
	  l_new->SetLineWidth(2);
	  l_new->Draw("SAME");

	  stringstream ss_fft; 
	  ss_fft.precision(2);
	  ss_fft << "f=" << f;
	  
	  TLatex tl_fft;
	  //tl_fft.SetNDC();
	  tl_fft.SetTextColor(kMangeta);
	  tl_fft.DrawLatex(0.95*f, ymax*0.35, ss_fft.str().c_str());
	}
      }
      }
    }
    else {
      TLine *l_gm2 = new TLine(omega_gm2, 0.0, omega_gm2, ymax*0.25);
      l_gm2->SetLineColor(kRed);
      l_gm2->SetLineWidth(2);
      l_gm2->Draw("SAME");

      stringstream ss_fft;    
      ss_fft << "f_{g-2}=#frac{1}{" << lambda_gm2 << "}";
      
      TLatex tl_fft;
      //tl_fft.SetNDC();
      tl_fft.SetTextColor(kRed);
      tl_fft.DrawLatex(0.95*omega_gm2, ymax*0.35, ss_fft.str().c_str());
      //myText(omega_gm2, ymax*0.35, kRed, ss_fft.str().c_str(), 0.055);
    }
  }

  ss.str("");
  TString hname = hist1d->GetName();
  if ( hname.Contains("MuonMomentum") && hname.Contains("G4") ) { ss << "|p_{#mu}| (G4)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("RingEntrance") ) { ss << "|p_{#mu}| (#theta=0)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("90degrees") ) { ss << "|p_{#mu}| (#theta=90)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("180degrees") ) { ss << "|p_{#mu}| (#theta=180)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("270degrees") ) { ss << "|p_{#mu}| (#theta=270)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("Turn2") ) { ss << "|p_{#mu}| (Turn=2)"; }
  if ( hname.Contains("MuonMomentum") && hname.Contains("Loss") ) { ss << "|p_{#mu}| Loss"; }
  if ( hname.Contains("Flange") && hname.Contains("Upstream") ) { ss << "Flange (US)"; }
  if ( hname.Contains("Flange") && hname.Contains("Downstream") ) { ss << "Flange (DS)"; }
  if ( hname.Contains("Window") && hname.Contains("Upstream") ) { ss << "Window (US)"; }
  if ( hname.Contains("Window") && hname.Contains("Downstream") ) { ss << "Window (DS)"; }
  if ( hname.Contains("Cryostat") ) { ss << "Cryostat Wall"; }
  if ( hname.Contains("Quad1") ) { ss << "Quad1"; }
  if ( hname.Contains("Quad2") ) { ss << "Quad2"; }
  if ( hname.Contains("Quad3") ) { ss << "Quad3"; }
  if ( hname.Contains("Quad4") ) { ss << "Quad4"; }
  if ( hname.Contains("Kicker1") ) { ss << "Kicker1"; }
  if ( hname.Contains("Kicker2") ) { ss << "Kicker2"; }
  if ( hname.Contains("Kicker3") ) { ss << "Kicker3"; }
  if ( hname.Contains("Mandrel") && hname.Contains("Upstream") ) { ss << "Mandrel (US)"; }
  if ( hname.Contains("Mandrel") && hname.Contains("Downstream") ) { ss << "Mandrel (DS)"; }
  
  //cout << "ss = " << ss.str() << endl;

  c1->RedrawAxis();
  if ( ss.str().size() > 0 ) {
    if ( r == -1 ) {
      if ( i != -1 ) {
	myText(0.2, 0.85, 1, ss.str().c_str());
	myText(0.65, 0.85, 1, ss_eff.str().c_str());
	myText(0.65, 0.77, 1, ss_abseff.str().c_str());
      }
    }
    else {
      myText(0.3, 0.85, 1, ss.str().c_str());
      myText(0.6, 0.85, 1, ss_eff.str().c_str());
      myText(0.6, 0.75, 1, ss_abseff.str().c_str());
    }
  }

  if ( hname.Contains("NgtEth")  ) {
    ss.str("");
    ss << "#tau_{#mu} = #frac{1}{10}#tau_{0}";
    //myText(0.6, 0.7, 1, ss.str().c_str());
  }

  if ( hname.Contains("Wavelength") || hname.Contains("FFT") ) {
    double maxval = -0.1;
    int maxbin = -1;
    for ( int bin = 1; bin < hist1d->FindBin(xmax); bin++ ) {
      if ( hist1d->GetBinContent(bin) > maxval ) { maxval = hist1d->GetBinContent(bin); maxbin = bin; }
    }
    ss.precision(3);
    ss.str("");
    ss << "#lambda_{max} = " << 1.0/hist1d->GetBinCenter(maxbin) << " Turns";
    //myText(0.5, 0.80, 1, ss.str().c_str());
  }

  if ( hname.Contains("Momentum") ||
       hname.Contains("thetaX") || 
       hname.Contains("thetaY") ) {
    TF1 *f1;

    if ( hname.Contains("Momentum") ) {
      f1 = new TF1("f1", "gaus", 1-0.25, 1+0.25);
      hist1d->Sumw2();
      hist1d->Fit("f1", "RQ");
      hist1d->GetXaxis()->SetRange(hist1d->FindBin(1-0.25), hist1d->FindBin(1+0.25));
    }
    else {
      double max = 0.01;
      if ( start_inflector ) { max = 0.05; }
      f1 = new TF1("f1", "gaus", -max, max);
      hist1d->Sumw2();
      hist1d->Fit("f1", "RQ");
      hist1d->GetXaxis()->SetRange(hist1d->FindBin(-max), hist1d->FindBin(max));
    }

    TVirtualFitter *fitter = TVirtualFitter::GetFitter();
    if(fitter) {
      //cout << fitter->GetNumberFreeParameters() << endl;
      double mean, mean_err;
      double sigma, sigma_err;
      mean = fitter->GetParameter(1);
      mean_err = fitter->GetParError(1);
      sigma = fitter->GetParameter(2);
      sigma_err = fitter->GetParError(2);

      stringstream fit, fit2;
      if ( hname.Contains("Momentum") ) {
	fit.precision(4);
	fit << "p'=" << mean << " MeV"; //<< "#pm" << mean_err;
	fit2.precision(3);
	fit2 << "#sigma=" << sigma << " MeV"; //<< "#pm" << sigma_err;
	myText(0.75, 0.88, 1, fit.str().c_str(), 0.04);
	myText(0.75, 0.80, 1, fit2.str().c_str(), 0.04);
      }
      else {
	fit.precision(3);
	fit << "#theta_{0}=" << 1000*mean << " mrad"; //<< "#pm" << sigma_err;
	myText(0.7, 0.88, 1, fit.str().c_str(), 0.04);
	fit2.precision(3);
	fit2 << "#sigma=" << 1000*sigma << " mrad"; //<< "#pm" << sigma_err;
	myText(0.7, 0.80, 1, fit2.str().c_str(), 0.04);
      }

    }
  }


  if ( hname.Contains("EqRad") || hname.Contains("Xe") ) {
    double mean, rms, skew, kurt;
    mean = hist1d->GetMean();
    rms = hist1d->GetRMS();
    skew = hist1d->GetSkewness();
    kurt = hist1d->GetKurtosis();
    
    stringstream mean_ss, rms_ss, skew_ss, kurt_ss;
    mean_ss.precision(2);
    rms_ss.precision(2);
    skew_ss.precision(2);
    kurt_ss.precision(2);
    mean_ss << "Mean = " << mean;
    rms_ss  << "RMS  = " << rms;
    skew_ss << "Skew = " << skew;
    kurt_ss << "Kurt = " << kurt;

    cout << mean_ss.str() << endl;
    cout << rms_ss.str() << endl;
    
    myText(0.7, 0.85, 1, mean_ss.str().c_str(), 0.035);
    myText(0.7, 0.80, 1, rms_ss.str().c_str(), 0.035);
    myText(0.7, 0.75, 1, skew_ss.str().c_str(), 0.035);
    myText(0.7, 0.70, 1, kurt_ss.str().c_str(), 0.035);
  }

  if ( hname.Contains("Degree") && hname.Contains("Rhat0") ) {
    double deg_K1_min = 60.3;
    double deg_K1_max = 74.4;
    double deg_K2_min = 75.2;
    double deg_K2_max = 89.3;
    double deg_K3_min = 90.2;
    double deg_K3_max = 104.3;
    TLine *lK1_min = new TLine(deg_K1_min, 0.0, deg_K1_min, ymax*0.25);
    lK1_min->SetLineColor(kRed);
    lK1_min->SetLineWidth(2);
    lK1_min->Draw("SAME");

    TLine *lK1_max = new TLine(deg_K1_max, 0.0, deg_K1_max, ymax*0.25);
    lK1_max->SetLineColor(kRed);
    lK1_max->SetLineWidth(2);
    lK1_max->Draw("SAME");

    TLine *lK2_min = new TLine(deg_K2_min, 0.0, deg_K2_min, ymax*0.25);
    lK2_min->SetLineColor(kMagenta);
    lK2_min->SetLineWidth(2);
    lK2_min->Draw("SAME");

    TLine *lK2_max = new TLine(deg_K2_max, 0.0, deg_K2_max, ymax*0.25);
    lK2_max->SetLineColor(kMagenta);
    lK2_max->SetLineWidth(2);
    lK2_max->Draw("SAME");

    TLine *lK3_min = new TLine(deg_K3_min, 0.0, deg_K3_min, ymax*0.25);
    lK3_min->SetLineColor(kOrange-6);
    lK3_min->SetLineWidth(2);
    lK3_min->Draw("SAME");

    TLine *lK3_max = new TLine(deg_K3_max, 0.0, deg_K3_max, ymax*0.25);
    lK3_max->SetLineColor(kOrange-6);
    lK3_max->SetLineWidth(2);
    lK3_max->Draw("SAME");
  }

  if ( hname.Contains("Degree") && hname.Contains("Quad") ) {
    double deg_K1_min = 16.38;
    double deg_K1_max = 28.73;
    double deg_K2_min = 33.5;
    double deg_K2_max = 60.2;
    TLine *lK1_min = new TLine(deg_K1_min, 0.0, deg_K1_min, ymax*0.25);
    lK1_min->SetLineColor(kRed);
    lK1_min->SetLineWidth(2);
    lK1_min->Draw("SAME");

    TLine *lK1_max = new TLine(deg_K1_max, 0.0, deg_K1_max, ymax*0.25);
    lK1_max->SetLineColor(kRed);
    lK1_max->SetLineWidth(2);
    lK1_max->Draw("SAME");

    TLine *lK2_min = new TLine(deg_K2_min, 0.0, deg_K2_min, ymax*0.25);
    lK2_min->SetLineColor(kMagenta);
    lK2_min->SetLineWidth(2);
    lK2_min->Draw("SAME");

    TLine *lK2_max = new TLine(deg_K2_max, 0.0, deg_K2_max, ymax*0.25);
    lK2_max->SetLineColor(kMagenta);
    lK2_max->SetLineWidth(2);
    lK2_max->Draw("SAME");
  }


  if ( hname.Contains("DegreeAt") || hname.Contains("KickAt") ) {
    double mean = hist1d->GetMean();
    double rms = hist1d->GetRMS();
    stringstream avg_rms;
    avg_rms.precision(3);
    if ( hname.Contains("Degree") ) {
      if ( mean < 100.0 ) { avg_rms.precision(2); }
      avg_rms << "<#theta> = " << mean << " #pm ";
      avg_rms.precision(2);
      avg_rms << rms << "#circ";
      
      stringstream momname;
      if ( hname.Contains("Mom0") ) { momname << "#frac{P}{P_{m}}-1 < -0.5%"; }
      if ( hname.Contains("Mom1") ) { momname << "| #frac{P}{P_{m}} - 1 | < 0.5%"; }
      if ( hname.Contains("Mom2") ) { momname << "#frac{P}{P_{m}}-1 > +0.5%"; }
      if ( hname.Contains("Mom3") ) { momname << "#frac{P}{P_{m}}-1 < -0.15%"; }
      if ( hname.Contains("Mom4") ) { momname << "| #frac{P}{P_{m}} - 1 | < 0.15%"; }
      if ( hname.Contains("Mom5") ) { momname << "#frac{P}{P_{m}}-1 > +0.15%"; }
      if ( momname.str().size() > 0 ) {
	myText(0.65, 0.77, kMagenta, momname.str().c_str(), 0.055);
      }
    }
    if ( hname.Contains("Kick") ) {
      avg_rms.precision(3);
      avg_rms << "<x'> = " << mean << " #pm ";
      avg_rms.precision(2);
      avg_rms << rms << " mrad";
      
      stringstream momname;
      if ( hname.Contains("Mom0") ) { momname << "#frac{P}{P_{m}}-1 < -0.5%"; }
      if ( hname.Contains("Mom1") ) { momname << "| #frac{P}{P_{m}} - 1 | < 0.5%"; }
      if ( hname.Contains("Mom2") ) { momname << "#frac{P}{P_{m}}-1 > +0.5%"; }
      if ( hname.Contains("Mom3") ) { momname << "#frac{P}{P_{m}}-1 < -0.15%"; }
      if ( hname.Contains("Mom4") ) { momname << "| #frac{P}{P_{m}} - 1 | < 0.15%"; }
      if ( hname.Contains("Mom5") ) { momname << "#frac{P}{P_{m}}-1 > +0.15%"; }
      if ( momname.str().size() > 0 ) {
	myText(0.65, 0.77, kMagenta, momname.str().c_str(), 0.055);
      }
    }
    myText(0.55, 0.86, kRed, avg_rms.str().c_str(), 0.055);

    if ( 0 ) {
      stringstream toteff;
      toteff << SurviveTextRaw(hist1d->GetEntries(), upstream_inflector_yield, "");
      myText(0.20, 0.86, kRed, toteff.str().c_str(), 0.055);

      stringstream fineff;
      fineff << SurviveText(eff_turn100, err_turn100, "100");
      myText(0.20, 0.79, kPink+6, fineff.str().c_str(), 0.055);
    }
  }
  
  bool plot_avgrms = false;
  if ( hname.Contains("DeltaPx") || hname.Contains("DeltaPy") ) { plot_avgrms = true; }
  if ( hname.Contains("TrackerY") || hname.Contains("TrackerRhat") ) { plot_avgrms = true; }
  if ( hname.Contains("Mom") ) { 
    plot_avgrms = true; 
    if ( hname.Contains("Degree") ) { plot_avgrms = false; }
    if ( hname.Contains("Kick") ) { plot_avgrms = false; }
  }
  if ( hname.Contains("Rhat") ) { 
    plot_avgrms = true; 
    if ( hname.Contains("AtRhat") ) { plot_avgrms = false; }
  }
  if ( hname.Contains("Vhat") ) { 
    plot_avgrms = true; 
    if ( hname.Contains("AtRhat") ) { plot_avgrms = false; }
  }
  if ( hname.Contains("Asym") ) { 
    plot_avgrms = true; 
  }
  if ( hname.Contains("FFT") ) { plot_avgrms = false; }
  if ( hname.Contains("Profile") && hname.Contains("BeamScan") ) { plot_avgrms = false; }
  if ( hname.Contains("Profile") && hname.Contains("MuonDecay") ) { plot_avgrms = false; }
  
  if ( plot_avgrms ) {
    double mean = hist1d->GetMean();
    double rms = hist1d->GetRMS();
    stringstream avg_rms;
    avg_rms.precision(2);
    if ( hname.Contains("Px") ) { avg_rms << "<#DeltaP_{x}>"; }
    if ( hname.Contains("Py") ) { avg_rms << "<#DeltaP_{y}>"; }
    if ( hname.Contains("_Mom") ) { avg_rms << "<#frac{p}{p_{m}}>"; }
    if ( hname.Contains("_Rhat") ) { avg_rms << "<x>"; }
    if ( hname.Contains("_Vhat") ) { avg_rms << "<y>"; }
    if ( hname.Contains("TrackerY") ) { avg_rms << "<y>"; }
    if ( hname.Contains("TrackerRhat") ) { avg_rms << "<x>"; }
    if ( hname.Contains("Nud") ) { avg_rms << "<A>"; }
    if ( hname.Contains("Nup") ) { avg_rms << "<N>"; }
    if ( hname.Contains("Ndown") ) { avg_rms << "<N>"; }
    
    string unit = "Mev";
    if ( hname.Contains("TrackerY") || hname.Contains("TrackerRhat") || hname.Contains("Rhat") || hname.Contains("Vhat") ) {
      unit = "mm";
    }
    if ( hname.Contains("_Mom") ) { unit = ""; }
    if ( hname.Contains("Nud") ) { unit = ""; }
    if ( hname.Contains("Nup") ) { unit = ""; }
    if ( hname.Contains("Ndown") ) { unit = ""; }

    if ( mean < 1 && mean > -1 && !hname.Contains("Mom") && !hname.Contains("Nud") && !hname.Contains("Nup") && !hname.Contains("Ndown") ) {
      avg_rms << " #approx 0.0 #pm ";
      avg_rms.precision(1);
      if ( rms >= 10 ) { avg_rms.precision(2); }
      avg_rms << rms << " " << unit;
    }
    else {
      if ( hname.Contains("_Mom") ) {
	avg_rms.precision(2);
	avg_rms << " = " << mean << " #pm ";
	avg_rms.precision(2);
	avg_rms << rms << " " << unit;
      }
      else if ( hname.Contains("Nud") || hname.Contains("Nup") || hname.Contains("Ndown") ) {
	mean = hist1d->GetMean(2);
	rms = hist1d->GetRMS(2);
	avg_rms.precision(3);
	avg_rms << " = " << mean << " #pm ";
	avg_rms.precision(3);
	avg_rms << rms << " " << unit;
      }
      else {
	avg_rms.precision(1);
	if ( mean >= 10 || mean <= -10 ) { avg_rms.precision(2); }
	avg_rms << " = " << mean << " #pm ";
	avg_rms.precision(1);
	if ( rms >= 10 ) { avg_rms.precision(2); }
	avg_rms << rms << " " << unit;
      }
    }

    if ( hname.Contains("_Mom") ) {
      myText(0.50, 0.86, 1, avg_rms.str().c_str(), 0.055);
    }
    else {
      myText(0.55, 0.86, 1, avg_rms.str().c_str(), 0.055);
    }
  }


  stringstream sumname;
  double intyield =  hist1d->Integral();
  if ( hname.Contains("Profile") || hname.Contains("FFT") ) {
    intyield = hist1d->GetEntries();
  }
  sumname << "N_{evt} = ";
  sumname.precision(2);
  if ( intyield > 1e9 ) { sumname << intyield/1e9 << "#times10^{9}"; }
  else if ( intyield > 1e8 ) { sumname.precision(3); sumname << intyield/1e6 << "M"; }
  else if ( intyield > 1e7 ) { sumname << intyield/1e6 << "M"; }
  else if ( intyield > 1e6 ) { sumname << intyield/1e6 << "M"; }
  else if ( intyield > 1e5 ) { sumname.precision(3); sumname << intyield/1e3 << "k"; }
  else if ( intyield > 1e4 ) { sumname << intyield/1e3 << "k"; }
  else if ( intyield > 1e3 ) { sumname << intyield/1e3 << "k"; }
  else if ( intyield > 1e2 ) { sumname.precision(3); sumname << intyield << ""; }
  else if ( intyield > 1e1 ) { sumname << intyield << ""; }
  else if ( intyield > 1e0 ) { sumname << intyield << ""; }
  if ( hname.Contains("Profile") || hname.Contains("FFT") ) {
    myText(0.29, 0.86, 1, sumname.str().c_str(), 0.035);
  }
  else {
    myText(0.19, 0.86, 1, sumname.str().c_str(), 0.035);
  }
  


  if ( hname.Contains("_MomTimeFinal") ||
       hname.Contains("TrackerY") || hname.Contains("TrackerRhat") ) {
    stringstream toteff;
    toteff << SurviveText(eff_turn100, err_turn100, "100");
    myText(0.20, 0.86, kRed, toteff.str().c_str(), 0.055);
  }


  //int result = hist1d->Fit("[0]*sin([1]*x)", "R");
  //cout << result << endl;
  
  bool doeps = false;

  stringstream c1name;
  if ( zoom ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist1d->GetName() << "_Zoom";
  }
  else if ( zoom2 ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist1d->GetName() << "_Zoom2";
  }
  else {
    c1name << "~/public_html/plots/" << dir << "/" << hist1d->GetName();
  }

  if ( dologx ) {
    c1->SetLogx(1);
    c1->SetGrid(0);
  }

  if ( dolog ) {
    c1->SetLogy(1);
    c1->SetGrid(1);
    c1->SetGrid(0);
  }
  else {
    c1->SetLogy(0);
    c1->SetGrid(0);
  }

  cout << "Saving." << endl;
  stringstream pngname, epsname, Cname;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  Cname   << c1name.str() << ".root";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  if ( saveC ) {
    TFile *outfile = new TFile(Cname.str().c_str(), "RECREATE");
    hist1d->Write();
    outfile->Close();
  }
  if ( saveC && 0 ) {
    c1->SaveAs(Cname.str().c_str());
  }
  cout << "Done" << endl;
  delete c1;
}

TH2D* GetHistogram(TFile *file, string name, double *int_prev, double *int_curr, double *int_start)
{
  if ( update ) {
    if ( name != update_plotname1 &&
	 name != update_plotname2 &&
	 name != update_plotname3 ) { 
      //cout << "Not updating " << name << endl;
      return( NULL );
    }
  }

  string histname = name;
  if ( rootdir.size() > 0 ) { histname = rootdir + "/" + name; }
  
  TH2D *hist = (TH2D*)file->Get(histname.c_str());
  if ( !hist  ) { cout << "Could not get " << histname << " from GetHistogram()" << endl; return( NULL ); }

  double curint = hist->Integral(); 
  if ( *int_prev == -1 ) {
    ;//*int_prev = hist->Integral(); 
    //if ( upstream_inflector_yield < 0 ) { *int_start = *int_prev; }
  }
  //if ( upstream_inflector_yield < 0 ) { *int_start = *int_prev; }
  //*int_curr = hist->Integral();

  if ( *int_start <= 0 && curint > 0 ) {
    *int_start = curint;
    *int_curr = curint;
    *int_prev = curint;
  }
  else if ( *int_start > 0 ) {
    *int_prev = *int_curr;
    *int_curr = curint;
  }

  return( hist );
}

TH1D* GetHistogram1D(TFile *file, string name, double *int_prev, double *int_curr, double *int_start)
{
  if ( update ) {
    if ( name != update_plotname1 &&
	 name != update_plotname2 &&
	 name != update_plotname3 ) { 
      //cout << "Not updating " << name << endl;
      return( NULL );
    }
  }

  string histname = name;
  if ( rootdir.size() > 0 ) { histname = rootdir + "/" + name; }


  TH1D *hist1d = (TH1D*)file->Get(histname.c_str());
  if ( !hist1d ) { cout << "Could not get " << histname << " from GetHistogram1D()" << endl; return( NULL ); }
  
  return( hist1d );
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
    else { loc_err = 0.0; }
  }
  *eff = loc_eff * 100.0; *err = loc_err * 100.0;
}


void plotinflector()
{
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasStyle.C");
  SetAtlasStyle();
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasUtils.C");
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasLabels.C");

  //K1
  double x0, z0, x1,z1, x2, z2, x3, z3, x4, z4, x5, z5, x6, z6, x7, z7, x8, z8;
  z1 = 6125;
  x1 = 3500;
  z2 = 6200;
  x2 = 3575;
  z3 = 6850;
  x3 = -1750;
  z4 = 6950;
  x4 = -1775;
  x5 = 90;
  z5 = 7175;
  x6 = -30;
  z6 = 7175;
  x7 = 1925;
  z7 = 6875;
  x8 = 1825;
  z8 = 6925;

  double q1x0, q1x1, q1z0, q1z1;
  q1x0 = 6800;
  q1x1 = 6200;
  q1z0 = 2000;
  q1z1 = 3400;

  double q2x0, q2x1, q2z0, q2z1;
  q2x0 = 5900;
  q2x1 = 3500;
  q2z0 = 3900;
  q2z1 = 6100;
  
  
  double degree0a = TMath::ATan2(0, 1) * 180 / TMath::Pi();
  double degree0b = TMath::ATan2(1, 0) * 180 / TMath::Pi();
  double degree1 = TMath::ATan2(z1, x1) * 180 / TMath::Pi();
  double degree2 = TMath::ATan2(z2, x2) * 180 / TMath::Pi();
  double degree3 = TMath::ATan2(z3, x3) * 180 / TMath::Pi();
  double degree4 = TMath::ATan2(z4, x4) * 180 / TMath::Pi();
  double degree5 = TMath::ATan2(z5, x5) * 180 / TMath::Pi();
  double degree6 = TMath::ATan2(z6, x6) * 180 / TMath::Pi();
  double degree7 = TMath::ATan2(z7, x7) * 180 / TMath::Pi();
  double degree8 = TMath::ATan2(z8, x8) * 180 / TMath::Pi();
  double degree8 = TMath::ATan2(z8, x8) * 180 / TMath::Pi();
  double degreeq1a = TMath::ATan2(q1z0, q1x0) * 180 / TMath::Pi();
  double degreeq1b = TMath::ATan2(q1z1, q1x1) * 180 / TMath::Pi();
  double degreeq2a = TMath::ATan2(q2z0, q2x0) * 180 / TMath::Pi();
  double degreeq2b = TMath::ATan2(q2z1, q2x1) * 180 / TMath::Pi();
  //cout << degree0a << "\t" << degree0b << "\t" << degree1 << "\t" << degree2 << "\t" << degree3 << "\t" << degree4 << "\t" << degree5 << "\t" << degree6 << "\t" << degree7 << "\t" << degree8 << endl;
  cout << degreeq1a << "\t" << degreeq1b << "\t" << degreeq2a << "\t" << degreeq2b << endl;

  //return;
  

  ifstream in;
  in.open("plotname.dat");
  in >> update;
  in >> update_plotname1;
  in >> update_plotname2;
  in >> update_plotname3;
  in >> base;
  in >> basedir;  
  in.close();
  if ( base.size() == 0 ) { base = "Basic"; }

  beamsize = -1;
  if ( base.find("BeamSize0pi") != string::npos ) { beamsize = 0; }
  if ( base.find("BeamSize40pi") != string::npos ) { beamsize = 40; }

  kick = -1;
  if ( base.find("NoKick") != string::npos ) { kick = 0; }
  if ( base.find("_25G") != string::npos ) { kick = 25; }
  if ( base.find("_50G") != string::npos ) { kick = 50; }
  if ( base.find("_75G") != string::npos ) { kick = 75; }
  if ( base.find("130G") != string::npos ) { kick = 130; }
  if ( base.find("140G") != string::npos ) { kick = 140; }
  if ( base.find("160G") != string::npos ) { kick = 160; }
  if ( base.find("185G") != string::npos ) { kick = 185; }
  if ( base.find("100G") != string::npos ) { kick = 100; }
  if ( base.find("210G") != string::npos ) { kick = 210; }
  if ( base.find("125G") != string::npos ) { kick = 125; }
  if ( base.find("200G") != string::npos ) { kick = 200; }
  if ( base.find("205G") != string::npos ) { kick = 205; }
  if ( base.find("215G") != string::npos ) { kick = 215; }
  if ( base.find("195G") != string::npos ) { kick = 195; }
  if ( base.find("185G") != string::npos ) { kick = 185; }
  if ( base.find("200G") != string::npos ) { kick = 200; }
  if ( base.find("230G") != string::npos ) { kick = 230; }
  if ( base.find("120G") != string::npos ) { kick = 120; }
  if ( base.find("190G") != string::npos ) { kick = 190; }
  if ( base.find("170G") != string::npos ) { kick = 170; }
  if ( base.find("220G") != string::npos ) { kick = 220; }
  if ( base.find("180G") != string::npos ) { kick = 180; }
  if ( base.find("150G") != string::npos ) { kick = 150; }
  if ( base.find("240G") != string::npos ) { kick = 240; }
  if ( base.find("250G") != string::npos ) { kick = 250; }
  if ( base.find("260G") != string::npos ) { kick = 260; }
  if ( base.find("280G") != string::npos ) { kick = 280; }
  if ( base.find("270G") != string::npos ) { kick = 270; }
  if ( base.find("300G") != string::npos ) { kick = 300; }
  if ( base.find("325G") != string::npos ) { kick = 325; }
  if ( base.find("350G") != string::npos ) { kick = 350; }
  if ( base.find("375G") != string::npos ) { kick = 375; }
  if ( base.find("400G") != string::npos ) { kick = 400; }
  if ( base.find("50kV") != string::npos ) { kick = -50; }
  if ( base.find("60kV") != string::npos ) { kick = -60; }
  if ( base.find("70kV") != string::npos ) { kick = -70; }
  if ( base.find("80kV") != string::npos ) { kick = -80; }
  if ( base.find("90kV") != string::npos ) { kick = -90; }
  if ( base.find("100kV") != string::npos ) { kick = -100; }
  if ( base.find("110kV") != string::npos ) { kick = -110; }
  if ( base.find("120kV") != string::npos ) { kick = -120; }
  if ( base.find("55kV") != string::npos ) { kick = -55; }
  if ( base.find("65kV") != string::npos ) { kick = -65; }
  if ( base.find("75kV") != string::npos ) { kick = -75; }
  if ( base.find("85kV") != string::npos ) { kick = -85; }
  if ( base.find("95kV") != string::npos ) { kick = -95; }
  if ( base.find("102kV") != string::npos ) { kick = -102; }
  if ( base.find("107kV") != string::npos ) { kick = -107; }
  if ( base.find("105kV") != string::npos ) { kick = -105; }
  if ( base.find("115kV") != string::npos ) { kick = -115; }
  if ( base.find("125kV") != string::npos ) { kick = -125; }
  if ( base.find("130kV") != string::npos ) { kick = -130; }
  if ( base.find("135kV") != string::npos ) { kick = -135; }
  if ( base.find("140kV") != string::npos ) { kick = -140; }

  string subdir = "";
  if ( base.find("_DM_") != string::npos ) { subdir = "DM"; }
  if ( base.find("_UM_") != string::npos ) { subdir = "UM"; }
  if ( base.find("_UC_") != string::npos ) { subdir = "UC"; }
  if ( base.find("_CO_") != string::npos ) { subdir = "CO"; }
  if ( base.find("CO_Offset77") != string::npos ) { subdir = "CO_Offset77"; }

  if ( base.find("InfOpen") != string::npos ) { subdir += "_InfOpen"; }
  if ( base.find("InfClosed") != string::npos ) { subdir += "_InfClosed"; }
  if ( base.find("InfPartial") != string::npos ) { subdir += "_InfPartial"; }

  
  if ( base.find("E821Match") != string::npos ) { isE821 = true; }

  if ( base.find("PerfectMatch") != string::npos ) { dPoverP = 0.0015; }
  if ( base.find("dP10") != string::npos ) { dPoverP = 0.10; }
  if ( base.find("dP5") != string::npos ) { dPoverP = 0.05; }
  if ( base.find("dP2") != string::npos ) { dPoverP = 0.02; }
  if ( base.find("dP075") != string::npos ) { dPoverP = 0.0075; }
  if ( base.find("dP05") != string::npos ) { dPoverP = 0.005; }
  if ( base.find("dP025") != string::npos ) { dPoverP = 0.0025; }
  if ( base.find("dP01") != string::npos ) { dPoverP = 0.001; }
  if ( base.find("dP0075") != string::npos ) { dPoverP = 0.00075; }
  if ( base.find("dP005") != string::npos ) { dPoverP = 0.0005; }
  if ( base.find("dP0025") != string::npos ) { dPoverP = 0.00025; }
  if ( base.find("dP001") != string::npos ) { dPoverP = 0.0001; }
  if ( base.find("dP00075") != string::npos ) { dPoverP = 0.000075; }
  if ( base.find("dP0005") != string::npos ) { dPoverP = 0.00005; }
  if ( base.find("dP00025") != string::npos ) { dPoverP = 0.000025; }
  if ( base.find("dP0001") != string::npos ) { dPoverP = 0.00001; }

  

  if ( dPoverP < 0.0025 ) { dPoverP = 0.0025; }

  cout << "dP/P Set to " << 100*dPoverP << "%" << endl;

  ifstream incmd;
  incmd.open("plotcmd");
  string cmd;
  if ( incmd.is_open() ) {
    in >> cmd;
  }

  ifstream effin;
  string effname;
  effname = "Eff/" + subdir + "/" + base + ".dat";
  effin.open(effname.c_str());
  //if ( effin.is_open() == false ) { cout << "Could not open " << effname << endl; return; }

  int Nstart_inflector = 0;
  int Nexit_inflector = 0;
  int Nstart_ring = 0;
  int Nstored1_ring = 0;
  int Nstored10_ring = 0;
  int Nstored100_ring = 0;
  int Ngen = 0;

  if ( effin.is_open() ) {
    effin >> Nstart_inflector >> Nexit_inflector;
    effin >> Nstart_ring;
    effin >> Nstored1_ring >> Nstored10_ring >> Nstored100_ring;
    effin >> Ngen;
    effin.close();
  }

  cout << "Ngen: " << Ngen << endl;

  if ( Ngen == Nstored100_ring ) { Ngen = Nstart_inflector; }
  if ( Ngen == 0 ) { Ngen = Nstart_inflector; }
  if ( Ngen > 0 ) { Nstart_inflector = Ngen; }
  upstream_inflector_yield = Nstart_inflector;
  
  cout << "Ngen: " << Ngen << endl;
  double eff, err;
  cout << "Ngen: " << upstream_inflector_yield << endl;
  geteff(Nexit_inflector, Nstart_inflector, &eff, &err);
  //cout << file << "\t" << Nstart_inflector << "\t" << Nexit_inflector << "\t" << eff << "\t" << err << endl;
  eff_inf = eff;
  err_inf = err;
  int Nring = Nstart_inflector;
  if ( Nstart_inflector <= 0 ) { Nring = Nstart_ring; }
  geteff(Nstored1_ring, Nring, &eff, &err);
  eff_turn1 = eff;
  err_turn1 = err;
  geteff(Nstored10_ring, Nring, &eff, &err);
  eff_turn10 = eff;
  err_turn10 = err;
  geteff(Nstored100_ring, Nring, &eff, &err);
  eff_turn100 = eff;
  err_turn100 = err;
  
  rootdir = "readRingTrackers";
  TString filename = "rootfiles/" + subdir + "/" + "gm2ringsim_" + base + ".root";
  TFile *file = TFile::Open(filename);
  cout << "Contents of " << filename << endl;
  file->cd("readRingTrackers");
  //file->ls();
  //return;
  dir = base;
  

  if ( base.find("CO") != string::npos ) { plotinf = false; plotringhits = true; }
  if ( base.find("UC") != string::npos ) { plotinf = true; }
  if ( base.find("UM") != string::npos ) { plotinf = true; }
  if ( base.find("DM") != string::npos ) { plotinf = true; }
  if ( base.find("CO") != string::npos ) { plotinf = true; }
  if ( plotinf ) { start_inflector = true; }

    
  dir = basedir + "/" + dir;

  style();
  
  if ( dir.find("_Offset60") != string::npos || dir.find("_Offset65") != string::npos ||
       dir.find("_Offset70") != string::npos || dir.find("_Offset75") != string::npos ||
       dir.find("_Offset80") != string::npos || dir.find("_Offset85") != string::npos ||
       dir.find("_Offset90") != string::npos || dir.find("_Offset95") != string::npos ) {
    start_inflector = true;
  }
       
  

  int maxturns = 11;
  if ( dir.find("_3Turns") != string::npos ) { maxturns = 3; maxturnsreal = 3; }
  if ( dir.find("_5Turns") != string::npos ) { maxturns = 4; maxturnsreal = 5; }
  if ( dir.find("_10Turns") != string::npos ) { maxturns = 5; maxturnsreal = 10; }
  if ( dir.find("_100Turns") != string::npos ) { maxturns = 7; maxturnsreal = 100; }
  if ( dir.find("_101Turns") != string::npos ) { maxturns = 7; maxturnsreal = 101; }
  if ( dir.find("_50Turns") != string::npos ) { maxturns = 6; maxturnsreal = 50; }
  if ( dir.find("_25Turns") != string::npos ) { maxturns = 6; maxturnsreal = 25; }
  if ( dir.find("_3Turns") != string::npos ) { maxturns = 3; maxturnsreal = 3; }
  if ( dir.find("_200Turns") != string::npos ) { maxturns = 8; maxturnsreal = 200; }
  if ( dir.find("_500Turns") != string::npos ) { maxturns = 9; maxturnsreal = 500; }
  if ( dir.find("_1000Turns") != string::npos ) { maxturns = 10; maxturnsreal = 1000; }
  if ( dir.find("_2000Turns") != string::npos ) { maxturns = 11; maxturnsreal = 2000; }
  if ( dir.find("_4000Turns") != string::npos ) { maxturns = 11; maxturnsreal = 4000; }
  if ( dir.find("_5000Turns") != string::npos ) { maxturns = 11; maxturnsreal = 5000; }
  if ( dir.find("_10000Turns") != string::npos ) { maxturns = 11; maxturnsreal = 10000; }
  if ( dir.find("_15000Turns") != string::npos ) { maxturns = 11; maxturnsreal = 15000; }
  maxtime = maxturnsreal * 0.15;

  bool plot_electron = true;
  bool plot_muon = true;
  bool plot_basic = false;
  
  int Nparticles = 0;
  string truth_particle_names[20];
  if ( dir.find("NoMuonDecay") != string::npos ) {
    Nparticles = 0;
    truth_particle_names[Nparticles++] = "BirthMuon";
    truth_particle_names[Nparticles++] = "LostMuon";
    truth_particle_names[Nparticles++] = "StoredMuon";
  }
  else {
    Nparticles = 0;
    if ( dir.find("MuonGas") != string::npos || dir.find("BeamTransport") != string::npos) {
      is_muongas = true;
      if ( test_gen ) {
	truth_particle_names[Nparticles++] = "DecayMuon";      
	truth_particle_names[Nparticles++] = "StrawElectron";
	truth_particle_names[Nparticles++] = "StrawCaloElectron";
	truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
	truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
	truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
	truth_particle_names[Nparticles++] = "DecayElectron";
      }
      else {
	if ( !plot_basic ) {
	  truth_particle_names[Nparticles++] = "DecayMuon";     
	  //truth_particle_names[Nparticles++] = "BirthElectron";
	  truth_particle_names[Nparticles++] = "BirthElectronEgtEth"; 
	  //truth_particle_names[Nparticles++] = "StrawElectron";
	  //truth_particle_names[Nparticles++] = "StrawCaloElectron";
	  //truth_particle_names[Nparticles++] = "GoodStrawElectron";
	  truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
	  //truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
	  //truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
	  truth_particle_names[Nparticles++] = "DecayElectron";
	  truth_particle_names[Nparticles++] = "DecayElectronEgtEth";
// 	  truth_particle_names[Nparticles++] = "DecayElectronQuadSection";
// 	  truth_particle_names[Nparticles++] = "DecayElectronKickerSection";
// 	  truth_particle_names[Nparticles++] = "DecayElectronEmptySection";
	}
      }
    }
    else {
      Nparticles = 0;
      truth_particle_names[Nparticles++] = "DecayMuon";     
      //truth_particle_names[Nparticles++] = "BirthElectron";
      truth_particle_names[Nparticles++] = "BirthElectronEgtEth"; 
      //truth_particle_names[Nparticles++] = "StrawElectron";
      //truth_particle_names[Nparticles++] = "StrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodStrawElectron";
      truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
      //truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
      truth_particle_names[Nparticles++] = "DecayElectron";
      truth_particle_names[Nparticles++] = "DecayElectronEgtEth";
      // 	  truth_particle_names[Nparticles++] = "DecayElectronQuadSection";
      // 	  truth_particle_names[Nparticles++] = "DecayElectronKickerSection";
      // 	  truth_particle_names[Nparticles++] = "DecayElectronEmptySection";
      
      if ( 0 ) {

	if ( plot_muon ) {
	  truth_particle_names[Nparticles++] = "DecayMuon";
	}
	if ( !plot_basic && 0 ) {
	  truth_particle_names[Nparticles++] = "BirthMuon";
	  //truth_particle_names[Nparticles++] = "LostMuon";
	  //truth_particle_names[Nparticles++] = "StoredMuon";
	}
	if ( plot_electron && 0 ) {
	  if ( !plot_basic ) { truth_particle_names[Nparticles++] = "BirthElectron"; }
	  truth_particle_names[Nparticles++] = "StrawElectron";
	  truth_particle_names[Nparticles++] = "StrawCaloElectron";
	  truth_particle_names[Nparticles++] = "GoodStrawCaloElectron";
	  truth_particle_names[Nparticles++] = "GoodOneStrawCaloElectron";
	  truth_particle_names[Nparticles++] = "GoodTwoStrawCaloElectron";
	  truth_particle_names[Nparticles++] = "DecayElectron";
	  if ( !plot_basic ) { 
	    truth_particle_names[Nparticles++] = "DecayElectronEgtEth";
	    truth_particle_names[Nparticles++] = "DecayElectronQuadSection";
	    truth_particle_names[Nparticles++] = "DecayElectronKickerSection";
	    truth_particle_names[Nparticles++] = "DecayElectronEmptySection";
	    //truth_particle_names[Nparticles++] = "DecayElectronEgtHghEth";
	  }
	}
      }
    }
  }
  
  
  double int_prev = -1.0;
  double int_start = -1.0;
  double int_curr = -1.0;
  stringstream hname;


  int maxringtracks = 9;
  int maxsyshits = 5;
  bool plotinfstart = false;
  bool plotdiagnostic = true;


  bool plot_inflector = true;

  bool plot_ringtrackers = true;
  bool plot_ringtrackers_phasespace = true;
  bool plot_ringtrackers_evolution = true;
  bool plot_ringtrackers_1stturn = true;

  bool plot_kickerquads = true;

  bool plot_detectors = true;

  bool plot_nums = true;

  bool plot_truth_phasespace = true;
  bool plot_truth = true;
  bool plot_truth_evolution = true;

  bool plot_phasespace_time = true;

  bool plot_beamscan = true;

  plot_inflector = false;

  //   plot_ringtrackers = false;
  //   plot_ringtrackers_phasespace = false;
  //   plot_ringtrackers_evolution = false;

  //plot_kickerquads = false;
  
  plot_detectors = false;

  //plot_truth_phasespace = false;
  //plot_truth = false;
  //plot_truth_evolution = false;
  if ( dir.find("NoMuonDecay") != string::npos ) {
    plot_truth_evolution = false;
    plot_truth = true;
    plot_truth_evolution = true;
    plot_ringtrackers = true;
    plot_ringtrackers_phasespace = true;
    plot_ringtrackers_evolution = true;
  }
  else {
    plot_ringtrackers = true;
    plot_truth_phasespace = true;
    plot_truth_evolution = true;
    plot_truth = true;
    //plot_truth_evolution = false;
    plot_nums = true;
    plot_phasespace_time = true;
  }
  
  if ( plot_phasespace_time ) {;
    //plot_ringtrackers = false;
    //plot_truth = false;
  }

  bool plot_profile_phasespace = true;
  if ( is_muongas ) { 
    plot_ringtrackers = false;
    plot_truth = true;
    plot_profile_phasespace = false;
  }
    
  if ( test_gen ) { 
    plot_truth = true;
    plot_truth_phasespace = true;
    plot_truth_evolution = false;
    only_cbo = false;
  }

  //plot_truth = false;
  //plot_truth_phasespace = false;

  if ( only_cbo ) { 
    plot_truth = true;
    plot_truth_evolution = true;
    plot_ringtrackers = false;
    test_gen = false;
    plot_beamscan = false;
  }

  if ( test_evolve ) {
    plot_beamscan = true;
    plot_truth = true;
    plot_truth_evolution = true;
    plot_truth_phasespace = true;
    plot_ringtrackers = false;
    test_gen = false;
    only_cbo = false;
    plot_profile_phasespace = false;
  }

  if ( plot_truth == false ) {
    plot_truth_evolution = false;
    plot_truth_phasespace = false;
    plot_phasespace_time = false;
    plot_nums = false;
  }
  
  if ( plot_ringtrackers == false ) {
    plot_ringtrackers_phasespace = false;
    plot_ringtrackers_evolution = false;
    plot_ringtrackers_1stturn = false;
    plot_kickerquads = false;
  }

  bool runFFT = false;
  if ( dir.find("CentralOrbit") != string::npos &&
       dir.find("Offset") == string::npos &&
       dir.find("ScrapingOFF") != string::npos ) { runFFT = true; }
  runFFT = false;
  if ( runFFT ) { cout << "We are going to run the FFT..." << endl; }
  else { cout << "Turning OFF FFT for trackers vs. time." << endl; }

  if ( dir.find("CentralOrbit") != string::npos ) { plot_inflector = false; }

  bool plot_Nud = false;
  if ( dir.find("EDM") != string::npos ) { plot_Nud = true; }

  //return;

  int Nringtrackertimes = kFinal + 1;
  int Ntruthtimes = kFinal + 1;
  string times[3];
  times[kBegin] = "Begin";
  times[kFinal] = "Final"; 
  int Ntimes = 2;

  string timevals[2];
  int Ntimevals = kRemainingDist + 1;
  timevals[kGeneratedDist] = "GeneratedDist";
  timevals[kRemainingDist] = "RemainingDist";


  int Ntimestamps = 0;
  string timestamps[4];
  //timestamps[Ntimestamps++] = "Time";
  timestamps[Ntimestamps++] = "TimeOncePerTurn";
  timestamps[Ntimestamps++] = "TimeSingleHit";
  //timestamps[Ntimestamps++] = "TimeTwicePerTurn";

  cout << endl;
  cout << "==========================================" << endl;
  cout << "Plotting The Following:" << endl;


  if ( plot_inflector ) { cout << "Plotting inflector " << endl; }

  if ( plot_ringtrackers ) { cout << "Plotting ring trackers " << endl; }
  if ( plot_ringtrackers_phasespace ) { cout << "Plotting ring trackers + PS" << endl; }
  if ( plot_ringtrackers_evolution ) { cout << "Plotting ring trackers vs time." << endl; }
  if ( plot_ringtrackers_1stturn ) { cout << "Plotting ring trackers for 1st turn " << endl; }

  if ( plot_kickerquads ) { cout << "Plotting kickers/quads " << endl; }

  if ( plot_detectors ) { cout << "Plotting detectors " << endl; }

  if ( plot_nums ) { cout << "Plotting nums. " << endl; }
  if ( plot_beamscan ) { cout << "Plotting beam scan. " << endl; }

  if ( plot_truth_phasespace ) { cout << "Plotting truth phase space" << endl; }
  if ( plot_truth ) { cout << "Plotting truth " << endl; }
  if ( plot_truth_evolution ) { cout << "Plotting truth vs time." << endl; }
  
  if ( plot_phasespace_time ) { cout << "Plotting truth phase space vs time." << endl; }
  cout << "==========================================" << endl;
  cout << endl;


  //
  // Plot Inflector
  //
  if ( plot_inflector ) {
    //string itnames[9] = {"0", "1", "2", "3", "4", "5", "6", "7", "8"};
    string itnames[3] = {"0", "4", "8"};
    int_prev = -1.0;
    zoom = false; zoom2 = false; zoom3 = false;
    bool plotinftrk = true;
    int ninfplots = 1;
    if ( ninfplots == 1 ) { plotinfstart = true; }
    if ( plotdiagnostic ) { ninfplots = 3; }
    cout << plotinf << "\t" << plotinftrk << endl;
    for ( int i = 0; i < ninfplots; i++ ) {
      //cout << i << "/" << ninfplots << endl;
      if ( !plotinf ) { continue; }
      if ( plotinftrk == false ) { continue; }
      
      hname << "InflectorTracker_" << itnames[i] << "_RhatY";
      TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");

      hname << "InflectorTracker_" << itnames[i] << "_XprimeX";
      TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
    
      hname << "InflectorTracker_" << itnames[i] << "_YprimeY";
      TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");   
    }
  }
  else {
    cout << endl;
    cout << "Not plotting inflector information." << endl;
    cout << endl;
  }
  
  //return;


  
  if ( plot_beamscan ) {
    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    Ring2Dhists[NRing2Dhists++] = "Rhat";
    //Ring2Dhists[NRing2Dhists++] = "Y";
//     Ring2Dhists[NRing2Dhists++] = "Mom";
//     Ring2Dhists[NRing2Dhists++] = "Xprime";
//     Ring2Dhists[NRing2Dhists++] = "Yprime";


    for ( int m = 0; m < 4; m++ ) {

    for ( int n = 0; n < NRing2Dhists; n++ ) {

      mintime_for_plot = -1.0;
      maxtime_for_plot = -1.0;    

      string histname = Ring2Dhists[n];
      
      string Ringbase = "BeamScan_";
      string timestamp = "Time";
      timestamp = "TimeSingleHit";
      stringstream hname;
      hname << Ringbase << histname << "_vs_" << timestamp;
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
      
      TH2D *hist_tmp = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      if ( !hist_tmp ) { hname.str(""); continue; }
      hname.str("");
      hname << Ringbase << histname;
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
      TH2D *hist = (TH2D*)hist_tmp->Clone(hname.str().c_str());
      if ( !hist ) { hname.str(""); continue; }
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");

      if ( histname == "Rhat" ) {
	if ( m == 0 ) { PrintRhatT0(hist, "BeamScan"); }
	if ( m == 1 ) { PrintRhatT0(hist, "BeamScan_HighMom"); }
	if ( m == 2 ) { PrintRhatT0(hist, "BeamScan_LowMom"); }
	if ( m == 3 ) { PrintRhatT0(hist, "BeamScan_MagicMom"); }
      }


      //------------------------------------------------------
      // Do Profiles instead of 2D plots to average things out
      //------------------------------------------------------
      bool do_profile = true;
      hname.str("");
      string Ringbase = "BeamScan_";
      hname << Ringbase << histname << "_Profile";
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
      TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");      
      MakePlot1D(hist1d_prof, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");


      //------------------------------------------------------
      // FFTs
      //------------------------------------------------------
      if ( histname != "Mom" ) {
	string Ringbase = "BeamScan_";
	hname << Ringbase << histname << "_FFT";
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
	TH1D *hist1d_fft = (TH1D*)hist1d_prof->Clone(hname.str().c_str());
	ComputeFFT(hist1d_fft, hname.str());
	hname.str("");
      }

      
      //-----
      // FFT zoom
      //-----
      if ( histname == "Rhat" ) {
	for ( int z = 0; z < 4; z++ ) {
	  mintime_for_plot = -1.0;
	  maxtime_for_plot = -1.0;
	  cout << "Z = " << z << endl;
	  if ( z == 3 ) {
	    mintime_for_plot = 0.0;
	    maxtime_for_plot = 20.0;
	  }
	  if ( z == 0 ) {
	    mintime_for_plot = 100.0;
	    maxtime_for_plot = 120.0;
	  }
	  if ( z == 1 ) {
	    mintime_for_plot = 400.0;
	    maxtime_for_plot = 420.0;
	  }
	  if ( z == 2 ) {
	    mintime_for_plot = 800.0;
	    maxtime_for_plot = 820.0;
	  }

	  cout << "Min/Max Time: " << mintime_for_plot << "\t" << maxtime_for_plot << "\t" << maxtime << endl;
	  if ( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	  cout << "Min/Max Time: " << mintime_for_plot << "\t" << maxtime_for_plot << "\t" << maxtime << endl;
	  if ( mintime_for_plot > maxtime ) { continue; }
	  if ( maxtime_for_plot <= mintime_for_plot ) { continue; }
	  cout << "Min/Max Time: " << mintime_for_plot << "\t" << maxtime_for_plot << "\t" << maxtime << endl;
	  
	  stringstream hname;
	  string Ringbase = "BeamScan_";
	  hname << Ringbase << histname << "_Profile_Zoom" << z;
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
	  cout << "hname = " << hname.str() << endl;
	  cout << "Z again." << endl;
	  if ( !hist ) { continue; }
	  TProfile *hist1d_prof_zoom = hist->ProfileX(hname.str().c_str(), 1, 20000000000, "i");
	  if ( !hist1d_prof_zoom ) { continue; }
	  cout << "Min/Max" << endl;
	  for ( int bin = 1; bin <= hist1d_prof_zoom->GetNbinsX(); bin++ ) {
	    if ( hist1d_prof_zoom->GetBinLowEdge(bin) < mintime_for_plot ||
		 hist1d_prof_zoom->GetBinLowEdge(bin) > maxtime_for_plot ) {
	      hist1d_prof_zoom->SetBinContent(bin, 0.0);
	    }
	  }
	  MakePlot1D(hist1d_prof_zoom, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	  cout << "Back from MakePlot1D" << endl;
	  continue;
	  
	  
	  hname << Ringbase << histname << "_FFT_Zoom" << z;
      if ( m == 1 ) { hname << "_HighMom"; }
      if ( m == 2 ) { hname << "_LowMom"; }
      if ( m == 3 ) { hname << "_MagicMom"; }
	  TH1D *hist1d_mean_prof_zoom = (TH1D*)hist1d_fft->Clone(hname.str().c_str());
	  for ( int bin = 1; bin <= hist1d_fft->GetNbinsX(); bin++ ) {
	    if ( bin < hist1d_fft->FindBin(mintime_for_plot) || bin > hist1d_fft->FindBin(maxtime_for_plot) ) {
	      hist1d_mean_prof_zoom->SetBinContent(bin, 0.0);
	    }
	  }
	  ComputeFFT(hist1d_mean_prof_zoom, hname.str());
	  hname.str("");
	}
      }
    }
    }
    mintime_for_plot = -1.0;
    maxtime_for_plot = -1.0;



    for ( int n = 0; n < NRing2Dhists; n++ ) {
      mintime_for_plot = -1.0;
      maxtime_for_plot = -1.0;    


      string histname = Ring2Dhists[n];
      

      string Ringbase = "G4Track_";
      string G4base = Ringbase;
      string truth_part_name = "DecayMuon";
      string timestamp = "TimeOncePerTurn";
      timestamp = "TimeSingleHit";
      //timestamp = "Time";
      stringstream hname;
      hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
      TH2D *hist_tmp = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      if ( !hist_tmp ) { hname.str(""); continue; }
      hname.str("");
      Ringbase = "MuonDecay_";
      hname << Ringbase << histname;
      TH2D *hist = (TH2D*)hist_tmp->Clone(hname.str().c_str());
      cout << "Int: " << hist->Integral() << "\t" << hist->GetEntries() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");

      if ( histname == "Rhat" && truth_part_name == "DecayMuon" ) {
	PrintRhatT0(hist, truth_part_name);
      }
      
      
      //------------------------------------------------------
      // Do Profiles instead of 2D plots to average things out
      //------------------------------------------------------
      bool do_profile = true;
      hname.str("");
      Ringbase = "MuonDecay_";
      hname << Ringbase << histname << "_Profile";
      TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");      
      MakePlot1D(hist1d_prof, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");


      //------------------------------------------------------
      // FFTs
      //------------------------------------------------------
      if ( histname != "Mom" ) {
	string Ringbase = "MuonDecay_";
	hname << Ringbase << histname << "_FFT";
	TH1D *hist1d_fft = (TH1D*)hist1d_prof->Clone(hname.str().c_str());
	ComputeFFT(hist1d_fft, hname.str());
	hname.str("");
      }

      
      //-----
      // FFT zoom
      //-----
      if ( histname == "Rhat" ) {
	for ( int z = 0; z < 4; z++ ) {
	  mintime_for_plot = -1.0;
	  maxtime_for_plot = -1.0;
	  cout << "Z = " << z << endl;
	  if ( z == 3 ) {
	    mintime_for_plot = 0.0;
	    maxtime_for_plot = 20.0;
	  }
	  if ( z == 0 ) {
	    mintime_for_plot = 100.0;
	    maxtime_for_plot = 120.0;
	  }
	  if ( z == 1 ) {
	    mintime_for_plot = 400.0;
	    maxtime_for_plot = 420.0;
	  }
	  if ( z == 2 ) {
	    mintime_for_plot = 800.0;
	    maxtime_for_plot = 820.0;
	  }
	  cout << "Z again." << endl;
	  cout << mintime_for_plot << "\t" << maxtime_for_plot << endl;

	  if ( mintime_for_plot > maxtime ) { continue; }
	  if ( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	  if ( maxtime_for_plot <= mintime_for_plot ) { continue; }
	  
	  stringstream hname;
	  string Ringbase = "MuonDecay_";
	  hname << Ringbase << histname << "_Profile_Zoom" << z;
	  cout << "hname = " << hname.str() << endl;
	  cout << "Z again." << endl;
	  if ( !hist ) { continue; }
	  TProfile *hist1d_prof_zoom = hist->ProfileX(hname.str().c_str(), 1, 20000000000, "i");
	  if ( !hist1d_prof_zoom ) { continue; }
	  cout << "Min/Max" << endl;
	  for ( int bin = 1; bin <= hist1d_prof_zoom->GetNbinsX(); bin++ ) {
	    if ( hist1d_prof_zoom->GetBinLowEdge(bin) < mintime_for_plot ||
		 hist1d_prof_zoom->GetBinLowEdge(bin) > maxtime_for_plot ) {
	      hist1d_prof_zoom->SetBinContent(bin, 0.0);
	    }
	  }
	  MakePlot1D(hist1d_prof_zoom, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	  cout << "Back from MakePlot1D" << endl;
	  continue;
	  
	  
	  hname << Ringbase << histname << "_FFT_Zoom" << z;
	  TH1D *hist1d_mean_prof_zoom = (TH1D*)hist1d_fft->Clone(hname.str().c_str());
	  for ( int bin = 1; bin <= hist1d_fft->GetNbinsX(); bin++ ) {
	    if ( bin < hist1d_fft->FindBin(mintime_for_plot) || bin > hist1d_fft->FindBin(maxtime_for_plot) ) {
	      hist1d_mean_prof_zoom->SetBinContent(bin, 0.0);
	    }
	  }
	  ComputeFFT(hist1d_mean_prof_zoom, hname.str());
	  hname.str("");
	}
      }
    }

    mintime_for_plot = -1.0;
    maxtime_for_plot = -1.0;    
  }
    


  //------------------------------------
  //
  // Phase Space in Time
  //
  //------------------------------------
  if ( plot_phasespace_time && plot_ringtrackers ) {
    for ( int nt = 0; nt < 2; nt++ ) {
      string ntrackers = "";
      if ( nt == 0 ) { ntrackers = "1Tracker"; }
      if ( nt == 1 ) { ntrackers = "2Trackers"; }
      
      for ( int t = 0; t < 4; t++ ) {
	string tname = "";
	if ( t == kEarly ) { tname = "Early"; }
	if ( t == kMiddle ) { tname = "Middle"; }
	if ( t == kLate ) { tname = "Late"; }
	if ( t == kAll ) { tname = "All"; }
	
	stringstream hname;
	hname << "VirtualRingTracker_XprimeX_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist ) { hname.str(""); continue; }
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");

	hname << "VirtualRingTracker_YprimeY_PhaseSpace" << "_" << tname << "_with" << ntrackers;
	TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist ) { hname.str(""); continue; }
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");
      }
    }
  }
  
  if ( plot_profile_phasespace ) {
    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    Ring2Dhists[NRing2Dhists++] = "Y";
    Ring2Dhists[NRing2Dhists++] = "Rhat";
    
    int NRing1Dhists = 0;
    string Ring1Dhists[20];
    Ring1Dhists[NRing1Dhists++] = "";


    string Ringbase = "Tracker_";
    string G4base = "G4Track_";
    string truth_part_name = "DecayMuon";
    for ( int type = 1; type < 2; type++ ) {
      for ( int t = 0; t < Ntimestamps; t++ ) {
	string timestamp = timestamps[t];
	//if ( timestamp != "TimeSingleHit" ) { continue; }
	
	for ( int n = 0; n < NRing2Dhists; n++ ) {
	  
	  string histname = Ring2Dhists[n];

	  if ( only_cbo ) { continue; }
	  
	  //if ( !PlotVariable(histname, timestamp) ) { continue; }
	  
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  
	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  if ( !hist ) { hname.str(""); continue; }
	  
	  if ( 0 ) {
	    TFile *f = new TFile("HistY.root", "RECREATE");
	    hist->Write();
	    f->Close();
	    return;
	    c1->SaveAs("HistY.C");
	    return;
	  }

	  mintime_for_plot = -1;
	  maxtime_for_plot = -1;	
	  
	  hname.str("");
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_Full_Profile";
	  TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");	
	  
	  hname.str("");
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_Width_MeanSquared";

	  TH1D *hist1d_width_prof = (TH1D*)(hist1d_prof->Clone(hname.str().c_str()));	
	  for ( int bin = 1; bin <= hist1d_width_prof->GetNbinsX(); bin++ ) {
	    double cont = hist1d_prof->GetBinError(bin);
	    hist1d_width_prof->SetBinContent(bin, cont);
	  }
	  
	  MakePlot1D(hist1d_width_prof, -1, -1, &int_prev, &int_curr, &int_start);
	  
	  
	  hname.str("");
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_MeanSquared";

	  TH1D *hMS = new TH1D(hname.str().c_str(), "", 100, 0.0, 4.5);
	  for ( int i = 1; i <= hist1d_prof->GetNbinsX(); i++ ) {
	    double rms = hist1d_prof->GetBinError(i) / cm;
	    hMS->Fill(rms*rms);
	  }
	  MakePlot1D(hMS, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	  continue;
	  //hist1d_width_prof->Draw();
	  //c1->SaveAs("y2.C");


// 	  //------------------
// 	  // This is <<var>>
// 	  //------------------
// 	  double n0 = 0.314;
// 	  double lq = 4.804*m;
// 	  double lin = 6.368*m;
// 	  double R = R_magic();
// 	  double cosPsi = TMath::Cos(sqrt(n0)*lq/R) - sqrt(n0)*lin/(2*R)*TMath::Sin(sqrt(n0)*lq/R);
// 	  double Psi0    = 4*TMath::ACos(cosPsi);
// 	  double beta0min = TMath::Sin(Psi0/4)/(sqrt(n0)*TMath::Sin(sqrt(n0)*lq/R));

// 	  cout << "N = " << hist1d_width_prof << endl;
// 	  for ( int bin = 1; bin <= hist1d_width_prof->GetNbinsX(); bin++ ) {
// 	    double bincontN   = hist1d_width_prof->GetBinContent(bin);
// 	    double bincontNp1 = hist1d_width_prof->GetBinContent(bin+1);
// 	    double time       = hist1d_width_prof->GetBinCenter(bin);
// 	    double width      = hist1d_width_prof->GetBinWidth(bin);
	    
// 	    double Zn2 = bincontNp1*bincontNp1;
// 	    double Zn  = bincontN*bincontN;
// 	    double phi = TMath::TwoPi()*bin;
// 	    double M11 = TMath::Cos(bin*Psi0) - (phi/beta0min)*TMath::Sin(bin*Psi0);
// 	    double M12 = beta0min * (1 + (phi/beta0min)*(phi/beta0min)) * TMath::Sin(Psi0);  

// 	    cout << bin << "\t" << phi << "\t" << Zn2 << "\t" << Zn << "M11,M12\t" << M11 << "\t" << M12 << endl;
// 	    //double theta2 = (bincontNp1*bincontNp1 - (M11_2*M11_2)*bincontN*bincontN)/(M12_2*M12_2);
// 	    //cout << theta2 << "\t" << endl; //theta2/(R_magic()*R_magic()) << endl;
// 	    continue;
// 	  }
	}
      }
    }
  }

  if ( plot_phasespace_time && plot_ringtrackers ) {
    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    //Ring2Dhists[NRing2Dhists++] = "Y2";
    Ring2Dhists[NRing2Dhists++] = "Rhat";
    
    int NRing1Dhists = 0;
    string Ring1Dhists[20];
    //Ring1Dhists[NRing1Dhists++] = "";


    string Ringbase = "Tracker_";
    string G4base = "G4Track_";
    string truth_part_name = "DecayMuon";
    for ( int type = 0; type < 1; type++ ) {
      for ( int t = 0; t < Ntimestamps; t++ ) {
      string timestamp = timestamps[t];
      
      for ( int n = 0; n < NRing2Dhists; n++ ) {
	
	string histname = Ring2Dhists[n];
	
	//if ( !PlotVariable(histname, timestamp) ) { continue; }

	if ( type == 0 ) {
	  hname << Ringbase << histname << "_vs_" << timestamp;
	}
	if ( type == 1 ) {
	  hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	}
	TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist ) { hname.str(""); continue; }
	if ( timestamp == "TimeSingleHit" && 0 ) {
	  string tmp_timestamp = "TimeSingleHit";
	  hname.str("");
	  if ( type == 0 ) {
	    hname << Ringbase << histname << "_vs_" << tmp_timestamp;
	  }
	  if ( type == 1 ) {
	    hname << G4base << histname << "_" << truth_part_name << "_vs_" << tmp_timestamp;
	  }
	  TH2D *hist_tmp = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  if ( !hist_tmp ) { hname.str(""); continue; }
	  hist->Add(hist_tmp);
	}

	for ( int np = 0; np < 2; np++ ) {
	  if ( np == 0 ) { 
	    mintime_for_plot = 50;
	    maxtime_for_plot = (int)(maxtime);
	  }
	  if ( np == 1 ) {
	    mintime_for_plot = 0;
	    maxtime_for_plot = 90;	    
	  }
	  if ( np == 2 ) {
	    mintime_for_plot = 340;
	    maxtime_for_plot = 360;	    
	  }
	  if ( np == 3 ) {
	    mintime_for_plot = 150;
	    maxtime_for_plot = 600;	    
	  }
		
	  if ( mintime_for_plot > maxtime ) { continue; }
	  if ( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	  if ( mintime_for_plot >= maxtime_for_plot ) { continue; }
	  
	  hname.str("");
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	  hname << "_Full_Profile";
	  TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");
	  MakePlot1D(hist1d_prof, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	  hname << "_Mean_Profile";
	  TProfile *hist1d_mean_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "i");
	  MakePlot1D(hist1d_mean_prof, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");

	  //-----
	  // FFT
	  //-----
	  if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	  if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	  hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	  hname << "_Mean_Profile_x";
	  TH1D *hist1d_mean_prof_zoom = (TH1D*)hist1d_mean_prof->Clone(hname.str().c_str());
	  for ( int bin = 1; bin <= hist1d_mean_prof->GetNbinsX(); bin++ ) {
	    if ( bin < hist1d_mean_prof->FindBin(mintime_for_plot) || bin > hist1d_mean_prof->FindBin(maxtime_for_plot) ) {
	      hist1d_mean_prof_zoom->SetBinContent(bin, 0.0);
	    }
	  }
	  ComputeFFT(hist1d_mean_prof_zoom, hname.str());
	  hname.str("");
	  
	  
	  if ( histname == "Rhat" ) {
	    if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	    if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	    hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	    hname << "_Width_Profile";
	    TH1D *hist1d_width_prof = (TH1D*)(hist1d_prof->Clone(hname.str().c_str()));	
	    double max = 0.0;
	    for ( int bin = 1; bin <= hist1d_width_prof->GetNbinsX(); bin++ ) {
	      double cont = hist1d_prof->GetBinError(bin);
	      hist1d_width_prof->SetBinContent(bin, cont);
	      if ( cont > max ) { max = cont; }
	    }
	    MakePlot1D(hist1d_width_prof, -1, -1, &int_prev, &int_curr, &int_start);
	    hname.str("");
	    
	    
	    //-----
	    // FFT
	    //-----
	    if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	    if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	    hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	    hname << "_Width_Profile_x";
	    TH1D *hist1d_width_prof_zoom = (TH1D*)hist1d_width_prof->Clone(hname.str().c_str());
	    for ( int bin = 1; bin <= hist1d_width_prof->GetNbinsX(); bin++ ) {
	      if ( bin < hist1d_width_prof->FindBin(mintime_for_plot) || bin > hist1d_width_prof->FindBin(maxtime_for_plot) ) {
		hist1d_width_prof_zoom->SetBinContent(bin, 0.0);
	      }
	    }
	    ComputeFFT(hist1d_width_prof_zoom, hname.str());
	    hname.str("");
	  }
	}
	mintime_for_plot = -1;
	maxtime_for_plot = -1;
	
	
	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Full_Profile";
	TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");
	MakePlot1D(hist1d_prof, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");


	//------
	// FFT
	//------
	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Full_Profile";
	hname << "_x";
	TH1D *hist1d_prof_fft = (TH1D*)(hist1d_prof->Clone(hname.str().c_str()));
	ComputeFFT(hist1d_prof_fft, hname.str());
	hname.str("");




	saveC = false;

	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Mean_Profile";
	TProfile *hist1d_mean_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "i");
	MakePlot1D(hist1d_mean_prof, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");


	//------
	// FFT
	//------
	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Mean_Profile";
	hname << "_x";
	TH1D *hist1d_mean_prof_fft = (TH1D*)(hist1d_mean_prof->Clone(hname.str().c_str()));
	ComputeFFT(hist1d_mean_prof_fft, hname.str());
	hname.str("");


	if ( histname == "Rhat" ) {
	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Width_Profile";
	TH1D *hist1d_width_prof = (TH1D*)(hist1d_prof->Clone(hname.str().c_str()));	
	double max = 0.0;
	for ( int bin = 1; bin <= hist1d_width_prof->GetNbinsX(); bin++ ) {
	  //cout << bin << "\t" << hist1d_prof->GetBinError(bin) << endl;
	  double cont = hist1d_prof->GetBinError(bin);
	  hist1d_width_prof->SetBinContent(bin, cont);
	  if ( cont > max ) { max = cont; }
	}
	hist1d_width_prof->SetMaximum(max);
	hist1d_width_prof->SetMinimum(0.0);
	MakePlot1D(hist1d_width_prof, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");


	//------
	// FFT
	//------
	hname.str("");
	if ( type == 0 ) { hname << Ringbase << histname << "_vs_" << timestamp; }
	if ( type == 1 ) { hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp; }
	hname << "_Width_Profile";
	hname << "_x";
	TH1D *hist1d_width_prof_fft = (TH1D*)(hist1d_width_prof->Clone(hname.str().c_str()));
	ComputeFFT(hist1d_width_prof_fft, hname.str());
	hname.str("");
	}

	saveC = false;
      }

      


      for ( int n = 0; n < NRing1Dhists; n++ ) {
	  
	string histname = Ring1Dhists[n];

	if ( !PlotVariable(histname, timestamp) ) { continue; }

	hname << Ringbase << histname << "_vs_" << timestamp;

	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist1d ) { hname.str(""); continue; }
	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");

      }
    }
  }
  }
  else {
    cout << endl;
    cout << "Not plotting phase space in time." << endl;
    cout << endl;
  }





  //------------------------------------
  //
  // Evolution of Phase Space (Ring) w/ Snaphots
  //
  //------------------------------------
  cout << plot_ringtrackers_1stturn << "\t" <<  plot_ringtrackers << endl;  
  if ( plot_ringtrackers_1stturn && plot_ringtrackers ) {
    string itnames[12] = {"0deg", "30deg", "60deg", "90deg", "120deg", "150deg", "180deg", "210deg", "240deg", "270deg", "300deg", "330deg"};
    int_prev = -1.0;
    zoom = false; zoom2 = false; zoom3 = false;

    string Ringbase = "RingTracker_Snapshot_";

    int NRingShapshothists = 0;
    string RingShapshothists[20];
    RingShapshothists[NRingShapshothists++] = "Rhat";
    RingShapshothists[NRingShapshothists++] = "Time";

    for ( int t = 0; t < 12; t++ ) {
      string timestamp = itnames[t];
      
      for ( int n = 0; n < NRingShapshothists; n++ ) {
	  
	string histname = RingShapshothists[n];
	cout << histname << endl;
	if ( !PlotVariable(histname, timestamp) ) { continue; }

	hname << Ringbase << timestamp << "_" << histname;
	
	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist1d ) { hname.str(""); continue; }
	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");   
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting ring shapshot information." << endl;
    cout << endl;
  }
  
  //return;

 





  //------------------------------------
  //
  // Summary of Ring Hits
  //
  //------------------------------------
  if ( plot_ringtrackers ) {
    hname << "RingTracker_Nhits";
    TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
    MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);
    hname.str("");

    hname << "Nparticles";
    TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
    MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);
    hname.str("");
  }
    


  //------------------------------------
  //
  // Evolution of Phase Space (Ring)
  //
  //------------------------------------
  if ( plot_ringtrackers_evolution && plot_ringtrackers ) {

    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    Ring2Dhists[NRing2Dhists++] = "Rhat";
    //Ring2Dhists[NRing2Dhists++] = "Y";
    Ring2Dhists[NRing2Dhists++] = "Prhat";
    Ring2Dhists[NRing2Dhists++] = "Pvhat";
    Ring2Dhists[NRing2Dhists++] = "Mom";
    Ring2Dhists[NRing2Dhists++] = "Pol";
    Ring2Dhists[NRing2Dhists++] = "EqRad";
    Ring2Dhists[NRing2Dhists++] = "Zhat";

    int NRing1Dhists = 0;
    string Ring1Dhists[20];
    Ring1Dhists[NRing1Dhists++] = "";


    string Ringbase = "Tracker_";
    for ( int t = 0; t < Ntimestamps; t++ ) {
      string timestamp = timestamps[t];
      
      for ( int n = 0; n < NRing2Dhists; n++ ) {
	  
	string histname = Ring2Dhists[n];

	if ( !PlotVariable(histname, timestamp) ) { continue; }

	hname << Ringbase << histname << "_vs_" << timestamp;

	TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist ) { hname.str(""); continue; }

	if ( 1 ) {
	  if ( histname == "Rhat" || histname == "Y" || histname == "Mom" ) {
	    for ( int np = 0; np < 3; np++ ) {
	      if ( timestamp == "Time" ) {
		if ( np == 0 ) {
		  mintime_for_plot = 0;
		  maxtime_for_plot = 5;
		}
		if ( np == 1 ) {
		  mintime_for_plot = 25;
		  maxtime_for_plot = 30;
		}
		if ( np == 2 ) {
		  mintime_for_plot = 12;
		  maxtime_for_plot = 17;
		}
		
		if( mintime_for_plot > maxtime ) { continue; }
		if( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
		if ( mintime_for_plot >= maxtime_for_plot ) { continue; }

		hname.str("");
		hname << Ringbase << histname << "_vs_" << timestamp;
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";

		TH2D *hist_maxturns = (TH2D*)hist->Clone(hname.str().c_str());
		MakePlot(hist_maxturns, -1, -1, &int_prev, &int_curr, &int_start);
		hname.str("");
	      }
	    }
	  }	
	  mintime_for_plot = -1.0;
	  maxtime_for_plot = -1.0;
	}

	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");

      }



      for ( int n = 0; n < NRing1Dhists; n++ ) {
	  
	string histname = Ring1Dhists[n];

	if ( !PlotVariable(histname, timestamp) ) { continue; }

	hname << Ringbase << histname << "_vs_" << timestamp;

	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	if ( !hist1d ) { hname.str(""); continue; }

	if ( 1 ) {
	  if ( histname == "Num" ) {
	    for ( int np = 0; np < 3; np++ ) {
	      if ( timestamp == "Time" ) {
		if ( np == 0 ) {
		  mintime_for_plot = 0;
		  maxtime_for_plot = 5;
		}
		if ( np == 1 ) {
		  mintime_for_plot = 25;
		  maxtime_for_plot = 30;
		}
		if ( np == 2 ) {
		  mintime_for_plot = 12;
		  maxtime_for_plot = 17;
		}
		
		if( mintime_for_plot > maxtime ) { continue; }
		if( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	  if ( mintime_for_plot >= maxtime_for_plot ) { continue; }

		hname.str("");
		hname << Ringbase << histname << "_vs_" << timestamp;
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
		
		TH1D *hist1d_maxturns = (TH1D*)hist1d->Clone(hname.str().c_str());
		MakePlot1D(hist1d_maxturns, -1, -1, &int_prev, &int_curr, &int_start);
		hname.str("");
	      }	
	    }
	  }
	  mintime_for_plot = -1.0;
	  maxtime_for_plot = -1.0;
	}

	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");

      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting ring evolution phase space distributions." << endl;
    cout << endl;
  }
  



  
  //------------------------------------
  //
  // Generated Phase Space (Ring)
  //
  //------------------------------------
  if ( plot_ringtrackers_phasespace && plot_ringtrackers ) {

    int NRinghists = 0;
    string Ringhists[20];
    Ringhists[NRinghists++] = "Rhat";
    Ringhists[NRinghists++] = "EqRad";
    Ringhists[NRinghists++] = "Vhat";
    Ringhists[NRinghists++] = "Xprime";
    Ringhists[NRinghists++] = "Yprime";
    Ringhists[NRinghists++] = "Mom";
    Ringhists[NRinghists++] = "Energy";
    Ringhists[NRinghists++] = "Pol";
    //Ringhists[NRinghists++] = "PolX";
    //Ringhists[NRinghists++] = "PolY";
    Ringhists[NRinghists++] = "Zhat";

    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    //Ring2Dhists[NRing2Dhists++] = "EqRadMom";
    Ring2Dhists[NRing2Dhists++] = "RhatY";
    Ring2Dhists[NRing2Dhists++] = "PolXY";
    Ring2Dhists[NRing2Dhists++] = "XprimeX";
    Ring2Dhists[NRing2Dhists++] = "YprimeY";

    string Ringbase = "RingTracker_";


    for ( int t = 0; t < Nringtrackertimes; t++ ) {
      string time = times[t];      
      for ( int n = 0; n < NRinghists; n++ ) {
	string histname = Ringhists[n];

	if ( !PlotVariable(histname, timestamp) ) { continue; }


	string timeval  = timevals[kGeneratedDist];
	
	hname << Ringbase << histname << "_" << time << "_" << timeval;
	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");

	timeval  = timevals[kRemainingDist];
	
	hname << Ringbase << histname << "_" << time << "_" << timeval;
	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");
      }

      for ( int n = 0; n < NRing2Dhists; n++ ) {
	string histname = Ring2Dhists[n];

	string timeval  = timevals[kGeneratedDist];
	
	hname << Ringbase << histname << "_" << time << "_" << timeval;
	TH2D *hist2d = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist2d, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");

	timeval  = timevals[kRemainingDist];
	
	hname << Ringbase << histname << "_" << time << "_" << timeval;
	TH2D *hist2d = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist2d, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting time averaged phase space distributions." << endl;
    cout << endl;
  }
    



  //------------------------------------
  //
  // Generated Phase Space (G4)
  //
  //------------------------------------
  TH1D *hDecayBirthRatio[20][4];
  TH2D *hDecayBirthRatio2D[20][4];
  for ( int i = 0; i < 20; i++ ) {
    for ( int j = 0; j < 4; j++ ) {
      hDecayBirthRatio[i][j] = NULL;    
      hDecayBirthRatio2D[i][j] = NULL;
    }
  }

  if ( plot_truth_phasespace && plot_truth ) {

    int NG4hists = 0;
    string G4hists[30];
    G4hists[NG4hists++] = "Xe";
    G4hists[NG4hists++] = "R";
    G4hists[NG4hists++] = "Rhat";
    G4hists[NG4hists++] = "Vhat";
    G4hists[NG4hists++] = "Xprime";
    G4hists[NG4hists++] = "Yprime";
    G4hists[NG4hists++] = "Mom";
    G4hists[NG4hists++] = "Energy";
    G4hists[NG4hists++] = "Pol";
    G4hists[NG4hists++] = "PolX";
    G4hists[NG4hists++] = "PolY";
    G4hists[NG4hists++] = "t0";
    G4hists[NG4hists++] = "NumStraw";
    G4hists[NG4hists++] = "NumStraw_with_SystemHits";
    G4hists[NG4hists++] = "NumStraw_without_SystemHits";
    G4hists[NG4hists++] = "NumCalo";
    G4hists[NG4hists++] = "NumCalo_with_SystemHits";
    G4hists[NG4hists++] = "NumCalo_without_SystemHits";
    G4hists[NG4hists++] = "Time";
    //G4hists[NG4hists++] = "Theta";
    G4hists[NG4hists++] = "DriftTime";
    G4hists[NG4hists++] = "DeltaP";
    G4hists[NG4hists++] = "DeltaR";
    G4hists[NG4hists++] = "SystemHits";
    //NG4hists=0;

    int NG42Dhists = 0;
    string G42Dhists[30];
    G42Dhists[NG42Dhists++] = "XZ";
    G42Dhists[NG42Dhists++] = "PolXY";
    G42Dhists[NG42Dhists++] = "Polt0";    
    G42Dhists[NG42Dhists++] = "RhatY";
    G42Dhists[NG42Dhists++] = "XprimeX";
    G42Dhists[NG42Dhists++] = "YprimeY";
    G42Dhists[NG42Dhists++] = "ThetaRhat";
    G42Dhists[NG42Dhists++] = "ThetaR";
    G42Dhists[NG42Dhists++] = "Rhat_vs_DeltaP";
    G42Dhists[NG42Dhists++] = "Rhat_vs_DeltaP";
    G42Dhists[NG42Dhists++] = "DeltaR_vs_DeltaP";
    G42Dhists[NG42Dhists++] = "CaloNum_vs_Energy";
    G42Dhists[NG42Dhists++] = "CaloNum_vs_Theta";
    G42Dhists[NG42Dhists++] = "StrawNum_vs_Energy";
    G42Dhists[NG42Dhists++] = "StrawNum_vs_Theta";
    G42Dhists[NG42Dhists++] = "NumStrawStations_vs_Energy";
    G42Dhists[NG42Dhists++] = "DriftTime_vs_DeltaR";
    //NG42Dhists = 0;

    //   TH1D *h_G4Tracker_NumCalo[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_NumCalo_with_SystemHits[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_NumCalo_without_SystemHits[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_NumCalo_with_Mom[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_DriftTime[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_DriftTime_vs_DeltaR[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_DeltaR[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_DeltaR_vs_Mom[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_CaloNum_vs_Mom[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_CaloNum_vs_Theta[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_Theta_with_SystemHits[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_Theta_without_SystemHits[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_DeltaTheta[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_DeltaTheta_vs_Mom[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_DeltaR_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_CaloNum_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];
    //   TH1D *h_G4Tracker_DeltaP[Ntruthtimestamps][24][MaxNparticles];
    //   TH2D *h_G4Tracker_Rhat_vs_DeltaP[Ntruthtimestamps][24][MaxNparticles];
    string G4base = "G4Track_";

    for ( int p = 0; p < Nparticles; p++ ) {
      string truth_part_name = truth_particle_names[p];
      
      //if ( plot_Nud == false && truth_part_name.find("DecayElectron") != string::npos ) { continue; }
      
      for ( int t = 0; t < Ntimevals; t++ ) {
	string timeval = timevals[t];

	for ( int n = 0; n < NG4hists; n++ ) {

	  string histname = G4hists[n];

	  if ( !PlotVariable(histname, truth_part_name, timeval) ) { continue; }
	  
	  hname << G4base << histname << "_" << truth_part_name << "_" + timeval;
	  TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);

	  if ( histname == "Xe" && truth_part_name == "DecayMuon" && timeval == "GeneratedDist" ) {
	    PrintXe(hist1d);
	  }

	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	  if ( timeval.find("GeneratedDist") != string::npos ) {
	    if ( truth_part_name == "DecayElectronEgtHghEth" ) {
	      hDecayBirthRatio[n][3] = hist1d;
	    }
	    if ( truth_part_name == "DecayElectronEgtEth" ) {
	      hDecayBirthRatio[n][2] = hist1d;
	    }
	    if ( truth_part_name == "DecayElectron" ) {
	      hDecayBirthRatio[n][1] = hist1d;
	    }
	    if ( truth_part_name == "BirthElectron" ) {
	      hDecayBirthRatio[n][0] = hist1d;
	    }
	  }
	}

	
	for ( int n = 0; n < NG42Dhists; n++ ) {

	  string histname = G42Dhists[n];

	  if ( !PlotVariable(histname, truth_part_name, timeval) ) { continue; }

	  hname << G4base << histname << "_" << truth_part_name << "_" << timeval;
	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);

	  if ( histname == "XprimeX" && truth_part_name == "DecayMuon" && (timeval == "GeneratedDist" ) ) {
	    PrintXprimeX(hist, timeval);
	  }

	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	  if ( timeval.find("GeneratedDist") != string::npos ) { 
	    if ( truth_part_name == "DecayElectronEgtHghEth" ) {
	      hDecayBirthRatio2D[n][3] = hist;
	    }
	    if ( truth_part_name == "DecayElectronEgtEth" ) {
	      hDecayBirthRatio2D[n][2] = hist;
	    }
	    if ( truth_part_name == "DecayElectron" ) {
	      hDecayBirthRatio2D[n][1] = hist;
	    }
	    if ( truth_part_name == "BirthElectron" ) {
	      hDecayBirthRatio2D[n][0] = hist;
	    }
	  }
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting truth phase space distributions." << endl;
    cout << endl;
  }


  //---------------------
  // Ratio of X'X and Y'Y for Straw Electron and Decay Electron
  //---------------------
  if ( plot_truth_phasespace && plot_truth ) {
  string particles[6];
  particles[1] = "StrawCaloElectron";
  particles[2] = "GoodStrawElectron";
  particles[3] = "GoodStrawCaloElectron";
  particles[4] = "GoodOneStrawCaloElectron";
  particles[5] = "GoodTwoStrawCaloElectron";
  particles[0] = "DecayElectron";

  string histnames[3];
  histnames[0] = "XprimeX";
  histnames[1] = "YprimeY";
  histnames[2] = "RhatY";

  string histnames1d[5];
  histnames1d[0] = "Rhat";
  histnames1d[1] = "Vhat";
  histnames1d[2] = "Energy";
  histnames1d[3] = "Mom";
  histnames1d[4] = "SpinPhase";
  
  for ( int p = 0; p < 6; p++ ) {

    
    for ( int n = 0; n < 4; n++ ) {
      string histname = histnames1d[n];
      
      // Get Electron histogram
      string truth_part_name = particles[p];
      string timeval = "RemainingDist";
      
      stringstream hname;
      hname << G4base << histname << "_" << truth_part_name << "_" << timeval;
      TH1D *hist_electron1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      hname.str("");
      if ( !hist_electron1d ) { continue; }
      
      truth_part_name = "DecayMuon";
      if ( histname == "Energy" || histname == "Mom" ) { truth_part_name = "BirthElectron"; }
      timeval = "GeneratedDist";
      hname << G4base << histname << "_" << truth_part_name << "_" << timeval;
      TH1D *hist_muon1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      hname.str("");
      if ( !hist_muon1d ) { continue; }
      
      hname << hist_electron1d->GetName() << "_Ratio";    
      TH1D *hist_electron_clone1d = (TH1D*)hist_electron1d->Clone(hname.str().c_str());    
      hist_electron_clone1d->Sumw2();
      hist_muon1d->Sumw2();
      hist_electron_clone1d->Divide(hist_electron_clone1d, hist_muon1d, 1, 1, "B");
      MakePlot1D(hist_electron_clone1d, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");    
    }
      
      
    for ( int n = 0; n < 3; n++ ) {
      string histname = histnames[n];
      
      string truth_part_name = particles[p];
      string timeval = "RemainingDist";

      stringstream hname;
      hname << G4base << histname << "_" << truth_part_name << "_" << timeval;
      TH2D *hist_electron = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      hname.str("");
      if ( !hist_electron ) { continue; }
      
      truth_part_name = "DecayMuon";
      timeval = "GeneratedDist";
      hname << G4base << histname << "_" << truth_part_name << "_" << timeval;
      TH2D *hist_muon = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      hname.str("");
      if ( !hist_muon ) { continue; }
      
      hname << hist_electron->GetName() << "_Ratio";    
      TH2D *hist_electron_clone = (TH2D*)hist_electron->Clone(hname.str().c_str());    
      hist_electron_clone->Divide(hist_muon);
      MakePlot(hist_electron_clone, -1, -1, &int_prev, &int_curr, &int_start);
      hname.str("");    
    }
  }
  }






  //------------------------------------
  //
  // Evolution of Phase Space (G4)
  //
  //------------------------------------
  if ( plot_truth_evolution && plot_truth ) {

    int NG42Dhists = 0;
    string G42Dhists[20];
    G42Dhists[NG42Dhists++] = "R";
    G42Dhists[NG42Dhists++] = "Rhat";
    //G42Dhists[NG42Dhists++] = "Y";
    //G42Dhists[NG42Dhists++] = "Y2";
    G42Dhists[NG42Dhists++] = "Prhat";
    G42Dhists[NG42Dhists++] = "Pvhat";
    G42Dhists[NG42Dhists++] = "Mom";
    G42Dhists[NG42Dhists++] = "Energy";
    G42Dhists[NG42Dhists++] = "Pol";
    G42Dhists[NG42Dhists++] = "SpinPhase";
    G42Dhists[NG42Dhists++] = "PolX";
    G42Dhists[NG42Dhists++] = "PolY";
    //G42Dhists[NG42Dhists++] = "Xe";
    G42Dhists[NG42Dhists++] = "NumCalo";
    G42Dhists[NG42Dhists++] = "NumStraw";
    G42Dhists[NG42Dhists++] = "Theta";
    //NG42Dhists = 0;

    int NG4hists = 0;
    string G4hists[20];
    //G4hists[NG4hists++] = "NgtEth";
    if ( plot_nums ) {
      G4hists[NG4hists++] = "NumAllStations";
      G4hists[NG4hists++] = "NumAllStationsNoFastRotation";
    }
    //G4hists[NG4hists++] = "NudAllStations";
    if ( plot_Nud ) {
      G4hists[NG4hists++] = "Vhat";
      G4hists[NG4hists++] = "Yprime";
      G4hists[NG4hists++] = "Nud";
    }

    string G4base = "G4Track_";
    for ( int t = 0; t < Ntimestamps; t++ ) {
      string timestamp = timestamps[t];
      
      for ( int p = 0; p < Nparticles; p++ ) {
	string truth_part_name = truth_particle_names[p];

	for ( int n = 0; n < NG42Dhists; n++ ) {

	  string histname = G42Dhists[n];

	  if ( !PlotVariable(histname, truth_part_name, timestamp) ) { continue; }
	  
	  string histname = G42Dhists[n];
	  
	  hname.str("");
	  hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  if ( !hist ) { hname.str(""); continue; }

	  if ( histname == "Rhat" && truth_part_name == "DecayMuon" && timestamp == "TimeSingleHit" ) {
	    PrintRhatT0(hist, truth_part_name);
	  }

	  if ( histname == "Rhat" || histname == "Energy" || histname == "Y" ) {
	    for ( int np = 0; np < 2; np++ ) {
	      if ( timestamps[t].find("Time") != string::npos ) {
		if ( histname == "Energy" ) {
		  continue;
		  if ( np == 0 ) {
		    mintime_for_plot = 0;
		    maxtime_for_plot = 10;
		  }
		  if ( np == 1 ) {
		    mintime_for_plot = 100;
		    maxtime_for_plot = 110;
		  }
		}
		else {
		  if ( np == 0 ) {
		    mintime_for_plot = 10;
		    maxtime_for_plot = 20;
		  }
		  if ( np == 1 ) {
		    mintime_for_plot = 50;
		    maxtime_for_plot = 250;
		  }
		  if ( np == 2 ) {
		    mintime_for_plot = 0;
		    maxtime_for_plot = 2;
		  }
		}

		
		if( mintime_for_plot > maxtime ) { continue; }
		if( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	  if ( mintime_for_plot >= maxtime_for_plot ) { continue; }
		
		hname.str("");
		hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
		TH2D *hist_zoom = (TH2D*)(hist->Clone(hname.str().c_str()));
		MakePlot(hist_zoom, -1, -1, &int_prev, &int_curr, &int_start);
		hname.str("");
	      }
	    }
	  }


	  //------------------------------------------------------
	  // Do Profiles instead of 2D plots to average things out
	  //------------------------------------------------------
	  bool do_profile = true;
	  if ( do_profile ) {
	    mintime_for_plot = 0.0;
	    TH1D *xprof = (TH1D*)hist->ProjectionX();
	    double intx = xprof->Integral();
	    double intx5 = xprof->Integral(1, xprof->FindBin(5));
	    double intx10 = xprof->Integral(1, xprof->FindBin(10));
	    double intx50 = xprof->Integral(1, xprof->FindBin(50));
	    double intx100 = xprof->Integral(1, xprof->FindBin(100));
	    double intx200 = xprof->Integral(1, xprof->FindBin(200));
	    double intx300 = xprof->Integral(1, xprof->FindBin(300));
	    double intx400 = xprof->Integral(1, xprof->FindBin(400));
	    double intx500 = xprof->Integral(1, xprof->FindBin(500));
	    double intx600 = xprof->Integral(1, xprof->FindBin(600));
	    double intx700 = xprof->Integral(1, xprof->FindBin(700));
	    double intx800 = xprof->Integral(1, xprof->FindBin(800));
	    maxtime_for_plot = xprof->FindLastBinAbove(1);
	    if ( intx5/intx > 0.8 ) { maxtime_for_plot = 5; }
	    else if ( intx10/intx > 0.8 ) { maxtime_for_plot = 10; }
	    else if ( intx50/intx > 0.8 ) { maxtime_for_plot = 50; }
	    else if ( intx100/intx > 0.8 ) { maxtime_for_plot = 100; }
	    else if ( intx200/intx > 0.8 ) { maxtime_for_plot = 200; }
	    else if ( intx300/intx > 0.8 ) { maxtime_for_plot = 300; }
	    else if ( intx400/intx > 0.8 ) { maxtime_for_plot = 400; }
	    else if ( intx500/intx > 0.8 ) { maxtime_for_plot = 500; }
	    else if ( intx600/intx > 0.8 ) { maxtime_for_plot = 600; }
	    else if ( intx700/intx > 0.8 ) { maxtime_for_plot = 700; }
	    else if ( intx800/intx > 0.8 ) { maxtime_for_plot = 800; }
	    hname.str("");
	    hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	    hname << "_Full_Profile";
	    TProfile *hist1d_prof = hist->ProfileX(hname.str().c_str(), 1, 200000000, "s");
	    cout << "Plotting Profile!!!!" << endl;
	    MakePlot1D(hist1d_prof, -1, -1, &int_prev, &int_curr, &int_start);
	    hname.str("");

	    bool do_fft = true;
	    if ( do_fft ) {
	      if ( histname == "Rhat" || histname == "Y" || histname == "Y2" || histname == "Pol" ) {
		hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
		hname << "_Full_Profile";
		hname << "_x";
		TH1D *hist1d_fft = (TH1D*)hist1d_prof->Clone(hname.str().c_str());
		ComputeFFT(hist1d_fft, hname.str());
		hname.str("");
	      }
	    }
	  }
	  else {
	    maxtime_for_plot = -1.0;
	    mintime_for_plot = -1.0;
	    hname.str("");	  
	    hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	    hname << "_Full_Profile";
	    TH2D *hist_copy = (TH2D*)hist->Clone(hname.str().c_str());
	    MakePlot(hist_copy, -1, -1, &int_prev, &int_curr, &int_start);
	    hname.str("");
	  }
	}


	for ( int n = 0; n < NG4hists; n++ ) {
	  string histname = G4hists[n];

	  if ( !PlotVariable(histname, truth_part_name, timestamp) ) { hname.str(""); continue; }

	  hname.str("");
	  hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	  TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  if ( !hist1d ) { hname.str(""); continue; }

	  if ( histname.find("Num") != string::npos && timestamps[t].find("Time") != string::npos ) {
	    double expbinwidth = 1.0;
	    for ( int np = 1; np < 3; np++ ) {
	      if ( maxtime <= 30 ) {
		if ( np == 0 ) {
		  mintime_for_plot = 1;
		  maxtime_for_plot = 2;
		  expbinwidth = 0.001;
		}
		if ( np == 1 ) {
		  mintime_for_plot = 10;
		  maxtime_for_plot = 20;
		  expbinwidth = 0.15;
		}
		if ( np == 2 ) {
		  mintime_for_plot = 11;
		  maxtime_for_plot = 12;
		  expbinwidth = 0.001;
		}
	      }
	      if ( maxtime <= 150 ) {
		if ( np == 0 ) {
		  mintime_for_plot = 1;
		  maxtime_for_plot = 2;
		  expbinwidth = 0.002;
		}
		if ( np == 1 ) {
		  mintime_for_plot = 0;
		  maxtime_for_plot = 20;
		  expbinwidth = 0.15;
		}
		if ( np == 2 ) {
		  mintime_for_plot = 50;
		  maxtime_for_plot = (int)(maxtime);
		  expbinwidth = 0.15;
		}
	      }
	      else {
		if ( np == 0 ) {
		  mintime_for_plot = 1;
		  maxtime_for_plot = 2;
		  expbinwidth = 0.002;
		}
		if ( np == 1 ) {
		  mintime_for_plot = 0;
		  maxtime_for_plot = 90;
		  expbinwidth = 0.15;
		}
		if ( np == 2 ) {
		  mintime_for_plot = 50;
		  maxtime_for_plot = (int)(maxtime);
		  expbinwidth = 0.15;
		}
	      }


	      if ( mintime_for_plot > maxtime ) { continue; }
	      if ( maxtime_for_plot > maxtime ) { maxtime_for_plot = maxtime; }
	      if ( mintime_for_plot >= maxtime_for_plot ) { continue; }
	      
	      if ( timestamp == "TimeOncePerTurn" ) { expbinwidth = 0.025; }
	      if ( timestamp == "TimeSingleHit" ) { expbinwidth = 0.15; }
	      
	      //-----
	      // FFT
	      //-----
	      hname.str("");
	      hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	      
	      if ( timestamps[t].find("Time") == string::npos ) {
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "Turns";
	      }
	      else {
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	      }

	      hname << "_x";
	      TH1D *hist1d_zoom_fft = (TH1D*)hist1d->Clone(hname.str().c_str());
	      for ( int bin = 1; bin <= hist1d->GetNbinsX(); bin++ ) {
		if ( bin < hist1d->FindBin(mintime_for_plot) || bin > hist1d->FindBin(maxtime_for_plot) ) {
		  hist1d_zoom_fft->SetBinContent(bin, 0.0);
		}
	      }

	      ComputeFFT(hist1d_zoom_fft, hname.str());
	      hname.str("");

	      
	      hname.str("");
	      hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	      
	      if ( timestamps[t].find("Time") == string::npos ) {
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "Turns";
	      }
	      else {
		hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	      }

	      TH1D *hist1d_zoom = (TH1D*)(hist1d->Clone(hname.str().c_str()));
	      double binwidth = hist1d_zoom->GetBinWidth(1);
	      //cout << "ExpBinWidth = " << expbinwidth << endl;
	      //cout << "BinWidth    = " << binwidth << endl;
	      double ratio = expbinwidth / binwidth;
	      hist1d_zoom->Rebin(ratio);
	      double binwidth = hist1d_zoom->GetBinWidth(1);
	      //cout << "NewBinWidth = " << binwidth << endl;
	      cout << "NAME = " << hname.str() << endl;
	      MakePlot1D(hist1d_zoom, -1, -1, &int_prev, &int_curr, &int_start);
	      hname.str("");
	    }
	  }
	  maxtime_for_plot = -1.0;
	  mintime_for_plot = -1.0;
	  


	  //-----
	  // FFT
	  //-----
	  hname.str("");
	  hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	  hname << "_x";
	  TH1D *hist1d_fft = (TH1D*)hist1d->Clone(hname.str().c_str());
	  ComputeFFT(hist1d_fft, hname.str());
	  hname.str("");



	  hname.str("");
	  //hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp << "_x";
	  //TH1D *hist1d_fft = (TH1D*)(hist1d->Clone(hname.str().c_str()));

	  if ( histname.find("Num") != string::npos || histname.find("Nud") != string::npos ) { 
	    ;//hist1d->Rebin(7);
	  }

	  double expbinwidth = 0.15;
	  double binwidth = hist1d->GetBinWidth(1);
	  if ( timestamp == "TimeOncePerTurn" ) { expbinwidth = 0.025; }
	  if ( timestamp == "TimeSingleHit" ) { expbinwidth = 0.15; }

	  double ratio = expbinwidth / binwidth;
	  cout << "Rebining by " << ratio << endl;
	  hist1d->Rebin(ratio);
	  double binwidth = hist1d->GetBinWidth(1);
	  cout << "NewBinWidth = " << binwidth << endl;
	  hname.str("");
	  hname << G4base << histname << "_" << truth_part_name << "_vs_" << timestamp;
	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);
	}

	// Special plots for Decay Electron
	if ( truth_part_name.find("DecayElectron") != string::npos &&
	     timestamp == "TimeOncePerTurn" ) {  
	  maxtime_for_plot = 15.0;
	  mintime_for_plot = 10.0;
	  for ( int s = 0; s < 24; s++ ) {
	    stringstream histname_ss;
	    histname_ss << "NumCaloStation" << s;

	    if ( !PlotVariable(histname_ss.str(), truth_part_name, timestamp) ) { continue; }
	    
	    hname.str("");
	    hname << G4base << histname_ss.str() << "_" << truth_part_name << "_vs_" << timestamp;
	    TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	    if ( !hist1d ) { hname.str(""); continue; }	   

	    hname << "_" << mintime_for_plot << "_" << maxtime_for_plot << "us";
	    TH1D *hist1d_zoom = (TH1D*)(hist1d->Clone(hname.str().c_str()));
	    MakePlot1D(hist1d_zoom, -1, -1, &int_prev, &int_curr, &int_start);
	    hname.str("");
	  }
	  maxtime_for_plot = -1.0;
	  mintime_for_plot = -1.0;
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting truth evolution phase space distributions." << endl;
    cout << endl;
  }

  
  



  //------------------------------------
  //
  // System Hits
  //
  //------------------------------------
  int Ninames = 22;
  string inames[22];
  inames[0] = "UpstreamEndFlange";
  inames[1] = "UpstreamEquivalentNbTi";  
  inames[2] = "UpstreamEquivalentAl";  
  inames[3] = "UpstreamEquivalentCu";  
  inames[4] = "UpstreamWindow";  
  inames[5] = "DownstreamWindow";  
  inames[6] = "DownstreamEquivalentNbTi";  
  inames[7] = "DownstreamEquivalentAl";  
  inames[8] = "DownstreamEquivalentCu";  
  inames[9] = "DownstreamEndFlange";
  inames[10] = "Mandrel";  
  inames[11] = "Quad10";  
  inames[12] = "Quad20";  
  inames[13] = "Quad30";  
  inames[14] = "Quad40";  
  inames[15] = "Quad11";  
  inames[16] = "Quad21";  
  inames[17] = "Quad31";  
  inames[18] = "Quad41";  
  inames[19] = "Kicker1";  
  inames[20] = "Kicker2";  
  inames[21] = "Kicker3";

  int Nsysnames = 0;
  string rhitnames[11];
  rhitnames[Nsysnames++] = "Inflector";
  rhitnames[Nsysnames++] = "Cryostat" ;
  rhitnames[Nsysnames++] = "Quad" ;
  rhitnames[Nsysnames++] = "Kicker" ;
  rhitnames[Nsysnames++] = "Collimator" ;
  //   rhitnames[Nsysnames++] = "Vacuum" ;
  //   rhitnames[Nsysnames++] = "StrawTracker" ;
  //   rhitnames[Nsysnames++] = "Calo" ;
  //   rhitnames[Nsysnames++] = "Xtal";
  //   rhitnames[Nsysnames++] = "Arc";
  rhitnames[Nsysnames++] = "AllSystems";


  int NSystem2Dhists = 0;
  string System2Dhists[20];
  System2Dhists[NSystem2Dhists++] = "XZ";
  System2Dhists[NSystem2Dhists++] = "RhatY";


  int NSystem1Dhists = 0;
  string System1Dhists[20];
  System1Dhists[NSystem1Dhists++] = "Nhits";
  
  if ( plot_detectors ) {
  
    int_prev = -1.0;
    zoom = false; zoom2 = false; zoom3 = false;

    for ( int i = 0; i < Nsysnames; i++ ) {
      for ( int st = 0; st < 1; st++ ) {
	string stname = "";
      
	for ( int n = 0; n < NSystem2Dhists; n++ ) {
	  hname << rhitnames[i] << "Hits" << stname << "_" << System2Dhists[n];
	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	}

	for ( int n = 0; n < NSystem1Dhists; n++ ) {
	  if ( System1Dhists[n] == "Nhits" ) {
	    hname << rhitnames[i] << stname << "_" << System1Dhists[n];
	  }
	  else {
	    hname << rhitnames[i] << "Hits" << stname << "_" << System1Dhists[n];
	  }
	  TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      }
    }
	
    // 	hname << rhitnames[i] << stname << "_Nhits";
    // 	TH1D *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
    // 	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
    // 	hname.str("");

    // 	if ( 0 ) {
    // 	  hname << rhitnames[i] << "Hits" << stname << "_RhatTime";
    // 	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    // 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    // 	  hname.str("");

    // 	  zoom = true; zoom2 = false; zoom3 = false;
    // 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    // 	  hname.str("");

    // 	  zoom = false; zoom2 = false; zoom3 = false;

    // 	  hname << rhitnames[i] << "Hits" << stname << "_YTime";
    // 	  TH2D *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    // 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    // 	  hname.str("");

    // 	  zoom = true; zoom2 = false; zoom3 = false;
    // 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    // 	  hname.str("");

    // 	  zoom = false; zoom2 = false; zoom3 = false;
    // 	}
    //       }
  }
  else {
    cout << endl;
    cout << "Not plotting detector hit information." << endl;
    cout << endl;
  }
}
