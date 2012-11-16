#include "WedgeGeom.hh"
#include <iostream>
gm2ringsim::WedgeGeometry::WedgeGeometry(std::string detName) :
  GeometryBase(detName),
  length(p.get<int>("length")),
  angle(p.get<double>("angle"))
{ }

void gm2ringsim::WedgeGeometry::Print(){
  std::cout<<"\nPrinting Geometry for "<<Name<<std::endl;
  std::cout<<"\tlength: "<<length<<std::endl;
  std::cout<<"\tangle: "<<angle<<std::endl;
  std::cout<<"End Geometry for "<<Name<<std::endl<<std::endl;
}
