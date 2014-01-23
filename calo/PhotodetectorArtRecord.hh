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
        
        /** There is one photodetectorArtRecord for each photodetector with any 
         activity in it. The hit is created when the first photon enters the
         photodetector volume.
         
         trackID, r, t, v, and time are for the the first photon that entered 
         the photodetector, thereby initiating the photodetector hit.
         
         nphoton and e are incremented for each additional photon detected by
         that photodetector.
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
        
        /** Track ID of the first photon to enter the photodetector volume */
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
        
        /** Photon energy, summed over detected photons, MeV */
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
