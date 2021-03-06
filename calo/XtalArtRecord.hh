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
        
        /** There is one xtalArtRecord for each crystal in the calorimeter with
         any activity in it. The hit is created when the first charged particle 
         enters the crystal. 
         
         trackID, r, t, v, time, pr, pt, pv, e and pdgID are for the the first
         charged particle that entered the xtal, thereby initiating the xtal hit
         
         edep, trackLength, nphoton and ephoton are incremented for each charged
         particle step taken within that crystal.
         */
        
        /** Current value of turn counter for the @b primary that caused
         this hit. */
        int turn;
        
        /** Which calorimeter was hit. */
        int caloNum;
        
        /** Which XTAL was hit. 
         From perspective of beam (looking at the front face of the calorimeter)
         numbering starts at bottom right and increases along the row toward the
         storage ring.
         For the standard 54-crystal calorimeter:
            xtal 0 = bottom right corner
            xtal 8 = bottom left corner
            xtal 45 = top right corner
            xtal 53 = top left corner
         */
        int xtalNum;
        
        /** The ID of the impinging particle; obtain particle information
         from the trackRecord. */
        int trackID;
        
        /** Not direct parent ID, but ID of calo hit that originates the cluster.
         Can be same as trackID.  */
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
        
        /** Total energy of charged track ( = total energy of first charged 
         particle to enter xtal), MeV. */
        float e;
        
        /** Total energy deposited by charged track in xtal 
         ( = sum of energy deposits by all charged particle steps in this xtal), 
         MeV. */
        float edep;
        
        /** Total path length of charged track in xtal 
         ( = sum of step lengths taken by charged particles in this xtal), mm. */
        float trackLength;
        
        /** PDG ID of charged track */
        int pdgID ;
        
        /** Number of Cerenkov photons emitted by this track
         ( = number of Cerenkov photons emitted by all charged particles in this xtal) */
        int nphoton ;
        
        /** Total energy of Cerenkov photons, MeV */
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
