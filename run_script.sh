#!/bin/bash
#default versions
Emin=1;
Emax=10;
Dmin=25;
Dmax=25;
Estep=1;
Dstep=1;
Nevent=100;

Help()
{

	cat <<-END
	${0##*/}: Script for running SiDetector
	Usage:
		--Emin <value(MeV)>
			Minimum energy. (default: 1 MeV)
		
		--Emax <value(MeV)>
			Maximum energy. (default: 10 MeV)
			
		--Dmin <value(um)>
			Minimum sensitive thickness. (default: 25 um)
			
		--Dmax <value(um)>
			Maximum sensitive thickness. (default: 25 um)
			
		--Estep <value>
			Energy step in for loop. (default: 1 MeV)
		
		--Dstep <value>
			Sensitive Thickness step in for loop. (default: 1 um)
		
		--Nevent <value>
			Number of events. (default: 100)
	END
	exit
}


while [ $# -gt 0 ]; do
	case $1 in
		--help)
			Help
    	exit
			;;
	  	--Emin)
			Emin=$2
			shift 2
			;;
		--Emax)
			Emax=$2
			shift 2
			;;
		--Dmin)
			Dmin=$2
			shift 2
			;;
		--Dmax)
			Dmax=$2
			shift 2
			;;
		--Estep)
			Estep=$2
			shift 2
			;;
		--Dstep)
			Dstep=$2
			shift 2
			;;
		--Nevent)
			Nevent=$2
			shift 2
			;;
	esac
done 
	

#Create macro and write it
#first loop in thickness

echo "/run/initialize" > macro_script.in
echo "/gun/particle proton" >> macro_script.in

for ((i = $Dmin; i <= $Dmax; i+=$Dstep)); do
  echo "/setTarget/sensitiveThickness $i um" >> macro_script.in
  for ((j = $Emin; j <= $Emax; j+=$Estep)); do  
    echo "/gun/energy $j MeV" >> macro_script.in
    echo "/run/beamOn $Nevent" >> macro_script.in
  done
done

#run geant4 simulation

./SiDetector macro_script.in
