#include "RunAction.hh"
#include "Run.hh"
#include "G4MTRunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmCalculator.hh"
#include <sstream>
#include <filesystem>
#include <iostream>

using namespace std;

RunAction::RunAction() : G4UserRunAction(), detectorConstruction(0)
{}

RunAction::~RunAction()
{}

G4Run* RunAction::GenerateRun(){
    cout << "Generating Run" << endl;
    run = new Run();
    cout << "Run generated successfully" << endl;
    return run;
}

void RunAction::BeginOfRunAction(const G4Run* aRun) {
    cout << "Begin of RunAction" << endl;
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
    //G4MTRunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(numOfEvent*0.1));

    const PrimaryGeneratorAction* primary =
            dynamic_cast<const PrimaryGeneratorAction*>(G4MTRunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    
    if (primary == nullptr) return;
    // add info to Run object
    run->SetPrimaryInformation(primary->GetParticleGun()->GetParticleDefinition(),  primary->GetParticleGun()->GetParticleEnergy());
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    cout << "End of RunAction" << endl;
    if (!IsMaster()) return; // if is not the master run, return 

    // Create a directory for storing the data 
    stringstream resultFolder;
    resultFolder << "./result/";
    
    // if forlder does not exist, create one
     if(!filesystem::exists(resultFolder.str().c_str())){
        G4String command_mkdir = "mkdir -p " + resultFolder.str();
        int mkdirFlag = system(command_mkdir.c_str());
        if(mkdirFlag == -1){
        G4cerr << "mkdir command is not performed in your system!!" << G4endl;
        }
     }

    // Get information from DetectorConstruction class
    detectorConstruction = static_cast<const DetectorConstruction*> (G4MTRunManager::GetRunManager()->GetUserDetectorConstruction());
    
    ofstream ofp;
    stringstream resultFileName;

    resultFileName << resultFolder.str() << "data.dat";

    // If file does not exist, create file and add header
    if(!filesystem::exists(resultFileName.str().c_str())){
        ofp.open(resultFileName.str().c_str());
        ofp << "Thickness(um) Edep(MeV) Error(%) KinecticEnergy(MeV)" << G4endl;
    } else {
        ofp.open(resultFileName.str().c_str(),ios_base::app);
    }

    Run* theRun = (Run*)aRun;
    //Calculate mean Edep and error

    G4double eDep = theRun->GetEDep()/(MeV);
    G4double nEvent = theRun->GetNEvent();
    G4double squaredEdep = theRun->GetSquaredEDep();
    G4double meanEDep = eDep/nEvent;
    G4double varianceEdep = (squaredEdep/nEvent - (meanEDep*meanEDep)/nEvent)/(nEvent-1);
    G4double primaryEnergy = theRun->GetKineticEnergy()/(MeV);

    ofp << detectorConstruction->GetSensitveThickness()/(um) << " " << meanEDep/(MeV) << " " 
        << sqrt(sqrt(varianceEdep*varianceEdep)) << " " << primaryEnergy << G4endl;

}


