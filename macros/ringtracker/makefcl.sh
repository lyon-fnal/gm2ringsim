#!/bin/sh

outfile="infgun.fcl"
if [ -a ${outfile} ]; then
    rm ${outfile}
fi

beamstartname=""

cat >> ${outfile} <<EOF
#include "geom/world.fcl"
#include "geom/arc.fcl"
#include "geom/vac.fcl"
#include "geom/inflector.fcl"
#include "geom/quad.fcl"
#include "geom/oct.fcl"
#include "geom/kicker_nokick.fcl"
#include "geom/fiberHarp.fcl"
#include "geom/station.fcl"
#include "geom/calorimeter.fcl"
#include "geom/collimator.fcl"
#include "geom/PGA.fcl"
#include "geom/g2GPS.fcl"
#include "geom/strawtracker.fcl"

process_name:myProcessName

source: {
  module_type: EmptyEvent
  maxEvents:  ${evts}
}

services: {

  message : {
     debugModules : ["*"]
     suppressInfo : []
     
     destinations : {
       LogToConsole : {
         type : "cout"
         threshold : "ERROR"
       }
       LogToFile : {
         type : "file"
         filename : "${fullDir}/${basename}.log"
         append : false
         threshold : "ERROR"
       }
     }
  }

  user : {
  
    // Mandatory ArtG4 services
    DetectorHolder: {}
    ActionHolder: {}
    PhysicsListHolder: {}
    RandomNumberGenerator: {}

    // Geometry
    Geometry: {
      world: @local::world_geom
      arc:   @local::arc_geom
      vac:   @local::vac_geom
      inflector: @local::inflector_geom
      quad: @local::quad_geom
      octupole: @local::oct_geom
      kicker: @local::kicker_geom
      fiberHarp: @local::fiberHarp_geom
      station: @local::station_geom
      calorimeter: @local::calorimeter_geom
      collimator: @local::collimator_geom
      pga: @local::PGA_geom
      strawtracker: @local::strawtracker_geom
     }

     // Global simulation settings
     RunSettings : {
       SpinTracking : {
        spinTrackingEnabled : true
       }
      G2GPSSettings:  @local::G2GPS_downstreamInflectorMandrel
    
    }




      // Action(s) for the simulation
      StackingAction: {  
            name: "stackingAction"
	    OnlyTrackMuons: true
            minWavelength:250 //nm
            maxWavelength:950  //nm
      }

    physicalVolumeStore: {}
    Gm2PhysicsList: {}

    TrackingAction : {
          name : "trackingAction"
	  OnlyTrackMuons: true
    }

    ClockAction: {}

//    G2PGA: {
//	      name: "primary"
//    }

//    MuonGasPGA: {
//        name: "muongas"
//        muonGasVerbosity: false
//    }

    InflectorPGA: {
        name: "inflectorgun"
        inflectorVerbosity: true
	SigmaT: 25
	Emittance: ${beamsize}
	BetaX:  ${betaX}
	BetaY:  ${betaY}
	AlphaX:  ${alphaX}
	AlphaY:  ${alphaY}
	Pmean: ${pmean}
	dPOverP: ${dPoverP}
EOF

if [ ${beamstart} == um ]; then
    beamstartname="UpstreamMandrel"
cat >> ${outfile} <<EOF
	StartUpstream:  true
EOF
else
cat >> ${outfile} <<EOF
	StartUpstream:  false
EOF
fi

if [ ${beamstart} == uc ]; then
    beamstartname="UpstreamCryo"
cat >> ${outfile} <<EOF
	StartUpstreamCryo:  true
EOF
else
cat >> ${outfile} <<EOF
	StartUpstreamCryo:  false
EOF
fi

if [ ${beamstart} == dm ]; then
    beamstartname="DownstreamMandrel"
cat >> ${outfile} <<EOF
	StartDownstream:  true
EOF
else
cat >> ${outfile} <<EOF
	StartDownstream:  false
EOF
fi

if [ ${beamstart} == co ]; then
    beamstartname="CentralOrbit"
cat >> ${outfile} <<EOF
	StartPerfect:  true
EOF
else
cat >> ${outfile} <<EOF
	StartPerfect:  false
EOF
fi
    
if [ ${gengauss} == 0 ]; then
cat >> ${outfile} <<EOF
	GenGaussian: false
EOF
else
cat >> ${outfile} <<EOF
	GenGaussian: true
EOF
fi

cat >> ${outfile} <<EOF
	LaunchAngle: ${launch}
	StorageOffset: ${offset}
    }


    // Detectors
    World: {}	
    Ring: {}    // This is for the RingHits
    Tracker: {} // This is for the virtual tracker planes in the vac
    Arc: {}
    VacuumChamber: {}
    Inflector:{}
    Quad: {}
    Octupole: {}
    Kicker: {}
    FiberHarp: {}
    Station: {}
    Calorimeter: {}
    Collimator : {}
    StrawTracker: {}


    MuonStorageStatusAction: {
      name: "MuonStorageStatusAction"
      turnsForStorage: ${numturns}
EOF

if [ ${evts} -gt 10 ]; then
    heartbeat=`echo " ${evts} / 10" | bc`
else
    heartbeat=1
fi

cat >> ${outfile} <<EOF
      heartbeatFreq: ${heartbeat}
    }

  } //user:
} //services:

outputs: {
out1: {
  module_type: RootOutput   
  fileName: "${fullDir}/${basename}.root"
  }
}

physics: {
  producers: {
    artg4 : { 
     module_type: artg4Main
     enableVisualization: false
     macroPath: ".:./macros:../macros:../../macros"
     visMacro: "vis.mac"
     afterEvent: pause  // (ui, pause, pass)
     }
  }



  path1: [ artg4 ]
  readPath: [ ]
  stream1:  [ out1 ]

  trigger_paths: [ path1 ]
  end_paths: [ readPath, stream1 ]

}
EOF

echo "cp ${outfile} ${fullDir}/runner.fcl"
cp ${outfile} ${fullDir}/runner.fcl


outfile="readRingTrackers.fcl"
if [ -a ${outfile} ]; then
    rm ${outfile}
fi

cat >> ${outfile} <<EOF
services: {

  TFileService: {
    fileName: "rootfiles/gm2ringsim_${basename}.root"
  }

  message : {
     debugModules : ["*"]
     suppressInfo : []
     
     destinations : {
       LogToConsole : {
         type : "cout"
         threshold : "DEBUG"
         
         categories : {
           default : { limit : 50 }
         }
       }
     }
  }
  
  user : {
    physicalVolumeStore : {}
    ActionHolder : {}
  }
}

process_name: readRingTrackers

source: {
   module_type: RootInput
   maxEvents:  -1
   fileNames: [ "${fullDir}/${basename}.root" ]
}

physics: {
  analyzers: {
    readRingTrackers: {
      module_type: ringTrackerAnalyzer
      hist_dir: ""
      tree_dir: ""
      beamstart: "${beamstartname}"
      basename: "${basename}"
      maxturns: ${numturns}
      LaunchAngle: ${launch}
      InflectorAngle: ${delta}
      StorageOffset: ${offset}
    }
  }
  path1: [ readRingTrackers ]
  end_paths: [ path1 ]
}
EOF

echo "cp ${outfile} ${fullDir}/reader.fcl"
cp ${outfile} ${fullDir}/reader.fcl