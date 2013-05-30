/** @file spinController.cc
    
    Provides the implementation of the centralized spin tracking
    callback interface.  

    The implementation is almost too trivical to bother with a
    separate file, except that we need SpinController::getInstance()
    to be generated in a single place in the object code. 

    @author Kevin Lynch 
    @date 2009-2011

    Artized by 
    @author Brendan Kiburg
    @date Dec 2012
*/

#include "gm2ringsim/common/spin/SpinController.hh"
//#include "spinMessenger.hh"

gm2ringsim::SpinController::SpinController() : spin_tracking_(false), edm_tracking_(false), eta_(0.0), gm2_(0.0) {
  //,  sm_(new spinMessenger(this)) {
  
}

gm2ringsim::SpinController::~SpinController(){
  //  delete sm_;
}

gm2ringsim::SpinController& gm2ringsim::SpinController::getInstance() {
  static SpinController s;
  return s;
}

void gm2ringsim::SpinController::enable_spintracking(){
  sig_( spin_tracking_ = true );
  
}

void gm2ringsim::SpinController::disable_spintracking(){
  sig_( spin_tracking_ = false );

}

void gm2ringsim::SpinController::enable_edmtracking(){
  sig_( edm_tracking_ = true );
  
}

void gm2ringsim::SpinController::disable_edmtracking(){
  sig_( edm_tracking_ = false );

}

gm2ringsim::connection_t gm2ringsim::SpinController::connect( e_connect_ordering e, function<void(bool)> f ){
  return sig_.connect(e, f);
}

void gm2ringsim::SpinController::disconnect( connection_t c){
  sig_.disconnect(c);
}

