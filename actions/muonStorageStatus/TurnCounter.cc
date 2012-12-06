#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

/** @file turnCounter.cc

    Thar be implementations for turnCounter here!  

    - Kevin Lynch 2009
 */

turnCounter& turnCounter::getInstance(){
  static turnCounter t;
  return t;
}
