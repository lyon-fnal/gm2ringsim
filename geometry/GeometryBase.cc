#include "gm2ringsim/geometry/BKGeom.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/services/Geometry_service.hh"


gm2ringsim::BKGeometry::BKGeometry(std::string detName){

  std::cout<<"Calling BKGeometry constructor with string "<<detName<<std::endl;
}

gm2ringsim::BKGeometry::BKGeometry(){

  std::cout<<"Inside BKGeometry() constructor"<<std::endl;
  art::ServiceHandle<artg4::GeometryService> g;

  fhicl::ParameterSet p = g->getDetParSet("bkGeom");
  mybot = p.get<int>("bottles");

  bottles = g->getInt("Bot");
  
  
  
}
