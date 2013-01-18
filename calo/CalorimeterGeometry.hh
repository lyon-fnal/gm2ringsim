//
//  CalorimeterGeometry.h
//  gm2ringsim
//
//  Created by Lawrence Gibbons on 12/15/12.
//  Copyright (c) 2012 Cornell University. All rights reserved.
//

// Include guards
#ifndef __gm2ringsim_CalorimeterGeometry__
#define __gm2ringsim_CalorimeterGeometry__

// system includes
#include <iostream>
#include <string>

// art + artG4 includes
#include "artg4/Core/GeometryBase.hh"


namespace gm2ringsim{
    
    struct CalorimeterGeometry : public artg4::GeometryBase {
        
        CalorimeterGeometry(std::string const &);
        void print();
        
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
        
        // viewing
        const bool displayCalorimeterBox;
        const bool displayWrappingVolumes;
        const bool displayCrystalArray;
        const std::vector<double> calorimeterColor;
        const std::vector<double> xtalColor;
        const std::vector<double> photodetectorColor;
        const std::vector<double> opticalCouplingColor;
        const std::vector<double> wrappingColor;
        
        // The following constants are derived

        // parameters of bounding calorimeter box.  Unlike in g2migtrace, we
        // define these parameters from crystal and array parameters
        // themselves so that they are guaranteed tojust barely enclose the
        // crystals + wrappings + photodetector
        double radial;
        double vertical;
        double thickness;
        
    }; // end of CalorimeterGeometry def
} // end of gm2ringsim


#endif /* defined(__gm2ringsim_CalorimeterGeometry__) */
