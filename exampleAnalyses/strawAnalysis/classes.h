// classes.h
// How ROOT knows how to deal with our data products
//

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "TrackerHitArtRecord.hh"
#include "WireID.hh"

// For WireID
// Template the vector (no typedef)
template class std::vector< gm2strawtracking::WireID >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2strawtracking::WireID >;

// For TrackerHits
// Template the vector (no typedef)
template class std::vector< gm2strawtracking::TrackerHitArtRecord >;
// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2strawtracking::TrackerHitArtRecordCollection >;

