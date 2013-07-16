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

double dPoverP = 0.01;

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
  double size = 4;
  if ( name.Contains("Num") ) { size = 1; }

  return( size );
}

double GetMarkerSize(TString name)
{
  double size = 1.25;
  if ( name.Contains("Num") ) { size = 0.1; }

  return( size );
}

void GetXminmax(TH2F *hist, double *xmin, double *xmax, double *ymin, double *ymax, double histxmin, double histxmax, double histymin, double histymax)
{
  bool isTurn = false;
  bool isRhatY = false;
  bool isVhat = false;
  bool isRhat = false;
  bool isR = false;
  bool isXZ = false;
  bool isXprimeX = false;
  bool isYprimeY = false;
  bool isXprime = false;
  bool isYprime = false;
  bool isN = false;
  bool isNavg = false;
  bool isPol = false;
  bool isXe = false;
  bool isMom = false;
  bool isZhat = false;
  bool isNum = false;

  TString name = hist->GetName();

  //cout << "2D: " << name << "\t" << hist->GetNbinsX() << endl;
  
  if ( name.Contains("_Turn") ) { isTurn = true; }
  if ( name.Contains("_OncePerTurn") ) { isTurn = true; }
  if ( name.Contains("_Time") ) { isTurn = true; }
  if ( name.Contains("G4Track_Time") ) { isTurn = false; }
  if ( name.Contains("_TimeOncePerTurn") ) { isTurn = true; }
  if ( isTurn ) {
    if ( name.Contains("TrackR") || name.Contains("TrackerR") ) { isR = true; }
    if ( name.Contains("TrackRhat") || name.Contains("TrackerRhat") ) { isRhat = true; isR = false; }
    if ( name.Contains("TrackY") || name.Contains("TrackerY") ) { isVhat = true; }
    if ( name.Contains("TrackVhat") || name.Contains("TrackerVhat") ) { isVhat = true; }
    if ( name.Contains("TrackMom") || name.Contains("TrackerMom") ) { isMom = true; }
    if ( name.Contains("TrackPrhat") || name.Contains("TrackerPrhat") ) { isXprime = true; }
    if ( name.Contains("TrackPvhat") || name.Contains("TrackerPvhat") ) { isYprime = true; }
    if ( name.Contains("TrackXe") || name.Contains("TrackerXe") ) { isXe = true; }
    if ( name.Contains("TrackPol") || name.Contains("TrackerPol") ) { isPol = true; }
    if ( name.Contains("TrackZhat") || name.Contains("TrackerZhat") ) { isZhat = true; }
    if ( name.Contains("TrackNumCounter") || name.Contains("TrackerNumCounter") ) { isNum = true; }
  }
  else {
    if ( name.Contains("PolXY") ) { isPol = true; }
    if ( name.Contains("RhatY") ) { isRhatY = true; }
    if ( name.Contains("XZ") ) { isXZ = true; }
    if ( name.Contains("XprimeX") ) { isXprimeX = true; }
    if ( name.Contains("YprimeY") ) { isYprimeY = true; }
  }


  if ( isTurn ) {
    *xmin = hist->GetXaxis()->GetXmin();
    *xmax = hist->GetXaxis()->GetXmax();

    if ( maxturns_for_plot > 0 ) { 
      *xmax = maxturns_for_plot;
      // since I scale by 1.25 down the road I need to by 1.25 now to undo that.
      //*xmax /= 1.25;
    }

    if ( isRhat || isVhat ) {
      *ymax = 300; *ymin = -300;
      for ( int mm = 300; mm >= 50; mm -= 50 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      for ( int mm = 50; mm >= 10; mm -= 5 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
      return;
    }

    if ( isNum ) {
      *ymax = histymax; *ymin = histymin;
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
      for ( double mm = TMath::Pi(); mm >= 0.1; mm -= TMath::Pi()/8.0 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
	cout << mm << "\t" << *ymin << "\t" << *ymax << endl;
      }
      return;
    }

    if ( isMom ) {
      *ymax = 2; *ymin = 0;
      if ( name.Contains("Electron") ) {
	*ymin = 0.0; *ymax = 1.0;
      }
      else {
	if ( name.Contains("BirthMuon") || name.Contains("DecayMuon") || name.Contains("StoredMuon") || name.Contains("RingTracker") || name.Contains("TrackerMom") ) {
	  *ymin = 1.0*(1-3*dPoverP);
	  *ymax = 1.0*(1+3*dPoverP);
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
    
    if ( isXe ) { 
      *ymax = 50; *ymin = -50;
      for ( int mm = 50; mm >= 10; mm -= 5 ) {
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

    if ( isXZ ) {
      *ymax = 8000.0; *ymin = -8000.0;
      *xmax = 8000.0; *xmin = -8000.0;
      return;
    }

    if ( isPol ) {
      *ymax = 1; *ymin = -1;
      *xmax = 1; *xmin = -1;
      for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
	if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
      }
      for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
	if ( histymax < mm && histymin > -mm ) { *ymax = mm; *ymin = -mm; }
      }
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
  
  cout << name << " has no limits." << endl;
}

void RebinHist(TH1F *hist1d)
{
  TString hname = hist1d->GetName();

  if ( hname.Contains("FFT") ) { return; }

  
  if ( hname.Contains("NgtEth") || hname.Contains("NwghtE") || hname.Contains("Nud") || hname.Contains("Nup") || hname.Contains("Ndown") || hname.Contains("Num") ) { ; }
  else {
    if ( hname.Contains("TimeOncePerTurn") ) { hist1d->Rebin(24); }

  }
  if ( maxturns_for_plot > 0 ) { return; }
  
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
      
      cout << "NAME: " << hname << "\t" << hist1d->Integral() << endl;
      
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
    }
  }
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

  if ( name.Contains("Turn") ) { isTurn = true; }
  if ( name.Contains("_R") ) { isR = true; }
  if ( name.Contains("_Rhat") ) { isRhat = true; isR = false; }
  if ( name.Contains("_Vhat") ) { isRhat = true; }
  if ( name.Contains("_Xprime") ) { isXprime = true; }
  if ( name.Contains("_Yprime") ) { isYprime = true; }
  if ( name.Contains("NgtEth") ) { isN = true; }
  if ( name.Contains("NwghtE") ) { isN = true; }
  if ( name.Contains("Nud") ) { isNavg = true; }
  if ( name.Contains("Num") ) { isN = true; }
  if ( name.Contains("TrackVhat") ) { isNavg = true; }
  if ( name.Contains("TrackYprime") ) { isNavg = true; }
  if ( name.Contains("Nud") ) { isNavg = true; }
  if ( name.Contains("_Pol") ) { isPol = true; }
  if ( name.Contains("_Mom") ) { isMom = true; }
  if ( name.Contains("_Xe") ) { isXe = true; }
  if ( name.Contains("FFT") ) { isFFT = true; }
  if ( name.Contains("Zhat") ) { isZhat = true; }
  
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
    *xmin = 0.0; *xmax = 1.0;

    return;
  }
    

  if ( isRhat || isVhat ) {
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
    *xmax = TMath::Pi(); *xmin = -TMath::Pi();
    for ( double mm = TMath::Pi(); mm >= 0.1; mm -= TMath::Pi()/8.0 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }
  
  if ( isPol ) {
    *xmax = 1; *xmin = -1;
    for ( int mm = 1; mm >= 0.1; mm -= 0.1 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }

  if ( isXe ) {
    *xmax = 50; *xmin = -50;
    for ( int mm = 50; mm >= 10; mm -= 5 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }

  if ( isNavg ) {
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
    return;
  }

  if ( isN ) {   
    *xmin = 0.0;
    if ( minturns_for_plot > 0 ) { *xmin = minturns_for_plot; }
    if ( maxturns_for_plot > 0 ) { *xmax = maxturns_for_plot; }
    cout << "Ymin = " << *ymin << endl;
    return;
  }
  
  if ( isXprime || isYprime ) {
    *xmax = 100; *xmin = -100;
    for ( int mm = 100; mm >= 20; mm -= 20 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    for ( int mm = 20; mm >= 5; mm -= 5 ) {
      if ( histxmax < mm && histxmin > -mm ) { *xmax = mm; *xmin = -mm; }
    }
    return;
  }  

  if ( isMom ) {
    *xmax = 2; *xmin = 0;
    if ( name.Contains("Electron") ) {
      *xmin = 0.0; *xmax = 1.0;
    }
    else {
      if ( name.Contains("BirthMuon") || name.Contains("DecayMuon") || name.Contains("StoredMuon") || name.Contains("RingTracker") ) {
	*xmin = 1.0*(1-3*dPoverP);
	*xmax = 1.0*(1+3*dPoverP);
      }
      else {
	*xmin = 0.0; *xmax = 1.0*(1+3*dPoverP);
      }
    }
    return;
  }
  
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

  TH1F *hist1d_x_tmp = (TH1F*)fCount->GetHistogram();
  TH1F *hist1d_x = (TH1F*)(hist1d_x_tmp->Clone(Fitname.c_str()));
  //Fitname += "_Fit";
  //TH1F *hist1d_x = (TH1F*)hist1d->Clone(Fitname.c_str());
  MakePlot1D(hist1d_x, -1, -1, &int_prev, &int_curr, &int_start);
}


void ComputeFFT(TH1F *hist1d, string FFTname)
{
  if ( hist1d == NULL ) { return; }
  cout << "=====================================" << endl;
  cout << "Running FFT for " << hist1d->GetName() << endl;
  cout << "=====================================" << endl;

  double int_prev;
  double int_curr;
  double int_start;

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
  MakePlot1D(h1dMag, -1, -1, &int_prev, &int_curr, &int_start);
  

  
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

    MakePlot1D(h_wavelength, -1, -1, &int_prev, &int_curr, &int_start);      
    MakePlot1D(h_frequency, -1, -1, &int_prev, &int_curr, &int_start);       
    MakePlot1D(h_mag, -1, -1, &int_prev, &int_curr, &int_start); 
    MakePlot1D(h_ph, -1, -1, &int_prev, &int_curr, &int_start);
  }
  cout << "=====================================" << endl;
  cout << endl;
}

void MakePlot(TH2F *hist, int r, int i, double *int_prev, double *int_curr, double *int_start)
{
  if ( update && hist == NULL ) { return; }
  if ( hist == NULL ) { cout << "Histogram is NULL" << endl; return; }
  if ( hist->GetEntries() <= 0 ) { cout << "Histogram [" << hist->GetName() << "] is empty" << endl; return; }
  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  
  double xmin, xmax;
  double ymin, ymax;

  double histymax = 0.0;
  double histymin = 0.0;
  double histxmax = 0.0;
  double histxmin = 0.0;


  TString hname = hist->GetName();

  double thresh = 0.05;

  //
  // Get HistXmin/max
  //
  //cout << hname << endl;
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

  TH1D *yprof = (TH1D*)hist->ProjectionY();
  double inty = yprof->Integral();
  for ( int bin = 1; bin < yprof->GetNbinsX()-1; bin++ ) {
    if ( yprof->Integral(bin, yprof->GetNbinsX()) / inty < 1 - thresh ) {
      histymin = yprof->GetBinCenter(bin);
      break;
    }
  }
  for ( int bin = yprof->GetNbinsX()-1; bin >= 1; bin-- ) {
    if ( yprof->Integral(bin, yprof->GetNbinsX()) / inty > thresh ) {
      histymax = yprof->GetBinCenter(bin);
      break;
    }
  }

  if ( hname.Contains("NumCounter") == false ) {
    histymax *= 1.25;
    histymin *= 1.25;
  }
  if ( hname.Contains("Num") == false ) {
    histxmax *= 1.25;
    histxmin *= 1.25;
  }

  GetXminmax(hist, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax);



  TString xtitle = hist->GetXaxis()->GetTitle();
  TString ytitle = hist->GetYaxis()->GetTitle();
  
  cout << "==========" << endl;
  cout << "Hist2D Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
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


  cout << hist->GetEntries() << "\t" << Ngen << endl;
  geteff(hist->GetEntries(), upstream_inflector_yield, &abseff, &abserr);
  ss_abseff << SurviveText(abseff, abserr, "");
  
  


  TString hname = hist->GetName();
  if ( hname.Contains("G4") == false ) {
    if ( hname.Contains("TrackerRhatTime") ||
	 hname.Contains("TrackerYTime")  ||
	 hname.Contains("TrackerPrhatTime") ||
	 hname.Contains("TrackerPvhatTime")  ||
	 hname.Contains("TrackerMomTime") ||
	 hname.Contains("TrackerRhatTurn") ||
	 hname.Contains("TrackerYTurn")  ||
	 hname.Contains("TrackerPrhatTurn") ||
	 hname.Contains("TrackerPvhatTurn")  ||
	 hname.Contains("TrackerMomTurn") ) {
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
	  cout << "INFLECTOR!!! : " << number_start_in_inflector << "\t" << number_start_in_ring << endl;
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

  pngname << c1name.str() << ".png";
  epsname << c1name.str() << ".eps";
  cout << c1name.str() << endl;
  c1->SaveAs(pngname.str().c_str());
  if ( makeeps ) {
    c1->SaveAs(epsname.str().c_str());
  }
  delete c1;
}



void MakePlot1D(TH1F *hist1d, int r, int i, double *int_prev, double *int_curr, double *int_start)
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


  RebinHist(hist1d);
  if ( hname.Contains("Num") && hname.Contains("LostMuon") ) { dolog = true; }
  if ( hname.Contains("FFT") ) { dofft = true; }

  //
  // Get HistXmin/max
  //
  histymax = hist1d->GetMaximum();
  histymin = hist1d->GetMinimum();
  for ( int bin = 1; bin < hist1d->GetNbinsX()-1; bin++ ) {
    if ( hist1d->GetBinContent(bin) > 0 ) { histxmin = hist1d->GetBinLowEdge(bin); break; }
  }
  for ( int bin = hist1d->GetNbinsX()-1; bin >= 1; bin-- ) {
    if ( hist1d->GetBinContent(bin) > 0 ) { histxmax = hist1d->GetBinLowEdge(bin+1); break; }
  }  

  if ( ( maxturns_for_plot > 0 || minturns_for_plot > 0) && dofft == false ) {
    histymax = 0.0;
    histymin = 0.0;
    
    for ( int bin = hist1d->FindBin(minturns_for_plot); bin < hist1d->FindBin(maxturns_for_plot); bin++ ) {
      if ( hist1d->GetBinContent(bin) > histymax ) { histymax =  hist1d->GetBinContent(bin); }
      if ( hist1d->GetBinContent(bin) < histymin ) { histymin =  hist1d->GetBinContent(bin); }
    }
  }


  if ( hname.Contains("Num") == false ) {
    histxmax *= 1.25;
    histxmin *= 1.25;
  }
    
  GetXminmax(hist1d, &xmin, &xmax, &ymin, &ymax, histxmin, histxmax, histymin, histymax, dolog);

  
  
  TString xtitle = hist1d->GetXaxis()->GetTitle();
  TString ytitle = hist1d->GetYaxis()->GetTitle();
  
  
  cout << "==========" << endl;
  cout << "Hist1D Name: " << hname << "\t\t" << xmin << "/" << xmax << "\t\t" << ymin << "/" << ymax << "\t<------ Using These." << endl;
  cout << "             " << hname << "\t\t" << histxmin <<"  / " << histxmax << "\t\t" << histymin << " / " << histymax << endl; 
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
    double lambda_gm2 = 29.0*0.14906;
    double omega_gm2 = 1.0/lambda_gm2;

    if ( hname.Contains("Freq") ) {
      TLine *l_gm2 = new TLine(lambda_gm2, 0.0, lambda_gm2, ymax*0.25);
      l_gm2->SetLineColor(kRed);
      l_gm2->SetLineWidth(2);
      l_gm2->Draw("SAME");

      stringstream ss_fft;    
      ss_fft << "#lambda_{g-2}=" << lambda_gm2;
      
      TLatex tl_fft;
      //tl_fft.SetNDC();
      tl_fft.SetTextColor(kRed);
      tl_fft.DrawLatex(0.95*lambda_gm2, ymax*0.35, ss_fft.str().c_str());

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

TH2F* GetHistogram(TFile *file, string name, double *int_prev, double *int_curr, double *int_start)
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

TH1F* GetHistogram1D(TFile *file, string name, double *int_prev, double *int_curr, double *int_start)
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


  TH1F *hist1d = (TH1F*)file->Get(histname.c_str());
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

  string subdir = "";
  if ( base.find("DownstreamMandrel") != string::npos ) { subdir = "DownstreamMandrel"; }
  if ( base.find("UpstreamMandrel") != string::npos ) { subdir = "UpstreamMandrel"; }
  if ( base.find("UpstreamCryo") != string::npos ) { subdir = "UpstreamCryo"; }
  if ( base.find("CentralOrbit") != string::npos ) { subdir = "CentralOrbit"; }
  if ( base.find("CentralOrbit_Offset77") != string::npos ) { subdir = "CentralOrbit_Offset77"; }

  if ( base.find("InflectorOpen") != string::npos ) { subdir += "_InflectorOpen"; }
  if ( base.find("ClosedInflector") != string::npos ) { subdir += "_ClosedInflector"; }
  if ( base.find("PartiallyOpen") != string::npos ) { subdir += "_PartiallyOpen"; }

  
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

  ifstream effin;
  string effname;
  effname = "Eff/" + subdir + "/" + base + ".dat";
  effin.open(effname.c_str());
  if ( effin.is_open() == false ) { cout << "Could not open " << effname << endl; return; }

  int Nstart_inflector = 0;
  int Nexit_inflector = 0;
  int Nstart_ring = 0;
  int Nstored1_ring = 0;
  int Nstored10_ring = 0;
  int Nstored100_ring = 0;
  int Ngen = 0;
  
  effin >> Nstart_inflector >> Nexit_inflector;
  effin >> Nstart_ring;
  effin >> Nstored1_ring >> Nstored10_ring >> Nstored100_ring;
  effin >> Ngen;

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
  

  if ( base.find("Perfect") != string::npos ) { plotinf = false; plotringhits = true; }
  if ( base.find("upstream") != string::npos ) { plotinf = true; }
  if ( base.find("Upstream") != string::npos ) { plotinf = true; }
  if ( base.find("downstream") != string::npos ) { plotinf = true; }
  if ( base.find("Downstream") != string::npos ) { plotinf = true; }
  if ( base.find("Mandrel") != string::npos ) { plotinf = true; }
  if ( base.find("Cryo") != string::npos ) { plotinf = true; }
  if ( base.find("Test") != string::npos ) { plotinf = true; }
  if ( base.find("test") != string::npos ) { plotinf = true; }
  if ( base.find("BeamSize40pi") != string::npos ) { plotinf = true; }
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


  int Nparticles = 0;
  string truth_particle_names[8];
  truth_particle_names[Nparticles++] = "BirthMuon";
  truth_particle_names[Nparticles++] = "DecayMuon";
  truth_particle_names[Nparticles++] = "StoredMuon";
  truth_particle_names[Nparticles++] = "LostMuon";
  truth_particle_names[Nparticles++] = "BirthElectron";
  truth_particle_names[Nparticles++] = "BirthElectronEgtEth";


  double int_prev = -1.0;
  double int_start = -1.0;
  double int_curr = -1.0;
  stringstream hname;


  int maxringtracks = 9;
  int maxsyshits = 5;
  int maxtimes = 3;
  bool plotinfstart = false;
  bool plotdiagnostic = true;


  bool plot_inflector = true;
  bool plot_ringtrackers = true;
  bool plot_kickerquads = true;
  bool plot_detectors = true;
  bool plot_phasespace = true;
  bool plot_truth = true;
  bool plot_evolution = true;


  //plot_inflector = false;
  plot_ringtrackers = false;
  plot_kickerquads = false;
  //plot_detectors = false;
  //plot_phasespace = false;
  //plot_truth = false;
  //plot_evolution = false;
  

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

  int Ntimestamps = 2;
  string timestamps[4];
  //timestamps[0] = "Turn";
  //timestamps[1] = "OncePerTurn";
  timestamps[0] = "Time";
  timestamps[1] = "TimeOncePerTurn";


  string times[3];
  times[0] = "Init";
  times[1] = "FinalAvg"; 
  maxtimes = 2;


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
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");

      hname << "InflectorTracker_" << itnames[i] << "_XprimeX";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
    
      hname << "InflectorTracker_" << itnames[i] << "_YprimeY";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
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


  
 



  //------------------------------------
  //
  // Evolution of Phase Space (Ring)
  //
  //------------------------------------
  if ( plot_evolution && plot_ringtrackers ) {

    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    Ring2Dhists[NRing2Dhists++] = "Rhat";
    Ring2Dhists[NRing2Dhists++] = "Y";
    Ring2Dhists[NRing2Dhists++] = "Prhat";
    Ring2Dhists[NRing2Dhists++] = "Pvhat";
    Ring2Dhists[NRing2Dhists++] = "Mom";
    Ring2Dhists[NRing2Dhists++] = "Pol";
    Ring2Dhists[NRing2Dhists++] = "PolX";
    Ring2Dhists[NRing2Dhists++] = "PolY";
    Ring2Dhists[NRing2Dhists++] = "Xe";
    Ring2Dhists[NRing2Dhists++] = "Zhat";


    string Ringbase = "Tracker";
    for ( int t = 0; t < Ntimestamps; t++ ) {
      string timestamp = timestamps[t];
      if ( timestamp == "OncePerTurn" ) { continue; }
      if ( timestamp == "TimeOncePerTurn" ) { continue; }
      
      for ( int n = 0; n < NRing2Dhists; n++ ) {
	  
	string histname = Ring2Dhists[n];

	hname << Ringbase << histname << "_" << timestamp;

	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);

	if ( histname == "Rhat" || histname == "Y" || histname == "Mom") {
	  if ( timestamp == "Turns" ) {
	    maxturns_for_plot = 25;
	    hname << "_" << maxturns_for_plot << "Turns";
	  }
	  if ( timestamp == "Time" ) {
	    maxturns_for_plot = 10;
	    hname << "_" << maxturns_for_plot << "ns";
	  }
	  TH2F *hist_maxturns = (TH2F*)hist->Clone(hname.str().c_str());
	  MakePlot(hist_maxturns, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	}	
	maxturns_for_plot = -1.0;

	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
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
  if ( plot_phasespace && plot_ringtrackers ) {

    int NRinghists = 0;
    string Ringhists[20];
    Ringhists[NRinghists++] = "Rhat";
    Ringhists[NRinghists++] = "Xe";
    Ringhists[NRinghists++] = "Vhat";
    Ringhists[NRinghists++] = "Xprime";
    Ringhists[NRinghists++] = "Yprime";
    Ringhists[NRinghists++] = "Mom";
    Ringhists[NRinghists++] = "Pol";
    Ringhists[NRinghists++] = "PolX";
    Ringhists[NRinghists++] = "PolY";
    Ringhists[NRinghists++] = "Zhat";

    int NRing2Dhists = 0;
    string Ring2Dhists[20];
    Ring2Dhists[NRing2Dhists++] = "RhatY";
    Ring2Dhists[NRing2Dhists++] = "XprimeX";
    Ring2Dhists[NRing2Dhists++] = "YprimeY";

    string Ringbase = "RingTracker_Time";


    for ( int t = 0; t < maxtimes; t++ ) {
      string time = times[t];      
      for ( int n = 0; n < NRinghists; n++ ) {
	string histname = Ringhists[n];
	  
	hname << Ringbase << "_" << histname << "_" << time << "_Init";
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
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
  if ( plot_phasespace && plot_truth ) {

    int NG4hists = 0;
    string G4hists[20];
    G4hists[NG4hists++] = "R";
    G4hists[NG4hists++] = "Rhat";
    G4hists[NG4hists++] = "Xe";
    G4hists[NG4hists++] = "Vhat";
    G4hists[NG4hists++] = "Xprime";
    G4hists[NG4hists++] = "Yprime";
    G4hists[NG4hists++] = "Mom";
    G4hists[NG4hists++] = "Pol";
    G4hists[NG4hists++] = "PolX";
    G4hists[NG4hists++] = "PolY";

    int NG42Dhists = 0;
    string G42Dhists[20];
    G42Dhists[NG42Dhists++] = "XZ";
    G42Dhists[NG42Dhists++] = "PolXY";
    G42Dhists[NG42Dhists++] = "RhatY";
    G42Dhists[NG42Dhists++] = "XprimeX";
    G42Dhists[NG42Dhists++] = "YprimeY";

    string G4base = "G4Track_Time";

    for ( int p = 0; p < Nparticles; p++ ) {
      string truth_part_name = truth_particle_names[p];

      if ( plot_Nud == false && truth_part_name.find("DecayElectron") != string::npos ) { continue; }
      
      for ( int t = 0; t < maxtimes; t++ ) {
	string time = times[t];

	if ( truth_part_name.find("LostMuon") != string::npos && time.find("Final") != string::npos ) { continue; }
	
	for ( int n = 0; n < NG4hists; n++ ) {

	  string histname = G4hists[n];
	  if ( truth_part_name.find("Electron") != string::npos ) {
	    if ( histname.find("Pol") != string::npos ) { continue; }
	    if ( histname.find("Xe") != string::npos ) { continue; }
	    if ( histname.find("Xprime") != string::npos ) { continue; }
	    if ( histname.find("Yprime") != string::npos ) { continue; }
	  }

	  hname << G4base << "_" << histname << "_" + time + "_" + truth_part_name;
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}

	
	for ( int n = 0; n < NG42Dhists; n++ ) {

	  string histname = G42Dhists[n];

	  if ( truth_part_name.find("LostMuon") != string::npos ) {
	    if ( histname.find("XprimeX") != string::npos ) { continue; }
	    if ( histname.find("YprimeY") != string::npos ) { continue; }
	  }

	  hname << G4base << "_" << histname << "_" + time + "_" + truth_part_name;
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting truth phase space distributions." << endl;
    cout << endl;
  }








  //------------------------------------
  //
  // Evolution of Phase Space (G4)
  //
  //------------------------------------
  if ( plot_evolution && plot_truth ) {

    int NG42Dhists = 0;
    string G42Dhists[20];
    G42Dhists[NG42Dhists++] = "R";
    G42Dhists[NG42Dhists++] = "Rhat";
    G42Dhists[NG42Dhists++] = "Y";
    G42Dhists[NG42Dhists++] = "Prhat";
    G42Dhists[NG42Dhists++] = "Pvhat";
    G42Dhists[NG42Dhists++] = "Mom";
    G42Dhists[NG42Dhists++] = "Pol";
    G42Dhists[NG42Dhists++] = "PolX";
    G42Dhists[NG42Dhists++] = "PolY";
    G42Dhists[NG42Dhists++] = "Xe";
    G42Dhists[NG42Dhists++] = "NumCounter";
    //NG42Dhists = 0;

    int NG4hists = 0;
    string G4hists[20];
    //G4hists[NG4hists++] = "NgtEth";
    G4hists[NG4hists++] = "Num";
    if ( plot_Nud ) {
      G4hists[NG4hists++] = "Vhat";
      G4hists[NG4hists++] = "Yprime";
      G4hists[NG4hists++] = "Nud";
    }

    string G4base = "G4Track";
    for ( int t = 0; t < Ntimestamps; t++ ) {
      string timestamp = timestamps[t];
      
      for ( int p = 0; p < Nparticles; p++ ) {
	string truth_part_name = truth_particle_names[p];

	if ( truth_part_name.find("BirthMuon") != string::npos ) { continue; }
	if ( truth_part_name.find("StoredMuon") != string::npos ) { continue; }
	//if ( truth_part_name.find("LostMuon") != string::npos ) { NG42Dhists = 0; }

	if ( plot_Nud == false && truth_part_name.find("DecayElectron") != string::npos ) { continue; }
	
	if ( (timestamp == "OncePerTurn" || timestamp == "TimeOncePerTurn") && truth_part_name.find("Muon") != string::npos ) { continue; }	

	for ( int n = 0; n < NG42Dhists; n++ ) {
	  
	  //cout << "2D: " << timestamp << "\t" << truth_part_name << "\t" << n << endl;

	  string histname = G42Dhists[n];
	  if ( truth_part_name.find("Electron") != string::npos ) {
	    if ( histname.find("Pol") != string::npos ) { continue; }
	    if ( histname.find("Xe") != string::npos ) { continue; }
	    if ( histname.find("Xprime") != string::npos ) { continue; }
	    if ( histname.find("Yprime") != string::npos ) { continue; }
	  }

	  if ( truth_part_name.find("LostMuon") != string::npos ) {
	    if ( histname.find("Prhat") != string::npos ) { continue; }
	    if ( histname.find("Pvhat") != string::npos ) { continue; }
	    if ( histname.find("Xe") != string::npos ) { continue; }
	  }

	  hname << G4base << histname << "_" << truth_part_name << "_" << timestamp;
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);

// 	  if ( truth_part_name.find("Muon") != string::npos || 1 ) {
// 	    maxturns_for_plot = 25;
// 	    hname << "_" << maxturns_for_plot << "Turns";
// 	    TH2F *hist_zoom = (TH2F*)hist->Clone(hname.str().c_str());
// 	    MakePlot(hist_zoom, -1, -1, &int_prev, &int_curr, &int_start);	    
// 	  }
// 	  minturns_for_plot = -1.0;
// 	  maxturns_for_plot = -1.0;

	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	}

	for ( int n = 0; n < NG4hists; n++ ) {

	  //cout << "1D: " << timestamp << "\t" << truth_part_name << "\t" << n << endl;

	  string histname = G4hists[n];

	  //cout << "1D: " << timestamp << "\t" << truth_part_name << "\t" << histname << endl;

	  if ( truth_part_name.find("Muon") != string::npos ) { 
	    if ( histname.find("Ngt") != string::npos ) { continue; }
	    if ( histname.find("Nwgt") != string::npos ) { continue; }
	  }

// 	  if ( truth_part_name.find("BirthElectron") != string::npos ||
// 	       truth_part_name.find("DecayElectron") != string::npos ) { 
// 	    if ( truth_part_name.find("EgtEth") != string::npos ) {
// 	      if ( histname.find("NgtEth") != string::npos ) { continue; }
// 	      if ( histname.find("Num") != string::npos ) { ; }
// 	    }
// 	    else {
// 	      if ( histname.find("NgtEth") != string::npos ) { continue; }
// 	      if ( histname.find("Num") != string::npos ) { continue; }

	  
	  hname << G4base << histname << "_" << truth_part_name << "_" << timestamps[t];
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);


	  if ( histname == "Num" ) {
	    for ( int np = 0; np < 3; np++ ) {
	      if ( np == 0 ) {
		minturns_for_plot = 1;
		maxturns_for_plot = 5;
	      }
	      if ( np == 1 ) {
		minturns_for_plot = 200;
		maxturns_for_plot = 225;
	      }
	      if ( np == 2 ) {
		minturns_for_plot = 10;
		maxturns_for_plot = 20;
	      }
	      if ( timestamps[t].find("Time") != string::npos ) {
		if ( np == 0 ) {
		  minturns_for_plot = 1;
		  maxturns_for_plot = 6;
		}
		if ( np == 1 ) {
		  minturns_for_plot = 30;
		  maxturns_for_plot = 35;
		}
		if ( np == 2 ) {
		  minturns_for_plot = 15;
		  maxturns_for_plot = 20;
		}
	      }


	      hname.str("");
	      hname << G4base << histname << "_" << truth_part_name << "_" << timestamps[t];
	      cout << hname.str() << endl;
	      
	      if ( timestamps[t].find("Time") == string::npos ) {
		hname << "_" << minturns_for_plot << "_" << maxturns_for_plot << "Turns";
	      }
	      else {
		hname << "_" << minturns_for_plot << "_" << maxturns_for_plot << "Time";
	      }
	      TH1F *hist1d_zoom = (TH1F*)(hist1d->Clone(hname.str().c_str()));
	      MakePlot1D(hist1d_zoom, -1, -1, &int_prev, &int_curr, &int_start);
	      hname.str("");
	      hname << G4base << histname << "_" << truth_part_name << "_" << timestamps[t] << "_x";
	      hname << "_" << minturns_for_plot << "_" << maxturns_for_plot << "Time";
	      TH1F *hist1d_zoom_fft = (TH1F*)(hist1d->Clone(hname.str().c_str()));
	      ComputeFFT(hist1d_zoom_fft, hname.str());
	      hname.str("");
	    }
	  }
	  maxturns_for_plot = -1.0;
	  minturns_for_plot = -1.0;

	  hname.str("");
	  hname << G4base << histname << "_" << truth_part_name << "_" << timestamps[t] << "_x";
	  TH1F *hist1d_fft = (TH1F*)(hist1d->Clone(hname.str().c_str()));
	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);

	  if ( truth_part_name.find("Electron") == string::npos ) { hname.str(""); continue; }
	  cout << "Bins = " << hist1d_fft->GetNbinsX() << endl;
	  ComputeFFT(hist1d_fft, hname.str());
	  hname.str("");
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

    bool plotdet = true;;
    for ( int i = 0; i < Nsysnames; i++ ) {
      for ( int st = 0; st < 1; st++ ) {
	if ( plotdet == false ) { continue; }
	string stname = "";
      
	if ( plot_detectors == false ) { continue; }
	for ( int n = 0; n < NSystem2Dhists; n++ ) {
	  hname << rhitnames[i] << "Hits" << stname << "_" << System2Dhists[n];
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
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
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      }
    }
	
// 	hname << rhitnames[i] << stname << "_Nhits";
// 	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
// 	MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
// 	hname.str("");

// 	if ( 0 ) {
// 	  hname << rhitnames[i] << "Hits" << stname << "_RhatTime";
// 	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
// 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
// 	  hname.str("");

// 	  zoom = true; zoom2 = false; zoom3 = false;
// 	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
// 	  hname.str("");

// 	  zoom = false; zoom2 = false; zoom3 = false;

// 	  hname << rhitnames[i] << "Hits" << stname << "_YTime";
// 	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
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
