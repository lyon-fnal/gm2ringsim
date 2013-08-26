#!/bin/sh

outfile="infgun.fcl"
if [ -a ${outfile} ]; then
    rm ${outfile}
fi

SPOTS=10000
let "die1 = $RANDOM % $SPOTS +1" # Roll first one.

beamstartname=""

cat >> ${outfile} <<EOF
#include "geom/world.fcl"
#include "geom/arc.fcl"
#include "geom/vac.fcl"
#include "geom/inflector.fcl"
#include "geom/quad.fcl"
#include "geom/kicker_nokick.fcl"
#include "geom/collimator.fcl"
#include "geom/PGA.fcl"
#include "geom/g2GPS.fcl"

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
      quad:   @local::quad_geom
      kicker: @local::kicker_geom
      collimator: @local::collimator_geom
      pga: @local::PGA_geom
     }

     // Global simulation settings
     RunSettings : {
       SpinTracking : {
	   spinTrackingEnabled : ${spintrackingname}
	   edmTrackingEnabled : ${edmtrackingname}
	   Q : ${charge}
	   Eta : ${edmval}
	   Gm2 : ${gm2val}
       }
      G2GPSSettings:  @local::G2GPS_downstreamInflectorMandrel
#      G2GPSSettings:  @local::G2GPS_downstreamInflectorMandrel_muminus
    
    }




      // Action(s) for the simulation
      StackingAction: {  
            name: "stackingAction"
	    OnlyTrackPrimary: false
  	    TrackPrimaryDecay: true
            TrackOrphans: false
            minWavelength:250 //nm
            maxWavelength:950  //nm
      }

    physicalVolumeStore: {}
    Gm2PhysicsList: {
	physicsListName: old
	decayMode: ${muondecay}
    }

    TrackingAction : {
          name : "trackingAction"
	  OnlyTrackPrimary: false
	  TrackPrimaryDecay: true
          TrackOrphans: false
    }

    ClockAction: {}

    InflectorPGA: {
        name: "inflectorgun"
        inflectorVerbosity: true
	SigmaT: ${sigmat}
EOF
if [ ${muondecay} == "none" ]; then
cat >> ${outfile} <<EOF
	FlatDecayTime: false
	MaxDecayTime: -1
EOF
else
cat >> ${outfile} <<EOF
	FlatDecayTime: true
#	MaxDecayTime: 20
	MaxDecayTime: ${numturns}
EOF
fi

cat >> ${outfile} <<EOF
	EmittanceX: ${beamsize}
	EmittanceY: ${beamsize}
	BetaX:  ${betaX}
	BetaY:  ${betaY}
	AlphaX:  ${alphaX}
	AlphaY:  ${alphaY}
	Pmean: ${pmean}
	dPOverP: ${dPoverP}
	Particle: "${particle}"
	DecayScaleFactor: 1
	Polarization: 100
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
    VirtualRingStation: {} // This is for the virtual tracker planes in the vac
    Arc: {}
    Quad: {}
    VacuumChamber: {}
    Inflector:{}
    Kicker: {}
    Collimator : {}

    DecayedPositronAction: {
      name: "DecayedPositronAction"
      stored_threshold: -50.0
    }

    LostMuonAction: {
      name: "LostMuonAction"
      stored_rmin: -50.0
      stored_rmax: 50.0
      stored_y: 50.0
    }

    MuonStorageStatusAction: {
      name: "MuonStorageStatusAction"
      turnsForStorage: ${numturns}
      TrackPositrons: true
      stored_rmin: 7.035
      stored_rmax: 7.215
EOF

if [ ${evts} -gt 10 ]; then
    heartbeat=`echo " ${evts} / 10" | bc`
else
    heartbeat=25
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

cat >> ${outfile} <<EOF
#
# Add-ons
#
services.user.Geometry.inflector.CryoWallMaterial: Vacuum
services.user.RunSettings.G2GPS_downstreamInflectorMandrel.particle: muminus
services.user.Geometry.quad.DoScraping: false
services.user.Geometry.quad.StoreHV: 24
services.user.Geometry.quad.ScrapeHV: 17
services.user.Geometry.quad.SupportMaterial: Macor
services.user.Geometry.quad.PlateMaterial: None
services.user.Geometry.vac.Frequency: 1
EOF
echo "kickhv=${kickhv}"
echo "kicksk=${kicksk}"

if [ ${kickhv} -ge 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.kicker.TypeOfKick: LCR
services.user.Geometry.kicker.kPlate1HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kPlate2HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kPlate3HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kickerHV : [${kickhv} , ${kickhv} , ${kickhv} ] //kilovolt
EOF
fi
if [ ${kicksk} -ge 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.kicker.TypeOfKick: SQUARE
services.user.Geometry.kicker.squareMag : [${kicksk} , ${kicksk} , ${kicksk} ] //gauss
EOF
fi

#cat >> ${outfile} <<EOF
#services.user.Geometry.kicker.TypeOfKick: SQUARE
#services.user.Geometry.kicker.squareMag : [0.0 , 0.0 , 0.0 ] //gauss
#EOF
#fi

echo "cp ${outfile} ${fullDir}/runner.fcl"
cp ${outfile} ${fullDir}/runner.fcl


outfile="readRingTrackers.fcl"
header_outfile="readRingTrackers_header.fcl"
footer_outfile="readRingTrackers_footer.fcl"
files_outfile="readRingTrackers_files.fcl"
if [ -a ${outfile} ]; then
    rm ${outfile}
fi
if [ -a ${footer_outfile} ]; then
    rm ${footer_outfile}
fi
if [ -a ${header_outfile} ]; then
    rm ${header_outfile}
fi
if [ -a ${files_outfile} ]; then
    rm ${files_outfile}
fi

files="${fullDir}/${basename}.root"
for file in ${fullDir}/*.root; do
    if [ -a ${file} ]; then
	if ! [ ${file} == ${fullDir}/${basename}.root ]; then
	    files="${files}, ${file}"
	fi
    fi
done

cat >> ${header_outfile} <<EOF
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
EOF


cat >> ${files_outfile} <<EOF
    fileNames: [ "${fullDir}/${basename}.root" ]
EOF

cat >> ${footer_outfile} <<EOF
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
      SaveInfHits: false
      SaveTruthHits: true
      SaveRingHits: false
      SaveVRingHits: true
      SaveVRing1PlaneHits: true
      debug: false
    }
  }
  path1: [ readRingTrackers ]
  end_paths: [ path1 ]
}
EOF

echo "cp ${footer_outfile} ${fullDir}/footer_reader.fcl"
cp ${footer_outfile} ${fullDir}/footer_reader.fcl
echo "cp ${header_outfile} ${fullDir}/header_reader.fcl"
cp ${header_outfile} ${fullDir}/header_reader.fcl
echo "cp ${files_outfile} ${fullDir}/files_reader.fcl"
cp ${files_outfile} ${fullDir}/files_reader.fcl