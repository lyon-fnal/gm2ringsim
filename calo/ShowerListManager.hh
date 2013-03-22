#ifndef ShowerListManager_hh
#define ShowerListManager_hh


/** @file ShowerListManager.hh

    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
 
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
    
    const std::vector<double>&  showerParentList() { return m_showerParentList; }
    particleStatus addToList( int trackID, int parentID );
    
private:
    ShowerListManager();                  // only accessible by instance()
    ShowerListManager( const ShowerListManager& );            // stop default
    ShowerListManager& operator=( const ShowerListManager& ); // stop default
    
    // --- member items
    
    // -- the singleton instance
    static ShowerListManager* m_theManager;
    
    // the working list
    std::vector<double> m_showerParentList;
    
    // remember the last run and event for which the list was re-initialized
    int m_lastEventReinit;
    int m_lastRunReinit;
};
    
} // namespace gm2ringsim

#endif // ShowerListManager_hh
