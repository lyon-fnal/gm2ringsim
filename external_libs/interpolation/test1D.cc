#include <iostream>
#include <vector>

#include "gm2ringsim/external_libs/interpolation/interpolators1D.hh"

int main(int argc, char **argv){

  double const point_array[] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.};
  double const data_array[] = {0.,1.,2.,3.,4.,5.,6.,7.,8.,9.};

  std::vector<double> data(data_array, data_array+10);

  std::vector<double> quadratic;
  for( int i=0; i!=10; ++i) {
    //    std::cout << i << ' ' << point_array[i] << ' '
    //	      << point_array[i]*point_array[i] << '\n';
    quadratic.push_back(point_array[i]*point_array[i]);
    //    std::cout << quadratic[i] << '\n';
  }

  uniform_linear_interpolator<double> uli(0., 9., data.begin(), data.end());

  double const bottom = -1., top = 10.;
  int const steps = 100;

  double const dx = (top-bottom)/(steps-1);

  double x = bottom;
  //  for(int i=0;i!=steps;++i,x+=dx)
  //    std::cout << x << ' ' <<  uli(x) << '\n';

  //  x = bottom;
  //  for(int i=0;i!=steps;++i,x+=dx)
  //    std::cout << x << ' ' <<  uli.derivative(x) << '\n';

  uniform_linear_interpolator<double> uli2(point_array, point_array+10, 
					   data.begin(),data.end());

  uniform_spline_interpolator<double> usi(0., 9., data.begin(), data.end());
  uniform_spline_interpolator<double> usi2(point_array, point_array+10, 
					   quadratic.begin(), quadratic.end());
  

  x = bottom;
  for(int i=0; i!=steps; ++i,x+=dx)
    std::cout << x << ' ' 
  	      << usi(x) << ' ' << usi.derivative(x) << ' '
	      << usi2(x) << ' ' << usi2.derivative(x) << '\n';

  uniform_spline_interpolator<double> usi3 = usi2;
  // should fail, right?
  // usi3 = usi;// it does

  return 0;
}
