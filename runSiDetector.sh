#!/bin/bash

while getopts "n:" opt; do
  case $opt in
    n) name=$OPTARG;;
  esac
done

# Run Geant4

#put your source <path>/geant4.sh

./SiDetector <put your macro name here>;

cd results;
mv data.dat ${name}.dat
cd ..;
