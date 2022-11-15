#!/bin/bash
#default versions
Emin=1;
Emax=10;
Thick=25;
Dstep=1;
Nevent=100;
Name=data-renamed;

Help()
{

	cat <<-END
	${0##*/}: Script for running SiDetector
	Usage:
		--Emin <value(MeV)>
			Minimum energy. (default: 1 MeV)
		
		--Emax <value(MeV)>
			Maximum energy. (default: 10 MeV)
			
		--Thick <value(um)>
			Minimum sensitive thickness. (default: 25 um)
			
		--step <value>
			step in for loop. (default: 1 MeV)
		
		--Nevent <value>
			Number of events. (default: 100)
		
		--Name <value>
			Name of file. (default: data-renamed)
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
		--Thick)
			Thick=$2
			shift 2
			;;
		--step)
			step=$2
			shift 2
			;;
		--Nevent)
			Nevent=$2
			shift 2
			;;
		--Name)
			Name=$2
			shift 2
			;;
	esac
done 
	

#Create macro and write it
#first loop in thickness

echo "/run/initialize" > macro_script.in
echo "/gun/particle proton" >> macro_script.in
echo "/setTarget/sensitiveThickness $Thick um" >> macro_script.in
for ((j = $Emin; j <= $Emax; j+=$step)); do  
  echo "/gun/energy $j MeV" >> macro_script.in
  echo "/run/beamOn $Nevent" >> macro_script.in
done


#run geant4 simulation

./SiDetector macro_script.in
mv result/data.dat result/${Name}.dat
