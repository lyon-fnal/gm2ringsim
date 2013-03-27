#ifndef CaloArtRecord_hh
#define CaloArtRecord_hh

/** @file CaloArtRecord.hh
 
    Implements the calorimeter hit stored in the Art output file
 
    Ported to Art from g2migtrace file pmtRecord.rhh
        @author Kevin Lynch
        @date 2009
 
    @author Robin Bjorkquist
    @date 2013
 */

#include <vector>

#ifndef __GCCXML__ // Don't let ROOT see things it shouldn't see
#include "CLHEP/Vector/ThreeVector.h"
#endif // __GCCXML__

namespace gm2ringsim {
    struct CaloArtRecord {
        
        /** Current value of turn counter for the @b primary that caused
         this hit. */
        int turn;
        
        /** Which calorimeter was hit. */
        int caloNum;

        /** The ID of the impinging particle; obtain particle information
         from the trackRecord. */
        int trackID;
        
        /** Local calorimeter coordinate, mm; this is the position along the
         "most radial like" direction. Zero in the calorimeter center,
         increasing outward. */
        float r;
        
        /** Local calorimeter coordinate, mm; this is the position along the
         "thickness" coordinate direction.  Zero in the center,
         increasing downstream. */
        float t;
        
        /** Local calorimeter coordinate, mm; this is the position along the
         "most vertical" coordinate direction, normal to @a r and @t.
         Zero in the center, increasing vertically. */
        float v;
        
        /** Global time of the calorimeter hit. */
        float time;
        
        /** Momentum along the @r direction, MeV. */
        float pr;
        
        /** Momentum along the @t direction, MeV. */
        float pt;
        
        /** Momentum along the @v direction, MeV. */
        float pv;
        
        CaloArtRecord() :
        turn(0), caloNum(0), trackID(0),
        r(0.), t(0.), v(0.), time(0.), pr(0.), pt(0.), pv(0.)
        {}
        
        virtual ~CaloArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        CaloArtRecord(int n, int cn, int id,
                      float r, float t, float v, float time,
                      float pr, float pt, float pv) :
        turn(n), caloNum(cn), trackID(id),
        r(r), t(t), v(v), time(time),
        pr(pr), pt(pt), pv(pv)
        {}
        
#endif // __GCCXML__
    }; //end of CaloArtRecord struct
    
    typedef std::vector<CaloArtRecord> CaloArtRecordCollection;
} // end namespace gm2ringsim

#endif // CaloArtRecord_hh
