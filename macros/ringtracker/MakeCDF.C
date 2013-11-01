void MakeCDF(TH1F *hist)
{
   double sum = hist->Integral();
   int fbin = hist->FindFirstBinAbove(0.995*sum);
   int lbin = hist->FindLastBinAbove(0.995*sum);
   
   cout << "  double cdf[" << lbin-fbin << "]={";
   for ( int i = fbin; i <= lbin; i++ ) {
     cout.precision(3);
     cout.precision(6);
     cout << hist->Integral(fbin, i)/sum << " , ";
   }
   cout << "};" << endl;

   cout << "  double dP[" << lbin-fbin << "]={";
   for ( int i = fbin; i <= lbin; i++ ) {
     cout.precision(6);
     cout << hist->GetBinCenter(i) << " , ";
   }
   cout << "};" << endl;
}
