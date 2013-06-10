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
TH2F *HitsRhoY[21];

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

  ymin = hist->GetYaxis()->GetXmin();
  ymax = hist->GetYaxis()->GetXmax();
  TString hname = hist->GetName();


  if ( zoom ) {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = hist->GetXaxis()->GetXmax()/20;
    xmin = 0.0;
    xmax = 10.0;
    if ( hname.Contains("Time") ) {
      xmax = 2.0;
    }
  }
  else if ( zoom2 ) {
    xmin = 400.0;
    xmax = 410.0;
    if ( hname.Contains("Time") ) {
      xmin = 20.0;
      xmax = 22.0;
    }
    
  }
  else if ( zoom3 ) {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = 1;
  }
  else {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = hist->GetXaxis()->GetXmax();
  }

  //cout << "Xmin: " << xmin << "\t" << "Xmax: " << xmax << endl;
  
  bool rename_x = false;
  if ( hname.Contains("RingTracker") && hname.Contains("RhoY") ) {
    xmin = -125.0; xmax = 125.0;
    ymin = -125.0; ymax = 125.0;
  }
  else if ( hname.Contains("RingTracker") && ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) ) {
    xmin = -125.0; xmax = 125.0;
    ymin = -1.5e-1; ymax = 1.5e-1;
  }
  else if ( hname.Contains("G4Track") && ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) ) {
    xmin = -125.0; xmax = 125.0;
    ymin = -10; ymax = 10;
  }
  else if ( hname.Contains("Inflector") && ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) ) {
    xmin = -125.0; xmax = 125.0;
    ymin = -10.0; ymax = 10.0;

    TH1D *xprof = (TH1D*)hist->ProjectionX();
    bool setxmin = false;
    int stopxbinmin = 1;
    int stopxbinmax = 1;
    int stopybinmin = 1;
    int stopybinmax = 1;

    if ( 1 ) {
      int xoffset = 4;
      int yoffset = 5;
      if ( xprof ) {
	stopxbinmax = xprof->GetNbinsX()-1;
	for ( int bin = 1; bin < xprof->GetNbinsX()-1; bin++ ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    xmin = xprof->GetBinLowEdge(bin-xoffset);
	    stopxbinmin = bin+1;
	    break;
	  }
	}
	for ( int bin = xprof->GetNbinsX()-1; bin >= stopxbinmin; bin-- ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    xmax = xprof->GetBinLowEdge(bin+xoffset);
	    stopxbinmax = bin;
	    break;
	  }
	}
      }
    }
  }
  else if ( hname.Contains("Muon") && hname.Contains("Position") ) { 
    rename_x = true;
    if ( hname.Contains("Radial") ) {
      ymin = -150.0; ymax = 300.0;
    }
    if ( hname.Contains("Vertical") ) {
      ymin = -100.0; ymax = 100.0;
    }
  }
  else if ( hname.Contains("Muon") && hname.Contains("Momentum") ) { 
    rename_x = true;
    if ( hname.Contains("Radial") ) {
      ymin = -0.25; ymax = 0.2;
    }
    if ( hname.Contains("Vertical") ) {
      ymin = -0.15; ymax = 0.15;
    }
  }
  else if ( ( hname.Contains("Time") || ( hname.Contains("Turn") && !hname.Contains("theta") ) ) ) {
    //if ( !zoom && !zoom2 && !zoom3 ) { xmax = 200.0; }
    if ( !zoom && !zoom2 && !zoom3 ) {
      TH1D *xprof = (TH1D*)hist->ProjectionX();
      if ( xprof ) {
	int stopbin = xprof->GetNbinsX()-1;
	for ( int bin = xprof->GetNbinsX()-1; bin > 2; bin-- ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    if ( xprof->GetBinContent(bin-1) > 0 ) {
	      stopbin = bin;
	      break;
	    }
	  }
	}
	xmax = xprof->GetBinLowEdge(stopbin) + 2*xprof->GetBinWidth(stopbin);
      }
    }

    //cout << "Xmin: " << xmin << "\t" << "Xmax: " << xmax << endl;
  }
  else if ( hname.Contains("Quadxxx") || hname.Contains("Kickerxxx") ) {
    TH1D *xprof = (TH1D*)hist->ProjectionX();
    bool setxmin = false;
    int stopxbinmin = 1;
    int stopxbinmax = 1;
    int stopybinmin = 1;
    int stopybinmax = 1;
    if ( 1 ) {
      if ( xprof ) {
	stopxbinmax = xprof->GetNbinsX()-1;
	for ( int bin = 1; bin < xprof->GetNbinsX()-1; bin++ ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    if ( xprof->GetBinContent(bin+1) > 0 ) {
	      xmin = xprof->GetBinLowEdge(bin-3);
	      //cout << xprof->GetBinLowEdge(bin) << "\t" << xprof->GetBinContent(bin) << "\t"<< xprof->GetBinContent(bin+1) << endl;
	      stopxbinmin = bin;
	      break;
	    }
	  }
	}
	for ( int bin = xprof->GetNbinsX()-1; bin > 0; bin-- ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    if ( xprof->GetBinContent(bin-1) > 0 ) {
	      xmax = xprof->GetBinLowEdge(bin+3);
	      stopxbinmax = bin;
	      break;
	    }
	  }
	}
      }
    }
  }
  else if ( hname.Contains("_XZ") ) {
    ;
  }
  else { 
    ;//ymin = -20.0; ymax = 20.0;
  }


  if ( hname.Contains("InflectorTracker") || hname.Contains("RingTracker") || hname.Contains("Upstream") || hname.Contains("Downstream") || hname.Contains("XprimeX") || hname.Contains("YprimeY") || hname.Contains("Flange") || hname.Contains("Mandrel") || hname.Contains("BeamChannel") || hname.Contains("Window") || hname.Contains("Cryostat") || hname.Contains("InflectorHits") || hname.Contains("Equivalent") || hname.Contains("Equivalent") || hname.Contains("thetaXYx") ) { 
    TH1D *xprof = (TH1D*)hist->ProjectionX();
    TH1D *yprof = (TH1D*)hist->ProjectionY();
    bool setxmin = false;
    int stopxbinmin = 1;
    int stopxbinmax = 1;
    int stopybinmin = 1;
    int stopybinmax = 1;
    int xoffset = 4;
    int yoffset = 5;
    if ( 1 ) {
      if ( xprof ) {
	stopxbinmax = xprof->GetNbinsX()-1;
	for ( int bin = 1; bin < xprof->GetNbinsX()-1; bin++ ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    xmin = xprof->GetBinLowEdge(bin-xoffset);
	    stopxbinmin = bin+1;
	    break;
	  }
	}
	for ( int bin = xprof->GetNbinsX()-1; bin >= stopxbinmin; bin-- ) {
	  if ( xprof->GetBinContent(bin) > 0 ) {
	    xmax = xprof->GetBinLowEdge(bin+xoffset);
	    stopxbinmax = bin;
	    break;
	  }
	}
      }
    }
      
    if ( 1 ) {
      if ( yprof ) {
	stopybinmax = yprof->GetNbinsX()-1;
	for ( int bin = 1; bin < yprof->GetNbinsX()-1; bin++ ) {
	  if ( yprof->GetBinContent(bin) > 0 ) {
	    ymin = yprof->GetBinLowEdge(bin-yoffset);
	    stopybinmin = bin+1;
	    break;
	  }
	}
	for ( int bin = yprof->GetNbinsX()-1; bin >= stopybinmin; bin-- ) {
	  if ( yprof->GetBinContent(bin) > 0 ) {
	    ymax = yprof->GetBinLowEdge(bin+yoffset);
	    stopybinmax = bin;
	    break;
	  }
	}
      }
    }
    ;//cout << hname << "\t" << "X=[" << xmin << "," << xmax << " - Y=[" << ymin <<"," << ymax << "]" << endl;
  }

  if ( 0 ) {
    if ( hname.Contains("InflectorTracker") ) {
      if ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) {
	// 	if ( ymax > 0.02 ) { ymax = 0.02; }
	// 	if ( ymax < 0.01 ) { ymax = 0.01; }
	// 	if ( ymin < -0.02 ) { ymin = -0.02; }
	// 	if ( ymin > -0.01 ) { ymin = -0.01; }
	// 	//ymax = TMath::Max(0.01, ymax);
	//ymin = TMath::Min(-0.01, ymin);
      }
    }
  }

  if ( hname.Contains("RhoY") || hname.Contains("YTime") ) {
    if ( hname.Contains("Turn") || hname.Contains("RingTracker") ) {
      xmin = -60.0; xmax = 60.0;
      ymin = -100.0/1.25; ymax = 100.0/1.25;
      if ( start_inflector ) {
	xmin = -60.0; xmax = 60.0;
	ymin = -60.0; ymax = 60.0;
      }
    }
    if ( hname.Contains("Inflector") ) {
      if ( xmin < -75 ) { xmin = -75.0; }
      if ( ymin < -75 ) { ymin = -75.0; }
      if ( ymax > 75 ) { ymax = 75.0; }
      if ( hname.Contains("RingTracker") ) {
	//if ( start_inflector ) {
	xmin = -60.0; xmax = 60.0;
      }
    }
    if ( hname.Contains("Kicker") ) {
      if ( xmax > 75 ) { xmax = 75.0; }
      if ( xmin < -75 ) { xmin = -75.0; }
      if ( ymin < -60 ) { ymin = -60.0; }
      if ( ymax > 60 ) { ymax = 60.0; }
    }
    if ( hname.Contains("Collimator") ) {
      if ( xmax > 75 ) { xmax = 75.0; }
      if ( xmin < -75 ) { xmin = -75.0; }
      if ( ymin < -75 ) { ymin = -75.0; }
      if ( ymax > 75 ) { ymax = 75.0; }
    }
    if ( hname.Contains("Quad") ) {
      if ( xmax > 150 ) { xmax = 150.0; }
      if ( xmin < -150 ) { xmin = -150.0; }
      if ( ymin < -75 ) { ymin = -75.0; }
      if ( ymax > 75 ) { ymax = 75.0; }
    }
    if ( hname.Contains("Vacuum") ) {
      if ( xmax > 150 ) { xmax = 150.0; }
      if ( xmin < -300 ) { xmin = -300.0; }
      if ( ymin < -100 ) { ymin = -100.0; }
      if ( ymax > 100 ) { ymax = 100.0; }
    }
    if ( hname.Contains("Straw") || hname.Contains("Calo") || hname.Contains("Xtal") ) {
      xmax = 0.0;
      if ( xmin < -300 ) { xmin = -300.0; }
      if ( ymin < -100 ) { ymin = -100.0; }
      if ( ymax > 100 ) { ymax = 100.0; }
    }
    ymax *= 1.25;
  }

  if ( hname.Contains("Time_XprimeX") || hname.Contains("Time_YprimeY") ) {
    xmin = -50.0;
    xmax = 50.0;
    ymax = 15;
    ymin = -15;
    //     if ( start_inflector ) {
    //       xmin = -100.0; xmax = 100.0;
    //     }
  }

  if ( hname.Contains("thetaXY") ) {
    xmax = TMath::Max(0.01, xmax);
    xmin = TMath::Min(-0.01, xmin);
    ymax = TMath::Max(0.01, ymax);
    ymin = TMath::Min(-0.01, ymin);
  }

  if ( hname.Contains("RingUnit") == false ) {
    if ( hname.Contains("InflectorTracker") ) {
      if ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) {
	ymax = TMath::Max(ymax, 10.0);
	ymin = TMath::Min(ymin, -10.0);
      }
    }
  }

  if ( hname.Contains("TrackerPvhat") || hname.Contains("TrackerPrhat")) {
    ymax = 15.0;
    ymin = -15.0;
  }

  if ( hname.Contains("RingTracker") ) {
    if ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) {
      if ( ymax > 25 ) { ymax = 25.0; }
      else if ( ymax < 10 ) { ymax = 10.0; }
      
      if ( ymin < -25 ) { ymin = -25.0; }
      else if ( ymin > -10 ) { ymin = -10.0; }
    }
  }

  if ( hname.Contains("20us") || hname.Contains("50us") ) {
    if ( hname.Contains("XprimeYprime") ) {
      if ( ymax < 5 ) { ymax = 5.0; }
      if ( ymin > -5 ) { ymin = -5.0; }
      if ( xmax < 5 ) { xmax = 5.0; }
      if ( xmin > -5 ) { xmin = -5.0; }
    }    
  }
  
  if ( hname.Contains("G4Track") ) {
    if ( hname.Contains("XprimeX") ) {
      xmin = -30; xmax = 30;
      ymin = -5; ymax = 5;
      if ( dir.find("NoLaunch") != string::npos ) { ymin = -5; ymax = 5; }
      if ( dir.find("Launchm7") != string::npos ) { ymin -= 7; ymax -= 7; }
      if ( dir.find("Launchm5") != string::npos ) { ymin -= 5; ymax -= 5; }
      if ( dir.find("Launchm3") != string::npos ) { ymin -= 3; ymax -= 3; }
      if ( dir.find("Launchp2") != string::npos ) { ymin += 2; ymax += 2; }
      if ( dir.find("Launchp5") != string::npos ) { ymin += 5; ymax += 5; }
      if ( dir.find("Launchm10") != string::npos ) { ymin -= 10; ymax -= 10; }
      if ( dir.find("Launchm15") != string::npos ) { ymin -= 15; ymax -= 15; }
      
      if ( dir.find("Deltam7") != string::npos ) { ymin -= 7; ymax -= 7; }
      if ( dir.find("Deltam5") != string::npos ) { ymin -= 5; ymax -= 5; }
      if ( dir.find("Deltam3") != string::npos ) { ymin -= 3; ymax -= 3; }
      if ( dir.find("Deltap2") != string::npos ) { ymin += 2; ymax += 2; }
      if ( dir.find("Deltap5") != string::npos ) { ymin += 5; ymax += 5; }
      if ( dir.find("Deltam10") != string::npos ) { ymin -= 10; ymax -= 10; }
      if ( dir.find("Deltam15") != string::npos ) { ymin -= 15; ymax -= 15; }
	
    }
    if ( hname.Contains("YprimeY") ) {
      xmin = -40.0; xmax = 40.0;
      ymin = -5; ymax = 5;
    }
    if ( hname.Contains("RhoY") ) {
      xmin = -30.0; xmax = 30.0;
      ymin = -40;   ymax = 40;
    }
    if ( 0 ) {
      if ( hname.Contains("RhoY") || hname.Contains("XprimeX") || hname.Contains("YprimeY") ) {
	TH1D *xprof = (TH1D*)hist->ProjectionX();
	TH1D *yprof = (TH1D*)hist->ProjectionY();
	bool setxmin = false;
	int stopxbinmin = 1;
	int stopxbinmax = 1;
	int stopybinmin = 1;
	int stopybinmax = 1;
	int xoffset = 4;
	int yoffset = 5;
	if ( 1 ) {
	  if ( xprof ) {
	    stopxbinmax = xprof->GetNbinsX()-1;
	    for ( int bin = 1; bin < xprof->GetNbinsX()-1; bin++ ) {
	      if ( xprof->GetBinContent(bin) > 0 ) {
		xmin = xprof->GetBinLowEdge(bin-xoffset);
		stopxbinmin = bin+1;
		break;
	      }
	    }
	    for ( int bin = xprof->GetNbinsX()-1; bin >= stopxbinmin; bin-- ) {
	      if ( xprof->GetBinContent(bin) > 0 ) {
		xmax = xprof->GetBinLowEdge(bin+xoffset);
		stopxbinmax = bin;
		break;
	      }
	    }
	  }
	}
      
	if ( 1 ) {
	  if ( yprof ) {
	    stopybinmax = yprof->GetNbinsX()-1;
	    for ( int bin = 1; bin < yprof->GetNbinsX()-1; bin++ ) {
	      if ( yprof->GetBinContent(bin) > 0 ) {
		ymin = yprof->GetBinLowEdge(bin-yoffset);
		stopybinmin = bin+1;
		break;
	      }
	    }
	    for ( int bin = yprof->GetNbinsX()-1; bin >= stopybinmin; bin-- ) {
	      if ( yprof->GetBinContent(bin) > 0 ) {
		ymax = yprof->GetBinLowEdge(bin+yoffset);
		stopybinmax = bin;
		break;
	      }
	    }
	  }
	}
      }
    }
  }



  // Check hist min, max
  TH1D *yprof = (TH1D*)hist->ProjectionY();
  for ( int bin = 1; bin < yprof->GetNbinsX()-1; bin++ ) {
    double bincont = yprof->GetBinContent(bin);
    if ( bincont > histymax ) { histymax = bincont; }
    if ( bincont < histymin ) { histymin = bincont; }
  }


  if ( hname.Contains("RingTracker") && hname.Contains("Time") ) {     

    if ( hname.Contains("RhoY") ) {      
      double ymin_loc = -49;
      double ymax_loc = 49;     
      double xmin_loc = -49;
      double xmax_loc = 49;

      if ( xmax_loc < 25 ) { xmax = 25.0; }
      else if ( xmax_loc < 50 ) { xmax = 50.0; }
      else if ( xmax_loc < 75 ) { xmax = 75.0; }

      if ( xmin_loc > -25 ) { xmin = -25.0; }
      else if ( xmin_loc > -50 ) { xmin = -50.0; }
      else if ( xmin_loc > -75 ) { xmin = -75.0; }      

      if ( ymax_loc < 25 ) { ymax = 25.0; }
      else if ( ymax_loc < 50 ) { ymax = 50.0; }
      else if ( ymax_loc < 75 ) { ymax = 75.0; }

      if ( ymin_loc > -25 ) { ymin = -25.0; }
      else if ( ymin_loc > -50 ) { ymin = -50.0; }
      else if ( ymin_loc > -75 ) { ymin = -75.0; }      
    }
  
    if ( hname.Contains("XprimeX") || hname.Contains("YprimeY") ) {
      if ( ymax > 30 ) { ymax = 30.0; }
      else if ( ymax < 20 ) { ymax = 20.0; }

      if ( ymin < -30 ) { ymin = -30.0; }
      else if ( ymin > -20 ) { ymin = -20.0; }
        
      //cout << xmin << "," << xmax << "\t" << ymin << "," << ymax << endl;
      if ( hname.Contains("20us") || hname.Contains("50us") ) {
	double xmin_loc = -49;
	double xmax_loc = 49;
	double ymin_loc = -6;
	double ymax_loc = 6;
	if ( ymax_loc < 7 ) { ymax = 7.0; }
	else if ( ymax_loc < 10 ) { ymax = 10.0; }
	else if ( ymax_loc < 15 ) { ymax = 15.0; }
	
	if ( ymin_loc > -7 ) { ymin = -7.0; }
	else if ( ymin_loc > -10 ) { ymin = -10.0; }
	else if ( ymin_loc > -15 ) { ymin = -15.0; }
	
	if ( xmax < 25 ) { xmax = 25.0; }
	else if ( xmax < 50 ) { xmax = 50.0; }
	else if ( xmax < 75 ) { xmax = 75.0; }
	
	if ( xmin > -25 ) { xmin = -25.0; }
	else if ( xmin > -50 ) { xmin = -50.0; }
	else if ( xmin > -75 ) { xmin = -75.0; }   

	xmax = 50.0;
	xmin = -50.0;
	ymin = -7.5;
	ymax = 7.5;
	
      }
    }
  
    if ( hname.Contains("XprimeYprime") ) {
      if ( hname.Contains("20us") || hname.Contains("50us") ) {
	xmax = 7.5;
	xmin = -7.5;
	ymin = -7.5;
	ymax = 7.5;
      }
    }
  }


  if ( hname.Contains("PrhatPvhat") ) {
    xmax = 0.3;
    xmin = -0.3;
    ymax = 0.025;
    ymin = -0.025;
  }


  if ( hname.Contains("MomTurn") ) {
    ymax = 1.01;
    ymin = 0.99;

    if ( histymax < 1+0.005 && histymin > 1-0.005 ) {
      ymax = 1+0.005; ymin = 1-0.005;
    }

    if ( histymax < 1+0.0025 && histymin > 1-0.0025 ) {
      ymax = 1+0.0025; ymin = 1-0.0025;
    }
  }


  if ( hname.Contains("PvhatTurn") || hname.Contains("PrhatTurn") ) {
    if ( hname.Contains("Electron") ) {
      ymax = 100.0; ymin = -100.0;
      if ( histymax < 50 && histymin > -50 ) {
	ymax = 50; ymin = -50;
      }
      
      if ( histymax < 25 && histymin > -25 ) {
	ymax = 25; ymin = -25;
      }
    }
    else {
      ymax = 12.5; ymin = -12.5;
      if ( histymax < 10 && histymin > -10 ) {
	ymax = 10; ymin = -10;
      }
      
      if ( histymax < 5 && histymin > -5 ) {
	ymax = 5; ymin = -5;
      }
    }
  }


  if ( hname.Contains("XeTurn") || hname.Contains("XeOncePerTurn") ) {
    ymax = 30.0; ymin = -30.0;
    if ( histymax < 20 && histymin > -20 ) {
      ymax = 20; ymin = -20;
    }

    if ( histymax < 10 && histymin > -10 ) {
      ymax = 10; ymin = -10;
    }
  }


  if ( hname.Contains("Electron") ) {
    if ( hname.Contains("Xprime") || hname.Contains("Yprime") ) {
      xmax = 45.0; xmin = -45.0;
      ymax = 100.0; ymin = -100.0;
      if ( histymax < 50 && histymin > -50 ) {
	ymax = 50; ymin = -50;
      }
      
      if ( histymax < 25 && histymin > -25 ) {
	ymax = 25; ymin = -25;
      }
    }
    if ( hname.Contains("RhoY") ) {
      xmax = 45.0; xmin = -45.0;
      ymax = 45.0; ymin = -45.0;
    }
  }



  TString xtitle = hist->GetXaxis()->GetTitle();
  TString ytitle = hist->GetYaxis()->GetTitle();

  if ( 0 ) {
    TH1D *xdist = (TH1D*)hist->ProjectionX();
    TH1D *ydist = (TH1D*)hist->ProjectionY();
    int minxbin = hist->FindBin(xmin);
    int maxxbin = hist->FindBin(xmax);
    int minybin = hist->FindBin(ymin);
    int maxybin = hist->FindBin(ymax);
  
    if ( minxbin > 1 ) {
      hist->AddBinContent(minxbin, hist->Integral(0, minxbin-1));
    }
    if ( maxxbin < hist->GetNbinsX() -1 ) {
      hist->AddBinContent(maxxbin, hist->Integral(minxbin+1, hist->GetNbinsX()));
    }
    int minybin = hist->FindBin(xmin);
    int maxybin = hist->FindBin(xmax);
    if ( minybin > 1 ) {
      hist->AddBinContent(minybin, hist->Integral(0, minybin-1));
    }
    if ( maxybin < hist->GetNbinsX() -1 ) {
      hist->AddBinContent(maxybin, hist->Integral(minybin+1, hist->GetNbinsX()));
    }
  }


  //cout << "\tX=[" << xmin << " , " << xmax << "]  ;  Y=[" << ymin << " , " << ymax << "]" << endl;

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


  cout << hist->GetEntries() << "\t" << Ngen << endl;
  geteff(hist->GetEntries(), upstream_inflector_yield, &abseff, &abserr);
  ss_abseff << SurviveText(abseff, abserr, "");
  
  
  if ( r == -1 ) {
    ss.str("");
    TString hname = hist->GetName();
    if ( hname.Contains("InflectorTracker") ) {
      if ( hname.Contains("_0") ) { ss << "#deltaZ=-L [mm]"; }
      if ( hname.Contains("_1") ) { ss << "#deltaZ=-7L/8 [mm]"; }
      if ( hname.Contains("_2") ) { ss << "#deltaZ=-3L/4 [mm]"; }
      if ( hname.Contains("_3") ) { ss << "#deltaZ=-5L/8 [mm]"; }
      if ( hname.Contains("_4") ) { ss << "#deltaZ=-L/2 [mm]"; }
      if ( hname.Contains("_5") ) { ss << "#deltaZ=-3L/8 [mm]"; }
      if ( hname.Contains("_6") ) { ss << "#deltaZ=-L/4 [mm]"; }
      if ( hname.Contains("_7") ) { ss << "#deltaZ=-L/8 [mm]"; }
      if ( hname.Contains("_8") ) { ss << "#deltaZ=0 [mm]"; }
    }
    if ( hname.Contains("Equivalent") ) {
      if ( hname.Contains("Upstream") ) {
	if ( hname.Contains("NbTi_") ) { ss << "#equivNbTi (US)"; } 
	if ( hname.Contains("Al_") ) { ss << "#equivAl (US)"; }
	if ( hname.Contains("Cu_") ) { ss << "#equivCu (US)"; }
      }
      if ( hname.Contains("Downstream") ) {
	if ( hname.Contains("NbTi_") ) { ss << "#equivNbTi (DS)"; } 
	if ( hname.Contains("Al_") ) { ss << "#equivAl (DS)"; }
	if ( hname.Contains("Cu_") ) { ss << "#equivCu (DS)"; }
      }
    }
    if ( hname.Contains("Flange") ) {
      if ( hname.Contains("Upstream") ) { ss << "Flange (US)"; }
      if ( hname.Contains("Downstream") ) { ss << "Flange (DS)"; }
    }
    if ( hname.Contains("Mandrel") ) { ss << "Mandrel"; }
    if ( hname.Contains("Window") ) {
      if ( hname.Contains("Upstream") ) { ss << "Window (US)"; }
      if ( hname.Contains("Downstream") ) { ss << "Window (DS)"; }
    }
    if ( hname.Contains("Cryostat") ) { ss << "Cryostat"; }
    if ( hname.Contains("Kicker") ) {
      if ( hname.Contains("Kicker1") ) { ss << "Kicker1"; }
      if ( hname.Contains("Kicker2") ) { ss << "Kicker2"; }
      if ( hname.Contains("Kicker3") ) { ss << "Kicker3"; }
    }
    if ( hname.Contains("Quad") ) {
      if ( hname.Contains("Quad10") ) { ss << "Quad10"; }
      if ( hname.Contains("Quad20") ) { ss << "Quad20"; }
      if ( hname.Contains("Quad30") ) { ss << "Quad30"; }
      if ( hname.Contains("Quad40") ) { ss << "Quad40"; }
      if ( hname.Contains("Quad11") ) { ss << "Quad11"; }
      if ( hname.Contains("Quad21") ) { ss << "Quad21"; }
      if ( hname.Contains("Quad31") ) { ss << "Quad31"; }
      if ( hname.Contains("Quad41") ) { ss << "Quad41"; }
    }
    if ( hname.Contains("Collimator") ) { ss << "Collimator"; }
    if ( hname.Contains("BeamChannel") ) {
      if ( hname.Contains("Downstream") ) { ss << "BeamChannel (DS)"; }
      else if ( hname.Contains("Upstream") ) { ss << "BeamChannel (US)"; }
      else { ss << "BeamChannel"; }
    }
    if ( hname.Contains("Tracker") && 0 ) {
      if ( hname.Contains("RhoTime") ) { ss << "#hat{R}(t}"; }
      if ( hname.Contains("YTime") ) { ss << "#hat{y}(t}"; }
      if ( hname.Contains("PrhatTime") ) { ss << "P_{#hat{R}}(t}"; }
      if ( hname.Contains("PvhatTime") ) { ss << "P_{#hat{y}}(t}"; }
      if ( hname.Contains("MomTime") ) { ss << "P(t}"; }
      if ( hname.Contains("RhoTurn") ) { ss << "#hat{R}(turn}"; }
      if ( hname.Contains("YTurn") ) { ss << "#hat{y}(turn}"; }
      if ( hname.Contains("PrhatTurn") ) { ss << "P_{#hat{R}}(turn}"; }
      if ( hname.Contains("PvhatTurn") ) { ss << "P_{#hat{y}}(turn}"; }
      if ( hname.Contains("MomTurn") ) { ss << "P(t}"; }
    }
    if ( hname.Contains("VacuumChamberWall") ) { ss << "Vacuum Wall"; }
    if ( hname.Contains("RingTracker") ) {
      if ( hname.Contains("_00_") ) { ss << "Ring (#theta=0)"; }
      if ( hname.Contains("_01_") ) { ss << "Ring (#theta=30^{#circ})"; }
      if ( hname.Contains("_02_") ) { ss << "Ring (#theta=60^{#circ})"; }
      if ( hname.Contains("_03_") ) { ss << "Ring (#theta=90^{#circ})"; }
      if ( hname.Contains("_04_") ) { ss << "Ring (#theta=120^{#circ})"; }
      if ( hname.Contains("_05_") ) { ss << "Ring (#theta=150^{#circ})"; }
      if ( hname.Contains("_06_") ) { ss << "Ring (#theta=180^{#circ})"; }
      if ( hname.Contains("_07_") ) { ss << "Ring (#theta=270^{#circ})"; }
      if ( hname.Contains("_08_") ) { ss << "Ring (#theta=1#times2#pi)"; }
      if ( hname.Contains("_09_") ) { ss << "Ring (#theta=5#times2#pi)"; }
      if ( hname.Contains("_10_") ) { ss << "Ring (#theta=10#times2#pi)"; }
      if ( hname.Contains("_11_") ) { ss << "Ring (#theta=50#times2#pi)"; }
      if ( hname.Contains("_12_") ) { ss << "Ring (#theta=100#times2#pi)"; }
      if ( hname.Contains("_13_") ) { ss << "Ring (#theta=200#times2#pi)"; }
      if ( hname.Contains("_14_") ) { ss << "Ring (#theta=500#times2#pi)"; }
      if ( hname.Contains("_15_") ) { ss << "Ring (#theta=1000#times2#pi)"; }
      if ( hname.Contains("_16_") ) { ss << "Ring (#theta=2000#times2#pi)"; }
    }
    
    if ( hname.Contains("RingTracker") ) {
      if ( hname.Contains("_0Turn") ) { ss << "After 0 Turns"; }
      if ( hname.Contains("_1Turn") ) { ss << "After 1 Turns"; }
      if ( hname.Contains("_2Turn") ) { ss << "After 2 Turns"; }
      if ( hname.Contains("_5Turn") ) { ss << "After 5 Turns"; }
      if ( hname.Contains("_10Turn") ) { ss << "After 10 Turns"; }
      if ( hname.Contains("_50Turn") ) { ss << "After 50 Turns"; }
      if ( hname.Contains("_100Turn") ) { ss << "After 100 Turns"; }
      if ( hname.Contains("_200Turn") ) { ss << "After 200 Turns"; }
      if ( hname.Contains("_500Turn") ) { ss << "After 500 Turns"; }
      if ( hname.Contains("_1000Turn") ) { ss << "After 1000 Turns"; }
      if ( hname.Contains("_2000Turn") ) { ss << "After 2000 Turns"; }
    }
  }
  else {
    ss << "#theta=" << 360*r + 30*i << "^{#circ}";
  }

  if ( hname.Contains("Quad") || hname.Contains("Cryostat") || hname.Contains("Kicker") || hname.Contains("Collimator") ||
       hname.Contains("Straw") || hname.Contains("Calo") || hname.Contains("Xtal") || hname.Contains("Vacuum") || hname.Contains("InflectorHits") ) {
    double x1 = 0.4;
    double y1 = 0.88;

    if ( hname.Contains("Quad") && hname.Contains("RhoY") ) { x1 = 0.2; }
    //if ( hname.Contains("Collimator") && hname.Contains("RhoY") ) { y1 = 0.6; }

    abseff = (double)hist->GetEntries() / upstream_inflector_yield;
    double m = *int_curr;
    double N = upstream_inflector_yield;
    double abserr = TMath::Sqrt(m*(1 - m/N)/N);
    abserr = TMath::Sqrt(abseff*(1-abseff)/N);
    
    stringstream ss_hits, ss_deposit;
    ss_hits.precision(2);
    if ( abseff > 0.01 ) { 
      ss_hits << "Hits/#mu = " << abseff;
      if ( abserr > 0.001 ) { ss_hits << abseff << "#pm" << abserr; }
      else { ss_hits << "#pm#approx0"; }
    }
    else { 
      ss_hits << "Hits/#mu < 0.01";
    }
    myText(x1, y1, 1, ss_hits.str().c_str(), 0.04);

    abseff = *int_curr / upstream_inflector_yield;
    m = *int_curr;
    N = upstream_inflector_yield;
    abserr = TMath::Sqrt(abseff*(1-abseff)/N);
    
    ss_deposit.precision(2);
    if ( abseff > 0.01 ) { 
      ss_deposit << "#DeltaE/#mu = " << abseff;
      if ( abserr > 0.001 ) { ss_deposit << abseff << "#pm" << abserr; }
      else { ss_deposit << "#pm#approx0"; }
    }
    else { 
      ss_deposit << "#DeltaE/#mu < 0.01";
    }
    ss_deposit << " MeV";
    myText(x1, y1-0.08, 1, ss_deposit.str().c_str(), 0.04);
    ss.str("");
  }

  TString hname = hist->GetName();
  if ( hname.Contains("G4") == false ) {
    if ( hname.Contains("TrackerRhoTime") ||
	 hname.Contains("TrackerYTime")  ||
	 hname.Contains("TrackerPrhatTime") ||
	 hname.Contains("TrackerPvhatTime")  ||
	 hname.Contains("TrackerMomTime") ||
	 hname.Contains("TrackerRhoTurn") ||
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



void MakePlot1D(TH1F *hist, int r, int i, double *int_prev, double *int_curr, double *int_start)
{
  if ( update && hist == NULL ) { return; }
  if ( hist == NULL ) { return; }

  TString hname = hist->GetName();
  if ( hist->GetEntries() <= 0 ) { cout << "Histogram [" << hist->GetName() << "] is empty" << endl; return; }

  if ( hname.Contains("_Nhits") ) {
    if ( hist->Integral(hist->FindBin(1), hist->FindBin(100)) <= 0 ) { return; }
  }
  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  
  double xmin, xmax;
  double ymin, ymax;
  double histxmin, histxmax;
  histxmin = 99999.9;
  histxmax = -99999.9;




  //hist->Scale(100.0 / upstream_inflector_yield);
  if ( !hname.Contains("Wavelength") && !hname.Contains("Mag") ) {
    ymin = 0.001;
    ymax = hist->GetMaximum()*1.25;
  }
  else {
    ymin = 0.0;
    ymax = hist->GetMaximum();
  }

  //cout << "ymax = " << hist->GetMaximum() << endl;

  if ( zoom ) {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = hist->GetXaxis()->GetXmax()/20;
    xmin = 0.0;
    xmax = 10.0;
    if ( hname.Contains("Time") ) { xmax = 2.0; }
  }
  else if ( zoom2 ) {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = hist->GetXaxis()->GetXmax()/20;
    xmin = 10.0;
    xmax = 50.0;
  }
  else {
    xmin = hist->GetXaxis()->GetXmin();
    xmax = hist->GetXaxis()->GetXmax();
  }

  if ( hname.Contains("RingTracker") ) { 
    xmin = -125.0; xmax = 125.0;
    ymin = -125.0; ymax = 125.0;
  }
  else if ( hname.Contains("Muon") && hname.Contains("Position") ) { 
    ;
  }
  else if ( hname.Contains("Muon") && hname.Contains("Momentum") ) { 
    ;
  }
  else { 
    ;//ymin = -15.0; ymax = 15.0;
  }



  bool setxmin = false;
  int stopxbinmin = 1;
  int stopxbinmax = 1;
  int xoffset = 4;
  xmin = hist->GetXaxis()->GetXmin();
  xmax = hist->GetXaxis()->GetXmax();
  stopxbinmax = hist->GetNbinsX()-1;
  for ( int bin = 1; bin < hist->GetNbinsX()-1; bin++ ) {
    if ( hist->GetBinContent(bin) > 0 ) {
      if ( hist->GetBinLowEdge(bin) < histxmin ) { histxmin = hist->GetBinLowEdge(bin); }
      stopxbinmin = TMath::Max(bin-xoffset, 1);
      xmin = hist->GetBinLowEdge(stopxbinmin);
      //cout << stopxbinmin << "\t" << xmin << endl;
      break;
    }
  }
  for ( int bin = hist->GetNbinsX()-1; bin >= stopxbinmin; bin-- ) {
    if ( hist->GetBinContent(bin) > 0 ) {
      if ( hist->GetBinLowEdge(bin) > histxmax ) { histxmax = hist->GetBinLowEdge(bin); }
      stopxbinmin = TMath::Min(bin+xoffset, hist->GetNbinsX()-1);
      xmax = hist->GetBinLowEdge(stopxbinmin);
      break;
    }
  }  
  
  
  if ( hname.Contains("DeltaPy") ) { 
    hist->Rebin(5);
    ;//xmin = -0.01; xmax = 0.01;
  }
  if ( hname.Contains("Nhits") ) { 
    ;//xmin = 0.0; xmax = 4.0;
  }

  //cout << "ymax = " << ymax << endl;


  if ( hname.Contains("DegreeAt") && hname.Contains("Rhat0") ) {
    hist->Rebin(2);
    xmin = 40.0;
    xmax = 125.0;
  }
  if ( hname.Contains("DegreeAt") && hname.Contains("Quad") ) {
    hist->Rebin(2);
    xmin = 0.0;
    xmax = 100.0;
  }
  

  if ( hname.Contains("KickAt") ) {
    hist->Rebin(15);
    xmin = -20.0;
    xmax = 0.0;
  }
  else if ( hname.Contains("DegreeAt") && hname.Contains("Rhat0") ) {
    hist->Rebin(2);
    xmin = 40.0;
    xmax = 125.0;
  }
  else if ( hname.Contains("_Mom") ) {
    if ( hname.Contains("_Electron") ) {
      hist->Rebin(2);
      xmin = 0.0;
      xmax = 1.0*(1+3*dPoverP);
    }
    else {
      hist->Rebin(2);
      //xmin = 3090*(1-3*dPoverP);
      //xmax = 3090*(1+3*dPoverP);
      xmin = 1.0*(1-3*dPoverP);
      xmax = 1.0*(1+3*dPoverP);
    }
  }


  if ( hname.Contains("RingTracker") && hname.Contains("Rhat") ) {
    if ( !hname.Contains("Degree") && !hname.Contains("Kick") ) {
      hist->Rebin(1);
      xmin = -50.0;
      xmax =  50.0;
    }
  }


  if ( hname.Contains("RingTracker") && hname.Contains("Vhat") ) {
    hist->Rebin(1);
    xmin = -50.0;
    xmax =  50.0;
  }


  if ( hname.Contains("Xprime") ) {
    hist->Rebin(1);
    xmin = -10.0;
    xmax =  10.0;
  }


  if ( hname.Contains("Yprime") ) {
    hist->Rebin(1);
    xmin = -5.0;
    xmax =  5.0;
  }

  if ( hname.Contains("TrackerRho") ||
       hname.Contains("TrackerY") ) {
    hist->Rebin(2);
    xmin = -60.0;
    xmax = 60.0;
  }

  if ( hname.Contains("Time_Xe") ) {    
    xmin = -30.0;
    xmax = 30.0;
    if ( histxmin > -10 && histxmax < 10 ) { xmin = -10.0; xmax = 10.0; }
    if ( histxmin > -20 && histxmax < 20 ) { xmin = -20.0; xmax = 20.0; }
  }

  if ( hname.Contains("Time_Rhat") || hname.Contains("Time_Vhat") ) {
    xmin = -45.0;
    xmax = 45.0;
    if ( histxmin > -30 && histxmax < 30 ) { xmin = -30.0; xmax = 30.0; }
    if ( histxmin > -20 && histxmax < 20 ) { xmin = -20.0; xmax = 20.0; }
  }

  if ( hname.Contains("Time_Xprime") || hname.Contains("Time_Yprime") ) {
    xmin = -10.0;
    xmax = 10.0;
    if ( histxmin > -5 && histxmax < 5 ) { xmin = -5.0; xmax = 5.0; }
  }

  if ( hname.Contains("_Electron") ) {
    if ( hname.Contains("dX") ) {
      hist->Rebin(5);
    }
    if ( hname.Contains("dCosTheta") ) {
      hist->Rebin(5);
    }
    if ( hname.Contains("_Mom") ) {
      hist->Rebin(20);
    }
  }



  bool dolog = true;
  bool dologx = false;
  if ( hname.Contains("Momentum") ||
       hname.Contains("_Mom") ||
       hname.Contains("theta") ||
       hname.Contains("Degree") ||
       hname.Contains("t0") ||
       hname.Contains("Kick") ||
       hname.Contains("Rhat") ||
       hname.Contains("Vhat") ||
       hname.Contains("TrackerRho") ||
       hname.Contains("Xe") ||
       hname.Contains("Rhat") ||
       hname.Contains("Vhat") ||
       hname.Contains("Xprime") ||
       hname.Contains("Yprime") ||
       hname.Contains("dCosTheta") ||
       hname.Contains("dX") ||
       hname.Contains("Pol") ||
       hname.Contains("PolX") ||
       hname.Contains("PolY") ||
       hname.Contains("TrackerY") ) {
    dolog = false;
    ymin = 0.0;
    ymax = hist->GetMaximum() * 1.25;
    //cout << hname << "\t" << ymin << "\t" << ymax << endl;
  }
  if ( hname.Contains("DeltaPx") ||
       hname.Contains("DeltaPy") ) {
    dolog = false;
    ymin = 0.0;
    ymax = hist->GetMaximum() * 1.25;
    if ( hname.Contains("Px") ) {
      if ( hname.Contains("Quad") || hname.Contains("Kicker") ) {
	xmin = hist->GetMean() - 3*hist->GetRMS();
	xmax = hist->GetMean() + 2*hist->GetRMS();
      }
      else {
	xmin = hist->GetMean() - 4*hist->GetRMS();
	xmax = hist->GetMean() + 4*hist->GetRMS();
      }
    }
    else {
      xmin = hist->GetMean() - 4*hist->GetRMS();
      xmax = hist->GetMean() + 4*hist->GetRMS();
    }
    //cout << hname << "\t" << ymin << "\t" << ymax << endl;
  }
  if ( hname.Contains("out_") || hname.Contains("1D") || hname.Contains("Wavelength") ) {;
    dologx = true;
    //dolog = true;
    //ymax = hist->GetMaximum() * 1.25;
    //ymin = hist->GetMinimum() * 1.25;
  }

  //cout << "ymax = " << ymax << endl;
  
  if ( dolog ) {
    ymin = 0.1;
    ymax = hist->GetMaximum()*5;
  }

  if ( ymax <= 0 ) { ymax = 1.0; }
  //cout << xmin << "\t" << xmax << endl;
  //cout << ymin << "\t" << ymax << endl;
  if ( ymax <= 0 ) { ymax = 1.0; }


  if ( hname.Contains("Wavelength") ) {
    hist->Rebin(1);
    //ymin = 0.0;    
    //xmax = hist->GetXaxis()->GetXmax()/2.0;
    //ymax = hist->GetMaximum()*1.25;
  }
  //cout << "ymax = " << ymax << endl;

  //ymin = 0.1;
  //ymax = hist->GetMaximum()*5;

  TString xtitle = hist->GetXaxis()->GetTitle();
  TString ytitle = hist->GetYaxis()->GetTitle();

  if ( hname.Contains("Degree") || hname.Contains("Kick") || hname.Contains("Mom") || hname.Contains("DeltaP") ) {
    if ( !dolog ) {
      ymax *= 1.1;
    }
  }

  cout << hname << "\t" << ymin << "\t" << ymax << endl;

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.1);
  h->GetXaxis()->SetTitleOffset(1.1);
  h->Draw();
  double binstep = (double)h->GetNbinsX() / hist->GetNbinsX();
  //cout << binstep << endl;




  double yields[5];
  string ynames[5];
  for ( int y = 0; y < 5; y++ ) { yields[y] = 0.0; }

  hist->SetLineColor(kBlue);
  hist->SetLineWidth(4);
  hist->SetMarkerSize(1.5);
  hist->SetMarkerColor(kBlue);
  if ( hname.Contains("out_") || hname.Contains("1D") || hname.Contains("Wavelength") ) {
    if ( 0 ) {
      hist->SetMarkerSize(0.75);
      hist->SetLineWidth(1); 
      hist->SetLineColor(kAzure+6);
      hist->SetMarkerColor(kAzure+6);
    }
  }

  double binw = hist->GetBinWidth(1);
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
  else if ( hname.Contains("Wavelength") ) { yt << " 1/#lambda"; }
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
  
  h->SetYTitle(yt.str().c_str());


  if ( hname.Contains("Wavelength") ) {
    hist->Draw("HIST,SAME");
  }
  else {
    hist->Draw("E1,HIST,SAME");
  }

  stringstream ss, ss_eff, ss_abseff;
  

  ss.str("");
  TString hname = hist->GetName();
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

  if ( hname.Contains("Wavelength")  ) {
    double maxval = -0.1;
    int maxbin = -1;
    for ( int bin = 1; bin < hist->FindBin(xmax); bin++ ) {
      if ( hist->GetBinContent(bin) > maxval ) { maxval = hist->GetBinContent(bin); maxbin = bin; }
    }
    ss.precision(3);
    ss.str("");
    ss << "#lambda_{max} = " << 1.0/hist->GetBinCenter(maxbin) << " Turns";
    myText(0.5, 0.80, 1, ss.str().c_str());
  }

  if ( hname.Contains("Momentum") ||
       hname.Contains("thetaX") || 
       hname.Contains("thetaY") ) {
    TF1 *f1;

    if ( hname.Contains("Momentum") ) {
      f1 = new TF1("f1", "gaus", 1-0.25, 1+0.25);
      hist->Sumw2();
      hist->Fit("f1", "RQ");
      hist->GetXaxis()->SetRange(hist->FindBin(1-0.25), hist->FindBin(1+0.25));
    }
    else {
      double max = 0.01;
      if ( start_inflector ) { max = 0.05; }
      f1 = new TF1("f1", "gaus", -max, max);
      hist->Sumw2();
      hist->Fit("f1", "RQ");
      hist->GetXaxis()->SetRange(hist->FindBin(-max), hist->FindBin(max));
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
    double mean = hist->GetMean();
    double rms = hist->GetRMS();
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
    toteff << SurviveTextRaw(hist->GetEntries(), upstream_inflector_yield, "");
    myText(0.20, 0.86, kRed, toteff.str().c_str(), 0.055);

    stringstream fineff;
    fineff << SurviveText(eff_turn100, err_turn100, "100");
    myText(0.20, 0.79, kPink+6, fineff.str().c_str(), 0.055);
    }
  }
  
  bool plot_avgrms = false;
  if ( hname.Contains("DeltaPx") || hname.Contains("DeltaPy") ) { plot_avgrms = true; }
  if ( hname.Contains("TrackerY") || hname.Contains("TrackerRho") ) { plot_avgrms = true; }
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

  if ( plot_avgrms ) {
    double mean = hist->GetMean();
    double rms = hist->GetRMS();
    stringstream avg_rms;
    avg_rms.precision(2);
    if ( hname.Contains("Px") ) { avg_rms << "<#DeltaP_{x}>"; }
    if ( hname.Contains("Py") ) { avg_rms << "<#DeltaP_{y}>"; }
    if ( hname.Contains("_Mom") ) { avg_rms << "<#frac{p}{p_{m}}>"; }
    if ( hname.Contains("_Rhat") ) { avg_rms << "<x>"; }
    if ( hname.Contains("_Vhat") ) { avg_rms << "<y>"; }
    if ( hname.Contains("TrackerY") ) { avg_rms << "<y>"; }
    if ( hname.Contains("TrackerRho") ) { avg_rms << "<x>"; }
    
    string unit = "Mev";
    if ( hname.Contains("TrackerY") || hname.Contains("TrackerRho") || hname.Contains("Rhat") || hname.Contains("Vhat") ) {
      unit = "mm";
    }
    if ( hname.Contains("_Mom") ) { unit = ""; }

    if ( mean < 1 && mean > -1 && !hname.Contains("Mom") ) {
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
       hname.Contains("TrackerY") || hname.Contains("TrackerRho") ) {
    stringstream toteff;
    toteff << SurviveText(eff_turn100, err_turn100, "100");
    myText(0.20, 0.86, kRed, toteff.str().c_str(), 0.055);
  }


  //int result = hist->Fit("[0]*sin([1]*x)", "R");
  //cout << result << endl;
  
  bool doeps = false;

  stringstream c1name;
  if ( zoom ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName() << "_Zoom";
  }
  else if ( zoom2 ) {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName() << "_Zoom2";
  }
  else {
    c1name << "~/public_html/plots/" << dir << "/" << hist->GetName();
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
  if ( !hist ) { cout << "Could not get " << histname << " from GetHistogram()" << endl; return( NULL ); }

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
  if ( !hist1d ) { cout << "Could not get " << histname << " from GetHistogram1D()" << endl; file->ls(); return( NULL ); }
  
  return( hist1d );
}

void PlotXZHits()
{
  TCanvas* c1 = new TCanvas("c1","g-2 Result",0,0,800,600);
  TPad* thePad = (TPad*)c1->cd();
  
  double xmin, xmax;
  double ymin, ymax;

  xmin = HitsXZ[0]->GetXaxis()->GetXmin();
  xmax = HitsXZ[0]->GetXaxis()->GetXmax();
  ymin = HitsXZ[0]->GetYaxis()->GetXmin();
  ymax = HitsXZ[0]->GetYaxis()->GetXmax();

  
  TString xtitle = HitsXZ[0]->GetXaxis()->GetTitle();
  TString ytitle = HitsXZ[0]->GetYaxis()->GetTitle();

  TH1F *h = thePad->DrawFrame(xmin,ymin,xmax,ymax);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleSize(0.06);
  h->SetXTitle(xtitle);
  h->SetYTitle(ytitle);
  h->GetYaxis()->SetTitleOffset(1.1);
  h->GetXaxis()->SetTitleOffset(1.1);
  h->Draw();

  for ( int i = 0; i < 20; i++ ) {
    if ( HitsXZ[i] ) {
      if ( HitsXZ[i]->GetEntries() > 0 ) {
	HitsXZ[i]->SetMarkerColor(i+1);
	HitsXZ[i]->SetLineColor(i+1);
	HitsXZ[i]->Draw("COL,SAME");
      }
    }
  }
  
  c1->RedrawAxis();
  gPad->SetRightMargin( 0.15 );
  TPaletteAxis* palette
    = dynamic_cast<TPaletteAxis*>( HitsXZ[0]->GetListOfFunctions()->FindObject( "palette" ) );
  if( palette ) {
    palette->SetX1NDC( 0.86 ); // Start the palette 86 % of the way across the image
    palette->SetX1NDC( 0.91 ); // End the palette 91% of the way across the image
    gPad->Modified(); // Update with the new position
  }
  
  
  stringstream c1name;
  c1name << "~/public_html/plots/" << dir << "/RingHits_XZ" << ".png";
  cout << c1name.str() << endl;
  c1->SaveAs(c1name.str().c_str());
  delete c1;  
}

bool plotinf = true;
bool plotringhits = false;
bool start_inflector = false;
int maxturns_real = 2000;
bool plotringeff = false;


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
  //cout << "Contents of " << filename << endl;
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

  

  double int_prev = -1.0;
  double int_start = -1.0;
  double int_curr = -1.0;
  stringstream hname;


  int maxringtracks = 9;
  int maxsyshits = 5;
  int maxtimes = 10;
  bool plotinfstart = false;
  bool plotdiagnostic = true;


  bool plot_inflector = true;
  bool plot_ringtrackers = true;
  bool plot_kickerquads = true;
  bool plot_detectors = true;
  bool plot_phasespace = true;

  //plot_inflector = false;
  plot_ringtrackers = false;
  plot_kickerquads = false;
  plot_detectors = false;
  //plot_phasespace = false;

  bool runFFT = false;
  if ( dir.find("CentralOrbit") != string::npos &&
       dir.find("Offset") == string::npos &&
       dir.find("ScrapingOFF") != string::npos ) { runFFT = true; }
  runFFT = false;
  if ( runFFT ) { cout << "We are going to run the FFT..." << endl; }
  else { cout << "Turning OFF FFT for trackers vs. time." << endl; }



  //return;

  string times[10] = {"-", "5us", "20us", "50us", "Final", "FinalAvg", "5turns", "10turns", "50turns", "100turns"};

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
      
      hname << "InflectorTracker_" << itnames[i] << "_RhoY";
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




  string rtnames[9] = {"00", "01", "02", "03", "04", "05", "06", "07", "08"};
  int_prev = -1.0;
  zoom = false; zoom2 = false; zoom3 = false;
  bool plotring = true;
  
  if ( maxturns < 7 ) { maxtimes = 1; maxringtracks = 9; }
  
  if ( plotinfstart ) { maxringtracks = 1; }
  
  if ( plotdiagnostic ) { maxringtracks = 11; maxringtracks = 9; maxtimes = 10; }
  
  
  
  if ( plot_kickerquads ) {
    hname << "RingTracker_PrhatPvhatEntrance";
    TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    //cout << hname.str() << endl;
    MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    hname.str("");
    
    hname << "InflectorTracker_PrhatPvhatEntrance";
    TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    //cout << hname.str() << endl;
    MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    hname.str("");
    
    hname << "InflectorTracker_PrhatPvhatExit";
    TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    //cout << hname.str() << endl;
    MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
    hname.str("");
    
    for ( int st = 0; st < 2; st++ ) {
      
      string stname = "";
      if ( st == 1 ) { stname = "_Stored"; }
      
      hname << "RingTracker" << stname << "_DegreeAtRhat0";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");

      hname << "RingTracker" << stname << "_DegreeAtRhat0_Mom1";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");

      hname << "RingTracker" << stname << "_DegreeAtRhat0_Mom4";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_DegreeAtQuad";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_DegreeAtRhat0RhatInit";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_DegreeAtRhat0KickAtRhat0";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_DegreeAtRhat0KickAtRhat0_Mom1";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_DegreeAtRhat0KickAtRhat0_Mom4";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_KickAtRhat0";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
            
      hname << "RingTracker" << stname << "_KickAtRhat0_Mom1";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_KickAtRhat0_Mom4";
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
            
      hname << "RingTracker" << stname << "_KickAtRhat0RhatInit";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker" << stname << "_FirstTurnX";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
      hname.str("");
    }
  }
  else {
    cout << endl;
    cout << "Not plotting diagnostic kicker and quad information." << endl;
    cout << endl;
  }


  if ( plot_ringtrackers ) {
    //plotring = false;
    for ( int i = 0; i < maxringtracks; i++ ) {
      //if ( plotinf ) { continue; }
      //continue;
    
      hname << "RingTracker_" << rtnames[i] << "_RhoY";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //cout << hname.str() << endl;
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");

      if ( 1 ) {
	hname << "RingTracker_" << rtnames[i] << "_XprimeX";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
      
	hname << "RingTracker_" << rtnames[i] << "_YprimeY";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
      
	if ( 1 ) {
	  hname << "RingTracker_" << rtnames[i] << "_Mom";
	  TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  hname << "RingTracker_" << rtnames[i] << "_Rhat";
	  TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  hname << "RingTracker_" << rtnames[i] << "_Vhat";
	  TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting ring trackers." << endl;
    cout << endl;
  }
  
  

  string trknames[9] = {"TrackerRho", "TrackerY", "TrackerPrhat", "TrackerPvhat", "TrackerMom", "TrackerPol", "TrackerPolX", "TrackerPolY", "TrackerXe"};
  string suffixes[3] = {"Turn", "OncePerTurn", "Turn_Electron"};

  int maxturnplots = 1;
  if ( plotdiagnostic ) { 
    maxturnplots = 5;
    for ( int i = 0; i < 3; i++ ) {
      if ( i == 2 ) { continue; }
      for ( int j = 0; j < 9; j++ ) {
	if ( j == 2 || j == 3 ) { continue; }
	
	zoom = false; zoom2 = false; zoom3 = false;
	//if ( plotinf ) { continue; }
	int_prev = -1.0;
	hname << trknames[j] << suffixes[i];
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      
	zoom = false; zoom2 = false; zoom3 = false;
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	hname.str("");
	
	continue;
	
	if ( i == 0 && j == 0 ) {
	  if ( hist ) {
	    int first_bin = 12*10;
	    int last_bin = 12*5000;
	    TH1F *h_X_tmp = (TH1F*)(hist->ProjectionY("_py1", first_bin, last_bin));
	    if ( h_X_tmp ) {
	      TH1F *h_X = (TH1F*)h_X_tmp->Clone("TrackerRho");
	      MakePlot1D(h_X, -1, i, &int_prev, &int_curr, &int_start);    
	      hname.str("");
	    }
	  }
	}
	if ( i == 0 && j == 1 ) {
	  if ( hist ) {
	    int first_bin = 12*10;
	    int last_bin = 12*5000;
	    TH1F *h_Y_tmp = (TH1F*)(hist->ProjectionY("_py2", first_bin, last_bin));
	    if ( h_Y_tmp ) {
	      TH1F *h_Y = (TH1F*)h_Y_tmp->Clone("TrackerY");
	      MakePlot1D(h_Y, -1, i, &int_prev, &int_curr, &int_start);     
	      hname.str("");  
	    }
	  }
	}
	
	if ( ( runFFT ) && ( i == 0 && ( j == 0 || j == 1  ) ) ) {
	  TH1F *hist1D = (TH1F*)hist->ProjectionX();
	  for ( int bin = 1; bin <= hist1D->GetNbinsX(); bin++ ) {
	    TH1D *yaxis = hist->ProjectionY("tmp", bin, bin);
	    hist1D->SetBinContent(bin, yaxis->GetMean()+yaxis->GetRMS());
	    if ( yaxis->GetMean() > 0 || yaxis->GetMean() < 0 ) {
	      ;//cout << hist1D->GetBinCenter(bin) << "\t" << hist1D->GetBinContent(bin) << endl;
	    }
	  }
	  
	  TH1F *profX = (TH1F*)hist1D->Clone("RhoTurn1D");
	  TH1 *h_FFT_Mag = 0;
	  TH1 *h_FFT_Ph = 0;
	  TVirtualFFT::SetTransform(0);
	  h_FFT_Mag = profX->FFT(h_FFT_Mag, "MAG");
	  h_FFT_Ph  = profX->FFT(h_FFT_Ph, "PH");
	  cout << "Bins = " << profX->GetNbinsX() << endl;
	  cout << "Bins = " << h_FFT_Mag->GetNbinsX() << endl;
	  TH1F *hprofX2 = (TH1F*)hist1D->Clone("RhoTurn1D");
	  double max = -9.9;
	  double maxbin = -1;
	  double range = profX->GetBinLowEdge(profX->GetNbinsX()-1) + profX->GetBinWidth(1) - profX->GetBinLowEdge(1);
	  TH1F *wavelength = 0;
	  TH1F *mag = 0;
	  TH1F *ph = 0;
	  
	  if ( i == 0 && j == 0 ) {
	    wavelength = new TH1F("WavelengthX", "", profX->GetNbinsX(), 0.0, profX->GetNbinsX()/range);
	    wavelength->SetXTitle("Radial Betatron Frequency [1/#lambda_{x}]");
	    mag = (TH1F*)h_FFT_Mag->Clone("MagX");
	    ph = (TH1F*)h_FFT_Ph->Clone("PhX");
	  }
	  if ( i == 0 && j == 1 ) {
	    wavelength = new TH1F("WavelengthY", "", profX->GetNbinsX(), 0.0, profX->GetNbinsX()/range);
	    wavelength->SetXTitle("Vertical Betatron Frequency [1/#lambda_{y}]");
	    mag = (TH1F*)h_FFT_Mag->Clone("MagY");
	    ph = (TH1F*)h_FFT_Ph->Clone("PhY");
	  }
	  
	  for ( int bin = 1; bin < h_FFT_Mag->GetNbinsX(); bin++ ) {
	    wavelength->SetBinContent(bin, h_FFT_Mag->GetBinContent(bin)/range);
	    if ( h_FFT_Mag->GetBinContent(bin) > max ) {
	      max = h_FFT_Mag->GetBinContent(bin);
	      maxbin = bin;
	    }
	    //double binval = h_FFT_Mag->GetBinCenter(bin);
	    //cout << binval / h_FFT_Mag->GetNbinsX() << "\t" << h_FFT_Mag->GetBinContent(bin) << endl;
	  }
	  
	  // 	if ( wavelength ) {
	  // 	  wavelength->AddBinContent(1, wavelength->GetBinContent(0));
	  // 	  wavelength->AddBinContent(wavelength->GetNbinsX()-1, wavelength->GetBinContent(wavelength->GetNbinsX()));
	  // 	}
	  
	  cout << "Max Freq: " << wavelength->GetMaximum() << endl;
	  cout << "Max Frequency @ " << h_FFT_Mag->GetBinCenter(maxbin)/range << endl;
	  cout << "1/Max Freq is " << 1.0/(h_FFT_Mag->GetBinCenter(maxbin)/range) << endl;
	  cout << "Wavelength is " << TMath::TwoPi() / (h_FFT_Mag->GetBinCenter(maxbin)/range) << endl;      
	  MakePlot1D(wavelength, -1, j, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	  //MakePlot1D(mag, -1, i, &int_prev, &int_curr, &int_start);    
	  //hname.str("");
	  //MakePlot1D(ph, -1, i, &int_prev, &int_curr, &int_start);    
	  //hname.str("");
	  //MakePlot1D((TH1F*)h_FFT_Mag, -1, i, &int_prev, &int_curr, &int_start);
	  //hname.str("");
	  //MakePlot1D((TH1F*)h_FFT_Ph, -1, i, &int_prev, &int_curr, &int_start);    
	  //hname.str("");
	  
	  delete wavelength;
	}
	
	
	
	if ( 0 ) {
	  zoom = true; zoom2 = false; zoom3 = false;
	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	  
	  zoom = false; zoom2 = true; zoom3 = false;
	  //hist->RebinX(2);
	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	  //hist->RebinY(2);
	  
	  zoom = false; zoom2 = false; zoom3 = true;
	  MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);
	  hname.str("");
	  zoom = false; zoom2 = false; zoom3 = false;
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting ring tracking vs time/turn." << endl;
    cout << endl;
  }

  cout << "YO" << endl;
  

  if ( plot_phasespace ) {
    plotringeff = true;
    for ( int t = 5; t < maxtimes; t++ ) {
      string time = times[t];
      if ( times[t] == "-" ) { time = ""; }
      else { time = "_" + time; }
      if ( time.find("0us") != string::npos ) { continue; }
      if ( time.find("5us") != string::npos ) { continue; }
      if ( time.find("10us") != string::npos ) { continue; }
      if ( time.find("50us") != string::npos ) { continue; }
      if ( time.find("100us") != string::npos ) { continue; }
      if ( time.find("5turns") != string::npos ) { continue; }
      if ( time.find("10turns") != string::npos ) { continue; }
      if ( time.find("50turns") != string::npos ) { continue; }
      if ( time.find("100turns") != string::npos ) { continue; }
      
      hname << "RingTracker_Time_Rhat" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      cout << time << endl;
      hname << "RingTracker_Time_Xe" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_Vhat" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_Pol" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_PolX" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_PolY" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_Xprime" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_Time_Yprime" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      //       hname << "RingTracker_Time_RhoY" + time;
      //       TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      //       MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      //       hname.str("");
      
      hname << "RingTracker_Time_Mom" + time;
      TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
    }
    
    plotringeff = false;
    plotringeff = true;

    
    cout << "Truth" << endl;
    
    for ( int p = 0; p < 2; p++ ) {
      string truth_part_name;
      if ( p == 0 ) { truth_part_name = ""; }
      if ( p == 1 ) { truth_part_name = "_Electron"; }
      
      for ( int t = 0; t < maxtimes; t++ ) {
	string time = times[t];
	if ( times[t] == "-" ) { time = ""; }
	else { time = "_" + time; }
	
	if ( time.find("0us") != string::npos ) { continue; }
	if ( time.find("5us") != string::npos ) { continue; }
	if ( time.find("10us") != string::npos ) { continue; }
	if ( time.find("50us") != string::npos ) { continue; }
	if ( time.find("100us") != string::npos ) { continue; }
	if ( time.find("5turns") != string::npos ) { continue; }
	if ( time.find("10turns") != string::npos ) { continue; }
	if ( time.find("50turns") != string::npos ) { continue; }
	if ( time.find("100turns") != string::npos ) { continue; }
	
	if ( p == 1 && t > 0 ) { continue; }
	
	
	hname << "G4Track_Time_Rhat" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
	
	hname << "G4Track_Time_Xe" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
	
	if ( p == 1 ) {
	  hname << "G4Track_Time_dXdCosTheta" + time + truth_part_name;
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  hname << "G4Track_Time_dX" + time + truth_part_name;
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  hname << "G4Track_Time_dCosTheta" + time + truth_part_name;
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      
	hname << "G4Track_Time_Vhat" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
	
	hname << "G4Track_Time_Xprime" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
	
	hname << "G4Track_Time_Yprime" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << "G4Track_Time_Mom" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << "G4Track_Time_Pol" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << "G4Track_Time_PolX" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << "G4Track_Time_PolY" + time + truth_part_name;
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");


	hname << "G4Track_Time_XprimeX" + time + truth_part_name;
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << "G4Track_Time_YprimeY" + time + truth_part_name;
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	//     hname << "G4Track_Time_Mom" + time + truth_part_name;
	//     TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	//     MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	//     hname.str("");
    
	//     hname << "G4Track_Time_t0" + time + truth_part_name;
	//     TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	//     MakePlot1D(hist1d, -1, -1, &int_prev, &int_curr, &int_start);    
	//     hname.str("");
    
	hname << "G4Track_Time_RhoY" + time + truth_part_name;
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	//     hname << "G4Track_Time_XZ" + time + truth_part_name;
	//     TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	//     MakePlot(hist, -1, -1, &int_prev, &int_curr, &int_start);    
	//     hname.str("");
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting time averaged phase space distributions." << endl;
    cout << endl;
  }

  cout << "Down here" << endl;

  plotringeff = false;

  //return;


  if ( plotdiagnostic == false ) { return; }

  string rtsnames[11] = {"0Turn", "1Turn", "2Turn", "5Turn", "10Turn", "50Turn", "100Turn", "200Turn", "500Turn", "1000Turn", "2000Turn"};
  int_prev = -1.0;
  zoom = false; zoom2 = false; zoom3 = false;
  maxturns=0;
  
  for ( int i = 0; i < maxturns; i++ ) {
    //if ( plotinf ) { continue; }
    //if ( plotring == false   ) { continue; }
    //continue;

    hname << "RingTracker_" << rtsnames[i] << "_RhoY";
    TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
    //cout << hname.str() << endl;
    MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
    hname.str("");

    if ( 0 ) {
      hname << "RingTracker_" << rtsnames[i] << "_XprimeX";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_" << rtsnames[i] << "_YprimeY";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
    }
    else {
      hname << "RingTracker_" << rtsnames[i] << "_thetaX";
      TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_" << rtsnames[i] << "_thetaY";
      TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
      
      hname << "RingTracker_" << rtsnames[i] << "_thetaXY";
      TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
      MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
      hname.str("");
    }
      
    hname << "RingTracker_" << rtsnames[i] << "_Momentum";
    TH1F *hist1D = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
    if ( hist1D ) { if ( i < 2 ) { hist1D->Rebin(5); } }
    MakePlot1D(hist1D, -1, i, &int_prev, &int_curr, &int_start);    
    hname.str("");
  }

  //return;
  

  //
  // Plot Inflector
  //
  int Ninames = 23;
  //string inames[23] = {"UpstreamEndFlange",  "UpstreamEquivalentNbTi", "UpstreamEquivalentAl", "UpstreamEquivalentCu", "UpstreamWindow", "DownstreamWindow", "DownstreamEquivalentNbTi", "DownstreamEquivalentAl", "DownstreamEquivalentCu", "DownstreamEndFlange",  "Mandrel", "Cryostat", "Quad10", "Quad20", "Quad30", "Quad40", "Quad11", "Quad21", "Quad31", "Quad41", "Kicker1", "Kicker2", "Kicker3"};
  
  //Ninames = 11;
  //string inames[11] = {"UpstreamEndFlange",  "UpstreamEquivalentNbTi", "UpstreamEquivalentAl", "UpstreamEquivalentCu", "UpstreamWindow", "DownstreamWindow", "DownstreamEquivalentNbTi", "DownstreamEquivalentAl", "DownstreamEquivalentCu", "DownstreamEndFlange",  "Mandrel"};
  
  Ninames = 22;
  string inames[22] = {"UpstreamEndFlange",  "UpstreamEquivalentNbTi", "UpstreamEquivalentAl", "UpstreamEquivalentCu", "UpstreamWindow", "DownstreamWindow", "DownstreamEquivalentNbTi", "DownstreamEquivalentAl", "DownstreamEquivalentCu", "DownstreamEndFlange",  "Mandrel", "Quad10", "Quad20", "Quad30", "Quad40", "Quad11", "Quad21", "Quad31", "Quad41", "Kicker1", "Kicker2", "Kicker3"};

  string rhitnames[9] = {"Inflector", "Cryostat", "Quad", "Kicker", "Collimator", "Vacuum", "StrawTracker", "Calo", "Xtal"};
  Ninames = 0;

  if ( plot_detectors ) {
  
    int_prev = -1.0;
    zoom = false; zoom2 = false; zoom3 = false;

    bool plotdet = true;;
    if ( plotinf == false ) { Ninames = 0; }

    if ( plotinf ) { Ninames = 11; }

    if ( plotdiagnostic ) { Ninames = 22; plotdet = true; }
    Ninames = 0;
  
  
    for ( int i = 0; i < Ninames; i++ ) {
      for ( int st = 0; st < 2; st++ ) {
	//cout << i << "/" << Ninames << endl;
	if ( plotdet == false ) { continue; }
      
	if ( plot_detectors == false ) { continue; }
      
	string stname = "";
	if ( st == 1 ) { stname = "_Stored"; }
      
	hname << inames[i] << "Hits" << stname << "_XZ";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
      
	hname << inames[i] << "Hits" << stname << "_RhoY";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
      
	hname << inames[i] << "Hits" << stname << "_DeltaPy";
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
      
	if ( 1 ) {
	  hname << inames[i] << "Hits" << stname << "_DeltaPx";
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      
	if ( 1 ) {
	  hname << inames[i] << stname << "_Nhits";
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
      }
    }




    if ( maxturnsreal < 100 ) { maxsyshits = 0; }
    if ( plotinf ) { maxsyshits = 3; plotdet = true; }

    if ( plotdiagnostic ) { maxsyshits = 5; plotdet = true; }
    //plotdet = false;

    //cout << "MAX = " << maxsyshits << endl;
    for ( int i = 0; i < maxsyshits; i++ ) {
      for ( int st = 0; st < 2; st++ ) {
	//cout << i << "/" << maxsyshits << endl;
	if ( plotdet == false ) { continue; }
    
      
	if ( plot_detectors == false ) { continue; }

	string stname = "";
	if ( st == 1 ) { stname = "_Stored"; }

	hname << rhitnames[i] << "Hits" << stname << "_XZ";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");

	hname << rhitnames[i] << "Hits" << stname << "_RhoY";
	TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	hname << rhitnames[i] << "Hits" << stname << "_DeltaPy";
	TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	hname.str("");
    
	if ( 1 ) {
	  hname << rhitnames[i] << "Hits" << stname << "_DeltaPx";
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}
    
	if ( 1 ) {
	  hname << rhitnames[i] << stname << "_Nhits";
	  TH1F *hist1d = GetHistogram1D(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot1D(hist1d, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");
	}

	if ( 0 ) {
	  hname << rhitnames[i] << "Hits" << stname << "_RhoTime";
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  zoom = true; zoom2 = false; zoom3 = false;
	  MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  zoom = false; zoom2 = false; zoom3 = false;

	  hname << rhitnames[i] << "Hits" << stname << "_YTime";
	  TH2F *hist = GetHistogram(file, hname.str(), &int_prev, &int_curr, &int_start);
	  MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  zoom = true; zoom2 = false; zoom3 = false;
	  MakePlot(hist, -1, i, &int_prev, &int_curr, &int_start);    
	  hname.str("");

	  zoom = false; zoom2 = false; zoom3 = false;
	}
      }
    }
  }
  else {
    cout << endl;
    cout << "Not plotting detector hit information." << endl;
    cout << endl;
  }
}
