#!/bin/sh

FindFiles()
{
    test1=${1}
    test2=${2}
    test3=${3}

    FILES=""
    let numf=0
    curdir=`pwd`
    cd ${outputdir}

    comp=""
    if [ ${test1} == "x" ]; then
	if [ ${test3} == "-" ]; then
#	    echo "11"
	    for file in *${test2}; do
		if [ -a ${file} ]; then
		    ((numf++))
		    FILES="${file} ${FILES}"
		fi
	    done
	elif [ ${test3} == "x" ]; then
	    echo "12"
	    for file in *${test2}*; do
		if [ -a ${file} ]; then
		    ((numf++))
		    FILES="${file} ${FILES}"
		fi
	    done
	else
#	    echo "13"
	    for file in *${test2}*${test3}; do
		if [ -a ${file} ]; then
		    ((numf++))
		    FILES="${file} ${FILES}"
		fi
	    done
	fi
    else
	if [ ${test3} == "-" ]; then
#	    echo "21"
	    for file in ${test1}*${test2}; do
		if [ -a ${file} ]; then
		    ((numf++))
	    FILES="${file} ${FILES}"
		fi
	    done
	elif [ ${test3} == "x" ]; then

	    if [ ${test2} == "x" ]; then
		for file in ${test1}*; do
		    if [ -a ${file} ]; then
			((numf++))
			FILES="${file} ${FILES}"
		    fi
		done
	    else
#	    echo "22"
		for file in ${test1}*${test2}*; do
		    if [ -a ${file} ]; then
			((numf++))
			FILES="${file} ${FILES}"
		    fi
		done
	    fi
	else
	    for file in ${test1}*${test2}*${test3}; do
		if [ -a ${file} ]; then
		    ((numf++))
		    FILES="${file} ${FILES}"
		fi
	    done
	fi
    fi	

#    echo ${test1} ${test2} ${test3}
#    echo "FILES=${FILES}"
    cd ${curdir}
}

WriteFiles()
{
    timestamp=${1}
    timestamp2=${2}
    exclude=""
    include=""
    if ! [ -z ${timestamp2} ]; then
	if [ ${timestamp2} == "excl" ] || [ ${timestamp2} == "Excl" ]; then
	    exclude=${3}
	fi
	if [ ${timestamp2} == "incl" ] || [ ${timestamp2} == "Incl" ]; then
	    include=${3}
	fi
    fi
    if ! [ -z ${3} ]; then
	if [ ${3} == "excl" ] || [ ${3} == "Excl" ]; then
	    exclude=${4}
	fi
	if [ ${3} == "incl" ] || [ ${3} == "Incl" ]; then
	    include=${4}
	fi
    fi
    
#    echo "  ${include} - ${exclude} , ${FILES}"

    let nf=0
    for file in ${FILES}; do
	
	if ! [ -z ${exclude} ]; then
	    exclcheck=${file/${exclude}/}
#	    echo "    ${exclcheck} , ${file}"
	    if ! [ ${exclcheck} == ${file} ]; then
		continue;
	    fi
	fi

	if ! [ -z ${include} ]; then
	    inclcheck=${file/${include}/}
	    if [ ${exclcheck} == ${file} ]; then
		continue;
	    fi
	fi

	#echo ${nf} `expr ${nf} % 5`
	if [ ${nf} == 0 ]; then
	    BeginRow ${timestamp} ${timestamp2}
	elif [ `expr ${nf} % 5` == 0 ]; then
	    EndRow ${timestamp}
	    BeginRow ${timestamp} ${timestamp2}
	fi

	caption=${file/.png/}
	caption=${caption/"NumAllStationsNoFastRotation"/"Observed Count (w/o FR) -- "}
	caption=${caption/"NumAllStations"/"Observed Count -- "}
	caption=${caption/"Tracker_"/}
	caption=${caption/"G4Track_"/}
	caption=${caption/"_x_FFT"/" -- FFT"}
	caption=${caption/"_DecayElectronQuadSection"/}
	caption=${caption/"_DecayElectronKickerSection"/}
	caption=${caption/"_DecayElectronEmptySection"/}
	caption=${caption/"_DecayElectron"/}
	caption=${caption/"_GoodStrawElectron"/}
	caption=${caption/"_GoodStrawCaloElectron"/}
	caption=${caption/"_GoodOneStrawCaloElectron"/}
	caption=${caption/"_GoodTwoStrawCaloElectron"/}
	caption=${caption/"_StrawCaloElectron"/}
	caption=${caption/"_StrawElectron"/}
	caption=${caption/"_DecayMuon"/}
	caption=${caption/"EgtEth"/}	
	caption=${caption/"VirtualRing"/}
	caption=${caption/"_Middle_"/" [ t < 5&mu;s ] "}
	caption=${caption/"_Early_"/" [ t < 1&mu;s ] "}
	caption=${caption/"_Late_"/" [ t < 20&mu;s ] "}
	caption=${caption/"_All_"/" [ t < 1000&mu;s ] "}
	caption=${caption/"with1Tracker"/" -- w/ 1 tracker"}
	caption=${caption/"with2Trackers"/" -- w/ 2 trackers"}
	caption=${caption/"_PhaseSpace"/}
	caption=${caption/"_Profile"/}
	caption=${caption/"_vs_TimeSingleHit_"/" - [11] - "}
	caption=${caption/"_vs_TimeSingleHit"/" - [11] - "}
	caption=${caption/"_vs_TimeOncePerTurn_"/" - [all] - "}
	caption=${caption/"_vs_TimeOncePerTurn"/" - [all] - "}
	caption=${caption/"_vs_TimeTwicePerTurn"/" - [8,11] -"}
	caption=${caption/"_Width"/" -- Width"}
	caption=${caption/"_Mean"/" -- Mean"}
	caption=${caption/"Rhat_"/"Radial Position -- "}
	caption=${caption/"Y_"/"Vertical Position -- "}
	caption=${caption/"1_3us"/" [ 1 < t < 3&mu;s ] "}
	caption=${caption/"1_4us"/" [ 1 < t < 4&mu;s ] "}
	caption=${caption/"1_5us"/" [ 1 < t < 5&mu;s ] "}
	caption=${caption/"1_20us"/" [ 1 < t < 20&mu;s ] "}
	caption=${caption/"0_20us"/" [ t < 20&mu;s ] "}
	caption=${caption/"150_600us"/" [ 150 < t < 600&mu;s ] "}
	caption=${caption/"1_50us"/" [ 1 < t < 50&mu;s ] "}
	caption=${caption/"1_500us"/" [ 1 < t < 500&mu;s ] "}
	caption=${caption/"340_360us"/" [ 340 < t < 360&mu;s ] "}
	caption=${caption/"50_600us"/" [ 50 < t < 600&mu;s ] "}
	caption=${caption/"50_750us"/" [ 50 < t < 750&mu;s ] "}
	caption=${caption/"0_90us"/" [ t < 90&mu;s ] "}

	write ${outputdir}/index.html ${file} ${file/.png/} ${caption}
	((nf++))
    done
    
    if ! [ `expr ${nf} % 5` == 0 ]; then
	EndRow ${timestamp}
    fi
}

NiceTimestampName()
{
    if [ ${1} == "AllTimes" ]; then
	nicetimestamp="t > 0"
    elif [ ${1} == "SubTimes" ]; then
	nicetimestamp="Specific Times"
    elif [ ${1} == "AllTimesFFTs" ]; then
	nicetimestamp="t > 0 ; FFTs"
    elif [ ${1} == "SubTimesFFTs" ]; then
	nicetimestamp="Specific Times ; FFTs"
    elif [ ${1} == "Init_Init" ]; then
	nicetimestamp="Initial Phase Space"
    elif [ ${1} == "FinalAvg_Init" ]; then
	nicetimestamp="Time Averged Phase Space for Last 30 Turns"
    elif [ ${1} == "Turn" ]; then
	nicetimestamp="12 Hits/Turn"
    elif [ ${1} == "Time" ]; then
	nicetimestamp="Hits/Time"
    elif [ ${1} == "Turn_25Turns" ]; then
	nicetimestamp="12 Hits/Turn for 10 Turns"
    elif [ ${1} == "OncePerTurn" ]; then
	nicetimestamp="1 Hit/Turn"
    elif [ ${1} == "GeneratedDist" ]; then
	nicetimestamp="Generated<br>Distributions"
    elif [ ${1} == "RemainingDist" ]; then
	nicetimestamp="Remaining<br>Distributions"
    else
	nicetimestamp=${1}
    fi
}

EndBeginRow()
{
    EndRow
    BeginRow ${1}
}

BeginRow()
{
cat >> ${outputdir}/index.html <<EOF
      <tr>
EOF

if ! [ -z "${1}" ]; then
    NiceTimestampName ${1}
    if [ -z ${2} ]; then
cat >> ${outputdir}/index.html <<EOF
        <td><b><i><center>${nicetimestamp}</center></i></b></td>
EOF
    else
	nicetimestamp1=${nicetimestamp}
	NiceTimestampName ${2}
cat >> ${outputdir}/index.html <<EOF
        <td><b><i><center>${nicetimestamp1}<br>${nicetimestamp}</center></i></b></td>
EOF
    fi
fi
    
}

EndRow()
{
cat >> ${outputdir}/index.html <<EOF
      </tr>
EOF
}

BeginTable()
{
cat >> ${outputdir}/index.html <<EOF
    <br>
    <table cellspacing=1 cellpadding=3>
EOF
}

EndTable()
{
cat >> ${outputdir}/index.html <<EOF
    </table>
    <br>
    <hr>
    <br>
EOF
}

write()
{
    outfile=${1}
    filename=${2}
    myhistogram=${3}    
    if [ -z ${4} ]; then
	caption=${myhistogram}
    fi
    
    if [ -a ${outputdir}/${filename} ]; then
	epsname=${filename/png/eps}
	linkname="${filename}"
	if [ -a ${outputdir}/${epsname} ]; then
	    linkname=${epsname}
	fi
cat >> ${outfile} <<EOF
	  <td><center><a href="${linkname}"><figure><img alt="" width="100%" src="${filename}"><figcaption>${caption}</figcaption></figure></a></td>
EOF
    else
#	echo "[${outputdir}/${filename}] not found."
	echo "	  <td>Missing<br>${filename}</td>" >> ${outfile}
    fi
}

makeindex()
{
    thetype=${1}
    
    if [ -a ${outputdir}/index.html ]; then
	rm ${outputdir}/index.html
    fi
    
cat >> ${outputdir}/index.html <<EOF
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>g-2</title>
    <link rel="stylesheet" type="text/css" href="http://www-d0.fnal.gov/Run2Physics/top/public/fall06/singletop/style.css">
  </head>

  <body>
EOF









#############################
#
#
# Phase Space in Time
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="PhaseSpacewithTime"></a>
    <h2><center>Phase Space w/ Time</center></h2>
EOF

BeginTable
timestamps="TimeOncePerTurn" # TimeTwicePerTurn"
hists="Rhat Y2"
if [ ${plotmuongas} == 1 ]; then
    timestamps=""
fi

histograms="XprimeX YprimeY"
BeginTable  
for histogram in ${histograms}; do
    BeginTable  
    FindFiles VirtualRingTracker_${histogram} PhaseSpace 1Tracker.png
    WriteFiles ${timestamp}
    FindFiles VirtualRingTracker_${histogram} PhaseSpace 2Trackers.png
    WriteFiles ${timestamp}
done


for timestamp in ${timestamps}; do
BeginTable
    for hist in ${hists}; do
	FindFiles x ${hist}_vs_${timestamp} Full_Profile.png
	WriteFiles ${timestamp}
	FindFiles x ${hist}_vs_${timestamp} Full_Profile_x_FFT.png
	WriteFiles ${timestamp}
	FindFiles x ${hist}_vs_${timestamp} Mean_Profile.png
	WriteFiles ${timestamp}
	FindFiles x ${hist}_vs_${timestamp} Mean_Profile_x_FFT.png
	WriteFiles ${timestamp}
	FindFiles x ${hist}_vs_${timestamp} Width_Profile.png
	WriteFiles ${timestamp}
	FindFiles x ${hist}_vs_${timestamp} Width_Profile_x_FFT.png
	WriteFiles ${timestamp}
    done
EndTable
done




#############################
#
#
# BeamScan
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="BeamScan"></a>
    <h2><center>Beam Scan</center></h2>
EOF

BeginTable

cat >> ${outputdir}/index.html <<EOF
  <tr>
    <td>
    <td><center>BeamScan<br>Rhat</center></td>
    <td><center>MuonDecay<br>Rhat</center></td>
    <td><center>BeamScan<br>Y</center></td>
    <td><center>MuonDecay<br>Y</center></td>
    <td><center>MuonDecay<br>Y</center></td>
  </tr>
EOF

histograms="Rhat"
FILES=""
for histogram in ${histograms}; do
    FILES="${FILES} BeamScan_${histogram}.png"
    FILES="${FILES} BeamScan_${histogram}_LowMom.png"
    FILES="${FILES} BeamScan_${histogram}_HighMom.png"
    FILES="${FILES} BeamScan_${histogram}_MagicMom.png"
    FILES="${FILES} MuonDecay_${histogram}.png"
done
WriteFiles "BeamScan<br>MuonDecay"

FILES=""
for histogram in ${histograms}; do
    FILES="${FILES} BeamScan_${histogram}_Profile.png"
    FILES="${FILES} BeamScan_${histogram}_Profile_LowMom.png"
    FILES="${FILES} BeamScan_${histogram}_Profile_HighMom.png"
    FILES="${FILES} BeamScan_${histogram}_Profile_MagicMom.png"
    FILES="${FILES} MuonDecay_${histogram}_Profile.png"
done
WriteFiles "BeamScan<br>MuonDecay Profile"

zooms="0 1 2 3"
for zoom in ${zooms}; do
    FILES=""
    for histogram in ${histograms}; do
	FILES="${FILES} BeamScan_${histogram}_Profile_Zoom${zoom}.png"
	FILES="${FILES} BeamScan_${histogram}_Profile_Zoom${zoom}_LowMom.png"
	FILES="${FILES} BeamScan_${histogram}_Profile_Zoom${zoom}_HighMom.png"
	FILES="${FILES} BeamScan_${histogram}_Profile_Zoom${zoom}_MagicMom.png"
	FILES="${FILES} MuonDecay_${histogram}_Profile_Zoom${zoom}.png"
    done
    WriteFiles "BeamScan<br>MuonDecay Zoomed Profile"
done

FILES=""
for histogram in ${histograms}; do
    FILES="${FILES} BeamScan_${histogram}_FFT_FFT.png"
    FILES="${FILES} BeamScan_${histogram}_FFT_LowMom_FFT.png"
    FILES="${FILES} BeamScan_${histogram}_FFT_HighMom_FFT.png"
    FILES="${FILES} BeamScan_${histogram}_FFT_MagicMom_FFT.png"
    FILES="${FILES} MuonDecay_${histogram}_FFT_FFT.png"
done
WriteFiles "BeamScan<br>MuonDecay FFT"
EndTable




#############################
#
#
# CBO
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="CBO"></a>
    <h2><center>CBO</center></h2>
EOF

BeginTable
FindFiles CBO DecayMuon.png -
WriteFiles "CBO<br>MuonDecay"
EndTable

BeginTable
FindFiles CBO BeamScan.png -
WriteFiles "CBO<br>BeamScan"
EndTable

BeginTable
FindFiles CBO BeamScan_LowMom.png -
WriteFiles "CBO<br>BeamScan<br>LowMom"
EndTable

BeginTable
FindFiles CBO BeamScan_HighMom.png -
WriteFiles "CBO<br>BeamScan<br>HighMom"
EndTable

BeginTable
FindFiles CBO BeamScan_MagicMom.png -
WriteFiles "CBO<br>BeamScan<br>MagicMom"
EndTable

BeginTable
FindFiles G4Track_XprimeX_DecayMuon Dist x
WriteFiles "Rhat for CBO"
EndTable

BeginTable
FindFiles G4Track_Rhat_DecayMuon TimeSingleHit x
WriteFiles "Rhat for CBO"
EndTable


#############################
#
#
# Corrections
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="PhaseSpacewithTime"></a>
    <h2><center>Corrections</center></h2>
EOF

timestamps="TimeOncePerTurn TimeSingleHit" # TimeTwicePerTurn"
hists="Rhat Y"

for timestamp in ${timestamps}; do
BeginTable
    for hist in ${hists}; do
	FindFiles G4Track_${hist} vs_${timestamp} MeanSquared.png
	WriteFiles ${timestamp}
    done
EndTable
done





#############################
#
#
# Ring Trackers (Just Muons)
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="RingTracker"></a>
    <h2><center>Ring Tracker Stats For Specific Turns</center></h2>
EOF



BeginTable
histograms="RingTracker_Nhits Nparticles"
for histogram in ${histograms}; do
    FindFiles ${histogram}.png x x
    WriteFiles "Snapshot"
done
EndTable


histograms="Rhat Time"
BeginTable
degs="0 30 60 90 120 150 180 210 240 270 300 330"
for histogram in ${histograms}; do
    FILES=""
    for deg in ${degs}; do
	FILES="${FILES} RingTracker_Snapshot_${deg}deg_${histogram}.png"
    done
#	FindFiles RingTracker_Snapshot _${deg}deg_${histogram}.png -
    WriteFiles "Snapshot"
#    done
done
EndTable



#############################
#
#
# Ring Trackers for Muons That Survive
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="RingTrackerFinalAvg"></a>
    <h2><center>Ring Tracker Stats For Last 30 Turns</center></h2>
EOF

BeginTable

histograms="Rhat Vhat Xprime Yprime Mom Pol Zhat"
histograms="Rhat Vhat Xprime Yprime Mom EqRad"
timestamps="Begin_GeneratedDist Final_GeneratedDist Final_RemainingDist"
if [ ${plotmuongas} == 1 ]; then
    timestamps=""
fi
     
BeginTable
for timestamp in ${timestamps}; do
    FindFiles RingTracker_ x _${timestamp}.png
    WriteFiles ${timestamp}
done
EndTable



#############################
#
#
# Evolution of Ring Tracker Hits
#
#
#############################

cat >> ${outputdir}/index.html <<EOF
    <a name="RingTrackerwithTime"></a>
    <h2><center>Ring Tracker Stats For All Turns</center></h2>
EOF

timestamps="TimeOncePerTurn"
if [ ${plotmuongas} == 1 ]; then
    timestamps=""
fi
     
BeginTable
for timestamp in ${timestamps}; do
    FindFiles Tracker_ x _vs_${timestamp}.png
    WriteFiles ${timestamp}
done
EndTable




#############################
#
#
# Generated Phase Space(s)
#
#
#############################
particles="BirthMuon DecayMuon LostMuon StoredMuon BirthElectron BirthElectronEgtEth StrawElectron StrawCaloElectron GoodStrawElectron GoodStrawCaloElectron DecayElectron DecayElectronEgtEth DecayElectronEgtHghEth DecayElectronQuadSection DecayElectronKickerSection DecayElectronEmptySection"
if [ ${plotelectron} == 0 ]; then
    particles="BirthMuon LostMuon StoredMuon"
fi
if [ ${plotmuongas} == 1 ]; then
    particles="DecayMuon BirthElectronEgtEth StrawElectron GoodStrawElectron StrawCaloElectron GoodStrawCaloElectron GoodOneStrawCaloElectron GoodTwoStrawCaloElectron DecayElectron DecayElectronEgtEth  DecayElectronQuadSection DecayElectronKickerSection DecayElectronEmptySection"
fi

timestamps="GeneratedDist RemainingDist"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PS"></a>
    <h2><center>Generated (G4) Phase Space for: ${particle}</center></h2>
EOF

     
    BeginTable
    for timestamp in ${timestamps}; do
	FindFiles G4Track_ _${particle}_${timestamp}.png -
	WriteFiles ${timestamp}
    done
    EndTable
done





#############################
#
#
# Ratio of Generated Phase Space(s)
#
#
#############################
particles="StrawElectron StrawCaloElectron GoodStrawElectron GoodStrawCaloElectron  GoodOneStrawCaloElectron GoodTwoStrawCaloElectron DecayElectron"
timestamp="RemainingDist"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PS"></a>
    <h2><center>Ratio of Generated (G4) Phase Space for: ${particle}</center></h2>
EOF

    BeginTable
    FindFiles G4Track _${particle}_ ${timestamp}_Ratio.png
    WriteFiles ${timestamp}
    EndTable

done



#############################
#
#
# Evolution of Phase Space(s) with time
#
#
#############################
particles="DecayMuon LostMuon BirthElectron BirthElectronEgtEth StrawElectron GoodStrawElectron StrawCaloElectron GoodStrawCaloElectron  GoodOneStrawCaloElectron GoodTwoStrawCaloElectron DecayElectron DecayElectronEgtEth DecayElectronEgtHghEth DecayElectronEgtEth  DecayElectronQuadSection DecayElectronKickerSection DecayElectronEmptySection"
if [ ${plotelectron} == 0 ]; then
    particles="LostMuon"
fi
if [ ${plotmuongas} == 1 ]; then
    particles="DecayMuon BirthElectronEgtEth StrawElectron GoodStrawElectron StrawCaloElectron GoodStrawCaloElectron  GoodOneStrawCaloElectron GoodTwoStrawCaloElectron DecayElectron DecayElectronEgtEth  DecayElectronQuadSection DecayElectronKickerSection DecayElectronEmptySection"
fi
timestamps="TimeSingleHit TimeOncePerTurn"

for particle in ${particles}; do

cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PSwithTime"></a>
    <h2><center>Evolution of (G4) Phase Space for: ${particle}</center></h2>
EOF
     
echo "Starting w/ ${particle}"
    BeginTable
    for timestamp in ${timestamps}; do
	FindFiles G4Track ${particle}_vs_${timestamp}_Full_Profile.png -
	WriteFiles ${timestamp} excl NumAllStations
	FindFiles G4Track ${particle}_vs_${timestamp}_Full_Profile_x_FFT.png -
	WriteFiles ${timestamp} excl NumAllStations
    done
    EndTable

echo "Starting w/ ${particle} for specific times"
    BeginTable
    for timestamp in ${timestamps}; do
	FindFiles G4Track ${particle}_vs_${timestamp} us.png
	WriteFiles ${timestamp} excl NumAllStations
    done
    EndTable


cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PSwithTime"></a>
    <h2><center>Number Count Evolution of (G4) Phase Space for: ${particle}</center></h2>
EOF


    BeginTable
    histograms="NumAllStations NumAllStationsNoFastRotation" # NudAllStations"
    for timestamp in ${timestamps}; do
#	for histogram in ${histograms}; do
#	continue;
	    FindFiles G4Track_ ${particle}_vs ${timestamp}.png
	    WriteFiles ${timestamp} "AllTimes" incl NumAllStations
	    FindFiles G4Track_ ${particle}_vs ${timestamp}_x_FFT.png
	    WriteFiles ${timestamp} "AllTimesFFTs" incl NumAllStations
	    FindFiles G4Track_ ${particle}_vs_${timestamp} us.png
	    WriteFiles ${timestamp} "SubTimes" incl NumAllStations
	    FindFiles G4Track_ ${particle}_vs_${timestamp} us_x_FFT.png
	    WriteFiles ${timestamp} "SubTimesFFTs" incl NumAllStations
#	done
    done
    EndTable
    
    BeginRow ${timestamp}
    for histogram in ${histograms}; do
	continue
	BeginRow ${timestamp} ${histogram}
	write ${outputdir}/index.html G4Track_${histogram}_${particle}_vs_${timestamp}_x_FFT.png ${histogram}
	
	if [ ${timestamp} == "TimeOncePerTurn" ] || [ ${timestamp} == "Time" ]; then
	    write ${outputdir}/index.html G4Track_${histogram}_${particle}_vs_${timestamp}_x_1_2Time_FFT.png
	    write ${outputdir}/index.html G4Track_${histogram}_${particle}_vs_${timestamp}_x_11_12Time_FFT.png
	    write ${outputdir}/index.html G4Track_${histogram}_${particle}_vs_${timestamp}_x_10_20Time_FFT.png
	fi
	EndRow
    done
done






#############################
#
#
# Detector Hits
#
#
#############################
systems="Inflector Cryostat Quad Kicker Collimator Vacuum StrawTracker Calo Xtal Arc AllSystems"
systems="Inflector Cryostat Quad Kicker Collimator AllSystems"
histograms2d="XZ RhatY"
histograms1d="Nhits"

cat >> ${outputdir}/index.html <<EOF
    <a name="${system}Hits"></a>
    <h2><center>Detector System Hits</center></h2>
EOF
     
     
BeginTable
for system in ${systems}; do
    FindFiles ${system}Hits x .png
    WriteFiles ${system}
done
EndTable





cat >> ${outputdir}/index.html <<EOF
    <br>
    <br>
    <br>
    <hr>
    <address><a href="mailto:tgadfort@bnl.gov">Thomas Gadfort</a></address>
<!-- Created: Mon Nov  1 16:10:08 EDT 2010 -->
<!-- hhmts start -->
Last modified: Thu Mar  8 16:44:05 EST 2012
<!-- hhmts end -->
  </body>
</html>
EOF


}


input=${1}
input2=${input/g2MIGTRACE_/}
input3=${input2/.root/}
input=${input3}

del=1
makeplots=1
update=0
update_plotname1="dummy"
update_plotname2="dummy"
update_plotname3="dummy"
if ! [ -z ${2} ]; then
    if [ ${2} == "html" ]; then
	del=0
	makeplots=0
    fi
    if [ ${2} == "update" ]; then
	del=0
	makeplots=1
	update=1
	if ! [ -z ${3} ]; then
	    update_plotname1=${3}
	    if ! [ -z ${4} ]; then
		update_plotname2=${4}
		if ! [ -z ${5} ]; then
		    update_plotname3=${5}
		fi
	    fi
	fi
    fi
fi

beamstarts="UM DM UC DC CO"
basedir=""
for beamstart in ${beamstarts}; do
    check_beamstart=${input/${beamstart}/}
    if ! [ ${check_beamstart} == ${input} ]; then
	subdir=${beamstart}
	basedir=${beamstart}
	break;
    fi
done

infs="InfClosed InfOpen InfPartial"
for inf in ${infs}; do
    check_inf=${input/${inf}/}
    if ! [ ${check_inf} == ${input} ]; then
	subdir="${subdir}_${inf}"
    fi
done

echo "Subdir=${subdir}"

#
# Checking for root file
#
echo "Checking for files like this:"
echo "  [rootfiles/${subdir}/*${input}.root]"
found=0
for file in rootfiles/${subdir}/*${input}.root; do
    if [ -a ${file} ]; then
	found=1
    fi
done

if [ ${found} == 1 ]; then
    echo "Found it!"
else
    echo "Could not find the file...."
    found=0
fi    

if [ ${found} == 1 ]; then

echo ""
echo "==== Setting basedir = [${basedir}] ====="
echo ""

echo ${update} > plotname.dat
echo ${update_plotname1} >> plotname.dat
echo ${update_plotname2} >> plotname.dat
echo ${update_plotname3} >> plotname.dat
echo ${input} >> plotname.dat
echo ${basedir} >> plotname.dat
echo "-- Contents of plotname.dat --"
cat plotname.dat
echo "------------------------------"
#echo ${input}
#echo ${basedir}
sleep 2

input="${basedir}/${input}"

outputdir="/afs/fnal.gov/files/home/room3/tgadfort/public_html/plots/${input}"
echo "${outputdir}"
if [ ${del} == 1 ]; then
    if [ -d ${outputdir} ]; then
	echo "Removing old directory."
	rm -fr ${outputdir}
    fi
else
    echo "Not deleting output directory."
fi
mkdir -p ${outputdir}
#ls ${outputdir}
#makeindex ${input}


if [ ${makeplots} == 1 ]; then
    root -l plotinflector.C -b -q
else
    echo "Not making new plots."
fi

checkforelectron=${input/"NoMuonDecay"/}
plotelectron=0
if [ ${checkforelectron} == ${input} ]; then
    plotelectron=1
fi

checkformuongas=${input/"MuonGas"/}
plotmuongas=0
if ! [ ${checkformuongas} == ${input} ]; then
    plotmuongas=1
fi
if [ ${plotmuongas} == 0 ]; then
    checkformuongas=${input/"BeamTransport"/}
    if ! [ ${checkformuongas} == ${input} ]; then
	plotmuongas=1
    fi
fi

checkforspin=${input/"NoSpinTracking"/}
plotspin=0
if [ ${checkforspin} == ${input} ]; then
    plotspin=1
fi

echo "PlotElectron = ${plotelectron}"
echo "PlotSpin     = ${plotspin}"

makeindex ${input}
echo ""
echo "Wrote ${outputdir}/index.html"
echo ""

rm plotname.dat
cd /afs/fnal.gov/files/home/room3/tgadfort/public_html/plots
#cd UpstreamMandrel
#./gen.sh
#cd ../UpstreamCryo
#./gen.sh
cd DM ; ./gen.sh ; cd -
cd CO ; ./gen.sh ; cd -


fi
