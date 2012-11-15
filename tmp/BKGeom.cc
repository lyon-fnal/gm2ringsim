#include "gm2ringsim/tmp/BKGeom.hh"
#include "art/Framework/Services/Registry/ServiceHandle.h"

#include "artg4/services/Geometry_service.hh"


gm2ringsim::BKGeometry::BKGeometry(){

  std::cout<<"Inside BKGeometry() constructor"<<std::endl;
  art::ServiceHandle<artg4::GeometryService> g;
  
  
  fhicl::ParameterSet p = g->getDetector("bkGeom");

  mybot = p.get<int>("bottles");
  
  bottles = g->GetBot();
  
  
  
}
