#ifndef EVENTRECORD_RHH
#define EVENTRECORD_RHH

/** @file eventRecord.rhh
    Provides a class that stores global data for each Geant event. 
    - Kevin Lynch 2009
    - ARTified based on eventRecord.rhh
    - @author Brendan Kiburg
    - @date Dec 2012
*/

#include <vector>


/** Root storable type that holds global event outcome data for each
    Geant event. */
namespace gm2ringsim{
  struct EventRecord {
    /** Was the injected muon stored according to the criteria in place
	during the run? */
    bool muWasStored;
    /** If the muon was lost, this indicates the last orbit on which it
	hit the turnCounter. */
    int lastTurn;
    int EventStatus; 
    
    EventRecord() : muWasStored(false), lastTurn(0), EventStatus(-1) {}
    ~EventRecord() {}
    
#ifndef __GCCXML__
    EventRecord(bool stored,int turn, int status) :
      muWasStored(stored),
      lastTurn(turn),
      EventStatus(status) {}
#endif //__GCCXML__
  };
  
  typedef std::vector<EventRecord> EventRecordCollection;
  
} //namespace gm2ringsim

#endif // EVENTRECORD_RHH
