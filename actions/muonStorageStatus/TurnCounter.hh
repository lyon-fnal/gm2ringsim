#ifndef TURNCOUNTER_HH
#define TURNCOUNTER_HH

#include "Geant4/globals.hh"

/** @file turnCounter.hh

    Provides an interface to track muons orbits in the ring.

    - Kevin Lynch 2009
 */

// Meyer's singleton
/** Meyer's singleton that keeps track of the number of muon orbits
    around the ring.

    Works with the turnCounterSD that is attached to the
    turnCounterTubs
    -# Reset the counter in our eventAction,
    -# Increment in the ProcessHits member of the turnCounterSD, and 
    -# Test for storage in the UserSteppingAction method of
    steppingAction.
*/
class turnCounter {
public:

  static turnCounter& getInstance();

  /** Return the current turn count. */
  G4int turns() const { return turns_; }

  /** Reset the counter. 

      Only do this at Begin of Event.
   */
  void reset() { turns_ = 0; }

  /** Increment the counter.

      This should only be done by the turnCounterSD, which performs
      the right checks to make sure the counter really should be
      incremented. 
  */
  G4int increment() { return ++turns_; }

private:
  turnCounter() : turns_(0) {}

  G4int turns_;

  turnCounter(turnCounter const&); // don't implement
  turnCounter& operator=(turnCounter const&); // don't implement
};

#endif // TURNCOUNTER_HH
