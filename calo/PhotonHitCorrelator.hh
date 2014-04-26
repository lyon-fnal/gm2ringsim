#ifndef PhotonHitCorrelator_hh
#define PhotonHitCorrelator_hh

/** @file  PhotonHitCorrelator.hh
 
    A singleton class for correlating the transmission and photodetection
    efficiences from PhotdetectorSD to the XtalPhotonHits created in XtalSD
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
    @date 2014
 */

#include <vector>

namespace gm2ringsim {

class XtalPhotonHit;

class PhotonHitCorrelator {
public:
    static PhotonHitCorrelator& getInstance();
    
    void reset();
    
    // add the XtalHit associated with the geant track number for a cerenkov photon
    void addTrack( int photonTrackId, XtalPhotonHit* hit );
    
    // flag the cerenkov photon as transmitted and its detection status
    void registerPhotodetectorTrack( int photonTrackId, bool photoElectron );
    
private:
    PhotonHitCorrelator(); // only public access via getInstance()
    ~PhotonHitCorrelator();
    
    // prevent automatic implementation
    PhotonHitCorrelator( PhotonHitCorrelator const & );
    PhotonHitCorrelator& operator=(PhotonHitCorrelator const &);
    
    // private member data
    std::map<int, XtalPhotonHit*> photonList_;
    // This map has    key = trackID for photon
    //               value = that photon's XtalPhotonHit
    
    // handle to the single instance
    static PhotonHitCorrelator* instance_;
};

} // namespace gm2ringsim

#endif // PhotonHitCorrelator_hh
