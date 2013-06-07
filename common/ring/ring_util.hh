#ifndef RING_UTIL_HH
#define RING_UTIL_HH

#include "gm2ringsim/common/ring/RingArtRecord.hh"
#include "gm2ringsim/common/ring/RingHit.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

//helper function
namespace gm2ringsim {
  RingArtRecord convert(RingHit* prh){
    RingArtRecord rr;
    
    G4ThreeVector const& pos = prh->position;
    G4ThreeVector const& mom = prh->momentum;
    
    G4ThreeVector const& loc_pos = prh->local_position;
    //  G4ThreeVector const& loc_mom = prh->local_momentum;
    
    rr.x_loc = loc_pos.x();
    rr.y_loc = loc_pos.y();
    rr.z_loc = loc_pos.z();
    
    
    rr.prhat_loc = 0.0;
    rr.pvhat_loc = mom.y()/rr.p;
    
    
    rr.rhat = std::sqrt(pos.x()*pos.x() + pos.z()*pos.z()) - R_magic();
    rr.vhat = pos.y();
    rr.theta = std::atan2(pos.z(),pos.x());
    
    if( rr.theta < 0 )
      rr.theta+= 2.*M_PI;
    /*
      \vec{proj p->rhat} = \frac{\vec{rhat}\dot\vec{p}}{rhat^2}
      \vec{rhat} 
    */
    
    rr.time = prh->time;
    
    rr.p = mom.mag();
    // a dot product to get the sign right...
    double r = std::sqrt(pos.x()*pos.x()+pos.z()*pos.z());
    rr.prhat = 
      ((mom.x()*pos.x() + mom.z()*pos.z())/r)/rr.p;
    rr.pvhat = mom.y()/rr.p;
    
    rr.turn = prh->turnNum;
    
    rr.volumeUID = prh->volumeUID;
    rr.trackID = prh->trackID;
    
    // G4cout << rr.trackID << '\n';
    
    /*
      std::string name = 
      rootStorageManager::getInstance().getUOM().lookup(rr.volumeUID);
      G4cout << name << ' ' << prh->time << ' ' << pos << '\n';
    */
    
    rr.deltaE = prh->deltaE;
    
    G4ThreeVector const& momf = prh->momentum_final;
    
    rr.deltaP = momf.mag() - rr.p; // OK
    G4ThreeVector delta_mom = momf-mom; // OK
    rr.deltaPrhat = 
      (delta_mom.x()*pos.x()+ delta_mom.z()*pos.z())/r;
    rr.deltaPvhat = delta_mom.y();
    
    //    G4cout << rr.p << ' ' << rr.prhat << ' ' << rr.pvhat << '\n';
    //    G4cout << '\t' << delta_mom << '\n';
    //    G4cout << '\t' << deltaP << ' ' << deltaPrhat << ' ' << deltaPvhat
    //    << '\n';
    
    return rr;
    
  } // convert RingHit*


} //namespace gm2ringsim


#endif
