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
  tracebackRadial( p.get<std::vector<double>>("tracebackRadial")),
  tracebackZ( p.get<double>("tracebackZ")),
  tracebackTheta( p.get<double>("tracebackTheta")),
  tracebackRadialShiftAngle( p.get<double>("tracebackRadialShiftAngle") * deg),
  strawLocation( p.get<std::vector<int>>("strawLocation")),
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
  
  for (unsigned int i = 0 ; i < 44; ++i){
    tracebackRadialHalf[i] = tracebackRadial[i]/2;
  }

  tracebackZHalf = tracebackZ/2;
  tracebackThetaHalf = tracebackTheta/2;
  tanTracebackRadialShiftAngle = tan(tracebackRadialShiftAngle);
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
  oss << "  tracebackRadial= "; for (auto entry : tracebackRadial) { oss << " " << entry; }; oss << "\n";
  oss << "  tracebackZ="<< tracebackZ << "\n";
  oss << "  tracebackTheta="<< tracebackTheta << "\n";
  oss << "  strawLocations="; for (auto entry : strawLocation) { oss << " " << entry; }; oss << "\n";
  oss << "  whichTracebackLocations="; for (auto entry : whichTracebackLocations) { oss << " " << entry; }; oss << "\n";
  oss << "  displayTraceback=" << displayTraceback << "\n";
  oss << "  tracebackColor= "; for (auto entry : tracebackColor) { oss << " " << entry; }; oss << "\n";
  mf::LogInfo("TRACEBACK") << oss.str();
}
