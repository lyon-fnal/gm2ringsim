// classes.h
// Created by Tasha Arvanitis, 23 July 2012

#include <vector>

#include "art/Persistency/Common/Wrapper.h"

#include "TrackArtHit.hh"

// Template the vector (no typedef)
template class std::vector< gm2ringsim::TrackArtHit >;

// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2ringsim::TrackArtHitCollection >;
