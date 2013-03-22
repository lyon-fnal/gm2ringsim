#ifndef XtalPhotonArtRecord_hh
#define XtalPhotonArtRecord_hh

/** @file XtalPhotonArtRecord.hh
 
    Implements the xtal photon hit stored in the Art output file
 
    Ported to Art from g2migtrace file xtalPhotonRecord.rhh
        @author Lawrence Gibbons
        @date 2012
 
    @author Robin Bjorkquist
    @date 2013
 */

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif // __GCCXML__

namespace gm2ringsim {
    struct XtalPhotonArtRecord {
        
        /** Which calorimeter was hit. */
        int caloNum;
        
        /** Which crystal was hit. */
        int xtalNum;
        
        /** The ID of the impinging particle; obtain particle information
         from the trackRecord. */
        int trackID;
        
        /** Local (xtal) photon production coordinate, mm; this is the
         horizontal direction transverse to the crystal long dimension.
         Zero in the crystal center */
        float x;
        
        /** Local (xtal) photon production coordinate, mm; this is the
         vertical direction transverse to the crystal long dimension.
         Zero in the crystal center */
        float y;
        
        /** Local (xtal) photon production coordinate, mm; this is the
         depth into the crystal along the crystal long dimension.
         Zero in the crystal center */
        float depth;
        
        /** cos(angle), flight angle measured relative the crystal longitudinal axis */
        float costheta;
        
        /** flight azimuthal angle around the crystal longitudinal axis*/
        float phi;
        
        /** Photon energy */
        float e;
        
        /** whether this photon was transmitted to photodetector */
        bool transmitted;
        
        /** whether this photon was detected */
        bool detected;
        
        XtalPhotonArtRecord() :
        caloNum(0), xtalNum(0), trackID(0),
        x(0.), y(0.), depth(0.),
        costheta(0.), phi(0.), e(0.),
        transmitted(false), detected(false)
        {}
        
        virtual ~XtalPhotonArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        XtalPhotonArtRecord(int cn, int xn, int id,
                      float x, float y, float d,
                      float costheta, float phi, float e,
                      bool trans, bool det ) :
        caloNum(cn), xtalNum(xn), trackID(id),
        x(x), y(y), depth(d),
        costheta(costheta), phi(phi), e(e),
        transmitted(trans), detected(det)
        {}
        
#endif // __GCCXML__
    }; //end of XtalPhotonArtRecord struct
    
    typedef std::vector<XtalPhotonArtRecord> XtalPhotonArtRecordCollection;
} // end namespace gm2ringsim

#endif // XtalPhotonArtRecord_hh
