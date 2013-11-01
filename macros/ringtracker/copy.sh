#!/bin/sh

targetdir="../../fcl/"
filestomove="allrun.sh   makefcl.sh   move.sh  plotinflector.C  plotinf.sh  run.sh  submit.sh  plotfastrotation.C  plotfast.sh fasrof.cxx g2gen.cxx sub2.sh FastRotation.C runner.sh plotTwiss.C twiss_n18.dat 
twiss_n137.dat"

for file in ${filestomove}; do
    echo "cp -f ${targetdir}/${file} ."
    cp -f ${targetdir}/${file} .
done
