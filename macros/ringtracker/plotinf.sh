#!/bin/sh

NiceTimestampName()
{
    if [ ${1} == "Init_Init" ]; then
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
cat >> ${outputdir}/index.html <<EOF
        <td><b><i><center>${nicetimestamp}</center></i></b></td>
EOF
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

    
    if [ -a ${outputdir}/${filename} ]; then
	epsname=${filename/png/eps}
	linkname="${filename}"
	if [ -a ${outputdir}/${epsname} ]; then
	    linkname=${epsname}
	fi
cat >> ${outfile} <<EOF
	  <td><center><a href="${linkname}"><img alt="" width="100%" src="${filename}"></a></td>
EOF
    else
	echo "[${outputdir}/${filename}] not found."
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

BeginTable
BeginRow
cat >> ${outputdir}/index.html <<EOF
      <td><a href="#RingTracker">Ring Tracker (30 deg)</a></td>
      <td><a href="#RingTrackerFinalAvg">Ring Tracker <Avg></a></td>
      <td><a href="#RingTrackerwithTime">Ring Tracker w/ Time</a></td>
EOF
EndBeginRow
cat >> ${outputdir}/index.html <<EOF
      <td><a href="#BirthMuonPS">Birth Muon PS</a></td>
      <td><a href="#DecayMuonPS">Decay Muon PS</a></td>
      <td><a href="#StoredMuonPS">Stored Muon PS</a></td>
      <td><a href="#LostMuonPS">Lost Muon PS</a></td>
      <td><a href="#BirthElectronPS">Birth Electron PS</a></td>
      <td><a href="#DecayElectronPS">Decay Electron PS</a></td>
      <td><a href="#BirthElectronEgtEthPS">Birth Electron (E>1.8) PS</a></td>
      <td><a href="#DecayElectronEgtEthPS">Decay Electron (E>1.8) PS</a></td>
EOF
EndBeginRow
cat >> ${outputdir}/index.html <<EOF
      <td><a href="#DecayMuonPSwithTime">Decay Muon PS w/ Time</a></td>
      <td><a href="#LostMuonPSwithTime">Lost Muon PS w/ Time</a></td>
      <td><a href="#BirthElectronPSwithTime">Birth Electron PS w/ Time</a></td>
      <td><a href="#DecayElectronPSwithTime">Decay Electron PS w/ Time</a></td>
      <td><a href="#BirthElectronEgtEthPSwithTime">Birth Electron (E>1.8) PS w/ Time</a></td>
      <td><a href="#DecayElectronEgtEthPSwithTime">Decay Electron (E>1.8) PS w/ Time</a></td>
EOF
EndRow
EndTable


#############################
#
#
# Ring Trackers (Just Muons)
#
#
#############################


timestamps="Turn OncePerTurn"
timestamps=""
BeginTable
for timestamp in ${timestamps}; do
    BeginRow ${timestmap}

    histograms="Rhat Y Mom Pol PolY Xe Zhat"
    for histogram in ${histograms}; do
	write ${outputdir}/index.html Tracker${histogram}${timestamp}.png ${histogram}
    done
    
    EndRow
done
EndTable




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

trackers="00 01 02 03 04 05 06 07 08"
trackers=""
histograms="RhatY XprimeX YprimeY Mom Rhat Vhat"
for tracker in ${trackers}; do
    BeginRow ${tracker}
    
    for histogram in ${histograms}; do
	write ${outputdir}/index.html RingTracker_${tracker}_${histogram}.png ${histogram}
    done

    EndRow
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
timestamps="Init_Init FinalAvg_Init"
timestamps=""
for timestamp in ${timestamps}; do
    BeginRow ${timestamp}
    
    for histogram in ${histograms}; do
	write ${outputdir}/index.html RingTracker_Time_${histogram}_${timestamp}.png ${histogram}
    done

    EndRow
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

BeginTable


timestamps="Turn OncePerTurn Turn_25Turns"
timestamps="Turn"
timestamps=""
histograms1="Rhat Y Prhat Pvhat Mom"
histograms2="Pol PolY Xe Zhat"
for timestamp in ${timestamps}; do
    BeginRow ${timestamp}
    
    for histogram in ${histograms1}; do
	write ${outputdir}/index.html Tracker${histogram}_${timestamp}.png ${histogram}
    done

    EndRow
    BeginRow ${timestamp}
    
    for histogram in ${histograms2}; do
	write ${outputdir}/index.html Tracker${histogram}_${timestamp}.png ${histogram}
    done

    EndRow
done

EndTable



#############################
#
#
# Generated Phase Space(s)
#
#
#############################
particles="BirthMuon DecayMuon LostMuon StoredMuon BirthElectron BirthElectronEgtEth"
timehistograms="Rhat Vhat Xprime Yprime Mom Pol PolXY Xe"
timestamp="Init"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PS"></a>
    <h2><center>Generated (G4) Phase Space for: ${particle}</center></h2>
EOF

    timehistograms1=""
    timehistograms2=""

    if [ ${particle} == "BirthMuon" ] || [ ${particle} == "DecayMuon" ] || [ ${particle} == "StoredMuon" ]; then
	timehistograms1="RhatY XprimeX YprimeY XZ"
	timehistograms2="Mom Pol PolXY Xe"
    fi
    if [ ${particle} == "LostMuon" ]; then
	timehistograms1="RhatY R Mom XZ"
	timehistograms2=""
    fi
    if [ ${particle} == "BirthElectron" ] || [ ${particle} == "DecayElectron" ] || [ ${particle} == "BirthElectronEgtEth" ] || [ ${particle} == "DecayElectronEgtEth" ]; then
	timehistograms1="RhatY XZ Mom"
	timehistograms2=""
    fi

    BeginTable
    BeginRow
    
    for histogram in ${timehistograms1}; do
	write ${outputdir}/index.html G4Track_Time_${histogram}_${timestamp}_${particle}.png ${histogram}
    done

    EndBeginRow
    
    for histogram in ${timehistograms2}; do
	write ${outputdir}/index.html G4Track_Time_${histogram}_${timestamp}_${particle}.png ${histogram}
    done
    
    EndRow
    EndTable
done








#############################
#
#
# Evolution of Phase Space(s) with time
#
#
#############################
particles="DecayMuon LostMuon BirthElectron BirthElectronEgtEth"
timehistograms="Rhat Vhat Xprime Yprime Mom Pol PolX PolY Xe"
timestamps_muons="Turn OncePerTurn Time TimeOncePerTurn"
timestamps_muons="Time TimeOncePerTurn"
timestamps_electrons="Turn Time OncePerTurn TimeOncePerTurn"
timestamps_electrons="Time TimeOncePerTurn"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PSwithTime"></a>
    <h2><center>Evolution of (G4) Phase Space for: ${particle}</center></h2>
EOF
     
    if [ ${particle} == "DecayMuon" ] || [ ${particle} == "LostMuon" ]; then
	timestamps="${timestamps_muons}"
    fi
    if [ ${particle} == "BirthElectron" ] || [ ${particle} == "BirthElectronEgtEth" ]; then
	timestamps="${timestamps_electrons}"
    fi


    BeginTable
    for timestamp in ${timestamps}; do
	if [ ${particle} == "DecayMuon" ] || [ ${particle} == "LostMuon" ]; then
	    if [ ${timestamp} == "OncePerTurn" ]; then
		continue;
	    fi
	fi

	BeginRow ${timestamp}
	
	if [ ${particle} == "LostMuon" ]; then
	    histograms="R Mom"
	    histograms=""
	fi
	if [ ${particle} == "DecayMuon" ]; then
	    histograms="Rhat Y Prhat Pvhat Mom"
	fi
	if [ ${particle} == "BirthElectron" ] || [ ${particle} == "BirthElectronEgtEth" ]; then
	    histograms="Rhat Y Prhat Pvhat Mom"
	fi
	for histogram in ${histograms}; do
	    write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}.png ${histogram}
	done
	
	EndRow



#	if [ ${particle} == "DecayMuon" ] || [ ${particle} == "BirthElectron" ] || [ ${particle} == "LostMuon" ]; then
#	    BeginRow ${timestamp}
#	    for histogram in ${histograms}; do
#		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_25Turns.png ${histogram}
#	    done
#	    EndRow
#	fi
    done
	    

    EndTable
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PSwithTime"></a>
    <h2><center>Number Count Evolution of (G4) Phase Space for: ${particle}</center></h2>
EOF
    BeginTable

    for timestamp in ${timestamps}; do
	histograms="Num NumCounter"
	histograms="Num"

	BeginRow ${timestamp}
	for histogram in ${histograms}; do
	    write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}.png ${histogram}
	    
	    if [ ${timestamp} == "TimeOncePerTurn" ] || [ ${timestamp} == "Time" ]; then
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_1_6Time.png
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_15_20Time.png
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_30_35Time.png
	    fi
	done
	
	EndRow

	BeginRow ${timestamp}
	for histogram in ${histograms}; do
	    write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_x_FFT.png ${histogram}
	    
	    if [ ${timestamp} == "TimeOncePerTurn" ] || [ ${timestamp} == "Time" ]; then
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_x_1_6Time_FFT.png
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_x_15_20Time_FFT.png
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_x_30_35Time_FFT.png
	    fi
	done
	
	EndRow
    done

    EndTable
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
    BeginRow ${system}
    for histogram in ${histograms2d}; do
	write ${outputdir}/index.html ${system}Hits_${histogram}.png
    done

    for histogram in ${histograms1d}; do
	write ${outputdir}/index.html ${system}_${histogram}.png
    done

    EndRow
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

beamstarts="UpstreamMandrel UpstreamCryo DownstreamMandrel PerfectStorage CentralOrbit_Offset77 CentralOrbit"
basedir=""
for beamstart in ${beamstarts}; do
    check_beamstart=${input/${beamstart}/}
    if ! [ ${check_beamstart} == ${input} ]; then
	subdir=${beamstart}
	basedir=${beamstart}
	break;
    fi
done

infs="ClosedInflector InflectorOpen PartiallyOpen"
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

makeindex ${input}
rm plotname.dat
cd /afs/fnal.gov/files/home/room3/tgadfort/public_html/plots
cd UpstreamMandrel
./gen.sh
cd ../UpstreamCryo
./gen.sh
cd ../DownstreamMandrel
./gen.sh
cd ../CentralOrbit
./gen.sh
cd ../CentralOrbit_Offset77
./gen.sh
cd -

fi