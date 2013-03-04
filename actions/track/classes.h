// classes.h
// Created by Tasha Arvanitis, 23 July 2012

#include <vector>

#include "art/Persistency/Common/Wrapper.h"

#include "TrackingActionArtRecord.hh"

// Template the vector (no typedef)
template class std::vector< gm2ringsim::TrackingActionArtRecord >;

// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2ringsim::TrackingActionArtRecordCollection >;
