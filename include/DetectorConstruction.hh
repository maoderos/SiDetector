#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "G4Version.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "DetectorMessenger.hh"
#include "G4EqMagElectricField.hh"
#include "G4UniformElectricField.hh"
#include "G4DormandPrince745.hh"
#include "G4ChordFinder.hh"

class G4Region;

class DetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        DetectorConstruction();
        virtual ~DetectorConstruction();

        virtual G4VPhysicalVolume* Construct();

        virtual void ConstructSDandField();

        void SetTargetGeometry(const G4String& value);
        void SetSensitiveThickness(G4double value);
        G4bool SetSensitiveMaterial(const G4String& value);

        inline G4double GetSensitveThickness() const {return sensitiveThickness;};


    private:
        void DefineMaterials();

        DetectorMessenger* detectorMessenger;

        // World
        G4LogicalVolume* logicWorld;
        G4VPhysicalVolume* physWorld;
        G4Material* worldMaterial;

        // Metal
        G4Box* solidMetal;
        G4LogicalVolume* logicMetal;
        G4VPhysicalVolume* physMetal;
        G4Material* metalMaterial;
        G4double metalThickness;

        // Sensitive voluem
        G4Box* solidSensitive;
        G4LogicalVolume* logicSensitive;
        G4VPhysicalVolume* physSensitive;
        G4Material* sensitiveMaterial;
        G4double sensitiveThickness;

        //Substrate volume
        G4Box* solidSubstrate;
        G4LogicalVolume* logicSubstrate;
        G4VPhysicalVolume* physSubstrate;
        G4Material* substrateMaterial;
        G4double substrateThickness;

        // Materials
        G4Material* Air;
        G4Material* Silicon;
        G4Material* Metal;

        // Messenger parameter
        G4String targetGeometry;

        //Chip constants
        G4double chip_half_x;
        G4double chip_half_y;

        //field
        G4ElectricField* pEMfield;
        G4EqMagElectricField* pEquation;
        G4ChordFinder* pChordFinder;
        G4double volts; // in Volts
        G4double length; // in um

        G4Region* fRegion;
};
#endif
