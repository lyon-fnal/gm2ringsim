#include "gm2ringsim/traceback/TracebackGeometry.hh"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "Geant4/globals.hh"
#include <iostream>
#include <sstream>

gm2ringsim::TracebackGeometry::TracebackGeometry(std::string const & detName) :
  GeometryBase(detName),
  r( p.get<double>("r") * mm),
  v( p.get<double>("v") * mm),
  t( p.get<double>("t") * mm),
  rIn( p.get<double>("rIn") * in),
  rOut( p.get<double>("rOut") * in),
  windowAngle( p.get<double>("windowAngle") * deg),
  thetaOut( p.get<std::vector<double>>("thetaOut") ),
  tOffset( p.get<double>("tOffset") * mm),
  rOffset( p.get<double>("rOffset") * mm),
  vOffset( p.get<double>("vOffset") * mm),
  tRotation( p.get<double>("tRotation") * deg),
  rRotation( p.get<double>("rRotation") * deg),
  vRotation( p.get<double>("vRotation") * deg),
  tracebackZ( p.get<double>("tracebackZ")),
  tracebackTheta( p.get<double>("tracebackTheta")),
  strawLocation( p.get<std::vector<double>>("strawLocation")),
  strawRadialExtent( p.get<std::vector<double>>("strawRadialExtent")),
  whichTracebackLocations( p.get<std::vector<int>>("whichTracebackLocations")),
  displayTraceback( p.get<bool>("displayTraceback") ),
  tracebackColor( p.get<std::vector<double>>("tracebackColor")),
  displayStraw( p.get<bool>("displayStraw")),
  strawColor( p.get<std::vector<double>>("strawColor")),
  rC((rIn + rOut)/2)
{
  for (auto& entry : thetaOut ) { entry *= deg; }
  for (auto& entry : thetaIn ) { entry *= deg; }
  for (auto& entry : thetaC ) { entry *= deg; }
  
  //Derived quantities
  
  for (unsigned int i = 0; i < 2; ++i ) {
    thetaIn[i] = (thetaOut[i] - 0.12*deg);
    thetaC[i] = (thetaIn[i] + thetaOut[i])/2;
  }
  
  for (unsigned int i = 0 ; i < strawRadialExtent.size() ; i ++){
    strawRadialExtentHalf.push_back(strawRadialExtent[i]/2);
  }
  
  tracebackZHalf = tracebackZ/2;
  tracebackThetaHalf = tracebackTheta/2;
}

void gm2ringsim::TracebackGeometry::print() const{
  std::ostringstream oss;
  oss << "  r=" << r << "\n";
  oss << "  v=" << v << "\n";
  oss << "  t=" << t << "\n";
  oss << "  rIn=" << rIn << "\n";
  oss << "  rOut=" << rOut << "\n";
  oss << "  windowAngle=" << windowAngle << "\n";
  oss << "  thetaOut= "; for (auto entry : thetaOut) { oss << " " << entry; }; oss << "\n";
  oss << "  tOffset=" << tOffset << "\n";
  oss << "  rOffset=" << rOffset << "\n";
  oss << "  vOffset=" << vOffset << "\n";
  oss << "  tRotation=" << tRotation << "\n";
  oss << "  rRotation=" << rRotation << "\n";
  oss << "  vRotation=" << vRotation << "\n";
  oss << "  tracebackZ="<< tracebackZ << "\n";
  oss << "  tracebackTheta="<< tracebackTheta << "\n";
  oss << "  strawRadialExtent="; for (auto entry: strawRadialExtent) { oss << " " << entry; }; oss<< "\n";
  oss << "  strawLocations="; for (auto entry : strawLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichTracebackLocations="; for (auto entry : whichTracebackLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
