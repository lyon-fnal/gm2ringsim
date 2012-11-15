#ifndef BKGEOM_HH
#define BKGEOM_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "artg4/services/Geometry_service.hh"

#include <string>
namespace gm2ringsim {

  struct BKGeometry { 

    BKGeometry() ;
    BKGeometry(std::string detName);
    ~BKGeometry() {}
    int bottles;
    int mybot;
  };



}

#endif // BKGEOM_HH
