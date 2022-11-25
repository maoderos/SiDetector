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
  G4double flagParticle=0.;
  G4double flagProcess=0.;
  G4double eDep = step->GetTotalEnergyDeposit();
  G4double z = step->GetPreStepPoint()->GetPosition().z();
  if (step->GetTrack()->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "e-") flagParticle = 1;    
  if (step->GetTrack()->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "proton") flagParticle = 2;
  if (step->GetTrack()->GetDynamicParticle()->GetDefinition() ->GetParticleName() == "GenericIon") flagParticle = 3;

  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="msc") flagProcess = 1;
  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="eIoni") flagProcess = 2;
  if (step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()=="hIoni") flagProcess = 3;
   
  if (eDep <= 0) return;

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // fill ntuple
  analysisManager->FillNtupleDColumn(0, flagParticle);
  analysisManager->FillNtupleDColumn(1, flagProcess);
  analysisManager->AddNtupleRow();  
  //If Particle is inside the sensitive volume
  if (step->GetTrack()->GetVolume()->GetName() == "Sensitive" && step->GetTrack()->GetParentID() == 0) {
    if (step->IsFirstStepInVolume()) return;
     
    analysisManager->FillH1(0, z/um, eDep/MeV);

    ofstream file;
    file.open("bragg_output.out", std::ios_base::app);
    file << eDep/(MeV) << "," << z/(um) << G4endl;  
       
  }


  
}
