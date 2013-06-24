/** @file PhotonHitCorrelator.cc
 
    Allows PhotodetectorSD to modify XtalPhotonHit information from XtalSD efficiently
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
 */

#include "XtalPhotonHit.hh"
#include "PhotonHitCorrelator.hh"

const int kInitialListSize = 300000;
const int kSizeIncrement   = 100000;

gm2ringsim::PhotonHitCorrelator * gm2ringsim::PhotonHitCorrelator::instance_ = 0;

gm2ringsim::PhotonHitCorrelator&
gm2ringsim::PhotonHitCorrelator::getInstance() {
    if ( instance_ == 0 ) {
        instance_ = new PhotonHitCorrelator();
    }
    return *instance_;
}

gm2ringsim::PhotonHitCorrelator::PhotonHitCorrelator() {
}

gm2ringsim::PhotonHitCorrelator::~PhotonHitCorrelator() {
}

void
gm2ringsim::PhotonHitCorrelator::reset() {
    photonTrackToHit_.clear();
    photonTrackToHit_.resize(kInitialListSize);
}

void
gm2ringsim::PhotonHitCorrelator::addTrack( int photonTrackId, XtalPhotonHit* hit ) {
    increaseListSize(photonTrackId);
    photonTrackToHit_[photonTrackId] = hit;
}

void
gm2ringsim::PhotonHitCorrelator::registerPhotodetectorTrack( int photonTrackId, bool photoElectron ) {
    increaseListSize(photonTrackId);
    if ( photonTrackToHit_[photonTrackId] == 0 ) {
        std::cout << "Warning!  No hit registered with PhotonHitCorrelator for track id " << photonTrackId << std::endl;
        return;
    }
    
    photonTrackToHit_[photonTrackId]->transmitted = true;
    photonTrackToHit_[photonTrackId]->detected = photoElectron;
}

void
gm2ringsim::PhotonHitCorrelator::increaseListSize(int photonTrackId) {
    int veclength = photonTrackToHit_.size();
    if ( photonTrackId >= veclength ) {
        photonTrackToHit_.resize( photonTrackId + kSizeIncrement );
    }
}
