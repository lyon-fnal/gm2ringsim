#ifndef ShowerListManager_hh
#define ShowerListManager_hh


/** @file ShowerListManager.hh

    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
    @date 2014
 
 */

#include <iostream>
#include <vector>

namespace gm2ringsim {

class ShowerListManager {
    
public:
    
    enum particleStatus { kAddedToShower, kAlreadyPartOfShower, kInitiatedShower };
    
    static ShowerListManager& instance();
    
    void resetList();
    
    particleStatus addToList( int caloNum, int trackID, int parentID );
    void addEnteringParticle( int caloNum, int trackID );
    int getShowerParentID( int caloNum, int trackID );
    
private:
    ShowerListManager();                  // only accessible by instance()
    ShowerListManager( const ShowerListManager& );            // stop default
    ShowerListManager& operator=( const ShowerListManager& ); // stop default
    
    // --- member items
    
    // -- the singleton instance
    static ShowerListManager* m_theManager;
    
    // the working list, stored as a map of maps:
    //   key = calorimeter number
    // value = map containing information for each particle seen by this calo
    //         within this map:
    //               key = trackID of particle
    //             value = trackID of the particle's "ancestor." To find the
    //                     ancestor, we track back through the parent list until
    //                     we find a particle that caused a calo hit. If a
    //                     particle's parent is not on the list, then that
    //                     particle is its own ancestor.
    std::map< int, std::map<int, int> > m_showerParentList;
    
    // remember the last run and event for which the list was re-initialized
    int m_lastEventReinit;
    int m_lastRunReinit;
};
    
} // namespace gm2ringsim

#endif // ShowerListManager_hh
