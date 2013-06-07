// classes.h

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "gm2ringsim/fiberHarp/FiberHarpArtRecord.hh"

// Template the vector (don't use typedef)
template class std::vector< gm2ringsim::FiberHarpArtRecord >;

// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2ringsim::FiberHarpArtRecordCollection >;
