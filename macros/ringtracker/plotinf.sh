#!/bin/sh

NiceTimestampName()
{
    if [ ${1} == "Init_Init" ]; then
	nicetimestamp="Initial Phase Space"
    elif [ ${1} == "FinalAvg_Init" ]; then
	nicetimestamp="Time Averged Phase Space for Last 30 Turns"
    elif [ ${1} == "Turn" ]; then
	nicetimestamp="12 Hits/Turn"
    elif [ ${1} == "OncePerTurn" ]; then
	nicetimestamp="1 Hit/Turn"
    else
	nicetimestamp=${1}
    fi
}

EndBeginRow()
{
    EndRow
    BeginRow
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
#	echo "[${outputdir}/${filename}] not found."
	echo "	  <td>${filename}</td>" >> ${outfile}
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
      <td><a href="#DecayMuonPS">Birth Muon PS</a></td>
      <td><a href="#BirthElectronPS">Birth Electron PS</a></td>
      <td><a href="#DecayElectronPS">Decay Electron PS</a></td>
EOF
EndBeginRow
cat >> ${outputdir}/index.html <<EOF
      <td><a href="#DecayMuonPSwithTime">Birth Muon PS w/ Time</a></td>
      <td><a href="#BirthElectronPSwithTime">Birth Electron PS w/ Time</a></td>
      <td><a href="#DecayElectronPSwithTime">Decay Electron PS w/ Time</a></td>
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
BeginTable
for timestamp in ${timestamps}; do
    BeginRow ${timestmap}

    histograms="Rhat Y Mom Pol PolY Xe"
    for histogram in ${histograms}; do
	write ${outputdir}/index.html Tracker${histogram}${timestamp}.png
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
histograms="RhatY XprimeX YprimeY Mom Rhat Vhat"
for tracker in ${trackers}; do
    BeginRow ${tracker}
    
    for histogram in ${histograms}; do
	write ${outputdir}/index.html RingTracker_${tracker}_${histogram}.png
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

histograms="Rhat Vhat Xprime Yprime Mom Pol"
timestamps="Init_Init FinalAvg_Init"
for timestamp in ${timestamps}; do
    BeginRow ${timestamp}
    
    for histogram in ${histograms}; do
	write ${outputdir}/index.html RingTracker_Time_${histogram}_${timestamp}.png
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


timestamps="Turn OncePerTurn"
histograms="Rhat Y Prhat Pvhat Pol PolY Xe"
for timestamp in ${timestamps}; do
    BeginRow ${timestamp}
    
    for histogram in ${histograms}; do
	write ${outputdir}/index.html Tracker${histogram}_${timestamp}.png
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
particles="BirthMuon DecayMuon BirthElectron DecayElectron BirthElectronEgtEth DecayElectronEgtEth"
timehistograms="Rhat Vhat Xprime Yprime Mom Pol PolX PolY Xe"
timestamp="Init"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PS"></a>
    <h2><center>Generated (G4) Phase Space for: ${particle}</center></h2>
EOF

    if [ ${particle} == "BirthMuon" ] || [ ${particle} == "DecayMuon" ]; then
	timehistograms="RhatY XprimeX YprimeY Mom Pol PolX PolY Xe"
    fi
    if [ ${particle} == "BirthElectron" ] || [ ${particle} == "DecayElectron" ]; then
	timehistograms="RhatY Xprime Yprime Mom Polarization"
    fi
    if [ ${particle} == "BirthElectronEgtEth" ] || [ ${particle} == "DecayElectronEgtEth" ]; then
	timehistograms="RhatY Xprime Yprime Mom Polarization"
    fi
	

    BeginTable
    BeginRow
    
    for histogram in ${timehistograms}; do
	write ${outputdir}/index.html G4Track_Time_${histogram}_${timestamp}_${particle}.png
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
particles="DecayMuon BirthElectron DecayElectron BirthElectronEgtEth DecayElectronEgtEth"
timehistograms="Rhat Vhat Xprime Yprime Mom Pol PolX PolY Xe"
timestamp="Turn"
for particle in ${particles}; do
cat >> ${outputdir}/index.html <<EOF
    <a name="${particle}PSwithTime"></a>
    <h2><center>Evolution of (G4) Phase Space for: ${particle}</center></h2>
EOF
    
    BeginTable
    if [ ${particle} == "DecayMuon" ]; then
	BeginRow
	
	if [ ${particle} == "DecayMuon" ]; then
	    histograms="Rhat Y Prhat Pvhat Mom"
	    for histogram in ${histograms}; do
		write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}.png
	    done
	fi

	EndRow
    fi
    
    if [ ${particle} == "BirthElectron" ] || [ ${particle} == "DecayElectron" ] || [ ${particle} == "BirthElectronEgtEth" ] || [ ${particle} == "DecayElectronEgtEth" ]; then
	BeginRow
	
	histograms="NgtEth Yprime Vhat Nud"
	for histogram in ${histograms}; do
	    write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}.png
	done
	
	EndBeginRow
	
	histograms="NgtEth Yprime Vhat Nud"
	for histogram in ${histograms}; do
	    write ${outputdir}/index.html G4Track${histogram}_${particle}_${timestamp}_x_FFT.png
	done
		
	EndRow
    fi

    EndTable
done


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
cd ../PerfectStorage
./gen.sh
cd ../CentralOrbit
./gen.sh
cd ../CentralOrbit_Offset77
./gen.sh
cd -

fi