#ifndef __gm2ringsim_CalorimeterGeometry__
#define __gm2ringsim_CalorimeterGeometry__

/** @file CalorimeterGeometry.hh
 
 @author Lawrence Gibbons
 @date 2012
 
*/

// system includes
#include <iostream>
#include <string>

// art + artG4 includes
#include "artg4/Core/GeometryBase.hh"


namespace gm2ringsim{
    
    struct CalorimeterGeometry : public artg4::GeometryBase {

      CalorimeterGeometry(std::string const & detName="calorimeter");
      void print();
      
        // Number of calorimeters (defaults to 24)
        const int nCalorimeters;
      
        // crystal array and inter-crystal gap (for "wrapping")
        const int nXtalRows;
        const int nXtalCols;
        const double wrappingGap; // "epsilon" in g2migtrace
        const double crystalCaloBuffer;

        // crystal dimensions
        const double xtalWidth;
        const double xtalHeight;
        const double xtalDepth;
        
        // Wrapping materials.  Different faces can have different wrappings
        const std::string sideWrapping;  // long faces
        const std::string frontWrapping; // upstream face
        const std::string backWrapping;  // back face (shared with photodetector)
        
        // optical coupling and photodetector
        const std::string photodetectorShape; // round or square
        const double photodetectorSize;
        const double photodetectorDepth;   // "pmtDepth" in g2migtrace
        const double opticalCouplingDepth; // "epoxyDepth" in g2migtrace
        
        // diffuser
        const double diffuserDepth;
        
        // viewing
        const bool displayCalorimeterBox;
        const bool displayWrappingVolumes;
        const bool displayCrystalArray;
        const std::vector<double> calorimeterColor;
        const std::vector<double> xtalColor;
        const std::vector<double> photodetectorColor;
        const std::vector<double> opticalCouplingColor;
        const std::vector<double> diffuserColor;
        const std::vector<double> wrappingColor;
        
        // whether or not to track particles through the calorimeter
        // killShowers = true kills all tracks that enter calo
        //      (caloHits are recorded)
        // killShowers = false allows particles to enter the calorimeter
        //       and interact with the PbF2 crystals
        const bool killShowers;
        
        // The following constants are derived

        // parameters of bounding calorimeter box.  Unlike in g2migtrace, we
        // define these parameters from crystal and array parameters
        // themselves so that they are guaranteed to just barely enclose the
        // crystals + wrappings + photodetector
        double radial;
        double vertical;
        double thickness;
        
    }; // end of CalorimeterGeometry def
} // end of gm2ringsim


#endif /* defined(__gm2ringsim_CalorimeterGeometry__) */
