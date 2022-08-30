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

// Constructor
DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(), logicWorld(0),physWorld(0), worldMaterial(0), solidMetal(0), logicMetal(0), physMetal(0),metalMaterial(0), metalThickness(10.*um/2),
solidSensitive(0), logicSensitive(0), physSensitive(0), sensitiveMaterial(0), sensitiveThickness(25.*um/2), solidSubstrate(0), logicSubstrate(0),
physSubstrate(0), substrateMaterial(0), substrateThickness(15.*um/2), Air(0), Silicon(0), Metal(0), targetGeometry("Chip"), chip_half_x(1.5*cm/2), chip_half_y(3.0*cm/2)
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

    sensitiveMaterial = Air;
    substrateMaterial = Silicon;

    // One can add the Silicon Carbide material here using the same logic

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



    return physWorld;

}

void DetectorConstruction::ConstructSDandField()
{
    G4cout << "Space left to set sensitive detectors and field (electromagnetic)" << G4endl;
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
  G4MTRunManager::GetRunManager()->ReinitializeGeometry();
}

G4bool DetectorConstruction::SetSensitiveMaterial(const G4String& value) {
  if (value == "Si") {
    sensitiveMaterial = Silicon;
  } // Add SiC here in an else-if
  if (logicSensitive){
    logicSensitive->SetMaterial(sensitiveMaterial);
    G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
    G4MTRunManager::GetRunManager()->GeometryHasBeenModified();
    G4cout << "The material of the sensitive volume has been changed to " << sensitiveMaterial << G4endl;
  }
  return true;
}
