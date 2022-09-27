#include <iostream>
#include "SensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4Electron.hh"

using namespace std;

SensitiveDetector::SensitiveDetector(const G4String& name, const G4String& hitsCollectionName)
: G4VSensitiveDetector(name), hitsCollection(0), hcid(-1)
{
  collectionName.insert(hitsCollectionName);
  cout << "Creating Sensitive Detector named: " << name << endl;


}

SensitiveDetector::~SensitiveDetector() {}

void SensitiveDetector::Initialize(G4HCofThisEvent* hce) 
{
  // Create hitsCollection
  hitsCollection = new HitsCollection(SensitiveDetectorName, collectionName[0]);
  if (hcid<0) hcid = G4SDManager::GetSDMpointer()->GetCollectionID(hitsCollection);
  hce->AddHitsCollection(hcid, hitsCollection);
}


G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) 
{
    //cout << "Proccessing hit "<< endl;
    G4double energyDep = step->GetTotalEnergyDeposit();
    const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();
    G4int trackID = step->GetTrack()->GetTrackID();
    
    Hits* hit = new Hits();
    
    // Energy deposit by primary particle in sensitive volume:
    if (trackID == 1) {
        hit->AddEnergyDeposit(energyDep);
    }
    
    // If electron is entering the metal volume
    if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && 
        step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Metal"
        && particle == G4Electron::Electron()) {
            //cout << "Electrons Hit in Metal" << endl;
            hit->AddNumberOfElectronsinMetal();
       }

    hitsCollection->insert(hit);
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* hce)
{
    
}