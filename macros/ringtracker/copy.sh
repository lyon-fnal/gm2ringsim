#!/bin/sh

targetdir="../../fcl/"
filestomove="allrun.sh   makefcl.sh   move.sh  plotinflector.C  plotinf.sh  run.sh  submit.sh"
for file in ${filestomove}; do
    echo "cp -f ${targetdir}/${file} ."
    cp -f ${targetdir}/${file} .
done
