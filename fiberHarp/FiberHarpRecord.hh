#ifndef FiberHarpRecord_hh__
#define FiberHarpRecord_hh__

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif

namespace gm2ringsim {
  struct FiberHarpRecord {
    int x;

    FiberHarpRecord(){};
    virtual ~FiberHarpRecord(){};

    // ROOT doesn't need to know the rest
#ifndef __GCCXML__
    FiberHarpRecord(int i)
      : x(i)
    {}
#endif
  }; //end of FiberHarpRecord struct

  typedef std::vector<FiberHarpRecord> FiberHarpRecordCollection;
} // end namespace gm2ringsim

#endif
