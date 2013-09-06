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
        
        /** Global position coordinates, mm. */
        float x;
        float y;
        float z;
        
        /** Global time of the calorimeter hit. */
        float time;
        
        /** Global momentum coordinates, MeV. */
        float px;
        float py;
        float pz;
        
        float energy;

        CaloArtRecord() :
        turn(0), caloNum(0), trackID(0),
        x(0.), y(0.), z(0.), time(0.), px(0.), py(0.), pz(0.), energy(0.)
        {}
        
        virtual ~CaloArtRecord(){};
        
        // ROOT doesn't need to know the rest
#ifndef __GCCXML__
        CaloArtRecord(int n, int cn, int id,
                      float x, float y, float z, float time,
                      float px, float py, float pz, float e) :
        turn(n), caloNum(cn), trackID(id),
        x(x), y(y), z(z), time(time),
        px(px), py(py), pz(pz), energy(e) 
        {}
        
#endif // __GCCXML__
    }; //end of CaloArtRecord struct
    
    typedef std::vector<CaloArtRecord> CaloArtRecordCollection;
} // end namespace gm2ringsim

#endif // CaloArtRecord_hh
