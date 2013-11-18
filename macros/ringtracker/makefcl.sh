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
EOF

usecalo=1
if [ ${beamtransport} == 1 ]; then
    usecalo=1
fi

if [ ${usecalo} == 1 ]; then
cat >> ${outfile} <<EOF
#include "geom/station.fcl"
#include "geom/calorimeter.fcl"
EOF
fi

cat >> ${outfile} <<EOF

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
//         filename : "${fullDir}/${basename}.log"
         filename : "dummy.log"
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
EOF

if [ ${usecalo} == 1 ]; then
cat >> ${outfile} <<EOF      
      station: @local::station_geom
      calorimeter: @local::calorimeter_geom
EOF
fi
cat >> ${outfile} <<EOF      
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
      G2GPSSettings:  @local::G2GPS_downstreamInflectorMandrel_muplus
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

EOF

if [ ${gengauss} == 1 ] || [ ${gengauss} == 0 ]; then
cat >> ${outfile} <<EOF
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
decaytime="false"
if [ ${flatdecay} == 1 ]; then
    decaytime="true"
fi
cat >> ${outfile} <<EOF
	FlatDecayTime: ${decaytime}
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
	Polarization: ${polarization}
        TurnCounter: 11
	RotAngle: 0.54
EOF


###	RotAngle: 0.52 ### little gap
###	RotAngle: 0.53 ### no events
###      RotAngle: 0.525 ### no events
###	RotAngle: 0.5125 ### smaller gap
###        RotAngle: 0.515 ### more gap?
###	RotAngle: 0.5225 ### no events
###	RotAngle: 0.5175 ### more gap?
###	RotAngle: 0.51 ### same?
###	RotAngle: 0.5 ### definitely more gap
###	RotAngle: 0.512 ###
###	RotAngle: 0.51 ### for BS2
###	RotAngle: 0.52 ### for BS1
###	RotAngle: 0.515 ### for BS3
###	RotAngle: 0.53 ### for BS4
###	RotAngle: 0.55 ### for BS5
###	RotAngle: 0.54 // for BS6

if [ ${beamstart} == um ]; then
    beamstartname="UM"
cat >> ${outfile} <<EOF
	StartUpstream:  true
EOF
else
cat >> ${outfile} <<EOF
	StartUpstream:  false
EOF
fi

if [ ${beamstart} == uc ]; then
    beamstartname="UC"
cat >> ${outfile} <<EOF
	StartUpstreamCryo:  true
EOF
else
cat >> ${outfile} <<EOF
	StartUpstreamCryo:  false
EOF
fi

if [ ${beamstart} == dm ]; then
    beamstartname="DM"
cat >> ${outfile} <<EOF
	StartDownstream:  true
EOF
else
cat >> ${outfile} <<EOF
	StartDownstream:  false
EOF
fi

if [ ${beamstart} == co ]; then
    beamstartname="CO"
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
EOF
fi

#
# Run muon gas gun
#
echo "MuonGas = ${muongas}"
if [ ${muongas} == 1 ]; then
    maxdecaytime=`echo " ${numturns} * 0.15" | bc`
    decaytime="false"
    if [ ${flatdecay} == 1 ]; then
	decaytime="true"
    fi
    
    useconstbeta=false
    if [ ${betaname} == "ConstBeta" ]; then
	useconstbeta="true"
    elif [ ${betaname} == "RingBeta" ]; then
	useconstbeta="false"
    fi
    
    usedispersion=false
    if [ ${dispersionname} == "Dispersion" ]; then
	usedispersion="true"
    fi
cat >> ${outfile} <<EOF

    MuonGasPGA: {
        name: "muongas"
        muonGasVerbosity: false
	FlatDecayTime: ${decaytime}
	MaxDecayTime: ${maxdecaytime}
	EmittanceX: ${beamsize}
	EmittanceY: ${beamsize}
	BetaX:  ${betaX}
	BetaY:  ${betaY}
	AlphaX:  ${alphaX}
	AlphaY:  ${alphaY}
	UseConstBeta:  ${useconstbeta}
	Particle: "${particle}"
	Polarization: ${polarization}
        Debug: false
        NoTransMom: false
	PosOffset: false
EOF
if [ ${beamstart} == co ]; then
    beamstartname="CO"
cat >> ${outfile} <<EOF
	StartPerfect:  true
	Kick: 0
	RotAngle: 0.54
EOF
else
cat >> ${outfile} <<EOF
	StartPerfect:  false
	Kick: ${kicksk}
	RotAngle: 0.0
EOF
fi
cat >> ${outfile} <<EOF
    }

EOF
fi

#
# Run muon gas gun
#
echo "BeamTransport = ${beamtransport}"
if [ ${beamtransport} == 1 ]; then
    maxdecaytime=`echo " ${numturns} * 0.15" | bc`
    decaytime="false"
    if [ ${flatdecay} == 1 ]; then
	decaytime="true"
    fi
    
    useconstbeta=false
    if [ ${betaname} == "ConstBeta" ]; then
	useconstbeta="true"
    elif [ ${betaname} == "RingBeta" ]; then
	useconstbeta="false"
    fi
    
    usedispersion=false
    if [ ${dispersionname} == "Dispersion" ]; then
	usedispersion="true"
    fi
cat >> ${outfile} <<EOF

    BeamTransportPGA: {
        name: "beamtransport"
	FlatDecayTime: ${decaytime}
	MaxDecayTime: ${maxdecaytime}
	EmittanceX: ${beamsize}
	EmittanceY: ${beamsize}
	BetaX:  ${betaX}
	BetaY:  ${betaY}
	AlphaX:  ${alphaX}
	AlphaY:  ${alphaY}
	UseConstBeta:  ${useconstbeta}
	UseDispersion: ${usedispersion}
	UseFNALt0: true
	Particle: "${particle}"
	Polarization: ${polarization}
        Debug: false
        NoTransMom: false
	PosOffset: false
EOF
if [ ${beamstart} == co ]; then
    beamstartname="CO"
cat >> ${outfile} <<EOF
	StartPerfect:  true
	Kick: 0
	RotAngle: 0.0
//	RotAngle: 0.54
EOF
else
cat >> ${outfile} <<EOF
	StartPerfect:  false
	Kick: ${kicksk}
	RotAngle: 0.0
EOF
fi
cat >> ${outfile} <<EOF
    }

EOF
fi

cat >> ${outfile} <<EOF

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
EOF

if [ ${usecalo} == 1 ]; then
cat >> ${outfile} <<EOF
    Station: {}
    Calorimeter: {}
EOF
fi

if [ ${muongas} == 0 ] || [ ${beamtransport} == 0 ]; then
cat >> ${outfile} <<EOF

    LostMuonAction: {
      name: "LostMuonAction"
EOF
if [ ${beamstart} == co ]; then
cat >> ${outfile} <<EOF
      stored_rmin: -75.0
      stored_rmax: 75.0
      stored_y: 75
EOF
else
cat >> ${outfile} <<EOF
    stored_rmin: -300.0
    stored_rmax: 300.0
    stored_y: 50
EOF
fi

cat >> ${outfile} <<EOF
    }
EOF
fi

if [ ${numturns} -le 10 ]; then
    mustore=`echo " ${numturns} + 1" | bc`
elif [ ${numturns} -le 200 ]; then
    mustore=`echo " ${numturns} + 5" | bc`
else
    mustore=`echo " ${numturns} + 15" | bc`
fi

cat >> ${outfile} <<EOF

    MuonStorageStatusAction: {
      name: "MuonStorageStatusAction"
      turnsForStorage: ${mustore}
      TrackPositrons: true
      stored_rmin: 0.0
      stored_rmax: 10.0
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
services.user.RunSettings.G2GPS_downstreamInflectorMandrel.particle: muplus
EOF
if [ ${scraping} == "ON" ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.quad.DoScraping: true
EOF
else
cat >> ${outfile} <<EOF
services.user.Geometry.quad.DoScraping: false
EOF
fi

cat >> ${outfile} <<EOF
services.user.Geometry.quad.StoreHV: 24
services.user.Geometry.quad.ScrapeHV: 17
EOF

if [ ${quad} == "NewSupports" ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.quad.SupportMaterial: Macor
services.user.Geometry.quad.PlateMaterial: Al
EOF
elif [ ${quad} == "NoQuads" ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.quad.SupportMaterial: None
services.user.Geometry.quad.PlateMaterial: None
EOF
elif [ ${quad} == "NoQuadSupports" ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.quad.SupportMaterial: None
services.user.Geometry.quad.PlateMaterial: Al
EOF
else
cat >> ${outfile} <<EOF
services.user.Geometry.quad.SupportMaterial: None
services.user.Geometry.quad.PlateMaterial: None
EOF
fi


#
# Open-Open
#
if [ ${inftype} == 2 ]; then
cat >> ${outfile} <<EOF
# Both Open
services.user.Geometry.inflector.useUpstreamWindow:       false
services.user.Geometry.inflector.useDownstreamWindow :    false
services.user.Geometry.inflector.useUpstreamConductor :   false
services.user.Geometry.inflector.useDownstreamConductor : false
services.user.Geometry.inflector.useUpstreamEndFlange :   false
services.user.Geometry.inflector.useDownstreamEndFlange : false
EOF
elif [ ${inftype} == 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.useUpstreamWindow:       true
services.user.Geometry.inflector.useDownstreamWindow :    true
services.user.Geometry.inflector.useUpstreamConductor :   true
services.user.Geometry.inflector.useDownstreamConductor : true
services.user.Geometry.inflector.useUpstreamEndFlange :   true
services.user.Geometry.inflector.useDownstreamEndFlange : true
EOF
elif [ ${inftype} == 1 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.useUpstreamWindow:       true
services.user.Geometry.inflector.useDownstreamWindow :    true
services.user.Geometry.inflector.useUpstreamConductor :   false
services.user.Geometry.inflector.useDownstreamConductor : false
services.user.Geometry.inflector.useUpstreamEndFlange :   false
services.user.Geometry.inflector.useDownstreamEndFlange : false
EOF
else
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.useUpstreamWindow:       false
services.user.Geometry.inflector.useDownstreamWindow :    false
services.user.Geometry.inflector.useUpstreamConductor :   false
services.user.Geometry.inflector.useDownstreamConductor : false
services.user.Geometry.inflector.useUpstreamEndFlange :   false
services.user.Geometry.inflector.useDownstreamEndFlange : false
EOF
fi


#
# Inflector field
#
#swingAngle: 2.4 // mrad
#tiltAngle:  0.0 // mrad
#rotAngle :  5.587 //degree


if [ ${field} == 1 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.fieldType : MAPPED
EOF
elif [ ${field} == 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.fieldType : VANISH
EOF
fi

#
# Inflector Angles
#
cat >> ${outfile} <<EOF
services.user.Geometry.inflector.swingAngle : ${delta}
EOF



if [ ${muongas} == 1 ] || [ ${beamtransport} == 1 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.vac.Frequency: 1
EOF
else
    if [ ${beamstart} == co ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.vac.Frequency: 1
EOF
    else
cat >> ${outfile} <<EOF
services.user.Geometry.vac.Frequency: 2
EOF
    fi
fi


echo "//kickhv=${kickhv}" >> ${outfile}
echo "//kicksk=${kicksk}" >> ${outfile}


if [ ${kickhv} -gt 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.kicker.TypeOfKick: LCR
services.user.Geometry.kicker.kPlate1HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kPlate2HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kPlate3HV : ${kickhv} //kilovolt 
services.user.Geometry.kicker.kickerHV : [${kickhv} , ${kickhv} , ${kickhv} ] //kilovolt
EOF
elif [ ${kicksk} -gt 0 ]; then
cat >> ${outfile} <<EOF
services.user.Geometry.kicker.TypeOfKick: SQUARE
services.user.Geometry.kicker.squareMag : [${kicksk}.0 , ${kicksk}.0 , ${kicksk}.0 ] //gauss
EOF
else
cat >> ${outfile} <<EOF
services.user.Geometry.kicker.TypeOfKick: SQUARE
services.user.Geometry.kicker.squareMag : [0.0 , 0.0 , 0.0 ] //gauss
EOF
fi

#if [ ${gengauss} == -1 ]; then
#    outfile="muongas_gun.fcl"
#fi

#echo "more ${outfile}"
#sleep 1000000000

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
      maxdecaytime: 149.15
      LaunchAngle: ${launch}
      InflectorAngle: ${delta}
      StorageOffset: ${offset}
      TurnCounter: 11
      SaveInfHits: false
      SaveTruthHits: true
      SaveRingHits: false
      SaveCaloHits: true
      SaveVRingHits: true
      SaveVRing1PlaneHits: true
      debug: false
      fill: true
      SavePhaseSpace: false
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