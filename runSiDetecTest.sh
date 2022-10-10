#!/bin/bash

while getopts "n:" opt; do
  case $opt in
    n) nome=$OPTARG;;
  esac
done

for ((j = 100; j <= 500; j+=250));
do 
perl -i -pe 's/eng/"$j"/g' init0.in 
./SiDetector ini0.in 
done


mv result/data.dat result/$nome.dat
