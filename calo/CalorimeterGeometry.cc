/** @file CalorimeterGeometry.cc
 
    @author Lawrence Gibbons
    @date 2012
 
    @author Robin Bjorkquist
    @date 2013
 
*/

// Geant includes
#include "Geant4/globals.hh"

// g-2 includes
#include "gm2ringsim/common/conversions.hh"

#include "CalorimeterGeometry.hh"
// Calling the delegated constructor in the initialization list is a nice feature in C++11
//gm2ringsim::CalorimeterGeometry::CalorimeterGeometry() : CalorimeterGeometry("calorimeter"){ }
  
gm2ringsim::CalorimeterGeometry::CalorimeterGeometry(std::string const & detName) :
  GeometryBase(detName),
  nCalorimeters(          p.get<int>("nCalorimeters", 24)),
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
  frontWrappingHole(     p.get<bool>("frontWrappingHole")           ),
  frontWrappingHoleSize( p.get<double>("frontWrappingHoleSize")* cm ),
  photodetectorShape(    p.get<std::string>("photodetectorShape")   ),
  photodetectorSize(     p.get<double>("photodetectorSize")    * cm ),
  photodetectorDepth(    p.get<double>("photodetectorDepth")   * cm ),
  opticalCouplingDepth(  p.get<double>("opticalCouplingDepth") * cm ),
  diffuser(              p.get<bool>("diffuser")                    ),
  diffuserDepth(         p.get<double>("diffuserDepth")        * cm ),
  displayCalorimeterBox( p.get<bool>("displayCalorimeterBox")       ),
  displayWrappingVolumes(p.get<bool>("displayWrappingVolumes")      ),
  displayCrystalArray(   p.get<bool>("displayCrystalArray")      ),
  calorimeterColor(      p.get<std::vector<double>>("calorimeterColor")    ),
  xtalColor(             p.get<std::vector<double>>("xtalColor")           ),
  photodetectorColor(    p.get<std::vector<double>>("photodetectorColor")  ),
  opticalCouplingColor(  p.get<std::vector<double>>("opticalCouplingColor")),
  diffuserColor(         p.get<std::vector<double>>("diffuserColor")       ),
  wrappingColor(         p.get<std::vector<double>>("wrappingColor")       ),
  killShowers(           p.get<bool>("killShowers")                 ),
  radial(0),
  vertical(0),
  thickness(0)
{
    //Derived quantities
    
    // a volume with these parameters will  bound the enclosed crystals, 
    // including gaps, tightly. In order to define wrapping materials via
    // optical surfaces, we must include a wrappingGap on all sides of the
    // crystal (except the back, where the optical coupling volume must be
    // flush with the crystal volume).
    
    // To allow you to see particles before they enter the PbF2, the calo
    // mother volume must have these dimensions PLUS an extra crystalCaloBuffer
    // space, so that all particles entering the calorimeter will pass through
    // the calo volume (and activate the CaloSD) before entering any sub-volume.
    
    radial   = ( nXtalCols * xtalWidth ) + ( (nXtalCols+1) * wrappingGap );
    vertical = ( nXtalRows * xtalWidth ) + ( (nXtalRows+1) * wrappingGap );
    thickness = wrappingGap + diffuserDepth + wrappingGap + xtalDepth + opticalCouplingDepth + photodetectorDepth;
  
}

void gm2ringsim::CalorimeterGeometry::print() {
    std::ostringstream oss;
    oss << "  nCalorimeters = " << nCalorimeters << "\n";
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
    
    oss << "  frontWrappingHole =     " << frontWrappingHole << "\n";
    oss << "  frontWrappingHoleSize = " << frontWrappingHoleSize << "\n";
    
    oss << "  photodetectorShape =   " << photodetectorShape   << "\n";
    oss << "  photodetectorDepth =   " << photodetectorDepth   << "\n";
    oss << "  opticalCouplingDepth = " << opticalCouplingDepth << "\n";

    oss << "  diffuser =      " << diffuser << "\n";
    oss << "  diffuserDepth = " << diffuserDepth << "\n";
    
    oss << "  displayCalorimeterBox =  " << displayCalorimeterBox << "\n";
    oss << "  displayWrappingVolumes = " << displayWrappingVolumes << "\n";
    oss << "  displayCrystalArray =    " << displayCrystalArray << "\n";
    oss << "  calorimeterColor:     "; for (auto entry : calorimeterColor) { oss << " " << entry; }; oss << "\n";
    oss << "  xtalColor:            "; for (auto entry : xtalColor) { oss << " " << entry; }; oss << "\n";
    oss << "  photodetectorColor:   "; for (auto entry : photodetectorColor) { oss << " " << entry; }; oss << "\n";
    oss << "  opticalCouplingColor: "; for (auto entry : opticalCouplingColor) { oss << " " << entry; }; oss << "\n";
    oss << "  diffuserColor:        "; for (auto entry : diffuserColor) { oss << " " << entry; }; oss << "\n";
    oss << "  wrappingColor:        "; for (auto entry : wrappingColor) { oss << " " << entry; }; oss << "\n";
    
    oss << "  killShowers = " << killShowers << "\n";
	
    mf::LogInfo("CALORIMETERGEOMETRY") << oss.str();
    
}
