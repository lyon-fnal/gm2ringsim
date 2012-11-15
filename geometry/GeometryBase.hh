#ifndef BKGEOM_HH
#define BKGEOM_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/Geometry_service.hh"

#include "fhiclcpp/ParameterSet.h"

#include <string>
namespace gm2ringsim {

  struct GeometryBase { 
    
    GeometryBase(std::string detName);
    ~GeometryBase() {}

    std::string Name;
    fhicl::ParameterSet p_;
  };



}

#endif // BKGEOM_HH
