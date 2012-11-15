#include "gm2ringsim/geometry/GeometryBase.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/services/Geometry_service.hh"


gm2ringsim::GeometryBase::GeometryBase(std::string detName) :
  Name(detName),
  p()
{
  //std::cout<<"Inside GeometryBase(std::string)"<<std::endl;
  //std::cout<<"Trying to get "<<detName<<" from the fhicl"<<std::endl;

  art::ServiceHandle <artg4::GeometryService> g;
  p = g->getDetParSet(detName);
}

