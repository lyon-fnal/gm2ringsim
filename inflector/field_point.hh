#ifndef FIELD_POINT_HH
#define FIELD_POINT_HH

/** @file field_point.hh
    
    Provides a declaraiton for the  class template field_point.

    @sa field_point.icc
    - Kevin Lynch 2009
    @author Brendan Kiburg
    @date Nov 2012 artified
 */

#include <cstring>
#include <ostream>

#if 0
struct Point3D {
  typedef double value_type;

  Point3D() { pt[0] = pt[1] = pt[2] = 0.; }
  Point3D(double x, double y, double z){ pt[0] = x; pt[1] = y; pt[2] = z; }
  Point3D(double *xyz){ std::memcpy(pt, xyz, 3*sizeof(double)); }

  double operator[](std::size_t i) const { return pt[i]; }
  double distance_to(Point3D const& o);

  double pt[3];
};

bool operator==(Point3D const& lhs, Point3D const& rhs);
std::ostream& operator<<(std::ostream& o, Point3D const& pt);
#endif


/** This class template provides an interface to libkdtree++.

    The interface provdes both the \a Point interface needed to insert
    a point into a KD tree, along with a \a Payload value at that
    point.  A KD tree allows efficient lookup for points in space.
 */
template<class Point, class Payload> class field_point {

public:

  /** Needed by the kdtree++ interface */
  typedef double value_type;

  field_point() : pt_(), pl_() {}

  field_point(Point pt, Payload pl) : pt_(pt), pl_(pl) {}

  double operator[](std::size_t i) const { return pt_[i]; }

  double distance_to(field_point const& other);

  /** \a Point accessor 

      A \a Point must not be modified after creation.
   */
  Point const& point() const { return pt_; }

  /** \a Payload accessor */
  Payload const& payload() const { return pl_; }
  /** The \a Payload can be modified after insertion */
  Payload payload(Payload const& pl) {
    Payload t = pl_;
    pl_ = pl;
    return t;
  }

private:
  Point pt_;
  Payload pl_;

};

#if 0
// I probably don't need this function...
template<class T, class U> inline double 
accessor(field_point<T,U> const& fp, std::size_t k){
  return fp[k];
}
#endif

template<class T, class U>
bool operator==(field_point<T,U> const&, field_point<T,U> const&);
template<class T, class U>
std::ostream& operator<<(std::ostream&, field_point<T,U> const&);

#define FIELD_POINT_ICC_INCLUDE
#include "field_point.icc"
#undef FIELD_POINT_ICC_INCLUDE

#endif // FIELD_POINT_HH
