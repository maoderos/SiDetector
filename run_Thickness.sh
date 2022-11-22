#!/bin/bash
#default versions
E=10;
Dmin=25;
Dmax=50;
Estep=1;
Dstep=1;
Nevent=100;
Name=data-renamed;

Help()
{

	cat <<-END
	${0##*/}: Script for running SiDetector
	Usage:
		--E <value(MeV)>
			Particle energy. (default: 1 MeV)
		
		--Dmin <value(um)>
			Minimum sensitive thickness. (default: 25 um)
			
		--Dmax <value(um)>
			Maximum sensitive thickness. (default: 50 um)
			
		--step <value>
			step in for loop. (default: 1 um)
		
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
	  	--E)
			E=$2
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
echo "/gun/energy $E MeV" >> macro_script.in
for ((i = $Dmin; i <= $Dmax; i+=$step)); do
  echo "/setTarget/sensitiveThickness $i um" >> macro_script.in
  echo "/run/beamOn $Nevent" >> macro_script.in
  echo "/control/shell mv bragg_output.out EdepProfile_${j}MeV_${Thick}um.out" >> macro_script.in
  echo "/control/shell mv braggPeak.root EdepProfile_${j}MeV_${Thick}um.root" >> macro_script.in
done


#run geant4 simulation

./SiDetector macro_script.in
mv result/data.dat result/${Name}.dat
