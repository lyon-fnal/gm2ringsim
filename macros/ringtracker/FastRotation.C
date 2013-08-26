#define MAXTIMES 100000

string suffix="veryearly";
//string suffix="early";
//string suffix="late";
string suffix="verylate";

void FastRotation()
{
  TGraph *g[10];
  TGraph *p1[10];
  TGraph *p2[10];
  TGraph *p1_zone1[10];
  TGraph *p2_zone1[10];
  TGraph *p1_zone2[10];
  TGraph *p2_zone2[10];
  TGraph *p1_zone3[10];
  TGraph *p2_zone3[10];
  TGraph *guess[10];
  for ( int i = 0; i < 10; i++ ) { 
    g[i] = new TGraph(); guess[i] = new TGraph();
    p1[i] = new TGraph(); p2[i] = new TGraph();
    p1_zone1[i] = new TGraph(); p2_zone1[i] = new TGraph();
    p1_zone2[i] = new TGraph(); p2_zone2[i] = new TGraph();
    p1_zone3[i] = new TGraph(); p2_zone3[i] = new TGraph();
  }

  double pmax[10], pmin[10];
  double pmax_zone1[10], pmin_zone1[10];
  double pmax_zone2[10], pmin_zone2[10];
  double pmax_zone3[10], pmin_zone3[10];
  double p2max_zone1[10], p2min_zone1[10];
  double p2max_zone2[10], p2min_zone2[10];
  double p2max_zone3[10], p2min_zone3[10];

  double zone1max[10], zone1min[10];
  double zone2max[10], zone2min[10];
  double zone3max[10], zone3min[10];

  for ( int i = 0; i < 10; i++ ) {
    zone1max[i] = -99.9;
    zone1min[i] = -99.9;
    zone2max[i] = -99.9;
    zone2min[i] = -99.9;
    zone3max[i] = -99.9;
    zone3min[i] = -99.9;
  }

  double delvals[10];
  double Cavals[10];
  double deltavals[10];

  int ndel = 0;
  double delta = 20;  
  double dstep = delta/2;
  double dmax = delta;
  double xmax = delta;
  if ( suffix == "veryearly" ) { dstep = delta/10; dmax = delta/5; xmax = delta*1.25;}
  if ( suffix == "early" ) { dstep = 3*delta/4; dmax = delta; xmax = delta*1.25;}
  if ( suffix == "late" ) { dstep = 6*delta/4; dmax = 2*delta; xmax = dstep*1.25;}
  if ( suffix == "verylate" ) { dstep = 9*delta/4; dmax = 3*delta; xmax = dstep*1.25;}

 
  for ( double del = 0.05; del < dmax; del += dstep ) {
    double Ca = delta + del;
    delvals[ndel] = (int)del;
    Cavals[ndel] = Ca;
    deltavals[ndel] = delta;
    int n = 0;
    int n1 = 0;
    int n2 = 0;
    double qstep = Ca/5000;
    double offset_zone1 = 0.0;
    double offset_zone2 = 0.0;
    double offset_zone3 = 0.0;
    for ( double q = -2; q <= Ca+2; q += qstep ) {
      double beta = 0.0;
      if ( q-Ca < 0 ) {
	if ( del < delta ) {
	  if ( q-Ca > -del ) { 
	    beta = fabs(q-Ca)/del;
	    offset_zone2 = del/2.0;
	    if ( zone1min[ndel] <= -99 ) { zone1min[ndel] = q-Ca; }
	  }
	  else if ( q-Ca > -delta ) { 
	    beta = 1.0;
	    offset_zone1 = delta/2.0;
	    if ( zone2min[ndel] <= -99 ) { zone2min[ndel] = q-Ca; }
	  }
	  else if ( q-Ca > -del-delta ) { 
	    beta = q/del;
	    offset_zone3 = (del+delta)/2.0;
	    if ( zone3min[ndel] <= -99 ) { zone3min[ndel] = q-Ca; }
	  }
	}
	else {
	  offset = delta/2.0;
	  if ( q-Ca > -delta ) { 
	    offset_zone1 = delta/2.0;
	    beta = fabs(q-Ca)/del;
	    if ( zone1min[ndel] <= -99 ) { zone1min[ndel] = q-Ca; }
	  }
	  else if ( q-Ca > -del ) { 
	    beta = delta/del;
	    offset_zone2 = del/2.0;
	    if ( zone2min[ndel] <= -99 ) { zone2min[ndel] = q-Ca; }
	  }
	  else if ( q-Ca > -del-delta ) { 
	    beta = q/del;
	    offset_zone3 = (del+delta)/2.0;
	    if ( zone3min[ndel] <= -99 ) { zone3min[ndel] = q-Ca; }
	  }
	}
      }
    }

    double offset = 0.0;
    if ( del < delta ) {
      offset_zone1 = delta; //-del/2;
      offset_zone2 = del; //delta-del/2;
      offset_zone3 = 0.0; //delta+del-del/2;
    }
    else {
      offset_zone1 = delta; //-del/2;
      offset_zone2 = del; //delta-del/2;
      offset_zone3 = 0.0; //delta+del-del/2;
    }

    for ( double q = -2; q <= Ca+2; q += qstep ) {
      double beta = 0.0;
      double pulse1 = 0.0;
      if ( q >= 0 && q <= del ) { pulse1 = q/del; }
      else if ( q > del && q < delta + del ) { pulse1 = 1.0; }
      if ( q >= 0 && q < delta + del ) {
	p1[ndel]->SetPoint(n1++, q-(delta+del)/2, pulse1-0.5);
	if ( del < delta ) {
	  p1_zone1[ndel]->SetPoint(n1++, q-offset_zone1, pulse1-0.5);
	  p1_zone2[ndel]->SetPoint(n1++, q-offset_zone2, pulse1-0.5);
	  p1_zone3[ndel]->SetPoint(n1++, q-offset_zone3, pulse1-0.5);
	}
	else {
	  p1_zone1[ndel]->SetPoint(n1++, q-offset_zone1, pulse1-0.5);
	  p1_zone2[ndel]->SetPoint(n1++, q-offset_zone2, pulse1-0.5);
	  p1_zone3[ndel]->SetPoint(n1++, q-offset_zone3, pulse1-0.5);
	}
      }

      double pulse2 = 0.0;
      if ( q >= 0 && q <= delta ) { pulse2 = 0.0; }
      else if ( q > delta && q < delta + del ) { pulse2 = (q-delta)/del; }
      if ( q >= 0 && q < delta + del ) {
	p2[ndel]->SetPoint(n2++, q-(delta+del)/2, pulse2-0.5);
	if ( del < delta ) {
	  p2_zone1[ndel]->SetPoint(n1++, q-offset_zone1, pulse2-0.5);
	  p2_zone2[ndel]->SetPoint(n1++, q-offset_zone2, pulse2-0.5);
	  p2_zone3[ndel]->SetPoint(n1++, q-offset_zone3, pulse2-0.5);
	}
	else {
	  p2_zone1[ndel]->SetPoint(n1++, q-offset_zone1, pulse2-0.5);
	  p2_zone2[ndel]->SetPoint(n1++, q-offset_zone2, pulse2-0.5);
	  p2_zone3[ndel]->SetPoint(n1++, q-offset_zone3, pulse2-0.5);
	}
      }

      if ( q-offset_zone1 < 0 && q-offset_zone1 + qstep > 0 ) { pmax_zone1[ndel] = pulse1-0.5; pmin_zone1[ndel] = pulse2-0.5; }
      if ( q-offset_zone2 < 0 && q-offset_zone2 + qstep > 0 ) { pmax_zone2[ndel] = pulse1-0.5; pmin_zone2[ndel] = pulse2-0.5; }
      if ( q-offset_zone3 < 0 && q-offset_zone3 + qstep > 0 ) { pmax_zone3[ndel] = pulse1-0.5; pmin_zone3[ndel] = pulse2-0.5; }

      double beta_guess = fabs(pulse1-pulse2);
      if ( q-(delta+del)/2 < 0 && q-(delta+del)/2 + qstep > 0 ) { pmax[ndel] = pulse1-0.5; pmin[ndel] = pulse2-0.5; }
      g[ndel]->SetPoint(n, q, beta);
      guess[ndel]->SetPoint(n, q, beta_guess);
      n++;
      //cout << q << "\t" << pulse << endl;
    }
    ndel++;
  }

  cout << ndel << endl;
  
  TLegend *leg = new TLegend(0.6, 0.65, 0.85, 0.8);
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  for ( int n = ndel-1; n >= 0; n-- ) {
    if ( n == ndel - 1 ) {
      g[n]->SetMarkerColor(Color(n));
      g[n]->SetLineColor(Color(n));
      g[n]->Draw("AP");
      g[n]->GetXaxis()->SetLimits(-xmax, xmax);
      g[n]->SetMaximum(1.2);
      g[n]->GetYaxis()->SetTitle("#beta(t')|_{r=const}");
      g[n]->GetXaxis()->SetTitle("q #equiv t' = |fmod(t,#tau_{c})| + #frac{1}{2}(#delta+#Delta)");
    }
    else {
      g[n]->SetMarkerColor(Color(n));
      g[n]->SetLineColor(Color(n));
      g[n]->GetXaxis()->SetLimits(-xmax, xmax);
      g[n]->Draw("P");
    }
  }
  for ( int n = 0; n < ndel; n++ ) {
    stringstream ss;
    ss << "#delta=" << delvals[n] << " ns , #Delta=" << deltavals[n] << " ns";
    leg->AddEntry(g[n], ss.str().c_str(), "l");
  }
  leg->Draw("SAME");
  
  string c1name = "beta_" + suffix + ".eps";
  c1->SaveAs(c1name.c_str());
  c1->Clear();
  





  TLegend *leg = new TLegend(0.6, 0.65, 0.85, 0.8);
  leg->SetTextFont(42);
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  for ( int n = ndel-1; n >= 0; n-- ) {
    if ( n == ndel - 1 ) {
      guess[n]->SetMarkerColor(Color(n));
      guess[n]->SetLineColor(Color(n));
      guess[n]->GetXaxis()->SetLimits(-xmax, xmax);
      guess[n]->Draw("AP");
      guess[n]->SetMaximum(1.2);
      guess[n]->GetYaxis()->SetTitle("#beta(t')|_{r=const}");
      guess[n]->GetXaxis()->SetTitle("q #equiv t' = |fmod(t,#tau_{c})| + #frac{1}{2}(#delta+#Delta)");
    }
    else {
      guess[n]->SetMarkerColor(Color(n));
      guess[n]->SetLineColor(Color(n));
      guess[n]->GetXaxis()->SetLimits(-xmax, xmax);
      guess[n]->Draw("P");
    }
  }
  for ( int n = 0; n < ndel; n++ ) {
    stringstream ss;
    ss << "#delta=" << delvals[n] << " ns , #Delta=" << deltavals[n] << " ns";
    leg->AddEntry(guess[n], ss.str().c_str(), "l");
  }
  leg->Draw("SAME");
  
  string c1name = "beta_guess_" + suffix + ".eps";
  c1->SaveAs(c1name.c_str());

  c1->Clear();
  


  TLegend *leg = new TLegend(0.13, 0.7, 0.33, 0.85);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetFillColor(0);
  bool first = true;
  for ( int n = ndel-1; n >= 0; n-- ) {
    if ( delvals[n] > 1 ) {
      double xmin = -deltavals[n]-delvals[n]-5;
      double xmax = -xmin;
      string period = "";
      bool is_early = true;
      bool is_veryearly = false;
      bool is_verylate = false;
      if ( deltavals[n] < delvals[n] ) { is_early = false; period = "#Delta<#delta, late"; }
      if ( 2*deltavals[n] < delvals[n] ) { is_verylate = true; is_early = false; period = "#Delta<<#delta, late"; }
      if ( deltavals[n] >= delvals[n] ) { is_early = true; period = "#Delta>#delta, early"; }
      if ( deltavals[n] >= 2*delvals[n] ) { is_veryearly = true; is_early = true; period = "#Delta>>#delta, early"; }
      bool is_late = !is_early;
      if ( n == ndel - 1 ) {
	p1_zone1[n]->SetMarkerColor(Color(n, 1, is_early));
	p1_zone1[n]->SetLineColor(Color(n, 1, is_early));
	p1_zone1[n]->GetXaxis()->SetLimits(-xmax, xmax);
	p1_zone1[n]->Draw("AP");
	p1_zone1[n]->GetYaxis()->SetTitle("r' #equiv (R - R_{i}) / (#Delta R)");
	p1_zone1[n]->GetXaxis()->SetTitle("q #equiv t' = fmod(t,#tau_{c})");
      }
      else {
	p1_zone1[n]->SetMarkerColor(Color(n, 1, is_early));
	p1_zone1[n]->SetLineColor(Color(n, 1, is_early));
	p1_zone1[n]->Draw("P");
      }
     
      if ( n == ndel - 1 ) {
	p2_zone1[n]->SetMarkerColor(Color(n, 1, is_early));
	p2_zone1[n]->SetLineColor(Color(n, 1, is_early));
	p2_zone1[n]->Draw("P");
      }
      else {
	p2_zone1[n]->SetMarkerColor(Color(n, 1, is_early));
	p2_zone1[n]->SetLineColor(Color(n, 1, is_early));
	p2_zone1[n]->Draw("P");
      }

      TLine *l_zone1 = new TLine(0.0, pmin_zone1[n], 0.0, pmax_zone1[n]);
      l_zone1->SetLineColor(Color(n, 1, is_early));
      l_zone1->SetLineWidth(3);
      l_zone1->SetLineStyle(3);
      l_zone1->Draw("SAME");

      if ( is_late ) {
	double ydiff = 0.5 - pmax_zone1[n];
	cout << delvals[n]-deltavals[n] << "\t" << pmin_zone1[n] << "\t" << delvals[n]-deltavals[n] << "\t" << pmax_zone1[n] << endl;
	cout << delvals[n]-deltavals[n] << "\t" << pmin_zone1[n]+deltavals[n]/delvals[n] << "\t" << delvals[n]-deltavals[n] << "\t" << pmax_zone1[n]+deltavals[n]/delvals[n] << endl;
	TLine *l_zone1 = new TLine(delvals[n]-deltavals[n], pmin_zone1[n]+ydiff, delvals[n]-deltavals[n], pmax_zone1[n]+ydiff);
	l_zone1->SetLineColor(Color(n, 1, is_early));
	l_zone1->SetLineWidth(3);
	l_zone1->SetLineStyle(3);
	l_zone1->Draw("SAME");
      }

      if ( is_late ) { 
	double y_arrow_zone1 = 0.0;
	int xarrow_min, xarrow_max;
	if ( pmax_zone1[n] < 0 ) { xarrow_min = 5; xarrow_max = delvals[n]-deltavals[n] - 5; }
	else { xarrow_min = 2; xarrow_max = delvals[n]-deltavals[n]-2; }
	TArrow arrow_zone1(xarrow_min, y_arrow_zone1, xarrow_max, y_arrow_zone1, 0.01, "<|>");
	arrow_zone1.SetLineColor(Color(n, 1, is_early));
	arrow_zone1.SetLineWidth(2);
	arrow_zone1.SetFillColor(Color(n, 1, is_early));
	arrow_zone1.Draw();
      }

      if ( is_early && !is_veryearly) { 
	double y_arrow_zone1 = (pmin_zone1[n]+2*pmax_zone1[n])/3.0;
	TArrow arrow_zone1(2, y_arrow_zone1, 6, y_arrow_zone1, 0.01, "|>");
	arrow_zone1.SetLineColor(Color(n, 1, is_early));
	arrow_zone1.SetLineWidth(2);
	arrow_zone1.SetFillColor(Color(n, 1, is_early));
	arrow_zone1.Draw();
      }

      stringstream txt_zone1, txt_zone1_beta;
      txt_zone1 << "q - C_{A} > -#delta" << " (" << period << ")";
      double txtx, txty;
      txtx = 0.63; txty = 0.35;
      if ( is_veryearly ) { txtx = 0.58; txty = 0.35; }
      myText(txtx, txty, Color(n, 1, is_early), 0.0, txt_zone1.str().c_str());
      if ( is_late ) {
	txt_zone1_beta << "#beta = #frac{#Delta}{#delta}";
      }
      else {
	txt_zone1_beta << "#beta = #frac{|q-C_{A}|}{#delta}";
      }
      myText(txtx, txty-0.1, Color(n, 1, is_early), 0.0, txt_zone1_beta.str().c_str());

      string c1name;
      if ( is_early ) { c1name = "pulse_tailend_" + suffix + ".eps"; }
      if ( is_late ) { c1name = "pulse_center_" + suffix + ".eps"; }
      c1->SaveAs(c1name.c_str());
      c1->Clear();

      



      if ( n == ndel - 1 ) {
	p1_zone2[n]->SetMarkerColor(Color(n+1, 2, is_early));
	p1_zone2[n]->SetLineColor(Color(n+1, 2, is_early));
	p1_zone2[n]->GetXaxis()->SetLimits(-xmax, xmax);
	p1_zone2[n]->Draw("AP");
	p1_zone2[n]->GetYaxis()->SetTitle("r' #equiv (R - R_{i}) / (#Delta R)");
	p1_zone2[n]->GetXaxis()->SetTitle("q #equiv t' = fmod(t,#tau_{c})");
      }
      else {
	p1_zone2[n]->SetMarkerColor(Color(n+1, 2, is_early));
	p1_zone2[n]->SetLineColor(Color(n+1, 2, is_early));
	p1_zone2[n]->Draw("P");
      }
      
      if ( n == ndel - 1 ) {
	p2_zone2[n]->SetMarkerColor(Color(n+1, 2, is_early));
	p2_zone2[n]->SetLineColor(Color(n+1, 2, is_early));
	p2_zone2[n]->Draw("P");
      }
      else {
	p2_zone2[n]->SetMarkerColor(Color(n+1, 2, is_early));
	p2_zone2[n]->SetLineColor(Color(n+1, 2, is_early));
	p2_zone2[n]->Draw("P");
      }

      if ( is_late ) {
	TLine *l_zone2 = new TLine(0.0, pmin_zone2[n], 0.0, pmax_zone2[n]);
	l_zone2->SetLineColor(Color(n+1, 2, is_early));
	l_zone2->SetLineWidth(3);
	l_zone2->SetLineStyle(3);
	l_zone2->Draw("SAME");
      }

      if ( is_early ) {
	TLine *l_zone2 = new TLine(0.0, pmin_zone2[n], 0.0, pmax_zone2[n]);
	l_zone2->SetLineColor(Color(n+1, 2, is_early));
	l_zone2->SetLineWidth(3);
	l_zone2->SetLineStyle(3);
	l_zone2->Draw("SAME");

	double ydiff = 0.5 - pmax_zone2[n];
	TLine *l_zone2 = new TLine(deltavals[n]-delvals[n], pmin_zone2[n]+ydiff, deltavals[n]-delvals[n], pmax_zone2[n]+ydiff);
	l_zone2->SetLineColor(Color(n+1, 2, is_early));
	l_zone2->SetLineWidth(3);
	l_zone2->SetLineStyle(3);
	l_zone2->Draw("SAME");
      }

      if ( is_late ) {
	double y_arrow_zone2 = (pmin_zone2[n]+pmax_zone2[n])/2.0;
	TArrow arrow_zone2(1, y_arrow_zone2, 5, y_arrow_zone2, 0.01, "|>");
	arrow_zone2.SetLineColor(Color(n+1, 2, is_early));
	arrow_zone2.SetLineWidth(2);
	arrow_zone2.SetFillColor(Color(n+1, 2, is_early));
	arrow_zone2.Draw();
      }
      if ( is_early ) { 
	double y_arrow_zone2 = 0.0;
	TArrow arrow_zone2(1, y_arrow_zone2, deltavals[n]-delvals[n]-1, y_arrow_zone2, 0.01, "<|>");
	arrow_zone2.SetLineColor(Color(n+1, 2, is_early));
	arrow_zone2.SetLineWidth(2);
	arrow_zone2.SetFillColor(Color(n+1, 2, is_early));
	arrow_zone2.Draw();
      }

      stringstream txt_zone2, txt_zone2_beta;
      txt_zone2 << "q - C_{A} > -#Delta" << " (" << period << ")";
      double txtx, txty;
      txtx = 0.63; txty = 0.35;
      if ( is_veryearly ) { txtx = 0.2; txty = 0.6; }
      myText(txtx, txty, Color(n+1, 2, is_early), 0.0, txt_zone2.str().c_str());
      if ( is_late ) {
	txt_zone2_beta << "#beta = #frac{|q-C_{A}|}{#delta}";
      }
      else {
	txt_zone2_beta << "#beta = 1";
      }
      myText(txtx, txty-0.10, Color(n+1, 2, is_early), 0.0, txt_zone2_beta.str().c_str());

      string c1name;
      if ( is_early ) { c1name = "pulse_center_" + suffix + ".eps"; }
      if ( is_late ) { c1name = "pulse_tailend_" + suffix + ".eps"; }
      c1->SaveAs(c1name.c_str());
      c1->Clear();
      




      if ( n == ndel - 1 ) {
	p1_zone3[n]->SetMarkerColor(Color(n+2, 3, is_early));
	p1_zone3[n]->SetLineColor(Color(n+2, 3, is_early));
	p1_zone3[n]->GetXaxis()->SetLimits(-xmax, xmax);
	p1_zone3[n]->Draw("AP");
	p1_zone3[n]->GetYaxis()->SetTitle("r' #equiv (R - R_{i}) / (#Delta R)");
	p1_zone3[n]->GetXaxis()->SetTitle("q #equiv t' = fmod(t,#tau_{c})");
      }
      else {
	p1_zone3[n]->SetMarkerColor(Color(n+2, 3, is_early));
	p1_zone3[n]->SetLineColor(Color(n+2, 3, is_early));
	p1_zone3[n]->Draw("P");
      }
      
      if ( n == ndel - 1 ) {
	p2_zone3[n]->SetMarkerColor(Color(n+2, 3, is_early));
	p2_zone3[n]->SetLineColor(Color(n+2, 3, is_early));
	p2_zone3[n]->Draw("P");
      }
      else {
	p2_zone3[n]->SetMarkerColor(Color(n+2, 3, is_early));
	p2_zone3[n]->SetLineColor(Color(n+2, 3, is_early));
	p2_zone3[n]->Draw("P");
      }

      TLine *l_zone3 = new TLine(0.0, pmin_zone3[n], 0.0, pmax_zone3[n]);
      l_zone3->SetLineColor(Color(n+2, 3, is_early));
      l_zone3->SetLineWidth(3);
      l_zone3->SetLineStyle(3);
      l_zone3->Draw("SAME");

      if ( is_late ) {
	TLine *l_zone3 = new TLine(deltavals[n], pmin_zone3[n], deltavals[n], pmax_zone3[n]+deltavals[n]/delvals[n]);
	l_zone3->SetLineColor(Color(n+2, 3, is_early));
	l_zone3->SetLineWidth(3);
	l_zone3->SetLineStyle(3);
	l_zone3->Draw("SAME");
      }

      if ( is_early ) {
	TLine *l_zone3 = new TLine(delvals[n], pmin_zone3[n], delvals[n], pmax_zone3[n]+1);
	l_zone3->SetLineColor(Color(n+2, 3, is_early));
	l_zone3->SetLineWidth(3);
	l_zone3->SetLineStyle(3);
	l_zone3->Draw("SAME");
      }

      if ( is_late ) {
	double y_arrow_zone3 = (pmax_zone3[n]+deltavals[n]/delvals[n]+pmin_zone3[n])/2.0;
	TArrow arrow_zone3(deltavals[n]-1, y_arrow_zone3, deltavals[n]-6, y_arrow_zone3, 0.01, "|>");
	arrow_zone3.SetLineColor(Color(n+2, 3, is_early));
	arrow_zone3.SetLineWidth(2);
	arrow_zone3.SetFillColor(Color(n+2, 3, is_early));
	arrow_zone3.Draw();
      }

      if ( is_early && !is_veryearly ) {
	double y_arrow_zone3 = (pmax_zone3[n]+1+pmin_zone3[n])/2.0;
	TArrow arrow_zone3(delvals[n]-1, y_arrow_zone3, delvals[n]-6, y_arrow_zone3, 0.01, "|>");
	arrow_zone3.SetLineColor(Color(n+2, 3, is_early));
	arrow_zone3.SetLineWidth(2);
	arrow_zone3.SetFillColor(Color(n+2, 3, is_early));
	arrow_zone3.Draw();
      }

      stringstream txt_zone3, txt_zone3_beta;
      txt_zone3 << "q - C_{A} > -#Delta-#delta" << " (" << period << ")";
      double txtx, txty;
      txtx = 0.3; txty = 0.65;
      if ( is_veryearly ) { txtx = 0.2; txty = 0.6; }
      myText(txtx, txty, Color(n+2, 3, is_early), 0.0, txt_zone3.str().c_str());
      txt_zone3_beta << "#beta = #frac{q}{#delta}";
      myText(txtx, txty-0.10, Color(n+2, 3, is_early), 0.0, txt_zone3_beta.str().c_str());

      string c1name;
      if ( is_early ) { c1name = "pulse_frontend_" + suffix + ".eps"; }
      if ( is_late ) { c1name = "pulse_frontend_" + suffix + ".eps"; }
      c1->SaveAs(c1name.c_str());
      c1->Clear();
    }
  }

  string c1name = "pulse_guess_" + suffix + ".eps";
  c1->SaveAs(c1name.c_str());
  c1->Clear();


  TLegend *leg = new TLegend(0.13, 0.7, 0.33, 0.85);
  leg->SetTextFont(42);
  leg->SetTextSize(0.025);
  leg->SetFillColor(0);
  bool first = true;
  for ( int n = ndel-1; n >= 0; n-- ) {
    if ( n == ndel - 1 ) {
      p1[n]->SetMarkerColor(Color(n));
      p1[n]->SetLineColor(Color(n));
      p1[n]->GetXaxis()->SetLimits(-xmax, xmax);
      p1[n]->Draw("AP");
      p1[n]->GetYaxis()->SetTitle("r' #equiv (R - R_{i}) / (#Delta R)");
      p1[n]->GetXaxis()->SetTitle("q #equiv t' = fmod(t,#tau_{c})");
    }
    else {
      p1[n]->SetMarkerColor(Color(n));
      p1[n]->SetLineColor(Color(n));
      p1[n]->Draw("P");
    }

    if ( 1 ) {
    if ( n == ndel - 1 ) {
      p2[n]->SetMarkerColor(Color(n));
      p2[n]->SetLineColor(Color(n));
      p2[n]->Draw("P");
    }
    else {
      p2[n]->SetMarkerColor(Color(n));
      p2[n]->SetLineColor(Color(n));
      p2[n]->Draw("P");
    }
    }

    
    
    if ( (first && fabs(pmax[n] - pmin[n]) < 1) || delvals[n] > 1 ) {
      first = false;
      double xmin = -deltavals[n]/2.0-delvals[n]/2.0;
      double xmax = -deltavals[n]/2.0;
      cout << xmin << "\t" << xmax << endl;
      TLine *l = new TLine(xmin, 0.0, xmax, 0.0);
      l->SetLineColor(kBlue);
      l->SetLineWidth(3);
      l->SetLineStyle(3);
      l->Draw("SAME");

      if ( suffix == "early" ) {
	stringstream txt;
	txt << "#frac{1}{2}#delta";
	myText(0.28, 0.53, kBlue, 0.0, txt.str().c_str());
	stringstream txt2;
	txt2 << "#Delta";
	myText(0.5, 0.85, Color(0), 0.0, txt2.str().c_str());
	stringstream txt3;
	txt3 << "#delta = #frac{#Delta R}{R_{i}} #times t";
	myText(0.7, 0.37, kBlue, 0.0, txt3.str().c_str());

	TLine *early = new TLine(xmin, 0.0, xmax, 0.0);
	early->SetLineColor(kBlue);
	early->SetLineWidth(3);
	early->SetLineStyle(3);
	early->Draw("SAME");

	TLine *max = new TLine(xmin, 0.0, xmax, 0.0);
	max->SetLineColor(kBlue);
	max->SetLineWidth(3);
	max->SetLineStyle(3);
	max->Draw("SAME");

	TLine *late = new TLine(xmin, 0.0, xmax, 0.0);
	late->SetLineColor(kBlue);
	late->SetLineWidth(3);
	late->SetLineStyle(3);
	late->Draw("SAME");
      }

      if ( suffix == "late" ) {
	stringstream txt;
	txt << "#frac{1}{2}#delta";
	myText(0.31, 0.53, kBlue, 0.0, txt.str().c_str());
	stringstream txt2;
	txt2 << "#Delta";
	myText(0.5, 0.85, Color(0), 0.0, txt2.str().c_str());
	stringstream txt3;
	txt3 << "#delta = #frac{#Delta R}{R_{i}} #times t";
	myText(0.7, 0.37, kBlue, 0.0, txt3.str().c_str());

	cout << zone1min[n] << endl;
	cout << zone2min[n] << endl;
	cout << zone3min[n] << endl;
	if ( 0 ) {
	for ( int zone = 0; zone < 3; zone++ ) {
	  double delta = deltavals[n];
	  double del   = delvals[n];
	  double qmCa;
	  if ( zone == 0 ) { qmCa = -delta/2 - del; }
	  if ( zone == 1 ) { qmCa = -delta/2; }
	  if ( zone == 2 ) { qmCa = -del; }
	  double beta = 1.0;
	  double xval = qmCa*2.0;
	  if ( del < delta ) {
	    if ( qmCa > -del ) { beta = fabs(q-Ca)/del; }
	    else if ( qmCa > -delta ) { beta = 1.0; }
	    else if ( qmCa > -del-delta ) { beta = q/del; }
	  }
	  else {
	    if ( qmCa > -delta ) { beta = fabs(q-Ca)/del; }
	    else if ( qmCa > -del ) { beta = delta/del; }
	    else if ( qmCa > -del-delta ) { beta = q/del; }
	  }

	  cout << qmCa << "\t" << beta << endl;
	  for ( int fb = 0; fb < 1; fb++ ) {
	    TLine *early;
	    if ( fb == 0 ) { early = new TLine(qmCa/2, -0.5, qmCa/2, -0.25); }
	    if ( fb == 1 ) { early = new TLine(-qmCa/2, 0.5, -qmCa/2, 0.25); }
	    early->SetLineColor(kGreen);
	    early->SetLineWidth(3);
	    early->SetLineStyle(1);
	    early->Draw("SAME");
	    if ( fb == 0 ) {
	      stringstream txt4;
	      txt4 << "#beta = #frac{1}{#delta}|q-C_{A}|}";
	      myText(0.7, 0.28, kGreen, 0.0, txt4.str().c_str());
	    }
	  }
	}
	}

// 	TLine *max = new TLine(xmin, 0.0, xmax, 0.0);
// 	max->SetLineColor(kBlue);
// 	max->SetLineWidth(3);
// 	max->SetLineStyle(3);
// 	max->Draw("SAME");

// 	TLine *late = new TLine(xmin, 0.0, xmax, 0.0);
// 	late->SetLineColor(kBlue);
// 	late->SetLineWidth(3);
// 	late->SetLineStyle(3);
// 	late->Draw("SAME");
      }

      if ( suffix == "verylate" ) {
	stringstream txt;
	txt << "#frac{1}{2}#delta";
	myText(0.35, 0.53, kBlue, 0.0, txt.str().c_str());
	stringstream txt2;
	txt2 << "#Delta";
	myText(0.5, 0.85, Color(0), 0.0, txt2.str().c_str());
	stringstream txt3;
	txt3 << "#delta = #frac{#Delta R}{R_{i}} #times t";
	myText(0.7, 0.37, kBlue, 0.0, txt3.str().c_str());
      }

      double xmin = -deltavals[n]/2.0-delvals[n]/2.0;
      double xmax = -deltavals[n]/2.0;
      cout << xmin << "\t" << xmax << endl;
      TLine *l = new TLine(xmin, -0.5, xmin, 0.0);
      l->SetLineColor(Color(n));
      l->SetLineWidth(1);
      l->SetLineStyle(3);
      l->Draw("SAME");
      

      if ( n == ndel - 1 ) {
	TLine *l = new TLine(0.0, pmin[n], 0.0, pmax[n]);
	l->SetLineColor(Color(n));
	l->SetLineWidth(2);
	l->SetLineStyle(2);
	//l->Draw("SAME");
      }
      else {
	TLine *l = new TLine(0.0, pmin[n], 0.0, pmax[n]);
	l->SetLineColor(Color(n));
	l->SetLineWidth(2);
	l->SetLineStyle(2);
	//l->Draw("SAME");
      } 

      stringstream txt;
      txt << "#beta_{max} at t'=0";
      //myText(0.48, 0.42, 1, 90, txt.str().c_str());

      stringstream txt2;
      txt2 << "#beta(t') #equiv #frac{1}{R_{b}(r)}#int_{R'_{min}}^{R'_{max}} dR'";
      //myText(0.7, 0.2, 1, 0, txt2.str().c_str());
    }
  }
  for ( int n = 0; n < ndel; n++ ) {
    stringstream ss;
    ss << "#delta=" << delvals[n] << " ns , #Delta=" << deltavals[n] << " ns";
    leg->AddEntry(p1[n], ss.str().c_str(), "l");
  }
  leg->Draw("SAME");

  string c1name = "pulse_" + suffix + ".eps";
  c1->SaveAs(c1name.c_str());
  
}


void myText(Double_t x,Double_t y,Color_t color,int angle,char *text) 
{
  double tsize = 0.04;
  //Double_t tsize=0.05;
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextSize(tsize);
  l.SetTextColor(color);
  l.SetTextFont(42);
  l.SetTextAngle(angle);
  l.DrawLatex(x,y,text);
}

int Color(int hist)
{
  if ( hist == 0 ) { return( kRed ); }
  if ( hist == 1 ) { return( kAzure+6 ); }
  if ( hist == 2 ) { return( kMagenta ); }
  if ( hist == 3 ) { return( kOrange+6 ); }

  return( hist + 4 );
}


int Color(int hist, int zone, bool is_early)
{
  if ( zone == 3 ) {
    if ( is_early )  { return( Color(3) ); }
    if ( !is_early ) { return( Color(3) ); }
  }
  if ( zone == 2 ) {
    if ( is_early )  { return( Color(2) ); }
    if ( !is_early ) { return( Color(1) ); }
  }
  if ( zone == 1 ) {
    if ( is_early )  { return( Color(1) ); }
    if ( !is_early ) { return( Color(2) ); }
  }

  return( hist );
}
