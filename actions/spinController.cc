/** @file spinController.cc
    
    Provides the implementation of the centralized spin tracking
    callback interface.  

    The implementation is almost too trivical to bother with a
    separate file, except that we need spinController::getInstance()
    to be generated in a single place in the object code. 

    @author Kevin Lynch 
    @date 2009-2011
*/

#include "gm2ringsim/actions/spinController.hh"
//#include "spinMessenger.hh"

spinController::spinController() : spin_tracking_(false) {
  //,  sm_(new spinMessenger(this)) {
  
}

spinController::~spinController(){
  //  delete sm_;
}

spinController& spinController::getInstance() {
  static spinController s;
  return s;
}

void spinController::enable_spintracking(){
  sig_( spin_tracking_ = true );
  
}

void spinController::disable_spintracking(){
  sig_( spin_tracking_ = false );

}

connection_t spinController::connect( e_connect_ordering e, function<void(bool)> f ){
  return sig_.connect(e, f);
}

void spinController::disconnect( connection_t c){
  sig_.disconnect(c);
}

