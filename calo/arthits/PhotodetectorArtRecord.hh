#ifndef PhotodetectorArtRecord_hh
#define PhotodetectorArtRecord_hh

/** @file PhotodetectorArtRecord.hh
 
    Implements the photodetector hit stored in the Art output file
 
    Ported to Art from g2migtrace file pmtRecord.rhh
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
    struct PhotodetectorArtRecord {
        
        /** Current value of turn counter for the @b primary that caused
         this hit. */
        int turn;
        
        /** Which calorimeter was hit. */
        int caloNum;
        
        /** Which photodetector was hit. */
        int photodetectorNum;
        
        /** The ID of the impinging particle; obtain particle information
         from the trackRecord. */
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
        
        /** Global time of the photodetector hit. */
        float time;
        
        /** Number of photons with photon detection efficiency applied */
        int nphoton ;
        
        /** Photon energy, summed over detected photons */
        float e;
        
        PhotodetectorArtRecord() :
        turn(0), caloNum(0), photodetectorNum(0), trackID(0),
        r(0.), t(0.), v(0.), time(0.), nphoton(0), e(0.)
        {}
        
        virtual ~PhotodetectorArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        PhotodetectorArtRecord(int n, int cn, int pn, int id,
                               float r, float t, float v,
                               float time, float np, float e) :
        turn(n), caloNum(cn), photodetectorNum(pn), trackID(id),
        r(r), t(t), v(v), time(time), nphoton(np), e(e)
        {}
        
#endif // __GCCXML__
    }; //end of PhotodetectorArtRecord struct
    
    typedef std::vector<PhotodetectorArtRecord> PhotodetectorArtRecordCollection;
} // end namespace gm2ringsim

#endif // PhotodetectorArtRecord_hh
