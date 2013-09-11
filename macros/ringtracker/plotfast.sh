#!/bin/sh

echo ${1} > filename

core=${1}
core=${core/.root/}
core=${core/"g2gen_"/}
core=${core/"fasrof_"/}

runFA=1
if ! [ -z ${2} ]; then
    if [ ${2} == "html" ]; then
	runFA=0
    fi
fi

base=${core}
base=${base/"_Delta200ns"/}
base=${base/"_Delta150ns"/}
base=${base/"_Delta100ns"/}
base=${base/"_Delta50ns"/}
base=${base/"_Delta25ns"/}
base=${base/"_Delta10ns"/}
base=${base/"_Delta1ns"/}

echo ${core} >> filename
#echo ${core}
#sleep 100000
outdir="/afs/fnal.gov/files/home/room3/tgadfort/WWW/plots/FastRotation/${core}"
if [ -a ${outdir} ]; then
    rm -fr ${outdir}
fi
mkdir -p ${outdir}

if [ ${runFA} == 1 ]; then
    root -l plotfastrotation.C -b -q
fi
#sleep 1000

rm filename

let nfile=0
for file in ${outdir}/*.png; do
    if [ -a ${file} ]; then
	((nfile++))
    fi
done

if [ ${nfile} == 0 ]; then
    rmdir ${outdir}
fi

if [ ${nfile} -gt 0 ]; then

	    
html="index.html"
cat > ${html} <<EOF
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>g-2</title>
    <link rel="stylesheet" type="text/css" href="http://www-d0.fnal.gov/Run2Physics/top/public/fall06/singletop/style.css">
  </head>

  <body>
  
  <center><h2>Created from ${1}</h2></center>
EOF

plotgen=0
if [ ${plotgen} == 1 ]; then
cat > ${html} <<EOF
  <table cellspacing=1 cellpadding=3 border=1>
    <tr>
      <td><center><b>hMomSpec</b></center></td>
      <td><center><b>hnTurns</b></center></td>
      <td><center><b>hFreqSpec</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="../${base}/hMomSpec.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hnTurns.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hFreqSpec.png"></td>
    </tr>
    <tr>
      <td><center><b>hT0Pulse</b></center></td>
      <td><center><b>ht0</b></center></td>
      <td><center><b>ht0time</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="../${base}/hT0Pulse.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/ht0.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/ht0time.png"></td>
    </tr>
    <tr>
      <td><center><b>hTimeDecay</b></center></td>
      <td><center><b>hSignal00</b></center></td>
      <td><center><b>hSignal00 (1-6&mu;s)</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="../${base}/hTimeDecay.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hSignal00.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hSignal00_1000_2000ns.png"></td>
    </tr>
    <tr>
      <td><center><b>hSignal00 (6-7&mu;s)</b></center></td>
      <td><center><b>hSignal00 (11-12&mu;s)</b></center></td>
      <td><center><b>hSignal00 (16-17&mu;s)</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="../${base}/hSignal00_6000_7000ns.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hSignal00_11000_12000ns.png"></td>
      <td><center><img alt="" width="100%" src="../${base}/hSignal00_16000_17000ns.png"></td>
    </tr>
  </table>
EOF
fi

cat > ${html} <<EOF
  <br><hr><br>

  <table cellspacing=1 cellpadding=3 border=1>
    <tr>
      <td><center><b>< R - R<sub>m</sub> > (GEANT)</b></center></td>
      <td><center><b>x<sub>e</sub> = dP/P x 1/(1-n) x R<sub>m</sub></b></center></td>
      <td><center><b>Fast Rotation</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="RingTracker_EqRad_Final_RemainingDist.png"></td>
      <td><center><img alt="" width="100%" src="G4Track_Xe_DecayMuon_GeneratedDist.png"></td>
      <td><center><img alt="" width="100%" src="hEqRadConv.png"></td>
    </tr>
    <tr>
      <td><center><b>FR vs GEANT</b></center></td>
      <td><center><b>FR vs THEORY</b></center></td>
      <td><center><b>GEANT vs THEORY</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="FastRotation_GEANT.png"></td>
      <td><center><img alt="" width="100%" src="FastRotation_THEORY.png"></td>
      <td><center><img alt="" width="100%" src="GEANT_THEORY.png"></td>
    </tr>
    <tr>
      <td><center><b>t0</b></center></td>
      <td><center><b>t0 Weight</b></center></td>
      <td><center><b>T<sub>c</sub> vs Radius</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="gt0.png"></td>
      <td><center><img alt="" width="100%" src="gt0weight.png"></td>
      <td><center><img alt="" width="100%" src="gPeriodRadius.png"></td>
    </tr>
  </table>
  <br><p><br>
  <br><hr><br>

  <table cellspacing=1 cellpadding=3 border=1>
    <tr>
      <td><center><img alt="" width="100%" src="Data_MC_0_20000.png"></td>
      <td><center><img alt="" width="100%" src="Data_MC_1000_2000.png"></td>
      <td><center><img alt="" width="100%" src="Data_MC_10000_11000.png"></td>
      <td><center><img alt="" width="100%" src="Data_MC_19000_20000.png"></td>
    </tr>
  </table>
  <br><p><br>
  <br><hr><br>

  <table cellspacing=1 cellpadding=3 border=1>
 
    <tr>
      <td><center><b>x<sub>eq</sub> (Carey)</b></center></td>
      <td><center><b>x<sub>eq</sub> (Me 1st Pass)</b></center></td>
      <td><center><b>x<sub>eq</sub> (Me 2nd Pass)</b></center></td>
    </tr>
    <tr>
      <td><center><img alt="" width="100%" src="geqrad.png"></td>
      <td><center><img alt="" width="100%" src="gEqRad1.png"></td>
      <td><center><img alt="" width="100%" src="gEqRad2.png"></td>
    </tr>
  </table>

  <br><hr><br><br>

  <table cellspacing=1 cellpadding=3 border=1>
    <tr>
      <td></td>
      <td><center><b>Counts/ns (0-50&mu;s)</b></center></td>
      <td><center><b>Counts/ns (1-6&mu;s)</b></center></td>
      <td><center><b>Counts/ns (6-7&mu;s)</b></center></td>
      <td><center><b>Counts/ns (11-16&mu;s)</b></center></td>
      <td><center><b>Counts/ns (16-17&mu;s)</b></center></td>
    </tr>
    <tr>
      <td>MC</td>
      <td><center><img alt="" width="100%" src="hgen.png"></td>
      <td><center><img alt="" width="100%" src="hgen_1000_2000ns.png"></td>
      <td><center><img alt="" width="100%" src="hgen_6000_7000ns.png"></td>
      <td><center><img alt="" width="100%" src="hgen_11000_12000ns.png"></td>
      <td><center><img alt="" width="100%" src="hgen_16000_17000ns.png"></td>
    </tr>
    <tr>
      <td>Data</td>
      <td><center><img alt="" width="100%" src="hfr2.png"></td>
      <td><center><img alt="" width="100%" src="hfr2_1000_2000ns.png"></td>
      <td><center><img alt="" width="100%" src="hfr2_6000_7000ns.png"></td>
      <td><center><img alt="" width="100%" src="hfr2_11000_12000ns.png"></td>
      <td><center><img alt="" width="100%" src="hfr2_16000_17000ns.png"></td>
    </tr>
    <tr>
      <td>Data - MC</td>
      <td><center><img alt="" width="100%" src="hData_MC.png"></td>
      <td><center><img alt="" width="100%" src="hData_MC_1000_2000ns.png"></td>
      <td><center><img alt="" width="100%" src="hData_MC_6000_7000ns.png"></td>
      <td><center><img alt="" width="100%" src="hData_MC_11000_12000ns.png"></td>
      <td><center><img alt="" width="100%" src="hData_MC_16000_17000ns.png"></td>
    </tr>
  </table>
  <br><hr><br>
        
    <hr>
    <address><a href="mailto:tgadfort@bnl.gov">Thomas Gadfort</a></address>
<!-- Created: Mon Nov  1 16:10:08 EDT 2010 -->
<!-- hhmts start -->
Last modified: Thu Mar  8 16:44:05 EST 2012
<!-- hhmts end -->
  </body>
</html>
EOF

mv ${html} ${outdir}/

cd /afs/fnal.gov/files/home/room3/tgadfort/WWW/plots/FastRotation
./gen.sh
cd -

echo ""
echo "http://home.fnal.gov/~tgadfort/plots/FastRotation/${core}/${html}"
echo ""

fi