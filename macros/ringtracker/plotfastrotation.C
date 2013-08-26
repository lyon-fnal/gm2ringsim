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

double minturns_for_plot = -1.0;
double maxturns_for_plot = -1.0;

double eff_inf, err_inf;
double eff_turn1, err_turn1;
double eff_turn10, err_turn10;
double eff_turn100, err_turn100;
double eff_turn, err_turn;

double dPoverP = 0.0015;
int Nrad = 51;
double lambda;

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
bool plotringeff = false;

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
TH2F *HitsXZ[21];
TH2F *HitsRhatY[21];

double GetLineWidth(TString name)
{
  double size = 1;

  return( size );
}

double GetMarkerSize(TString name)
{
  double size = 0.1;

  return( size );
}


void GetXminmax(TH1F *hist, double *xmin, double *xmax, double *ymin, double *ymax, double histxmin, double histxmax, double histymin, double histymax, bool dolog)
{
  bool isTurn = false;
  bool isRhat = false;
  bool isVhat = false;
  bool isXprime = false;
  bool isYprime = false;
  bool isN = false;
  bool isNavg = false;
  bool isPol = false;
  bool isXe = false;
  bool isMom = false;
  bool isFFT = false;
  bool isZhat = false;
  bool isR = false;

  TString name = hist->GetName();
  
  *xmin = histxmin;
  *xmax = histxmax;
  *ymax = 1.5*histymax;
  *ymin = 1.5*histymin;

  if ( dolog ) { *ymin = *ymax / hist->GetEntries(); }

  if ( *ymax < 2 ) { *ymax = 2.0; }

  return;
  
  cout << name << " has no limits." << endl;
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

void FitOscillation(TH1F *hist1d, string Fitname)
{
  if ( hist1d == NULL ) { return; }
  cout << "=====================================" << endl;
  cout << "Running Fit for " << hist1d->GetName() << endl;
  cout << "=====================================" << endl;

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

  TH1F *hist1d_x_tmp = (TH1F*)fCount->GetHistogram();
  TH1F *hist1d_x = (TH1F*)(hist1d_x_tmp->Clone(Fitname.c_str()));
  //Fitname += "_Fit";
  //TH1F *hist1d_x = (TH1F*)hist1d->Clone(Fitname.c_str());
  MakePlot(hist1d_x, -1, -1);
}


void ComputeFFT(TH1F *hist1d, string FFTname)
{
  if ( hist1d == NULL ) { return; }
  cout << "=====================================" << endl;
  cout << "Running FFT for " << hist1d->GetName() << endl;
  cout << "=====================================" << endl;

  TString name = hist1d->GetName();
//   if ( !name.Contains("OncePerTurn") ) {
//     hist1d->Rebin(12);
//   }


  
  if ( 1 ) {
    cout << "Bins = " << hist1d->GetNbinsX() << endl;
  }
  double max = -9.9;
  double maxbin = -1;
  double range = hist1d->GetBinLowEdge(hist1d->GetNbinsX()-1) + hist1d->GetBinWidth(1) - hist1d->GetBinLowEdge(1);
  TH1F *h_wavelength = 0;
  TH1F *h_mag = 0;
  TH1F *h_ph = 0;
  cout << "Range = " << range << endl;

  FFTname += "_FFT";
  int nSamples = hist1d->GetNbinsX();
  // get the magnitude (i.e., power) of the transform of f(x)
  TH1* h1dMagRaw = NULL;
  h1dMagRaw = hist1d->FFT(h1dMagRaw, "MAG"); // this has units of 1/f_max
  TH1F *h1dMag = new TH1F(FFTname.c_str(), "Magnitude (i.e., Power Spectrum)", h1dMagRaw->GetNbinsX(), 0, h1dMagRaw->GetXaxis()->GetXmax()/hist1d->GetXaxis()->GetXmax()); 
  if ( name.Contains("Turn") && !name.Contains("TimeOncePerTurn")) {
    h1dMag->SetXTitle("Frequency [Turn^{-1}]");
  }
  else {
    h1dMag->SetXTitle("Frequency [#mus^{-1}]");
  }
  // rescale axis to get real units
  double freq_min = 0.0;
  double freq_max = 0.0;
  for (Int_t bin = 1; bin <= nSamples; bin++){
    h1dMag->SetBinContent(bin, h1dMagRaw->GetBinContent(bin));
  }
  
  cout << "Bins = " << h1dMag->GetNbinsX() << endl;
  cout << "Xmax = " << h1dMag->GetBinCenter(h1dMag->GetNbinsX()) << endl;
  MakePlot(h1dMag, -1, -1);
  

  
  if ( 0 ) {
    h_wavelength = new TH1F(FFTname.c_str(), "", hist1d->GetNbinsX(), 0.0, hist1d->GetNbinsX()/range);
    //h_wavelength->SetXTitle("Radial Betatron Frequency [1/#lambda_{x}]");
    string magname = FFTname + "_MagX";
    h_mag = (TH1F*)h_FFT_Mag->Clone(magname.c_str());
    string phname = FFTname + "_PhX";
    h_ph = (TH1F*)h_FFT_Ph->Clone(phname.c_str());

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
    TH1F *h_frequency = new TH1F(freqname.c_str(), "", hist1d->GetNbinsX(), freq_min, freq_max);
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

    MakePlot(h_wavelength, -1, -1);
    MakePlot(h_frequency, -1, -1);
    MakePlot(h_mag, -1, -1);
    MakePlot(h_ph, -1, -1);
  }
  cout << "=====================================" << endl;
  cout << endl;
}

TString Nicename(TString name)
{
  if ( name == "EqRad_FastRotation" ) { return( "Fast Rotation" ); }
  if ( name == "EqRad_GEANT" ) { return ( "GEANT4 R_{avg}" ); }
  if ( name == "EqRad_THEORY" ) { return ( "R = #frac{#delta p}{p} #frac{1}{1-n} #times R_{m}" ); }
  
  if ( name == "FastRotation" ) { return( "Fast Rotation" ); }
  if ( name == "GEANT" ) { return ( "GEANT4 R_{avg}" ); }
  if ( name == "THEORY" ) { return ( "R = #frac{#delta p}{p} #frac{1}{1-n} #times R_{m}" ); }
  
  cout << "Nice name not found for : " << name << endl;
  return( name );
}

void MakePlot(TGraph *gr, TString hname)
{
  if ( gr == NULL ) { return; }

  //if ( hist1d->GetEntries() <= 0 ) { cout << "Histogram [" << hist1d->GetName() << "] is empty" << endl; return; }


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


  //
  // Get HistXmin/max
  //
  int n = gr->GetN();
  Double_t *x = gr->GetX();
  Double_t *y = gr->GetY();

  for ( int i = 0; i < n; i++ ) {
    if ( x[i] > histxmax ) { histxmax = x[i]; }
    if ( y[i] > histymax ) { histymax = y[i]; }
    if ( x[i] < histxmin ) { histxmin = x[i]; }
    if ( y[i] < histymin ) { histymin = y[i]; }
  }
  xmax = histxmax;
  xmin = histxmin;
  ymax = histymax;
  ymin = histymin;
  
  TString xtitle = gr->GetXaxis()->GetTitle();
  TString ytitle = gr->GetYaxis()->GetTitle();
  
  
  cout << "==========" << endl;
  cout << "Graph Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
  cout << "==========" << endl;

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->Draw();

  gr->SetLineColor(kBlue);
  gr->SetLineWidth(GetLineWidth(hname));
  gr->SetMarkerSize(GetMarkerSize(hname));
  gr->SetMarkerColor(kBlue);

  if ( hname.Contains("t0") ) {
    gr->GetXaxis()->SetTitle("t_{0} (Pulse Width=#Delta) [ns]");
    gr->GetYaxis()->SetTitle("Counts / ns [ns^{-1}]");
  }
  if ( hname.Contains("t0weight") ) {
    gr->GetXaxis()->SetTitle("t_{0}  Bin (i#Delta/N_{t0})");
    gr->GetYaxis()->SetTitle("#omega(t_{0}) #equiv t_{0} Weight");
  }
  if ( hname.Contains("geqrad") ) {
    gr->GetXaxis()->SetTitle("x_{e} (Carey) [cm]");
    gr->GetYaxis()->SetTitle("a.u.");
  }

  if ( hname.Contains("gEqRad1") ) {
    gr->GetXaxis()->SetTitle("x_{e} (Gadfort, 1^{st} Pass) [cm]");
    gr->GetYaxis()->SetTitle("a.u.");
  }
  if ( hname.Contains("gEqRad2") ) {
    gr->GetXaxis()->SetTitle("x_{e} (Gadfort, 2^{nd} Pass) [cm]");
    gr->GetYaxis()->SetTitle("a.u.");
  }
  gr->Draw("ALP");




  if ( hname.Contains("gEqRad1") ) {
    stringstream chi2;
    chi2.precision(5);
    chi2 << "#chi^{2}/N_{dof} = " << chi2_gadfort1;
    myText(0.6, 0.88, 1, chi2.str().c_str(), 0.05);
  }
  if ( hname.Contains("gEqRad2") ) {
    stringstream chi2;
    chi2.precision(5);
    chi2 << "#chi^{2}/N_{dof} = " << chi2_gadfort2;
    myText(0.6, 0.88, 1, chi2.str().c_str(), 0.05);
  }

  if ( hname.Contains("EqRad") || hname.Contains("eqrad") ) {
    TLine *l_rm = new TLine(711.2, 0.0, 711.2, ymax*0.25);
    l_rm->SetLineColor(kRed);
    l_rm->SetLineWidth(2);
    l_rm->Draw("SAME");

    TLine *l_rm_n = new TLine(711.2-4.5, 0.0, 711.2-4.5, ymax*0.25);
    l_rm_n->SetLineColor(kOrange+6);
    l_rm_n->SetLineWidth(2);
    l_rm_n->Draw("SAME");

    TLine *l_rm_p = new TLine(711.2+4.5, 0.0, 711.2+4.5, ymax*0.25);
    l_rm_p->SetLineColor(kOrange+6);
    l_rm_p->SetLineWidth(2);
    l_rm_p->Draw("SAME");
  }
  
  if ( hname.Contains("PeriodRadius") ) {
    TLine *l_rm = new TLine(0.148, 711.2, 0.149146, 711.2);
    l_rm->SetLineColor(kRed);
    l_rm->SetLineWidth(2);
    l_rm->Draw("SAME");

    TLine *l_rm2 = new TLine(0.149146, 706.0, 0.149146, 711.2);
    l_rm2->SetLineColor(kRed);
    l_rm2->SetLineWidth(2);
    l_rm2->Draw("SAME");
  }

  bool doeps = false;

  stringstream c1name;
  if ( zoom ) {
    c1name << "~/public_html/plots/" << dir << "/" << hname << "_Zoom";
  }
  else if ( zoom2 ) {
    c1name << "~/public_html/plots/" << dir << "/" << hname << "_Zoom2";
  }
  else {
    c1name << "~/public_html/plots/" << dir << "/" << hname;
  }

  if ( dologx ) {
    c1->SetLogx(1);
  }
  
  if ( dolog ) {
    c1->SetLogy(1);
    c1->SetGrid(1);
  }
  else {
    c1->SetLogy(0);
    c1->SetGrid(0);
  }

  stringstream pngname, epsname;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  delete c1;
}

void GetMaxMin(TGraph *gr, double *max, double *min)
{
  int n = gr->GetN();
  Double_t *x = gr->GetX();
  Double_t *y = gr->GetY();

  double maxi = -9999.9;
  double mini = 99999.9;
  for ( int i = 0; i < n; i++ ) {
    if ( y[i] > maxi ) { maxi = y[i]; }
    if ( y[i] < mini ) { mini = y[i]; }
  }

  *max = maxi;
  *min = mini;
}

void MakePlot(TGraph *gr1, TGraph *gr2, string name1, string name2)
{

  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  string name = name1 + "_" + name2;
  string hname = name;
  
  double xmin, xmax;
  double ymin, ymax;
  double grxmin, grxmax;
  double grymin, grymax;
  grxmin = 99999.9;
  grxmax = -99999.9;
  double grx90min, grx90max;
  grx90min = 99999.9;
  grx90max = -99999.9;

  double gr1_max, gr1_min;
  GetMaxMin(gr1, &gr1_max, &gr1_min);
  double gr2_max, gr2_min;
  GetMaxMin(gr2, &gr2_max, &gr2_min);



  double grymax = TMath::Max(gr1_max, gr2_max);
  double grymin = TMath::Min(gr1_min, gr2_min);
  grxmin = -45;
  grxmax = 45;


  ymax = grymax * 1.2;
  ymin = grymin * 1.1;
  xmax = grxmax;
  xmin = grxmin;

  
  TString xtitle = "x_{e} [mm]";
  TString ytitle = "a.u.";
  
  
  cout << "==========" << endl;
  cout << "Graph1D Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << grxmin <<"  / " << grxmax << "\t\t" << grymin << " / " << grymax << endl; 
  cout << "==========" << endl;

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->Draw();

  int color1, color2;
  if ( name1 == "THEORY" ) { color1 = kOrange+6; }
  if ( name2 == "THEORY" ) { color2 = kOrange+6; }

  if ( name1 == "GEANT" )  { color1 = kAzure+6; }
  if ( name2 == "GEANT" )  { color2 = kAzure+6; }

  if ( name1 == "FastRotation" )  { color1 = kSpring-5; }
  if ( name2 == "FastRotation" )  { color2 = kSpring-5; }

  gr1->SetMarkerColor(color1);
  gr1->SetLineColor(color1);
  gr1->SetLineWidth(4);

  gr2->SetMarkerColor(color2);
  gr2->SetLineColor(color2);
  gr2->SetLineWidth(4);


  if ( gr1_max > gr2_max ) {
    gr1->Draw("LP");
    gr2->Draw("LP");
  }
  else {
    gr2->Draw("LP");
    gr1->Draw("LP");
  }

  
  TLegend *leg = new TLegend(0.75, 0.80, 0.85, 0.90);
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(gr1, Nicename(name1.c_str()), "l");
  leg->AddEntry(gr2, Nicename(name2.c_str()), "l");
  leg->Draw("SAME");



  stringstream dpname;
  dpname << "#frac{#deltap}{p} = " << dPoverP*100 << "%";
  myText(0.22, 0.90-0.05, 1, dpname.str().c_str(), 0.035);
  stringstream Nname;
  Nname << "n_{index} = 0.137";
  myText(0.22, 0.84-0.05, 1, Nname.str().c_str(), 0.035);
  stringstream Nradname;
  Nradname << "N_{rad} Bins = " << Nrad;
  myText(0.22, 0.78-0.05, 1, Nradname.str().c_str(), 0.035);
  stringstream lambname;
  lambname << "#lambda = " << lambda;
  myText(0.22, 0.72-0.05, 1, lambname.str().c_str(), 0.035);




  bool doeps = false;

  stringstream c1name;
  c1name << "~/public_html/plots/" << dir << "/" << name;

  stringstream pngname, epsname;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  delete c1;
}

void MakePlot(TH1F *hist1, TH1F *hist2, string name)
{

  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  string hname = name;

  hist1->Scale(1.0/hist1->Integral());
  hist2->Scale(1.0/hist2->Integral());
  
  double xmin, xmax;
  double ymin, ymax;
  double histxmin, histxmax;
  double histymin, histymax;
  histxmin = 99999.9;
  histxmax = -99999.9;
  double histx90min, histx90max;
  histx90min = 99999.9;
  histx90max = -99999.9;



  double histymax = TMath::Max(hist1->GetMaximum(), hist2->GetMaximum());
  double histymin = TMath::Min(hist1->GetMinimum(), hist2->GetMinimum());
  for ( int bin = 1; bin < hist1->GetNbinsX()-1; bin++ ) {
    if ( hist1->GetBinContent(bin) > 0 ) { histxmin = hist1->GetBinLowEdge(bin); break; }
  }
  for ( int bin = hist1->GetNbinsX()-1; bin >= 1; bin-- ) {
    if ( hist1->GetBinContent(bin) > 0 ) { histxmax = hist1->GetBinLowEdge(bin+1); break; }
  }
  for ( int bin = 1; bin < hist2->GetNbinsX()-1; bin++ ) {
    if ( hist2->GetBinContent(bin) > 0 ) { histxmin = hist2->GetBinLowEdge(bin); break; }
  }
  for ( int bin = hist2->GetNbinsX()-1; bin >= 1; bin-- ) {
    if ( hist2->GetBinContent(bin) > 0 ) { histxmax = hist2->GetBinLowEdge(bin+1); break; }
  }
  ymax = histymax * 1.2;
  ymin = histymin * 1.1;
  xmax = histxmax;
  xmin = histxmin;
  //GetXminmax(hist1, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax, false);

  TString xtitle = hist1->GetXaxis()->GetTitle();
  TString ytitle = hist1->GetYaxis()->GetTitle();
  
  
  cout << "==========" << endl;
  cout << "Hist1D Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
  cout << "==========" << endl;

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.2);
  h->Draw();

  if ( hist1->GetMaximum() > hist2->GetMaximum() ) {
    hist1->SetLineColor(kRed);
    hist1->SetLineWidth(4);
    hist1->Draw("HIST");
    
    hist2->SetLineColor(kBlue);
    hist2->SetLineWidth(4);
    hist2->Draw("HIST, SAME");
  }
  else {
    hist2->SetLineColor(kRed);
    hist2->SetLineWidth(4);
    hist2->Draw("HIST");
    
    hist1->SetLineColor(kBlue);
    hist1->SetLineWidth(4);
    hist1->Draw("HIST, SAME");
  }
  
  TLegend *leg = new TLegend(0.75, 0.75, 0.85, 0.85);
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->AddEntry(hist1, Nicename(hist1->GetName()), "l");
  leg->AddEntry(hist2, Nicename(hist2->GetName()), "l");
  leg->Draw("SAME");



  stringstream dpname;
  dpname << "#frac{#deltap}{p} = " << dPoverP*100 << "%";
  myText(0.75, 0.60, 1, dpname.str().c_str(), 0.035);
  stringstream Nname;
  Nname << "n_{index} = 0.137";
  myText(0.75, 0.53, 1, Nname.str().c_str(), 0.035);
  stringstream Nradname;
  Nradname << "N_{rad} Bins = " << Nrad;
  myText(0.75, 0.46, 1, Nradname.str().c_str(), 0.035);




  bool doeps = false;

  stringstream c1name;
  c1name << "~/public_html/plots/" << dir << "/" << name;

  stringstream pngname, epsname;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  delete c1;
}


void MakePlot(TH1F *hist1d)
{
  if ( update && hist1d == NULL ) { return; }
  if ( hist1d == NULL ) { return; }

  TString hname = hist1d->GetName();
  if ( hist1d->GetEntries() <= 0 ) { cout << "Histogram [" << hist1d->GetName() << "] is empty" << endl; return; }

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


  if ( hname.Contains("Signal00") && !hname.Contains("0ns") ) { dolog = true; }
  if ( hname.Contains("gen") && !hname.Contains("0ns")  ) { dolog = true; }
  if ( hname.Contains("fr") && !hname.Contains("0ns")  ) { dolog = true; }
  //
  // Get HistXmin/max
  //
  
  //histymax = hist1d->GetMaximum();
  //histymin = hist1d->GetMinimum();
  for ( int bin = 1; bin < hist1d->GetNbinsX()-1; bin++ ) {
    if ( hist1d->GetBinContent(bin) > histymax ) { histymax = hist1d->GetBinContent(bin); }
    if ( hist1d->GetBinContent(bin) < histymin ) { histymin = hist1d->GetBinContent(bin); }
  }
  for ( int bin = 1; bin < hist1d->GetNbinsX()-1; bin++ ) {
    if ( hist1d->GetBinContent(bin) > 0 ) { histxmin = hist1d->GetBinLowEdge(bin); break; }
  }
  for ( int bin = hist1d->GetNbinsX()-1; bin >= 1; bin-- ) {
    if ( hist1d->GetBinContent(bin) > 0 ) { histxmax = hist1d->GetBinLowEdge(bin+1); break; }
  }


  //cout << histxmin << "\t" << histxmax << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 


  if ( ( maxturns_for_plot > 0 || minturns_for_plot > 0) && dofft == false ) {
    histxmin = minturns_for_plot;
    histxmax = maxturns_for_plot;
    histymax = 0.0;
    histymin = 0.0;
    
    for ( int bin = hist1d->FindBin(minturns_for_plot); bin < hist1d->FindBin(maxturns_for_plot); bin++ ) {
      if ( hist1d->GetBinContent(bin) > histymax ) { histymax =  hist1d->GetBinContent(bin); }
      if ( hist1d->GetBinContent(bin) < histymin ) { histymin =  hist1d->GetBinContent(bin); }
    }
  }

  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 

    
  GetXminmax(hist1d, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax, dolog);

  
  
  TString xtitle = hist1d->GetXaxis()->GetTitle();
  TString ytitle = hist1d->GetYaxis()->GetTitle();
  
  
  cout << "==========" << endl;
  cout << "Hist1D Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
  cout << "==========" << endl;

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.05);
  h->GetYaxis()->SetTitleSize(0.05);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleOffset(1.2);
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

  if ( hname.Contains("Mom") ) { yt << " [GeV/c]"; }
  else if ( hname.Contains("Time") ) { yt << " [ns]"; }
  else if ( hname.Contains("Signal") ) { yt << " [ns]"; }
  else if ( hname.Contains("t0") ) { yt << " [t0 bin]"; }
  else if ( hname.Contains("T0") ) { yt << " [t0 bin]"; }
  else if ( hname.Contains("Freq") ) { yt << " [MHz]"; }
  else if ( hname.Contains("Wavelength") ) { yt << " #lambda^{-1}"; }
  else if ( hname.Contains("Turn") ) { yt << " [Turns]"; }
  else if ( hname.Contains("FFT") ) { yt << " Turns^{-1}"; }
  else if ( hname.Contains("NgtEth") ||
	    hname.Contains("NwghtE") ||
	    hname.Contains("Nud") ||
	    hname.Contains("Nup") ||
	    hname.Contains("Ndown") ||
	    hname.Contains("Num") ) { yt << " Turns"; }
  else if ( hname.Contains("TrackVhat") ) { yt << " mm"; }
  else if ( hname.Contains("TrackYprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Turn") ) { yt << " 30 degrees"; }
  else if ( hname.Contains("Kick") ) { yt << " mrad"; }
  else if ( hname.Contains("Px") || hname.Contains("Py") ) { yt << " MeV"; }
  else if ( hname.Contains("Xprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Yprime") ) { yt << " mrad"; }
  else if ( hname.Contains("Degree") ) { yt << " ^{#circ}"; }
  else if ( hname.Contains("Xe") || hname.Contains("Vhat") ) { yt << " mm"; }
  else if ( hname.Contains("Pol") || hname.Contains("PolX")|| hname.Contains("PolY") ) { yt << ""; }
  else if ( hname.Contains("Rhat") || hname.Contains("Vhat") ) { yt << " mm"; }
  else if ( hname.Contains("Time") ) { yt << " #mus"; }
  else { yt << " [Unit]"; }

  if ( hname.Contains("spread") ) {
    yt.str("");
    yt << "#beta^{g}(x,t) = #int [#beta^{g}(x,t;t_{0}) #times #omega(t_{0})] dt_{0}";
  }
  if ( hname.Contains("hum") ) {
    yt.str("");
    yt << "#beta^{g}(x=R/2,t) = #int [#beta^{g}(R/2,t;t_{0}) #times #omega(t_{0})] dt_{0}";
  }
  if ( hname.Contains("hfr2") ) {
    yt.str("");
    yt << "N^{obs}(t) / " << binw << " ns";
  }
  if ( hname.Contains("hgen") ) {
    yt.str("");
    yt << "N^{gen}(t) = #int [#beta^{g}(x,t)#times g'(x)] dx  /  " << binw << " ns";
    stringstream chi2;
    chi2.precision(3);
    chi2 << "p = " << pval_gen << "  (#chi^{2}/N_{dof} = " << chi2_gen << ")";
    myText(0.45, 0.88, 1, chi2.str().c_str(), 0.05);
    stringstream ks;
    ks.precision(3);
    ks << "KS = " << ks_gen;
    myText(0.45, 0.80, 1, ks.str().c_str(), 0.05);
  }
  if ( hname.Contains("hcheck") ) {
    yt.str("");
    yt << "N_{check}(t) = #beta^{g}(x_{sol},t) #times x_{sol}  /  " << binw << " ns";
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
  }

  if ( dolog ) {
    c1->SetLogy(1);
    c1->SetGrid(1);
  }
  else {
    c1->SetLogy(0);
    c1->SetGrid(0);
  }

  stringstream pngname, epsname;
  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  delete c1;
}

TGraph* GetGraph(TFile *file, string name)
{
  string graphname = name;
  if ( rootdir.size() > 0 ) { graphname = rootdir + "/" + name; }
  
  TGraph *gr = (TGraph*)file->Get(graphname.c_str());
  if ( !gr  ) { cout << "Could not get " << graphname << " from GetGraph()" << endl; return( NULL ); }

  return( gr );
}

TGraph2D* GetGraph2D(TFile *file, string name)
{
  string graphname = name;
  if ( rootdir.size() > 0 ) { graphname = rootdir + "/" + name; }
  
  TGraph2D *gr = (TGraph2D*)file->Get(graphname.c_str());
  if ( !gr  ) { cout << "Could not get " << graphname << " from GetGraph()" << endl; return( NULL ); }

  return( gr );
}

TH2F* GetHistogram(TFile *file, string name)
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
  
  TH2F *hist = (TH2F*)file->Get(histname.c_str());
  if ( !hist  ) { cout << "Could not get " << histname << " from GetHistogram()" << endl; return( NULL ); }

  return( hist );
}

TH1F* GetHistogram1D(TFile *file, string name)
{
  if ( file == NULL ) { return; }

  string histname = name;
  if ( rootdir.size() > 0 ) { histname = rootdir + "/" + name; }


  cout << file << "\t" << histname << endl;

  TH1F *hist1d = (TH1F*)file->Get(histname.c_str());
  if ( !hist1d ) { cout << "Could not get " << histname << " from GetHistogram1D()" << endl; return( NULL ); }

  cout << hist1d << endl;
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

double ks_gen;
double chi2_gen;
double pval_gen;
double chi2_gadfort1;
double chi2_gadfort2;


void plotfastrotation()
{
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasStyle.C");
  SetAtlasStyle();
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasUtils.C");
  gROOT->LoadMacro("atlasstyle-00-03-04/AtlasLabels.C");

  ifstream in;
  in.open("filename");
  string filename;
  in >> filename;
  string core;
  in >> core;
  in.close();


  TH1F *h_MCSignal;
  TH1F *h_DataSignal;

  TFile *file = NULL;
  int Nhists = 0;
  string Hists[30];
  string hbase = "h";
    
  rootdir = "";
  file = TFile::Open(filename.c_str());
  if ( file == NULL ) {
    cout << "Could not open " << filename << "." << endl;
    return;
  }
  cout << "Contents of " << filename << endl;
  file->ls();
  dir = "FastRotation";
  dir = dir + "/" + core;

  unsigned pos = filename.find(".root");
  string chi2file = filename.substr(0, pos);
  chi2file = chi2file + ".dat";

  in.open(chi2file.c_str());
  in >> chi2_gadfort1;
  in >> chi2_gadfort2;
  in >> chi2_gen;
  in >> pval_gen;
  in >> ks_gen;
  in.close();

  if ( filename.find("NRad5") != string::npos ) { Nrad = 5; }
  if ( filename.find("NRad51") != string::npos ) { Nrad = 51; }
  if ( filename.find("NRad25") != string::npos ) { Nrad = 25; }
  if ( filename.find("NRad15") != string::npos ) { Nrad = 15; }

  if ( filename.find("Lamb0") != string::npos ) { lambda = 0; }
  if ( filename.find("Lamb0.01") != string::npos ) { lambda = 0.01; }
  if ( filename.find("Lamb0.1") != string::npos ) { lambda = 0.1; }
  if ( filename.find("Lamb1") != string::npos ) { lambda = 1; }
  if ( filename.find("Lamb10") != string::npos ) { lambda = 10; }
  
  if ( filename.find("dP10") != string::npos ) { dPoverP = 0.10; }
  if ( filename.find("dP5") != string::npos ) { dPoverP = 0.05; }
  if ( filename.find("dP2") != string::npos ) { dPoverP = 0.02; }
  if ( filename.find("dP075") != string::npos ) { dPoverP = 0.0075; }
  if ( filename.find("dP05") != string::npos ) { dPoverP = 0.005; }
  if ( filename.find("dP025") != string::npos ) { dPoverP = 0.0025; }
  if ( filename.find("dP01") != string::npos ) { dPoverP = 0.001; }
  if ( filename.find("dP0075") != string::npos ) { dPoverP = 0.00075; }
  if ( filename.find("dP005") != string::npos ) { dPoverP = 0.0005; }
  if ( filename.find("dP0025") != string::npos ) { dPoverP = 0.00025; }
  if ( filename.find("dP001") != string::npos ) { dPoverP = 0.0001; }
  if ( filename.find("dP00075") != string::npos ) { dPoverP = 0.000075; }
  if ( filename.find("dP0005") != string::npos ) { dPoverP = 0.00005; }
  if ( filename.find("dP00025") != string::npos ) { dPoverP = 0.000025; }
  if ( filename.find("dP0001") != string::npos ) { dPoverP = 0.00001; }

  

  TH1F *h_EqRad_FastRotation = NULL;
  TH1F *h_EqRad_GEANT = NULL;
  TH1F *h_EqRad_THEORY = NULL;
    

  TH1F   *h_
  TGraph *g_EqRad_FastRotation = NULL;
  TGraph *g_EqRad_GEANT = NULL;
  TGraph *g_EqRad_THEORY = NULL;

  if ( filename.find("fasrof") != string::npos || 1 ) {

//     Hists[Nhists++] = "spread";
//     Hists[Nhists++] = "spread10";
//     Hists[Nhists++] = "spread30";
//     Hists[Nhists++] = "check";
    Hists[Nhists++] = "gen";
    //Hists[Nhists++] = "um";
    Hists[Nhists++] = "fr2";
    //Hists[Nhists++] = "Xe";
    //Hists[Nhists++] = "EqRad";
//     Hists[Nhists++] = "EqRadConv";
    //Hists[Nhists++] = "Data_MC";
    //Hists[Nhists++] = "eqrad";
    //       Hists[Nhists++] = "pulse00";
    //       Hists[Nhists++] = "pulse01";
    //       Hists[Nhists++] = "pulse02";
    //       Hists[Nhists++] = "pulse03";
    //       Hists[Nhists++] = "pulse04";
    //       Hists[Nhists++] = "pulse05";
    //       Hists[Nhists++] = "pulse06";
    //       Hists[Nhists++] = "pulse07";
    //       Hists[Nhists++] = "pulse08";
    //       Hists[Nhists++] = "pulse09";
    //       Hists[Nhists++] = "pulse10";
    //       Hists[Nhists++] = "pulse11";
    //       Hists[Nhists++] = "pulse12";
    //       Hists[Nhists++] = "pulse13";
    //       Hists[Nhists++] = "pulse14";

      
    TGraph *gr = GetGraph(file, "gt0");
    MakePlot(gr, "gt0");
    TGraph *gr = GetGraph(file, "gt0weight");
    MakePlot(gr, "gt0weight");
    TGraph *gr = GetGraph(file, "gEqRad1");
    MakePlot(gr, "gEqRad1");
    TGraph *gr = GetGraph(file, "gEqRad2");
    MakePlot(gr, "gEqRad2");
    TGraph *gr = GetGraph(file, "geqrad");
    MakePlot(gr, "geqrad");
    TGraph *gr = GetGraph(file, "gPeriodRadius");
    MakePlot(gr, "gPeriodRadius");


    g_EqRad_FastRotation = GetGraph(file, "g_EqRad_FastRotation");
    g_EqRad_GEANT        = GetGraph(file, "g_EqRad_GEANT");
    g_EqRad_THEORY       = GetGraph(file, "g_EqRad_THEORY");
  }

  if ( filename.find("g2gen") != string::npos ) {
      
    Hists[Nhists++] = "Detector";
    Hists[Nhists++] = "MomSpec";
    Hists[Nhists++] = "nTurns";
    Hists[Nhists++] = "FreqSpec";
    Hists[Nhists++] = "T0Pulse";
    Hists[Nhists++] = "t0";
    Hists[Nhists++] = "t0time";
    Hists[Nhists++] = "TimeDecay";
    Hists[Nhists++] = "Signal00";
  }

    
  stringstream hname;
    
  for ( int n = 0; n < Nhists; n++ ) {
      
    string histname = Hists[n];
      
    hname << hbase << histname;
      
    TH1F *hist1d = GetHistogram1D(file, hname.str());
    if ( !hist1d ) { continue; }

    if ( histname == "EqRadConv" ) { h_EqRad_FastRotation = (TH1F*)hist1d->Clone("EqRad_FastRotation"); }
    if ( histname == "EqRad" )     { h_EqRad_GEANT = (TH1F*)hist1d->Clone("EqRad_GEANT"); }
    if ( histname == "Xe" )        { h_EqRad_THEORY = (TH1F*)hist1d->Clone("EqRad_THEORY"); }
    
    if ( histname == "fr2" ) { h_DataSignal = (TH1F*)hist1d->Clone("datasignal"); }
    if ( histname == "gen" ) { h_MCSignal = (TH1F*)hist1d->Clone("mcsignal"); }
    
    MakePlot(hist1d);
    hname.str("");

    if ( 0 ) {
      if ( histname.find("Signal") != string::npos || histname.find("gen") != string::npos || histname.find("fr2") != string::npos || histname.find("Data_MC") != string::npos ) {
	for ( int mt = 1000; mt < 20000; mt += 5000 ) {
	  minturns_for_plot = mt;
	  maxturns_for_plot = mt+1000;
	  
	  hname << hbase << histname << "_" << minturns_for_plot << "_" << maxturns_for_plot << "ns";
	  TH1F *hist1d_zoom = (TH1F*)hist1d->Clone(hname.str().c_str());
	  MakePlot(hist1d_zoom);
	  hname.str("");
	}
      }
      minturns_for_plot = -1;
      maxturns_for_plot = -1;
    }
  }


  double binw = h_EqRad_FastRotation->GetBinWidth(1);
  if ( h_EqRad_FastRotation ) { h_EqRad_FastRotation->Rebin(2); }
  if ( h_EqRad_THEORY ) { h_EqRad_THEORY->Rebin(2); }
  if ( h_EqRad_GEANT ) { h_EqRad_GEANT->Rebin(2); }

  if ( g_EqRad_FastRotation && g_EqRad_GEANT ) {
    Double_t *x = g_EqRad_FastRotation->GetX();
    Double_t *y = g_EqRad_FastRotation->GetY();
    for ( int i = 0; i < g_EqRad_FastRotation->GetN(); i++ ) {
      cout << i << "\t" << x[i] << "\t" << y[i] << endl;
    }


    MakePlot(g_EqRad_FastRotation, g_EqRad_GEANT, "FastRotation", "GEANT");
  }
  if ( g_EqRad_FastRotation && g_EqRad_THEORY ) {
    MakePlot(g_EqRad_FastRotation, g_EqRad_THEORY, "FastRotation", "THEORY");
  }
  if ( g_EqRad_GEANT && g_EqRad_THEORY ) {
    MakePlot(g_EqRad_GEANT, g_EqRad_THEORY, "GEANT", "THEORY");
  }

//   if ( h_EqRad_FastRotation && h_EqRad_GEANT ) {
//     MakePlot(h_EqRad_FastRotation, h_EqRad_GEANT, "FastRotation_GEANT");
//   }
//   if ( h_EqRad_FastRotation && h_EqRad_THEORY ) {
//     MakePlot(h_EqRad_FastRotation, h_EqRad_THEORY, "FastRotation_THEORY");
//   }
//   if ( h_EqRad_GEANT && h_EqRad_THEORY ) {
//     MakePlot(h_EqRad_GEANT, h_EqRad_THEORY, "GEANT_THEORY");
//   }
  //h_signal->Add(h_gensignal, -1);
  //MakePlot(h_signal);
  //hname.str("");
    


  if ( 0 ) {
  for ( double pw = 0.01; pw < 0.2; pw += 0.05 ) {
    stringstream hname;
    TFile *file = NULL;
    int Nhists = 0;
    string hbase = "h";
    stringstream filename_ss;
    rootdir = "";
    filename_ss << "fasrofwide_" << pw << ".root";
    TString filename(filename_ss.str());
    
    file = TFile::Open(filename);
    cout << "Contents of " << filename << endl;
    file->ls();
    dir = "FastRotation";      

    cout << "Get hist" << endl;
    hname << "heqrad";
    cout << "file = " << file << endl;
    TH1F *hist1d = GetHistogram1D(file, hname.str());
    cout << "Got it." << endl;
    hname << "_" << pw;
    cout << hist1d << endl;
    TH1F *hist = (TH1F*)hist1d->Clone(hname.str().c_str());
    cout << hist << "\t" << hname.str() << endl;
    MakePlot(hist);
    hname.str("");    
  }
  } // if 0
}
  
