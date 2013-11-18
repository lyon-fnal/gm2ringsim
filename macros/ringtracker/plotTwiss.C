Double_t myfunction(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Float_t freq = par[2];
  Float_t corr = xx-par[3];
  Double_t f = par[0] + par[1]*sin(par[2]*xx + par[3]);
  return f;

}Double_t Invmyfunction(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Float_t freq = par[2];
  Float_t corr = xx-par[3];
  Double_t f = par[0] + par[1]*sin(par[2]*xx + par[3]);
  return 1/f;
}

Double_t myalpha(Double_t *x, Double_t *par)
{
  Float_t xx =x[0];
  Float_t freq = par[2];
  Float_t corr = xx-par[3];
  //Double_t f = par[0] + par[1]*(par[2]*xx + par[3]);
  Double_t f = -1.0/2*par[0]*cos(par[1]*xx + par[2]);
  return f;
}

void plotTwiss()
{
  TFile *f = TFile::Open("twiss_n18.root");
  
  double smax = 0.0;
  ifstream in;
  if ( !f ) {
    cout << "Creating new root output." << endl;
    f = new TFile("twiss_n18.root", "RECREATE");
    //in.open("twiss_functions.dat");
    in.open("twiss_n18.dat");
  
    string line;
    int n = 0;

    int np = 0;
    TGraph *g_betax = new TGraph();
    TGraph *g_betay = new TGraph();
    TGraph *g_alphax = new TGraph();
    TGraph *g_alphay = new TGraph();
    TGraph *g_etax = new TGraph();
    TGraph *g_etapx = new TGraph();


    double s, betax, betay, alphax, alphay, etax, etapx;
    string element;
    while ( getline(in, line) ) {
      if ( n == 0 ) { n++; continue; }
      stringstream ss(line);
      if ( line.find("#") != string::npos ) { n++; continue; }
      ss >> s >> betax >> alphax >> betay >> alphay >> etax >> etapx >> element;
      n++;

      if ( s > smax ) { smax = s; }
    
      g_betax->SetPoint(np, s, betax);
      g_betay->SetPoint(np, s, betay);
      g_alphax->SetPoint(np, s, alphax);
      g_alphay->SetPoint(np, s, alphay);
      g_etax->SetPoint(np, s, etax);
      g_etapx->SetPoint(np, s, etapx);
    
      np++;

      if ( (np-1) % 100 == 0 ) { cout << "s = " << s << endl; }

      //if ( np > 1500 ) { break; }    
    }

    in.close();

    g_betax->Write("betaX");
    g_betay->Write("betaY");
    g_alphax->Write("alphaX");
    g_alphay->Write("alphaY");
    g_etax->Write("etaX");
    g_etapx->Write("etapX");

    f->Close();
    return;
  }

  TGraph *g_betax = (TGraph*)f->Get("betaX");
  TGraph *g_betay = (TGraph*)f->Get("betaY");
  TGraph *g_alphax = (TGraph*)f->Get("alphaX");
  TGraph *g_alphay = (TGraph*)f->Get("alphaY");
  TGraph *g_etax = (TGraph*)f->Get("etaX");
  TGraph *g_etapx = (TGraph*)f->Get("etapX");
  
  
  if ( 0 ) {
    g_betay->SetMinimum(0.0);
    g_betay->Draw("ALP");
    g_betax->Draw("LP");
  }


  bool doeta = false;
  bool doetap = false;
  bool dobeta = false;
  bool dobetainv = false;
  bool dobetaY = false;
  bool dobetaYinv = true;
  
  //
  // Eta fits
  //
  if ( doeta || doetap ) {
    int etan = g_etax->GetN();
    Double_t *etax = g_etax->GetX();
    smax = etax[etan-1];
  
  
    TF1 *etafit = new TF1("etamyfuncx",myfunction,0,smax,4);
  
    double Off = 8.26;
    etafit->SetParameter(0, Off);
    etafit->SetParLimits(0, Off - 0.5, Off + 0.5);
  
    double A = 0.110;
    etafit->SetParameter(1, A);
    etafit->SetParLimits(1, A/2, A*2);
  
    double l = 0.56;
    etafit->SetParameter(2, l);
    etafit->SetParLimits(2, l/2, l*2);
  
    double phi = 1.56;
    etafit->SetParameter(3, phi);
    etafit->SetParLimits(3, phi - 0.2, phi + 0.2);

    g_etax->Fit("etamyfuncx");
    g_etax->Draw("ALP");

    for ( int i = 0; i < 4; i++ ) {
      cout << "fEtaX->SetParameter(" << i << ", " << etafit->GetParameter(i) << ");" << endl;
    }

    if ( doetap ) {
      g_etapx->Draw("ALP");
      etafit->DrawDerivative("SAME");
    }
  }

  

  if ( dobeta || dobetainv ) {

    int n = g_betax->GetN();
    Double_t *x = g_betax->GetX();
    smax = x[n-1];

  
    TF1 *fit = new TF1("myfuncx",myfunction,0,smax,4);

    double Off = 7.7;
    fit->SetParameter(0, Off);
    fit->SetParLimits(0, Off - 0.5, Off + 0.5);
  
    double A = 0.25;
    fit->SetParameter(1, A);
    fit->SetParLimits(1, A/2, A*2);
  
    double l = 0.56;
    fit->SetParameter(2, l);
    fit->SetParLimits(2, l/2, l*2);
  
    double phi = 1.57;
    fit->SetParameter(3, phi);
    fit->SetParLimits(3, phi - 0.2, phi + 0.2);

    g_betax->Fit("myfuncx");

    for ( int i = 0; i < 4; i++ ) {
      cout << "fBetaX->SetParameter(" << i << ", " << fit->GetParameter(i) << ");" << endl;
    }


    if ( dobetainv ) {
      TF1 *Invfit = new TF1("myfuncx",Invmyfunction,0,smax,4);
      Invfit->SetParameter(0, 7.86248);
      Invfit->SetParameter(1, 0.290301);
      Invfit->SetParameter(2, 0.562481);
      Invfit->SetParameter(3, 1.613);
      Invfit->DrawIntegral();
      return;
    }
  }  
    //g_betax->Draw("ALP");
  

  if ( dobetaY || dobetaYinv ) {

    int n = g_betay->GetN();
    Double_t *x = g_betay->GetX();
    smax = x[n-1];

      TF1 *fit = new TF1("myfuncy",myfunction,0,smax,4);

      double Off = 18.84;
      fit->SetParameter(0, Off);
      fit->SetParLimits(0, Off - 0.5, Off + 0.5);
  
      double A = 0.49;
      fit->SetParameter(1, A);
      fit->SetParLimits(1, A/2, A*2);
  
      double l = 0.56;
      fit->SetParameter(2, l);
      fit->SetParLimits(2, l/2, l*2);
  
      double phi = -1.58;
      fit->SetParameter(3, phi);
      fit->SetParLimits(3, phi - 0.2, phi + 0.2);

      g_betay->Fit("myfuncy");

      for ( int i = 0; i < 4; i++ ) {
	cout << "fBetaY->SetParameter(" << i << ", " << fit->GetParameter(i) << ");" << endl;
      }

      //g_betay->Draw("ALP");
      
      if ( dobetaYinv ) {
	TF1 *InvfitY = new TF1("myInvfuncY",Invmyfunction,0,smax,4);
	InvfitY->SetParameter(0, 18.34);
	InvfitY->SetParameter(1, 0.59329);
	InvfitY->SetParameter(2, 0.584521);
	InvfitY->SetParameter(3, -1.78);
	InvfitY->DrawIntegral();
	return;
      }
      
  }
}
