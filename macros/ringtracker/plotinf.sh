#!/bin/sh

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
	echo "	  <td></td>" >> ${outfile}
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
    <table cellspacing=1 cellpadding=3>
    <tr>
      <td><a href="#inflector">Inflector Tracker</a></td>
      <td><a href="#inflectorhits">Inflector Hits</a></td>
      <td><a href="#ringhits">Ring Hits</a></td>
      <td><a href="#systemhits">System Hits</a></td>
      <td><a href="#tangency">Tangency</a></td>
    </tr>
    <tr>
      <td><a href="#muon">Muon Info</a></td>
      <td><a href="#time">Tracking Vs Time</a></td>
      <td><a href="#rhoy">X and Y Distribution</a></td>
      <td><a href="#stored">Storage Vs Time</a></td>
      <td><a href="#fft">FFT Info</a></td>
    </tr>
    <tr>
      <td><a href="#kick">Intercept and Kick Required</a></td>
      <td><a href="#vac">Tracking Per 30 degrees</a></td>
      <td><a href="#ps">Final 1D Phase Space</a></td>
    </tr>
    <tr>
      <td><a href="#Start">Generated Phase Space</a></td>
      <td><a href="#10turns">Generated Phase Space (>20 us)</a></td>
      <td><a href="#50turns">Generated Phase Space (>50 us)</a></td>
      <td><a href="#FinalAvg">Generated Phase Space (Final)</a></td>
      <td></td>
    </tr>
    </table>

    <hr>
    <a name="inflector"></a>
    <h2><center>Inflector Tracker Stats</center></h2>
    <table cellspacing=1 cellpadding=3>
EOF
trackers="0 1 2 3 4 5 6 7 8"
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker}</td>
EOF
histograms="RhoY XprimeX YprimeY RhoY_RingUnit XprimeX_RingUnit YprimeY_RingUnit"
for histogram in ${histograms}; do
    write ${outputdir}/index.html InflectorTracker_${tracker}_${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done

cat >> ${outputdir}/index.html <<EOF
    </table>
    <br>
    <hr>
    <br>
    <a name="infletorhits"></a>
    <h2><center>Ring Hit Stats In The Inflector System</center></h2>
    <table cellspacing=1 cellpadding=3>
EOF




trackers="UpstreamEndFlange UpstreamEquivalentNbTi UpstreamEquivalentAl UpstreamEquivalentCu UpstreamWindow Mandrel DownstreamWindow DownstreamEquivalentAl DownstreamEquivalentCu DownstreamEquivalentNbTi DownstreamEndFlange"
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker} (all)</td>
EOF
histograms="Hits_RhoY Hits_XZ Hits_DeltaPx Hits_DeltaPy _Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
	<tr>
          <td>${tracker} (stored)</td>
EOF
histograms="Hits_Stored_RhoY Hits_Stored_XZ Hits_Stored_DeltaPx Hits_Stored_DeltaPy _Stored_Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done


cat >> ${outputdir}/index.html <<EOF
    </table>
    <br>
    <hr>
    <br>
    <a name="ringhits"></a>
    <h2><center>Ring Hit Stats For Specific Systems</center></h2>
    <table cellspacing=1 cellpadding=3>
EOF



trackers="Quad10 Quad11 Quad20 Quad21 Quad30 Quad31 Quad40 Quad41 Kicker1 Kicker2 Kicker3"
#trackers=""
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker} (all)</td>
EOF
histograms="Hits_RhoY Hits_XZ Hits_DeltaPx Hits_DeltaPy _Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
	<tr>
          <td>${tracker} (stored)</td>
EOF
histograms="Hits_Stored_RhoY Hits_Stored_XZ Hits_Stored_DeltaPx Hits_Stored_DeltaPy _Stored_Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done


cat >> ${outputdir}/index.html <<EOF
    </table>
    <br>
    <hr>
    <br>
    <a name="tangency"></a>
    <h2><center>Prhat and Pvhat At Inflector/Ring</center></h2>
    <table cellspacing=1 cellpadding=3>
    <tr>
EOF

write ${outputdir}/index.html InflectorTracker_PrhatPvhatEntrance.png
write ${outputdir}/index.html InflectorTracker_PrhatPvhatExit.png
write ${outputdir}/index.html RingTracker_PrhatPvhatEntrance.png


cat >> ${outputdir}/index.html <<EOF
      </tr>
    </table>
    <br>
    <hr>
    <br>
    <a name="systemhits"></a>
    <h2><center>Ring Hit Stats For Larger Systems</center></h2>
    <table cellspacing=1 cellpadding=3 border=1>
EOF



trackers="Inflector Cryostat Quad Kicker Collimator" # Vacuum StrawTracker Calo Xtal"
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker} (all)</td>
EOF
histograms="Hits_RhoY Hits_XZ Hits_DeltaPx Hits_DeltaPy _Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
	<tr>
          <td>${tracker} (stored)</td>
EOF
histograms="Hits_Stored_RhoY Hits_Stored_XZ Hits_Stored_DeltaPx Hits_Stored_DeltaPy _Stored_Nhits"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done

cat >> ${outputdir}/index.html <<EOF
    </table>
    <br>
    <hr>
    <br>
    <a name="muon"></a>
    <h2><center>Muon Momentum Stats For Specific Times</center></h2>
    <table cellspacing=1 cellpadding=3>
	<tr>
EOF
tracker="MuonMomentum"
histograms="G4 RingEntrance 90degrees"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}_${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
	</tr>
	<tr>
EOF



tracker="MuonMomentum"
histograms="}80degrees 270degrees"
for histogram in ${histograms}; do
    write ${outputdir}/index.html ${tracker}_${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
	</tr>
    </table>

    <br>
    <hr>
    <br>
    <a name="time"></a>
    <h2><center>Ring Tracker Stats Vs. Turn</center></h2>

    <table cellspacing=1 cellpadding=3 border=1>
EOF



histograms="Turn OncePerTurn"
for histogram in ${histograms}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
EOF
trackers="Rho Y Prhat Pvhat Mom Pol PolX Xe"
for tracker in ${trackers}; do
    write ${outputdir}/index.html Tracker${tracker}${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done

cat >> ${outputdir}/index.html <<EOF
    </table>

    <br>
    <hr>
    <br>

    <a name="rhoy"></a>
    <h2><center>X and Y Distributions</center></h2>
    <table cellspacing=1 cellpadding=3>
    <tr>
EOF
    write ${outputdir}/index.html TrackerRho.png
    write ${outputdir}/index.html TrackerY.png
cat >> ${outputdir}/index.html <<EOF
	</tr>
EOF




cat >> ${outputdir}/index.html <<EOF
    </table>

    <br>
    <hr>
    <br>

    <a name="fft"></a>
    <h2><center>FFT for X vs Turn and Y vs Turn</center></h2>

    <table cellspacing=1 cellpadding=3>
    <tr>
EOF
    write ${outputdir}/index.html WavelengthX.png
    write ${outputdir}/index.html WavelengthY.png
cat >> ${outputdir}/index.html <<EOF
	</tr>
EOF



cat >> ${outputdir}/index.html <<EOF
    </table>

    <br>
    <hr>
    <br>

    <a name="stored"></a>
    <h2><center>Ring Tracker Stats For Stored Muons</center></h2>

    <table cellspacing=1 cellpadding=3>
    <tr>
EOF




trackers="0Turn 1Turn 2Turn 5Turn 10Turn 50Turn 100Turn 200Turn 500Turn 1000Turn 2000Turn"
trackers=""
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker}</td>
EOF
histograms="RhoY thetaX thetaY thetaXY Momentum"
for histogram in ${histograms}; do
    write ${outputdir}/index.html RingTracker_${tracker}_${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done

cat >> ${outputdir}/index.html <<EOF
    </table>

    <br>
    <hr>
    <br>

    <a name="kick"></a>
    <h2><center>Ring Intercept and Kick Required Stas</center></h2>

    <table cellspacing=1 cellpadding=3>
    <tr>
EOF


write ${outputdir}/index.html RingTracker_DegreeAtRhat0.png
write ${outputdir}/index.html RingTracker_KickAtRhat0.png
write ${outputdir}/index.html RingTracker_DegreeAtRhat0KickAtRhat0.png

cat >> ${outputdir}/index.html <<EOF 
        </tr>
	<tr>
EOF

write ${outputdir}/index.html RingTracker_DegreeAtRhat0_Mom1.png
write ${outputdir}/index.html RingTracker_KickAtRhat0_Mom1.png
write ${outputdir}/index.html RingTracker_DegreeAtRhat0KickAtRhat0_Mom1.png

cat >> ${outputdir}/index.html <<EOF 
        </tr>
	<tr>
EOF

write ${outputdir}/index.html RingTracker_DegreeAtRhat0_Mom4.png
write ${outputdir}/index.html RingTracker_KickAtRhat0_Mom4.png
write ${outputdir}/index.html RingTracker_DegreeAtRhat0KickAtRhat0_Mom4.png

cat >> ${outputdir}/index.html <<EOF 
        </tr>
	<tr>
EOF

write ${outputdir}/index.html RingTracker_DegreeAtRhat0RhatInit.png
write ${outputdir}/index.html RingTracker_KickAtRhat0RhatInit.png

cat >> ${outputdir}/index.html <<EOF 
        </tr>
    </table>
    <br>
    <br>
EOF

cat >> ${outputdir}/index.html <<EOF
    <a name="vac"></a>
    <br>
    <h2><center>Ring Tracker Stats For Specific Turns</center></h2>
    <table cellspacing=1 cellpadding=3>
EOF


trackers="00 01 02 03 04 05 06 07 08"
for tracker in ${trackers}; do
cat >> ${outputdir}/index.html <<EOF
	<tr>
          <td>${tracker}</td>
EOF
histograms="RhoY XprimeX YprimeY Mom Rhat Vhat"
for histogram in ${histograms}; do
    write ${outputdir}/index.html RingTracker_${tracker}_${histogram}.png
done
cat >> ${outputdir}/index.html <<EOF
        </tr>
EOF
done

cat >> ${outputdir}/index.html <<EOF 
        </tr>
    </table>
    <br>
EOF

cat >> ${outputdir}/index.html <<EOF
    </table>

    <br>
    <hr>
    <br>

    <a name="Start"></a>
    <h2><center>Measured (Ring) and Generated (G4) Phase Space for t=0</sub></center></h2>

    <table cellspacing=1 cellpadding=3>
	<tr>
EOF
	write ${outputdir}/index.html RingTracker_Time_Rhat.png
	write ${outputdir}/index.html RingTracker_Time_Vhat.png
	write ${outputdir}/index.html RingTracker_Time_Xprime.png
	write ${outputdir}/index.html RingTracker_Time_Yprime.png
	write ${outputdir}/index.html RingTracker_Time_Mom.png
	write ${outputdir}/index.html RingTracker_Time_Pol.png
	write ${outputdir}/index.html RingTracker_Time_PolX.png
	write ${outputdir}/index.html RingTracker_Time_Xe.png
cat >> ${outputdir}/index.html <<EOF
        </tr>
        <tr>
EOF
	write ${outputdir}/index.html G4Track_Time_Rhat.png
	write ${outputdir}/index.html G4Track_Time_Vhat.png
	write ${outputdir}/index.html G4Track_Time_Xprime.png
	write ${outputdir}/index.html G4Track_Time_Yprime.png
	write ${outputdir}/index.html G4Track_Time_Mom.png
	write ${outputdir}/index.html G4Track_Time_Pol.png
	write ${outputdir}/index.html G4Track_Time_PolX.png
	write ${outputdir}/index.html G4Track_Time_Xe.png
cat >> ${outputdir}/index.html <<EOF
        </tr>
        <tr>
EOF
	write ${outputdir}/index.html G4Track_Time_RhoY.png
	write ${outputdir}/index.html G4Track_Time_XprimeX.png
	write ${outputdir}/index.html G4Track_Time_YprimeY.png
cat >> ${outputdir}/index.html <<EOF
        </tr>
    </table>
EOF


cat >> ${outputdir}/index.html <<EOF
    <br>
    <hr>
    <br>

    <a name="10turns"></a>
    <h2><center>Measured (Ring) and Fraction of Generated (G4) Phase Space for t>t<sub>10-turns</sub></center></h2>

    <table cellspacing=1 cellpadding=3>
      <tr>
EOF
	write ${outputdir}/index.html RingTracker_Time_Rhat_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Vhat_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Xprime_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Yprime_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Mom_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Pol_10turns.png
	write ${outputdir}/index.html RingTracker_Time_PolX_10turns.png
	write ${outputdir}/index.html RingTracker_Time_Xe_10turns.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
      <tr>
EOF
	write ${outputdir}/index.html G4Track_Time_Rhat_10turns.png
	write ${outputdir}/index.html G4Track_Time_Vhat_10turns.png
	write ${outputdir}/index.html G4Track_Time_Xprime_10turns.png
	write ${outputdir}/index.html G4Track_Time_Yprime_10turns.png
	write ${outputdir}/index.html G4Track_Time_Mom_10turns.png
	write ${outputdir}/index.html G4Track_Time_Pol_10turns.png
	write ${outputdir}/index.html G4Track_Time_PolX_10turns.png
	write ${outputdir}/index.html G4Track_Time_Xe_10turns.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
    </table>

    <br>
    <hr>
    <br>

    <a name="50turns"></a>
    <h2><center>Measured (Ring) and Fraction of Generated (G4) Phase Space for t>t<sub>50-turns</sub></center></h2>

    <table cellspacing=1 cellpadding=3>
      <tr>
EOF
	write ${outputdir}/index.html RingTracker_Time_Rhat_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Vhat_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Xprime_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Yprime_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Mom_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Pol_50turns.png
	write ${outputdir}/index.html RingTracker_Time_PolX_50turns.png
	write ${outputdir}/index.html RingTracker_Time_Xe_50turns.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
      <tr>
EOF
	write ${outputdir}/index.html G4Track_Time_Rhat_50turns.png
	write ${outputdir}/index.html G4Track_Time_Vhat_50turns.png
	write ${outputdir}/index.html G4Track_Time_Xprime_50turns.png
	write ${outputdir}/index.html G4Track_Time_Yprime_50turns.png
	write ${outputdir}/index.html G4Track_Time_Mom_50turns.png
	write ${outputdir}/index.html G4Track_Time_Pol_50turns.png
	write ${outputdir}/index.html G4Track_Time_PolX_50turns.png
	write ${outputdir}/index.html G4Track_Time_Xe_50turns.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
    </table>
EOF

cat >> ${outputdir}/index.html <<EOF
    <br>
    <hr>
    <br>

    <a name="Allturns"></a>
    <h2><center>Measured (Ring) and Fraction of Generated (G4) Phase Space for t>t<sub>max-turns</sub></center></h2>

    <table cellspacing=1 cellpadding=3>
      <tr>
EOF
	write ${outputdir}/index.html RingTracker_Time_Rhat_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Vhat_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Xprime_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Yprime_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Mom_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Pol_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_PolX_FinalAvg.png
	write ${outputdir}/index.html RingTracker_Time_Xe_FinalAvg.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
      <tr>
EOF
	write ${outputdir}/index.html G4Track_Time_Rhat_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Vhat_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Xprime_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Yprime_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Mom_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Pol_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_PolX_FinalAvg.png
	write ${outputdir}/index.html G4Track_Time_Xe_FinalAvg.png
cat >> ${outputdir}/index.html <<EOF
      </tr>
    </table>

    <br>
    <hr>
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