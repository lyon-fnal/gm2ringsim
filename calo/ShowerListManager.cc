/** @file ShowerListManager.cc
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
 
*/

#include <iostream>

#include "Geant4/G4RunManager.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4Run.hh"

#include "ShowerListManager.hh"

const int kInitialListSize = 500000;
const int kSizeIncrement = 100000;
const int kNoTrackNumber = -1;

gm2ringsim::ShowerListManager* gm2ringsim::ShowerListManager::m_theManager = 0;

gm2ringsim::ShowerListManager::ShowerListManager() :
    m_lastEventReinit( -666 ),
    m_lastRunReinit( -666 )
{}

gm2ringsim::ShowerListManager& gm2ringsim::ShowerListManager::instance() {
    if (m_theManager == 0 ) {
        m_theManager = new ShowerListManager();
    }
    return *m_theManager;
}

void gm2ringsim::ShowerListManager::resetList() {
    
    // make sure that we haven't already re-initialized the list this event
    int currentRun   = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
    int currentEvent = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    if ( m_lastEventReinit == currentEvent && m_lastRunReinit == currentRun ) {
        return;
    }
    
    // OK, we've not seen this run before.  Re-initialize
    m_showerParentList.clear() ;
    m_showerParentList.resize( kInitialListSize, kNoTrackNumber );
    
    // Now store this run / event
    m_lastEventReinit = currentEvent;
    m_lastRunReinit = currentRun;
    
    //    std::cout << "#_#_#_#_#_#_#_#_#_# Resetting shower list run/ev: " << currentRun << "/" << currentEvent << std::endl;
}

gm2ringsim::ShowerListManager::particleStatus
gm2ringsim::ShowerListManager::addToList( int trackID, int parentID ) {
    
    // make sure our list is long enough to accomodate this particle
    int listSize = m_showerParentList.size();
    if ( trackID >= listSize ) {
        m_showerParentList.resize( trackID+kSizeIncrement, kNoTrackNumber );
    }
    
    if ( m_showerParentList[trackID] != kNoTrackNumber ) {
        // we've seen this particule in a previous step
        return kAlreadyPartOfShower;
    } else {
        if ( m_showerParentList[parentID] != kNoTrackNumber ) {
            // the parent particle is already in this shower
            m_showerParentList[trackID] = m_showerParentList[parentID];
            return kAddedToShower;
        } else {
            // this particle must have initiated the shower
            m_showerParentList[trackID] = trackID;
            return kInitiatedShower;
        }
    }
}


