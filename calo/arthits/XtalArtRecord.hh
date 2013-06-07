#ifndef XtalArtRecord_hh
#define XtalArtRecord_hh

/** @file XtalArtRecord.hh
 
    Implements the xtal hit stored in the Art output file
 
    Ported to Art from g2migtrace file xtalRecord.rhh
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
    struct XtalArtRecord {
        
        /** Current value of turn counter for the @b primary that caused
         this hit. */
        int turn;
        
        /** Which calorimeter was hit. */
        int caloNum;
        
        /** Which XTAL was hit. */
        int xtalNum;
        
        /** The ID of the impinging particle; obtain particle information
         from the trackRecord. */
        int trackID;
        
        /** Not direct parent ID, but ID of e+ or e- that originates the cluster.
         Can be same as trackID.  If pdgID==13 (mu), then parentID is the ID of
         the electron daughter (if it decays) or ==trackID (if it does not) */
        int parentID;
        
        /** Local xtal coordinate, mm; this is the position along the
         "most radial like" direction. Zero in the xtal center,
         increasing outward. Charged track at entrance to xtal. */
        float r;
        
        /** Local xtal coordinate, mm; this is the position along the
         "thickness" coordinate direction.  Zero in the center,
         increasing downstream. Charged track at entrance to xtal. */
        float t;
        
        /** Local xtal coordinate, mm; this is the position along the
         "most vertical" coordinate direction, normal to @a r and @t.
         Zero in the center, increasing vertically. Charged track at entrance to xtal. */
        float v;
        
        /** Global time of the xtal hit. */
        float time;
        
        /** Momentum along the @r direction, MeV. Charged track at entrance to xtal. */
        float pr;
        
        /** Momentum along the @t direction, MeV. Charged track at entrance to xtal. */
        float pt;
        
        /** Momentum along the @v direction, MeV. Charged track at entrance to xtal. */
        float pv;
        
        /** Total energy of charged track */
        float e;
        
        /** Total energy deposited by charged track in xtal */
        float edep;
        
        /** Total path length of charged track in xtal */
        float trackLength;
        
        /** PDG ID of charged track */
        int pdgID ;
        
        /** Number of Cerenkov photons emitted by this track */
        int nphoton ;
        
        /** Total energy of Cerenkov photons */
        float ephoton ;
                
        XtalArtRecord() :
        turn(0), caloNum(0), xtalNum(0), trackID(0), parentID(0),
        r(0.), t(0.), v(0.), time(0.),
        pr(0.), pt(0.), pv(0.), e( 0. ),
        edep(0.), trackLength(0.), pdgID( 0 ),
        nphoton( 0 ), ephoton( 0. )
        {}
        
        virtual ~XtalArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        XtalArtRecord(int n, int cn, int xn, int id, int pid,
                      float r, float t, float v, float time,
                      float pr, float pt, float pv, float e,
                      float edep, float length, float pdg,
                      float np, float ep ) :
        turn(n), caloNum(cn), xtalNum(xn), trackID(id), parentID(pid),
        r(r), t(t), v(v), time(time),
        pr(pr), pt(pt), pv(pv), e(e),
        edep(edep), trackLength(length), pdgID(pdg),
        nphoton(np), ephoton(ep)
        {}
        
#endif // __GCCXML__
    }; //end of XtalArtRecord struct
    
    typedef std::vector<XtalArtRecord> XtalArtRecordCollection;
} // end namespace gm2ringsim

#endif // XtalArtRecord_hh
