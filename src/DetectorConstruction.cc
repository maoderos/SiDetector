#include "DetectorConstruction.hh"
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
: G4VUserDetectorConstruction(), logicWorld(0),physWorld(0), solidTarget(0),
logicTarget(0), physTarget(0), worldMaterial(0), targetMaterial(0), Air(0)
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

    worldMaterial = nist->FindOrBuildMaterial("G4_Galactic");
    Air = nist->FindOrBuildMaterial("G4_AIR");
    targetMaterial = Air;

}

G4VPhysicalVolume* DetectorConstruction::Construct(){

    //World Geometry

    G4double halfsize_xy = 2.5*m;
    G4double halfsize_z = 3*m;

    G4Box* solidWorld = new G4Box("BoxWorld", halfsize_xy, halfsize_xy, halfsize_z);
    logicWorld = new G4LogicalVolume(solidWorld, worldMaterial,"logicWorld");
    physWorld = new G4PVPlacement(0,G4ThreeVector(), logicWorld, "World",0,false,0);

    // Target geometry

    G4double halfSize_xy_target = halfsize_z;
    G4double halfsize_z_target = halfsize_z - 0.1*m;
    solidTarget = new G4Box("BoxTarget", halfSize_xy_target/2, halfSize_xy_target/2, halfsize_z_target/3);

    logicTarget = new G4LogicalVolume(solidTarget, targetMaterial, "logicTarget");
    physTarget = new G4PVPlacement(0,
                                    G4ThreeVector(0,0,1.5*m),
                                    logicTarget,
                                    "PhysTarget",
                                    logicWorld,
                                    false,
                                    0,
                                    true);



    // World Visualization
    G4VisAttributes* worldVis = new G4VisAttributes();
    worldVis->SetColor(0.1, 0.3, 0.8, 0.5);
    logicWorld->SetVisAttributes(worldVis);

    // Target Visualization
    G4VisAttributes* TargetVis = new G4VisAttributes();
    TargetVis->SetColor(0.6, 0.6, 0.1, 0.1);
    logicTarget->SetVisAttributes(TargetVis);

    return physWorld;

}

void DetectorConstruction::ConstructSDandField()
{
    G4cout << "Space left to set sensitive detectors and field (electromagnetic)" << G4endl;
}

void DetectorConstruction::SetTargetGeometry(const G4String& value){
  /*
  targetGeometry = value;
  if (logicTarget) {
    if (targetGeometry == logicTarget->GetSolid()->GetName()){
      G4cout << "Geometry already selected in simulation." << G4endl;
    } else {
      G4MTRunManager::GetRunManager()->ReinitializeGeometry();
    }
  }
  */
  G4cout << "changing target geometry" << G4endl;
}

void DetectorConstruction::SetSensitiveThickness(G4double value){
  //sensitiveThickness = value;
  //G4MTRunManager::GetRunManager()->ReinitializeGeometry();
  G4cout << "changing sensitive volume thickness" << G4endl;
}

G4bool DetectorConstruction::SetSensitiveMaterial(const G4String& value) {
  G4cout << "changing sensitive volume material" << G4endl;
}
