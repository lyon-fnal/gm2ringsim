#ifndef BKGEOM_HH
#define BKGEOM_HH

#include "art/Framework/Services/Registry/ServiceHandle.h"
//#include "artg4/services/DetectorHolder_service.hh"
#include "artg4/services/Geometry_service.hh"

namespace gm2ringsim {

  struct BKGeometry { 

    BKGeometry() ;

    ~BKGeometry() {}
    int bottles;
    int mybot;
  };



}

#endif // BKGEOM_HH
