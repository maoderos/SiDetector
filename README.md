# SiDetector

Simulation of a silicon detector for educational purposes

## How to run

At first, one must have Geant4 installed. Load the Geant4 environment:

``source <path>/bin/geant4.sh``

Inside the project folder, create another folder to build the application and enter in it:

``$ mkdir build && cd build``

``$ cmake ../ && make``

To enter in the simulation, type:

``$ ./SiDetector``

## Changing simulation parameters in the user interface


1) It is possible to change the geometry to Chip (metal + sensitive + substrate) or Material (sensitive)

``/setTarget/geometry Chip/Material``

2) Change the sensitive volume material (Si or SiC):

``/setTarget/sensitiveMaterial Si``

3) Change the sensitive volume thickness:

``/setTarget/sensitiveThickness value unit``
