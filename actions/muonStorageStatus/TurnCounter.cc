#include "gm2ringsim/actions/muonStorageStatus/TurnCounter.hh"

/** @file turnCounter.cc

    Thar be implementations for turnCounter here!  

    - Kevin Lynch 2009
 */

gm2ringsim::TurnCounter& gm2ringsim::TurnCounter::getInstance(){
  static TurnCounter t;
  return t;
}
