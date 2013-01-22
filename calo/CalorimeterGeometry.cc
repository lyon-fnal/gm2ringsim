//
//  CalorimeterGeometry.cc
//  gm2ringsim
//
//  Created by Lawrence Gibbons on 12/15/12.
//  Copyright (c) 2012 Cornell University. All rights reserved.
//

// Geant includes
#include "Geant4/globals.hh"

// g-2 includes
#include "gm2ringsim/common/conversions.hh"

#include "CalorimeterGeometry.hh"

gm2ringsim::CalorimeterGeometry::CalorimeterGeometry(std::string const & detName) :
  GeometryBase(detName),
  nXtalRows(             p.get<int>("nXtalRows")                    ),
  nXtalCols(             p.get<int>("nXtalCols")                    ),
  wrappingGap(           p.get<double>("wrappingGap")          * cm ),
  crystalCaloBuffer(     p.get<double>("crystalCaloBuffer")    * cm ),
  xtalWidth(             p.get<double>("xtalWidth")            * cm ),
  xtalHeight(            p.get<double>("xtalHeight")           * cm ),
  xtalDepth(             p.get<double>("xtalDepth")            * cm ),
  sideWrapping(          p.get<std::string>("sideWrapping")              ),
  frontWrapping(         p.get<std::string>("frontWrapping")             ),
  backWrapping(          p.get<std::string>("backWrapping")              ),
  photodetectorShape(    p.get<std::string>("photodetectorShape")   ),
  photodetectorSize(     p.get<double>("photodetectorSize")    * cm ),
  photodetectorDepth(    p.get<double>("photodetectorDepth")   * cm ),
  opticalCouplingDepth(  p.get<double>("opticalCouplingDepth") * cm ),
  displayCalorimeterBox( p.get<bool>("displayCalorimeterBox")       ),
  displayWrappingVolumes(p.get<bool>("displayWrappingVolumes")      ),
  displayCrystalArray(   p.get<bool>("displayCrystalArray")      ),
  calorimeterColor(      p.get<std::vector<double>>("calorimeterColor")    ),
  xtalColor(             p.get<std::vector<double>>("xtalColor")           ),
  photodetectorColor(    p.get<std::vector<double>>("photodetectorColor")  ),
  opticalCouplingColor(  p.get<std::vector<double>>("opticalCouplingColor")),
  wrappingColor(         p.get<std::vector<double>>("wrappingColor")       ),
  radial(0),
  vertical(0),
  thickness(0)
{
    //Derived quantities
    
    // a volume with these parameters will  bound the enclosed crystals, 
    // including gaps, tightly.  To allow you to see particles before they
    // entry the PbF2, the calo mother volume must be defined including the
    // extra crystalCalBuffer space.  Note that we must include a gap outside
    // the crystals as well as between the crystals
    radial   = (nXtalCols * xtalWidth ) + (nXtalCols * wrappingGap);
    vertical = (nXtalRows * xtalWidth ) + (nXtalRows * wrappingGap);
    thickness = xtalDepth + opticalCouplingDepth + photodetectorDepth;
}

void gm2ringsim::CalorimeterGeometry::print() {
    std::ostringstream oss;
    oss << "  radial    = " << radial    << "\n";
    oss << "  vertical  = " << vertical  << "\n";
    oss << "  thickness = " << thickness << "\n";
	
    oss << "  array size  = " << nXtalRows << " x " << nXtalCols << "\n";
    oss << "  crystal size (w x h x D) = " << xtalWidth << " x " << xtalHeight << " x " << xtalDepth << "\n";
	oss << "  wrappingGap = " << wrappingGap << "\n";
	oss << "  crystalCaloBuffer = " << crystalCaloBuffer << "\n";
    
    oss << "  sideWrapping  = " << sideWrapping  << "\n";
    oss << "  frontWrapping = " << frontWrapping << "\n";
    oss << "  backWrapping  = " << backWrapping  << "\n";
    
    oss << "  photodetectorShape =   " << photodetectorShape   << "\n";
    oss << "  photodetectorDepth =   " << photodetectorDepth   << "\n";
    oss << "  opticalCouplingDepth = " << opticalCouplingDepth << "\n";
    
    oss << "  displayCalorimeterBox = " << displayCalorimeterBox << "\n";
    oss << "  calorimeterColor: "; for (auto entry : calorimeterColor) { oss << " " << entry; }; oss << "\n";
    oss << "  xtalColor:        "; for (auto entry : xtalColor) { oss << " " << entry; }; oss << "\n";
    oss << "  xtalColor:        "; for (auto entry : xtalColor) { oss << " " << entry; }; oss << "\n";
    
	
    mf::LogInfo("CALORIMETERGEOMETRY") << oss.str();
    
}
