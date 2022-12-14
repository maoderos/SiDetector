#include <iostream>
#include "DetectorConstruction.hh"
#include "G4MTRunManager.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4Element.hh"
#include "G4Box.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "SensitiveDetector.hh"
#include "G4FieldManager.hh"
#include "G4ElectricField.hh"
#include "G4MagneticField.hh"
#include "G4TransportationManager.hh"
#include "G4IntegrationDriver.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"


using namespace std;

//G4ThreadLocal G4UniformElectricField* DetectorConstruction::fEMfield = 0;
// Constructor
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), logicWorld(0),physWorld(0), worldMaterial(0), solidMetal(0), logicMetal(0), physMetal(0),metalMaterial(0), metalThickness(10.*um/2),
solidSensitive(0), logicSensitive(0), physSensitive(0), sensitiveMaterial(0), sensitiveThickness(25.*um/2), solidSubstrate(0), logicSubstrate(0),
physSubstrate(0), substrateMaterial(0), substrateThickness(15.*um/2), Air(0), Silicon(0), Metal(0), targetGeometry("Chip"), chip_half_x(1.5*cm/2), chip_half_y(3.0*cm/2),
pEMfield(0), pEquation(0), pChordFinder(0), volts(3.0), length(24)
{
    detectorMessenger = new DetectorMessenger(this);
    DefineMaterials();
}

// Destructor
DetectorConstruction::~DetectorConstruction(){
  delete detectorMessenger;
}

void DetectorConstruction::DefineMaterials(){

    // Get materials from NIST database
    G4NistManager* nist = G4NistManager::Instance();

    // Creating world material - Vaccumm
    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");

    // Air
    Air = nist->FindOrBuildMaterial("G4_AIR");

    // Metal
    G4Material* Al = nist->FindOrBuildMaterial("G4_Al");
    Silicon = nist->FindOrBuildMaterial("G4_Si");
    G4Material* Oxygen = nist->FindOrBuildMaterial("G4_O");

    metalMaterial = new G4Material("Metal", 2.28*g/cm3, 3); // (name, density, number of components)
    metalMaterial->AddMaterial(Al, 17*perCent);
    metalMaterial->AddMaterial(Silicon, 38.8*perCent);
    metalMaterial->AddMaterial(Oxygen, 44.2*perCent);

    sensitiveMaterial = Silicon;
    substrateMaterial = Silicon;

    // Electron hole pair energy for materials
    electronHolePairEnergySi = 3.4 * eV;
    electronHolePairEnergy = electronHolePairEnergySi;

}

G4VPhysicalVolume* DetectorConstruction::Construct(){

    //World Geometry
    G4double halfsize_xy = 4*cm/2;
    G4double halfsize_z = 2*cm/2;

    G4Box* solidWorld = new G4Box("BoxWorld", halfsize_xy, halfsize_xy, halfsize_z);
    logicWorld = new G4LogicalVolume(solidWorld, worldMaterial,"logicWorld");
    physWorld = new G4PVPlacement(0,G4ThreeVector(), logicWorld, "World",0,false,0);

    // Target geometry
    if (targetGeometry == "Chip") {
      G4cout << "Drawing chip" << G4endl;
      G4double z_position = metalThickness;
      // Metal volume
      solidMetal = new G4Box("boxMetal", chip_half_x, chip_half_y, metalThickness);
      logicMetal = new G4LogicalVolume(solidMetal, metalMaterial, "logicMetal");
      physMetal = new G4PVPlacement(0,G4ThreeVector(0,0,z_position), logicMetal, "Metal", logicWorld, false, 0, true);
      // Sensitive volume
      z_position += metalThickness + sensitiveThickness;
      solidSensitive = new G4Box("boxSensitive", chip_half_x, chip_half_y, sensitiveThickness);
      logicSensitive = new G4LogicalVolume(solidSensitive, sensitiveMaterial, "logicSensitive");
      physSensitive = new G4PVPlacement(0,G4ThreeVector(0,0,z_position), logicSensitive, "Sensitive", logicWorld, false, 0, true);
      //Substrate
      z_position += sensitiveThickness + substrateThickness;
      solidSubstrate = new G4Box("boxSubstrate", chip_half_x, chip_half_y, substrateThickness);
      logicSubstrate = new G4LogicalVolume(solidSubstrate, substrateMaterial, "logicSubstrate");
      physSubstrate = new G4PVPlacement(0,G4ThreeVector(0,0,z_position), logicSubstrate, "Substrate", logicWorld, false, 0, true);

      // Metal and substrate visualization
      G4VisAttributes* metalVis = new G4VisAttributes();
      metalVis->SetColor(1.0, 1.0, 0); //yellow
      logicMetal->SetVisAttributes(metalVis);

      G4VisAttributes* substrateVis = new G4VisAttributes();
      substrateVis->SetColor(0.0, 1.0, 0);
      logicSubstrate->SetVisAttributes(substrateVis);


    } else if (targetGeometry == "Material") {
      G4cout << "Drawing Material" << G4endl;

      G4double z_position = sensitiveThickness;
      solidSensitive = new G4Box("boxSensitive", chip_half_x, chip_half_y, sensitiveThickness);
      logicSensitive = new G4LogicalVolume(solidSensitive, sensitiveMaterial, "logicSensitive");
      physSensitive = new G4PVPlacement(0,G4ThreeVector(0,0,z_position), logicSensitive, "Sensitive", logicWorld, false, 0, true);

    }

    // World Visualization
    G4VisAttributes* worldVis = new G4VisAttributes();
    worldVis->SetColor(0.1, 0.3, 0.8, 0.5);
    logicWorld->SetVisAttributes(worldVis);

    //Sensitive visualization
    G4VisAttributes* sensitiveVis = new G4VisAttributes();
    sensitiveVis->SetColor(1.0, 0.0, 0); //red
    logicSensitive->SetVisAttributes(sensitiveVis);


    // Create target region
    fRegion = new G4Region("Target");

    G4ProductionCuts* cuts = new G4ProductionCuts();
    G4double defCut = 1*nanometer;
    cuts->SetProductionCut(defCut, "gamma");
    cuts->SetProductionCut(defCut, "e-");
    cuts->SetProductionCut(defCut, "e+");
    cuts->SetProductionCut(defCut, "proton");

    fRegion->SetProductionCuts(cuts);
    fRegion->AddRootLogicalVolume(logicSensitive);
    

    return physWorld;

}

void DetectorConstruction::ConstructSDandField()
{
  if (pEMfield) {
    G4cout << "Existing Electric field found, deleting it" << G4endl;
    delete pEMfield;
    delete pEquation;
    delete pChordFinder;
  }
  
  if (G4SDManager::GetSDMpointer()->FindSensitiveDetector("Detector",0)) delete G4SDManager::GetSDMpointer()->FindSensitiveDetector("Detector");
  G4SDManager* sdMan = G4SDManager::GetSDMpointer();
  SensitiveDetector* sd = new SensitiveDetector("Detector", "DetectorCollection", this);
  sdMan->AddNewDetector(sd);
  SetSensitiveDetector(logicSensitive,sd);
  cout << "Sensitive Detector created" << endl;

  /*
  pEMfield = new G4UniformElectricField(G4ThreeVector(0.0,0.0,(volts/length)*volt/um));

  // Create an equation of motion for this field
  pEquation = new G4EqMagElectricField(pEMfield);

  G4int nvar = 8;

  // Create the Runge-Kutta 'stepper' using the efficient 'DoPri5' method
  auto pStepper = new G4DormandPrince745( pEquation, nvar );

  G4double minStep = 0.010*um ; // minimal step of 0.010 um

  // The driver will ensure that integration is control to give
  //   acceptable integration error
  auto pIntgrationDriver = new G4IntegrationDriver<G4DormandPrince745>(minStep,
                                                 pStepper,
                                                 nvar);

  pChordFinder = new G4ChordFinder(pIntgrationDriver);

  // Get the field manager
  auto fieldManager= new G4FieldManager();
  fieldManager->SetChordFinder( pChordFinder );

  // Set this field to the global field manager
  fieldManager->SetDetectorField( pEMfield );
  logicSensitive->SetFieldManager(fieldManager, true);
  cout << "Electric Field created" << endl;
  */
}

void DetectorConstruction::SetTargetGeometry(const G4String& value){
  if (logicMetal) {
    if (targetGeometry == value){
      G4cout << "Geometry already selected in simulation." << G4endl;
    } else {
      G4cout << "changing target geometry" << G4endl;
      targetGeometry = value;
      G4MTRunManager::GetRunManager()->ReinitializeGeometry();
    }
  }
}

void DetectorConstruction::SetSensitiveThickness(G4double value){
  sensitiveThickness = value/2;
  G4cout << "changing sensitive volume thickness" << G4endl;
  delete fRegion;
  G4MTRunManager::GetRunManager()->ReinitializeGeometry();
}

G4bool DetectorConstruction::SetSensitiveMaterial(const G4String& value) {
  if (value == "Si") {
    sensitiveMaterial = Silicon;
    electronHolePairEnergy = electronHolePairEnergySi;
  } // Add SiC here in an else-if
  if (logicSensitive){
    logicSensitive->SetMaterial(sensitiveMaterial);
    G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
    G4MTRunManager::GetRunManager()->GeometryHasBeenModified();
    G4cout << "The material of the sensitive volume has been changed to " << sensitiveMaterial << G4endl;
  }
  return true;
}
