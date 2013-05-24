#!/bin/sh

targetdir="../../fcl/"
filestomove="allrun.sh   makefcl.sh   move.sh  plotinflector.C  plotinf.sh  run.sh  submit.sh"
for file in ${filestomove}; do
    echo "cp -f ${file} ${targetdir}"
    cp -f ${file} ${targetdir}
done
