#ifndef TRACKER_UTIL_HH
#define TRACKER_UTIL_HH

#include "gm2ringsim/common/tracker/TrackerArtRecord.hh"
#include "gm2ringsim/common/tracker/TrackerHit.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"

//helper function
namespace gm2ringsim {
  TrackerArtRecord convert(TrackerHit* pth){

    //G4cout << "convert: trackerHit -> trackerRecord\n";
    TrackerArtRecord ts;

    G4ThreeVector const& pos = pth->position;
    G4ThreeVector const& mom = pth->momentum;
    G4ThreeVector const& pol = pth->polarization;
    
    ts.rhat = std::sqrt(pos.x()*pos.x() + pos.z()*pos.z()) - R_magic();
    ts.vhat = pos.y();
    ts.theta = std::atan2(pos.z(),pos.x());
    if( ts.theta < 0 )
      ts.theta+= 2.*M_PI;

    ts.time = pth->time;

    /*
      \vec{proj p->rhat} = \frac{\vec{rhat}\dot\vec{p}}{rhat^2}
      \vec{rhat} 
     */

    ts.p = mom.mag();
    // a dot product to get the sign right...
    double r = std::sqrt(pos.x()*pos.x()+pos.z()*pos.z());
    ts.prhat = 
      ((mom.x()*pos.x() + mom.z()*pos.z())/r)/ts.p;
    ts.pvhat = mom.y()/ts.p;
    
    //    G4cout << ts.p << ' ' << ts.prhat << ' ' << ts.pvhat << '\n';

    ts.turn = pth->turnNum;

    ts.volumeUID = pth->volumeUID;
    ts.trackID = pth->trackID;

    ts.polx = pol.x();
    ts.poly = pol.y();
    ts.polz = pol.z();

    return ts;    
  } // convert TrackerHit*
} //namespace gm2ringsim


#endif
