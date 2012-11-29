#ifndef G2MIGTRACE_INTERPOLATORS2D_HH
#define G2MIGTRACE_INTERPOLATORS2D_HH

// should I build any of this on 1D interpolators that I haven't
// written? 


////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///// 2D interpolator interface base class
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

// Requirements on the type Payload
// must perform as a numeric type, with standard numerical operators 

template<class Payload> class twoD_interpolator {
public:
  Payload value(double at_x, double at_y) const 
  { return this->operator()(at_x,at_y); }
  
  virtual Payload operator()(double at_x, double at_y) const = 0;
  // FIXME how to deal with creating derivatives?  policy?  load functions?  etc?
  virtual Payload derivative_x(double at_x, double at_y) const = 0;
  virtual Payload derivative_y(double at_x, double at_y) const = 0;
  //  virtual Payload derivative_xy(double at_x, double at_y) const = 0;

};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///// bilinear interpolators
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

#include <vector>

// how to deal with out of range values?  zero? nearest? template
// policy?
template<class Payload> class uniform_xy_bilinear_interpolator :
  public twoD_interpolator<Payload> {
public:
  // xlow, xhigh, xn, ylow, yhigh, yn,
  // databegin, dataend run from (xlow,ylow) along x first, then along
  // y

  template <class Iter>
  uniform_xy_bilinear_interpolator(double xlow, double xhigh, int xn,
				   double ylow, double yhigh, int yn,
				   Iter databegin, Iter dataend);
  template <class XIter, class YIter, class DataIter>
  uniform_xy_bilinear_interpolator(XIter xbegin, XIter xend,
				   YIter ybegin, YIter yend,
				   DataIter databegin, DataIter dataend);
  
  virtual Payload operator()(double at_x, double at_y) const; 
  virtual Payload derivative_x(double at_x, double at_y) const;
  virtual Payload derivative_y(double at_x, double at_y) const;
  // because of the interpolation order, this higher derivative
  // vanishes identically for this interpolator
  virtual Payload derivative_xy(double at_x, double at_y) const;

private:
  double const xlow_, xhigh_, ylow_, yhigh_;
  int const xn_, yn_;
  double const dx_, dy_;
  std::vector<Payload> const data_;
  int xoffset(double at_x) const;
  int yoffset(double at_y) const;
  Payload const& value_at(int xoff, int yoff) const;
};

// template function "make_uniform_xy_bilinear_interpolator" to deal
// with Iter dependency?  could use a real generator that takes the
// container as argument and makes up all this stuff using traits classes :-)

template<class T> class uniform_x_bilinear_interpolator :
  public twoD_interpolator<T> {

  // xlow, xhigh, xn, ybegin, yend,
  // databegin, dataend ... see above
  uniform_x_bilinear_interpolator(/*stuff*/);

private:

};

// map this one onto uniform_x_bilinear_interpolator
template<class T> class uniform_y_bilinear_interpolator :
  public twoD_interpolator<T> {

  // xbegin, xend, ylow, yhigh, yn
  // databegin, dataend ... see above
  uniform_y_bilinear_interpolator(/*stuff*/);  

private:
  
};

template<class T> class nonuniform_bilinear_interpolator :
  public twoD_interpolator<T> {

  // xbegin, xend, ybegin, yend
  // databegin, dataend
  nonuniform_bilinear_interpolator(/*stuff*/);

private:

};


template<class T> class uniform_radial_bilinear_interpolator :
  public twoD_interpolator<T> {

  // rlow, rhigh, rn,
  // thetan
  // databegin, dataend ... increase along theta for constant r 
  uniform_radial_bilinear_interpolator(/*stuff*/);

private:

};

template<class T> class nonuniform_radial_bilinear_interpolator :
  public twoD_interpolator<T> {

  // rbegin, rend,
  // thetan
  // databegin, dataend ... increase along theta for constant r 
  nonuniform_radial_bilinear_interpolator(/*stuff*/);

private:

};

////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
///// bicubic interpolators
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////

template<class Payload> class uniform_xy_bicubic_interpolator :
  public twoD_interpolator<Payload> {
public:
  // xlow, xhigh, xn, ylow, yhigh, yn,
  // databegin, dataend run from (xlow,ylow) along x first, then along
  // y

  template <class Iter>
  uniform_xy_bicubic_interpolator(double xlow, double xhigh, int xn,
				  double ylow, double yhigh, int yn,
				  Iter databegin, Iter dataend);
  template <class XIter, class YIter, class DataIter>
  uniform_xy_bicubic_interpolator(XIter xbegin, XIter xend,
				  YIter ybegin, YIter yend,
				  DataIter databegin, DataIter dataend);
  template <class Iter, class Iterx, class Itery, class Iterxy>
  uniform_xy_bicubic_interpolator(double xlow, double xhigh, int xn,
				  double ylow, double yhigh, int yn,
				  Iter databegin, Iter dataend,
				  Iterx dataxbegin, Iterx dataxend,
				  Itery dataybegin, Itery datayend,
				  Iterxy dataxybegin, Iterxy dataxyend
				  );
  template <class XIter, class YIter, class DataIter, 
	    class Iterx, class Itery, class Iterxy>
  uniform_xy_bicubic_interpolator(XIter xbegin, XIter xend,
				  YIter ybegin, YIter yend,
				  DataIter databegin, DataIter dataend,
				  Iterx dataxbegin, Iterx dataxend,
				  Itery dataybegin, Itery datayend,
				  Iterxy dataxybegin, Iterxy dataxyend
				  );
  
  virtual Payload operator()(double at_x, double at_y) const;
  virtual Payload derivative_x(double at_x, double at_y) const;
  virtual Payload derivative_y(double at_x, double at_y) const;
  virtual Payload derivative_xy(double at_x, double at_y) const;

private:
  double const xlow_, xhigh_, ylow_, yhigh_;
  int const xn_, yn_;
  double const dx_, dy_;
  std::vector<Payload> const data_;
  int xoffset(double at_x) const;
  int yoffset(double at_y) const;
  int offset(double at_x, double at_y) const;
  Payload const& value_at(int xoff, int yoff) const;
  void initialize();
  std::vector<Payload> data_x_;
  std::vector<Payload> data_y_;
  std::vector<Payload> data_xy_;
  struct holder {
    double x, y;
    Payload val, deriv_x, deriv_y, deriv_xy;
  };
  holder mutable result;
  void do_calc(double at_x, double at_y) const;
};

#define NOW_INCLUDE_INTERPOLATORS2D_ICC
#include "gm2ringsim/external_libs/interpolation/interpolators2D.icc"
#undef NOW_INCLUDE_INTERPOLATORS2D_ICC


#endif // G2MIGTRACE_INTERPOLATORS2D_HH
