#include "SampleGeom.hh"
#include <iostream>
gm2ringsim::SampleGeometry::SampleGeometry(std::string detName) :
  GeometryBase(detName),
  sampleInt(p_.get<int>("sampleInt")),
  sampleDouble(p_.get<double>("sampleDouble"))
{ }

void gm2ringsim::SampleGeometry::Print(){
  std::cout<<"\nPrinting Geometry for "<<Name<<std::endl;
  std::cout<<"\tsampleInt: "<<sampleInt<<std::endl;
  std::cout<<"\tsampleDouble: "<<sampleDouble<<std::endl;
  std::cout<<"End Geometry for "<<Name<<std::endl<<std::endl;
}
