// classes.h

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "gm2ringsim/calo/CaloArtRecord.hh"
#include "gm2ringsim/calo/PhotodetectorArtRecord.hh"
#include "gm2ringsim/calo/PhotodetectorPhotonArtRecord.hh"

// Template the vector (don't use typedef)
template class std::vector< gm2ringsim::CaloArtRecord >;
template class std::vector< gm2ringsim::PhotodetectorArtRecord >;
template class std::vector< gm2ringsim::PhotodetectorPhotonArtRecord >;

// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2ringsim::CaloArtRecordCollection >;
template class art::Wrapper< gm2ringsim::PhotodetectorArtRecordCollection >;
template class art::Wrapper< gm2ringsim::PhotodetectorPhotonArtRecordCollection >;

