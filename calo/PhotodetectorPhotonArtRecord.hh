#ifndef PhotodetectorPhotonArtRecord_hh
#define PhotodetectorPhotonArtRecord_hh

/** @file PhotodetectorPhotonArtRecord.hh
 
 Implements the photodetector photon hit stored in the Art output file
 
 Ported to Art from g2migtrace file pmtPhotonRecord.rhh
 @author Werner Sun
 @date 2011
 
 @author Robin Bjorkquist
 @date 2013
 */

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif // __GCCXML__

namespace gm2ringsim {
    struct PhotodetectorPhotonArtRecord {
        
        /**
         There is one photodetectorPhotonArtRecord for each photon that enters
         a photodetector volume, whether or not it passes the detection 
         efficiency test.
         */
        
        /** Current value of turn counter for the @b primary that caused
         this hit. */
        int turn;
        
        /** Which calorimeter was hit. */
        int caloNum;
        
        /** Which photodetector was hit. 
         From perspective of beam (looking at the front face of the calorimeter)
         numbering starts at bottom right and increases along the row toward the
         storage ring.
         For the standard 54-crystal calorimeter:
         xtal 0 = bottom right corner
         xtal 8 = bottom left corner
         xtal 45 = top right corner
         xtal 53 = top left corner
         */
        int photodetectorNum;
        
        /** The ID of the photon */
        int trackID;
        
        /** Local photodetector coordinate, mm; this is the position along the
         "most radial like" direction. Zero in the photodetector center,
         increasing outward. */
        float r;
        
        /** Local photodetector coordinate, mm; this is the position along the
         "thickness" coordinate direction.  Zero in the center,
         increasing downstream. */
        float t;
        
        /** Local photodetector coordinate, mm; this is the position along the
         "most vertical" coordinate direction, normal to @a r and @t.
         Zero in the center, increasing vertically. */
        float v;
        
        /** Global time of the photodetector photon hit. */
        float time;
        
        /** Momentum along the @r direction, MeV. */
        float pr;
        
        /** Momentum along the @t direction, MeV. */
        float pt;
        
        /** Momentum along the @v direction, MeV. */
        float pv;
        
        /** Photon energy, MeV */
        float e;
        
        /** Does photon pass efficiency simulation? */
        bool accepted ;
        
        PhotodetectorPhotonArtRecord() :
        turn(0), caloNum(0), photodetectorNum(0), trackID(0),
        r(0.), t(0.), v(0.), time(0.),
        pr(0.), pt(0.), pv(0.), e(0.), accepted( false )
        {}
        
        virtual ~PhotodetectorPhotonArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        PhotodetectorPhotonArtRecord(int n, int cn, int pn, int id,
                               float r, float t, float v, float time,
                               float pr, float pt, float pv, float e,
                               float accept) :
        turn(n), caloNum(cn), photodetectorNum(pn), trackID(id),
        r(r), t(t), v(v), time(time),
        pr(pr), pt(pt), pv(pv), e(e), accepted(accept)
        {}
        
#endif // __GCCXML__
    }; //end of PhotodetectorPhotonArtRecord struct
    
    typedef std::vector<PhotodetectorPhotonArtRecord> PhotodetectorPhotonArtRecordCollection;
} // end namespace gm2ringsim

#endif // PhotodetectorPhotonArtRecord_hh
