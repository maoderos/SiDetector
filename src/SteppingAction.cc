#include "SteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include "G4SteppingManager.hh"
#include <sstream>
#include <iostream>

using namespace std;

SteppingAction::SteppingAction(): G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4double eDep = step->GetTotalEnergyDeposit()/MeV;
  //std::cout << eDep/MeV << std::endl;
  G4double z = step->GetPreStepPoint()->GetPosition().z();
  G4double particleEnergy = step->GetPostStepPoint()->GetKineticEnergy(); 
  G4double stepLength = step->GetStepLength();

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  //If Particle is inside the sensitive volume
  if (step->GetTrack()->GetVolume()->GetName() == "Sensitive" && step->GetTrack()->GetParentID() == 0) {
    if (!step->IsFirstStepInVolume()) {
      analysisManager->FillH1(0, z/um, (eDep/eV)/(stepLength/angstrom));
    }
    if(step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
      analysisManager->FillNtupleDColumn(0, particleEnergy);
      analysisManager->AddNtupleRow();  

    }
    
       
  }


  
}
