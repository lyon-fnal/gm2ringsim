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
    
    // reset list to "No particle id's entered" state for new event
    void resetList();
    
    const std::vector<int>& showerParentList(int caloNum) { return m_showerParentList[caloNum]; }
    
    particleStatus addToList( int caloNum, int trackID, int parentID );
    void addEnteringParticle ( int caloNum, int trackID );
    
private:
    ShowerListManager();                  // only accessible by instance()
    ShowerListManager( const ShowerListManager& );            // stop default
    ShowerListManager& operator=( const ShowerListManager& ); // stop default
    
    // --- member items
    
    // -- the singleton instance
    static ShowerListManager* m_theManager;
    
    // the working list, stored as a map:
    //   key = calorimeter number
    // value = vector containing information for each particle seen by this calo
    //         vector[i] = -1 means the particle with trackID = i has not
    //                      been seen in this calorimeter
    //         vector[i] = n means the particle with trackID = i has been
    //                      seen in this calorimeter, and its "ancestor" is
    //                      trackID = n. To find the ancestor, we track back
    //                      through the parent list until we find a particle
    //                      that caused a calo hit. If a particle's parent is
    //                      not on the list, then that particle is it's own
    //                      ancestor.
    std::map< int, std::vector<int> > m_showerParentList;
    
    // remember the last run and event for which the list was re-initialized
    int m_lastEventReinit;
    int m_lastRunReinit;
};
    
} // namespace gm2ringsim

#endif // ShowerListManager_hh
