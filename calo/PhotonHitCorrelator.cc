/** @file PhotonHitCorrelator.cc
 
    Allows PhotodetectorSD to modify XtalPhotonHit information from XtalSD efficiently
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
    @date 2014
 */

#include "XtalPhotonHit.hh"
#include "PhotonHitCorrelator.hh"


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
    photonList_.clear();
}

void
gm2ringsim::PhotonHitCorrelator::addTrack( int photonTrackId, XtalPhotonHit* hit ) {
    photonList_[photonTrackId] = hit;
}

void
gm2ringsim::PhotonHitCorrelator::registerPhotodetectorTrack( int photonTrackId, bool photoElectron ) {
    if ( photonList_.count(photonTrackId) == 0 ) {
        std::cout << "Warning!  No hit registered with PhotonHitCorrelator for track id " << photonTrackId << std::endl;
        return;
    }
    
    photonList_[photonTrackId]->transmitted = true;
    photonList_[photonTrackId]->detected = photoElectron;
}