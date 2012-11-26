#include "gm2ringsim/fields/field_point.hh"

#if 0
double Point3D::distance_to(Point3D const& o){
  double dist = 0., temp = 0.;
  for(std::size_t i=0; i!=3; ++i){
    temp = pt[i] - o.pt[i];
    dist += temp*temp;
  }
  return dist;
}

bool operator==(Point3D const& lhs, Point3D const& rhs){
  for(std::size_t i=0; i!=3; ++i)
    if( lhs.pt[i] != rhs.pt[i] )
      return false;
  return true;
}

std::ostream& operator<<(std::ostream& o, Point3D const& pt){
  o << '(' << pt[0] << ',' << pt[1] << ',' << pt[2] << ')';
  return o;
}
#endif
