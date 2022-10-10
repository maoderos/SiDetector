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

  eventID = -1;

}


G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) 
{
    //cout << "Proccessing hit "<< endl;
    G4double energyDep = step->GetTotalEnergyDeposit();
    const G4ParticleDefinition* particle = step->GetTrack()->GetParticleDefinition();
    G4int trackID = step->GetTrack()->GetTrackID();
    G4int eventNumber = G4RunManager::GetRunManager()-> GetCurrentEvent()->GetEventID();
    
    Hits* hit = new Hits();
    
    // Energy deposit by primary particle in sensitive volume:
    if (trackID == 1) {
        hit->AddEnergyDeposit(energyDep);
        //cout << "Edep(SD) (MeV): " << energyDep/MeV << endl;
        if (eventNumber != eventID) {
          eventID = eventNumber;
          hit->AddEventID();
        }
    }
    //Get electron when it is generated
    if (particle == G4Electron::Electron() && step->IsFirstStepInVolume()) {
      hit->AddNumberOfElectrons();
    }
    /* Verbose test
    if (particle == G4Electron::Electron()){
      cout << "Electron: " << trackID << endl;
    }
    */
    //cout << step->GetPostStepPoint()->GetPhysicalVolume()->GetName() << endl;
    // If electron is entering the metal volume
    if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && 
        step->GetPostStepPoint()->GetPhysicalVolume()->GetName() == "Metal"
        && particle == G4Electron::Electron()) {
            //cout << "Electrons Hit in Metal" << endl;
            hit->AddNumberOfElectronsInMetal();
       }

    hitsCollection->insert(hit);
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent* hce)
{
    
}