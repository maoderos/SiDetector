#include "SteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"
#include <sstream>
#include <iostream>

using namespace std;

SteppingAction::SteppingAction(): G4UserSteppingAction()
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4double eDep = step->GetTotalEnergyDeposit();
  G4double z = step->GetPreStepPoint()->GetPosition().z();
  if (eDep <= 0) return;
  //If Particle is inside the sensitive volume
  if (step->GetTrack()->GetVolume()->GetName() == "Sensitive" && step->GetTrack()->GetTrackID() == 1) {
    //std::cout << "Step is inside the sensitive volume !" << std::endl;
    // cout << "Starting analysis manager" << endl;
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //cout << "Ending analysis manager1" << endl;
    
    analysisManager->FillH1(0, z/um, eDep/MeV);
    //cout << "Ending analysis manager2" << endl;
    //analysisManager->FillH1(2, x, edep);
    // Append to file
    ofstream file;
    file.open("bragg_output.out", std::ios_base::app);
    file << eDep/(MeV) << "," << z/(um) << G4endl;  
       
  }


  
}
