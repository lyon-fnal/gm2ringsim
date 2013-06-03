// classes.h

#include <vector>

#include "art/Persistency/Common/Wrapper.h"
#include "gm2ringsim/calo/arthits/CaloArtRecord.hh"
#include "gm2ringsim/calo/arthits/XtalArtRecord.hh"
#include "gm2ringsim/calo/arthits/XtalPhotonArtRecord.hh"
#include "gm2ringsim/calo/arthits/PhotodetectorArtRecord.hh"
#include "gm2ringsim/calo/arthits/PhotodetectorPhotonArtRecord.hh"

// Template the vector (don't use typedef)
template class std::vector< gm2ringsim::CaloArtRecord >;
template class std::vector< gm2ringsim::XtalArtRecord >;
template class std::vector< gm2ringsim::XtalPhotonArtRecord >;
template class std::vector< gm2ringsim::PhotodetectorArtRecord >;
template class std::vector< gm2ringsim::PhotodetectorPhotonArtRecord >;

// Template the wrapper for the vector (typedef okay)
template class art::Wrapper< gm2ringsim::CaloArtRecordCollection >;
template class art::Wrapper< gm2ringsim::XtalArtRecordCollection >;
template class art::Wrapper< gm2ringsim::XtalPhotonArtRecordCollection >;
template class art::Wrapper< gm2ringsim::PhotodetectorArtRecordCollection >;
template class art::Wrapper< gm2ringsim::PhotodetectorPhotonArtRecordCollection >;

