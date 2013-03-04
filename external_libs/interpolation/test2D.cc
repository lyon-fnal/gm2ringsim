#include <iostream>
#include <vector>

#include "gm2ringsim/external_libs/interpolation/interpolators2D.hh"


int main(int argc, char **argv){

  double const x_array[] = {0., 1., 2., 3., 4., 5.};
  int const xsize = 6;
  double const y_array[] = {0., 1., 2., 3., 4., 5., 6., 7.};
  int const ysize = 8;

  double data_array[xsize*ysize];
  double datax_array[xsize*ysize], datay_array[xsize*ysize], dataxy_array[xsize*ysize];
  for(int iy=0; iy!=ysize; ++iy){
    for(int ix=0; ix!=xsize; ++ix){
#if 0
      data_array[ix+iy*xsize] = x_array[ix]*x_array[ix]*x_array[ix] + 
	y_array[iy]*y_array[iy]*y_array[iy];
      //      std::cout << x_array[ix] << ' ' << y_array[iy] << ' ' 
      //      		<< data_array[ix+iy*xsize] << '\n';
      datax_array[ix+iy*xsize] = 3.*x_array[ix]*x_array[ix];
      datay_array[ix+iy*xsize] = 3.*y_array[iy]*y_array[iy];
      dataxy_array[ix+iy*xsize] = 0.;
#endif
#if 0
      data_array[ix+iy*xsize] = x_array[ix]*y_array[iy];
      datax_array[ix+iy*xsize] = y_array[iy];
      datay_array[ix+iy*xsize] = x_array[ix];
      dataxy_array[ix+iy*xsize] = 0.;
#endif
      data_array[ix+iy*xsize] = x_array[ix]*x_array[ix]*y_array[iy]*y_array[iy];
      datax_array[ix+iy*xsize] = 2.*x_array[ix]*y_array[iy]*y_array[iy];
      datay_array[ix+iy*xsize] = x_array[ix]*x_array[ix]*2.*y_array[iy];
      dataxy_array[ix+iy*xsize] = 4.*x_array[ix]*y_array[iy];
    }
  }

  uniform_xy_bilinear_interpolator<double> ubli(0., 5., xsize,
					       0., 7., ysize,
					       data_array, data_array+xsize*ysize);

  uniform_xy_bilinear_interpolator<double> ubli2(x_array, x_array+xsize,
						y_array, y_array+ysize,
						data_array, data_array+xsize*ysize);

#if 0
  std::cout << ubli(0., 0.) << ' '
	    << ubli(4., 4.) << ' '
	    << ubli(4.5, 4.5) << ' '
	    << ubli(5., 7.) << '\n';

  std::cout << ubli2(0., 0.) << ' '
	    << ubli2(4., 4.) << ' '
	    << ubli2(4.5, 4.5) << ' '
	    << ubli2(5., 7.) << '\n';

  std::cout << ubli2.derivative_x(0., 0.) << ' '
	    << ubli2.derivative_x(4., 4.) << ' '
	    << ubli2.derivative_x(4.5, 4.5) << ' '
	    << ubli2.derivative_x(5., 7.) << '\n';

  std::cout << ubli2.derivative_y(0., 0.) << ' '
	    << ubli2.derivative_y(4., 4.) << ' '
	    << ubli2.derivative_y(4.5, 4.5) << ' '
	    << ubli2.derivative_y(5., 7.) << '\n';
#endif

  uniform_xy_bicubic_interpolator<double> ubci(0., 5., xsize,
						0., 7., ysize,
						data_array, data_array+xsize*ysize);

  uniform_xy_bicubic_interpolator<double> ubci2(x_array, x_array+xsize,
						y_array, y_array+ysize,
						data_array, data_array+xsize*ysize);

  uniform_xy_bicubic_interpolator<double> ubci3(x_array, x_array+xsize,
						y_array, y_array+ysize,
						data_array, data_array+xsize*ysize,
						datax_array, datax_array+xsize*ysize,
						datay_array, datay_array+xsize*ysize,
						dataxy_array, dataxy_array+xsize*ysize);						

  int const xsteps = 100, ysteps = 100;
  double const dx = (x_array[xsize-1]-x_array[0])/(xsteps-1);
  double const dy = (y_array[ysize-1]-y_array[0])/(ysteps-1);
  double x = x_array[0], y = y_array[0];
  for(int iy=0; iy!=ysteps; ++iy, y+=dy){
    x = x_array[0];
    for(int ix=0; ix!=xsteps; ++ix, x+=dx){
      std::cout << x << ' ' << y << ' '
      		<< ubci3(x,y) << ' '
		<< ubci3.derivative_x(x,y) << ' '
		<< ubci3.derivative_y(x,y) << ' '
		<< ubci3.derivative_xy(x,y) << '\n';	
    }
  }

  

  return 0;
}
