#ifndef VIRTUALRINGSTATION_UTIL_HH
#define VIRTUALRINGSTATION_UTIL_HH

#include "gm2ringsim/common/virtualringstation/arthits/VirtualRingStationArtRecord.hh"
#include "gm2ringsim/common/virtualringstation/VirtualRingStationHit.hh"

#include "gm2ringsim/common/g2PreciseValues.hh"
#include "gm2ringsim/common/UsefulVariables.hh"


//helper function
namespace gm2ringsim {
  VirtualRingStationArtRecord convert(VirtualRingStationHit* pth){

    //G4cout << "convert: virtualringstationHit -> virtualringstationRecord\n";
    VirtualRingStationArtRecord ts;

    G4ThreeVector pos = pth->position;
    G4ThreeVector mom = pth->momentum;
    G4ThreeVector pol = pth->polarization;
    
    //---------------------
    // Basics
    //---------------------
    ts.turn = pth->turnNum;
    ts.volumeUID = pth->volumeUID;
    ts.trackID = pth->trackID;
    ts.time = pth->time;
    ts.pdgid = pth->PDGID;


    //-------------------
    // Position variables
    //-------------------
    G4double rhat = ComputeRhat(&pos);
    G4double vhat = ComputeVhat(&pos);
    G4double theta = ComputeTheta(&pos);
    ts.rhat = rhat;
    ts.vhat = vhat;
    ts.theta = theta;
    
    
    //-------------------
    // Momentum variables
    //-------------------
    G4double prhat = ComputePrhat(&pos, &mom);
    G4double pvhat = ComputePvhat(&pos, &mom);
    ts.p = mom.mag();
    ts.prhat = prhat;
    ts.pvhat = pvhat;
    
    
    //-----------------------
    // Polarization variables
    //-----------------------
    ts.polx = pol.x();
    ts.poly = pol.y();
    ts.polz = pol.z();

    


    return ts;    
  } // convert VirtualRingStationHit*
} //namespace gm2ringsim


#endif
