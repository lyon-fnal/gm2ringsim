#ifndef G2MIGTRACE_INTERPOLATORS1D_HH
#define G2MIGTRACE_INTERPOLATORS1D_HH

#include <utility>

/// 1D interplator abstract base class

template<class Payload> class interpolator1D {
public:
  Payload operator()(double at_x) const { return this->get_value(at_x); }
  virtual Payload get_value(double at_x) const = 0;
  Payload derivative(double at_x) const { return this->get_derivative(at_x); }
  virtual Payload get_derivative(double at_x) const = 0;
  // We would really really like virtual constructors ... but those
  // aren't available yet, so let's require derived classes to also
  // include the following constructors:
  /*
  template<class PointIter, class DataIter>
  interpolator1D(PointIter xbegin, PointIter xend,
		 DataIter databegin, DataIter dataend);
  // how to specify something taking an ordered list of std::pair?
  iterplator1D( iterators of type std::pair<double,Payload> p);
  // this might work!
  template<class PairIter. class Accessor = OperatorBracket> 
  interpolator1D(PairIter pbegin, PairIter pend);
  */
};

template<class Iter> class OperatorBracket {
  
};

/// Concrete Interpolators

template<class Payload> class uniform_linear_interpolator : 
  public interpolator1D<Payload> {
public:
  template<class Iter> 
  uniform_linear_interpolator(double xmin, double xmax, Iter databegin, Iter dataend);

  // "virtual" constructor
  template<class PointIter, class DataIter>
  uniform_linear_interpolator(PointIter xbegin, PointIter xend,
			      DataIter databegin, DataIter dataend);

  // I don't know how to make this work in the presence of those
  //  "const" members....
  // can't use default template arguments in template functions
  //  template<class PairIter, template<class Iter> class Accessor> 
  //  template<class PairIter, template<class Iter> class Accessor = OperatorBracket<PairIter>> 
  //  uniform_linear_interpolator(PairIter pbegin, PairIter pend);

  Payload get_value(double at_x) const;
  Payload get_derivative(double at_x) const;
  
private:
  std::vector<Payload> data_;
  double const xmin_, xmax_; 
  double const dx_;
  // FIXME fix return type!
  int calc_offset(double at_x) const;
};

template<class Payload> class nonuniform_linear_interpolator : 
  public interpolator1D<Payload> {
  // this needs an "x" and a "data" vector, and needs a bisection
  // search for x (and a static lookup?)
};

template<class Payload, int Order> class polynomial_interpolator : 
  public interpolator1D<Payload> {
};

// FIXME ... this also needs a way to specify first
// derivative boundary conditions....perhaps a policy?
// FIXME ... can splines be used for NON-scalar types?  Nope, not as
// written!  One needs, at a minimum, to specialize initialize() for
// non-scalar types.  This is the perfect place for Concepts!  That
// would be sweet!
template<class Payload> class uniform_spline_interpolator : 
  public interpolator1D<Payload> {
public:
  template<class Iter> 
  uniform_spline_interpolator(double xmin, double xmax, Iter databegin, Iter dataend);

  // "virtual" constructor
  template<class PointIter, class DataIter>
  uniform_spline_interpolator(PointIter xbegin, PointIter xend,
			      DataIter databegin, DataIter dataend);

  Payload get_value(double at_x) const;
  Payload get_derivative(double at_x) const;

private:
  std::vector<Payload> data_;
  double const xmin_, xmax_;
  double const dx_;
  std::vector<Payload> y2_;
  // FIXME fix return type!
  int calc_offset(double at_x) const;
  void initialize();
};

template<class Payload> class nonuniform_spline_interpolator : 
  public interpolator1D<Payload> {


};

#define G2MIGTRACE_INTERPOLATORS1D_ICC_INCLUDE
#include "gm2ringsim/external_libs/interpolation/interpolators1D.icc"
#undef G2MIGTRACE_INTERPOLATORS1D_ICC_INCLUDE

#endif // G2MIGTRACE_INTERPOLATORS1D_HH
