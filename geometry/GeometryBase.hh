#ifndef GEOMETRY_BASE_HH
#define GEOMETRY_BASE_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/Geometry_service.hh"

#include "fhiclcpp/ParameterSet.h"

#include <string>
namespace gm2ringsim {

  struct GeometryBase { 
    
    GeometryBase(std::string const & detName);
    virtual ~GeometryBase() {}

    std::string name;
    fhicl::ParameterSet p;

  };



}


#endif // GEOMETRY_BASE_HH

