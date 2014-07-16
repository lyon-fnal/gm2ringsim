/** @file ShowerListManager.cc
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013 port to Art
    @date 2014
 
*/

#include <iostream>

#include "Geant4/G4RunManager.hh"
#include "Geant4/G4Event.hh"
#include "Geant4/G4Run.hh"

#include "ShowerListManager.hh"


gm2ringsim::ShowerListManager* gm2ringsim::ShowerListManager::m_theManager = 0;

gm2ringsim::ShowerListManager::ShowerListManager() :
    m_lastEventReinit( -666 ),
    m_lastRunReinit( -666 )
{}

gm2ringsim::ShowerListManager& gm2ringsim::ShowerListManager::instance()
{
    if (m_theManager == 0 )
    {
        m_theManager = new ShowerListManager();
    }
    return *m_theManager;
}

void gm2ringsim::ShowerListManager::resetList()
{
    
    // make sure that we haven't already re-initialized the list this event
    int currentRun   = G4RunManager::GetRunManager()->GetCurrentRun()->GetRunID();
    int currentEvent = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    if ( m_lastEventReinit == currentEvent && m_lastRunReinit == currentRun )
    {
        return;
    }
    
    // OK, we've not seen this run before.  Clear the list.
    m_showerParentList.clear() ;
    
    // Now store this run / event
    m_lastEventReinit = currentEvent;
    m_lastRunReinit = currentRun;
    
}

gm2ringsim::ShowerListManager::particleStatus
gm2ringsim::ShowerListManager::addToList( int caloNum, int trackID, int parentID )
{
    if ( m_showerParentList[caloNum].count(trackID) == 1 )
    { // we've seen this particle in a previous step
        
        return kAlreadyPartOfShower;
    }
    else // this particle is not on the list yet; figure out the trackID of its
    {    // "ancestor" that entered the calorimeter from outside
        
        if ( m_showerParentList[caloNum].count(parentID) == 1 )
        { // the parent is already in this shower; this particle has the same ancestor as its parent
            
            m_showerParentList[caloNum][trackID] = m_showerParentList[caloNum][parentID];
            return kAddedToShower;
        }
        else
        { // this particle's parent is not on the list; mark it as its own ancestor
            
            m_showerParentList[caloNum][trackID] = trackID;
            return kInitiatedShower;
        }
    }
}

void gm2ringsim::ShowerListManager::addEnteringParticle( int caloNum, int trackID )
{
    // mark this particle with its own trackID because it entered the calo from outside
    // (it is a "shower initiator" rather than part of an existing shower)
    m_showerParentList[caloNum][trackID] = trackID;
}

int gm2ringsim::ShowerListManager::getShowerParentID(int caloNum, int trackID)
{
    // return -1 if the particle is not on the list
    if (m_showerParentList.count(caloNum) == 0) return -1;
    else if (m_showerParentList[caloNum].count(trackID) == 0) return -1;
    else return m_showerParentList[caloNum][trackID];
}


