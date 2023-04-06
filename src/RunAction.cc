#include "RunAction.hh"
#include "Run.hh"
#include "G4Run.hh"
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

RunAction::RunAction() : G4UserRunAction(), detectorConstruction(0), analysisManager(0)
{

  // Get information from DetectorConstruction class
  detectorConstruction = static_cast<const DetectorConstruction*> (G4MTRunManager::GetRunManager()->GetUserDetectorConstruction());
  analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);  // enable inactivation of histograms
  G4int nbins = 100;
  G4double sensitiveThickness = 50*um;
  analysisManager->CreateH1("Edep (eV/angstrom)","Edep in sensitive volume", nbins,(2*detectorConstruction->GetMetalThickness())/(um),(2*detectorConstruction->GetMetalThickness()/um + sensitiveThickness/(um)));
  analysisManager->CreateNtuple("SiDetector", "physics");
  analysisManager->SetNtupleMerging(true); //So that all is joined in one file
  analysisManager->CreateNtupleDColumn("LeavingParticleEnergy");
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{}
/*
G4Run* RunAction::GenerateRun(){
    cout << "Generating Run" << endl;
    run = new Run();
    cout << "Run generated successfully" << endl;
    return run;
}
*/
void RunAction::BeginOfRunAction(const G4Run* aRun) {

  if(IsMaster()) {
    cout << "Begin of RunAction" << endl;
    G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

    //Create output file for bragg peak
    ofstream file;
    file.open("bragg_output.out");
    file << "Edep(MeV),z(um)" << G4endl;
    file.close();
  }
  //BookHisto(aRun);   
  // G4MTRunManager::GetRunManager()->SetPrintProgress(static_cast<G4int>(numOfEvent*0.1));
  G4int runId = aRun->GetRunID();
  std::stringstream strRunId;
  strRunId << runId;
  G4String fileName =  "output_" + strRunId.str() + ".root"; 
  analysisManager->OpenFile(fileName);

    const PrimaryGeneratorAction* primary =
            dynamic_cast<const PrimaryGeneratorAction*>(G4MTRunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
    
    if (primary == nullptr) return;
    // add info to Run object
//    run->SetPrimaryInformation(primary->GetParticleGun()->GetParticleDefinition(),  primary->GetParticleGun()->GetParticleEnergy());
    cout << "Finished BeginOfRunAction" << endl;
}

void RunAction::EndOfRunAction(const G4Run* aRun) {
    cout << "End of RunAction" << endl;
    G4int nbofEvents = aRun->GetNumberOfEvent();
      //Normalize Histograms
    //if (nbofEvents == 0) {
      G4double binWidth = analysisManager->GetH1Width(0);
      std::cout << nbofEvents << std::endl;
      std::cout << binWidth << std::endl;
      G4double fac = 1/(nbofEvents*binWidth);
      if (IsMaster()) analysisManager->ScaleH1(0, fac);

    //}
      analysisManager->Write();
      analysisManager->CloseFile();
    if (!IsMaster()) return; // if is not the master run, return 

    //Close bragg peak output file
    ofstream file;
    file.open("bragg_output.out", std::ios_base::app);
    file.close();
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

    
    ofstream ofp;
    stringstream resultFileName;

    resultFileName << resultFolder.str() << "data.dat";

    // If file does not exist, create file and add header
    if(!filesystem::exists(resultFileName.str().c_str())){
        ofp.open(resultFileName.str().c_str());
        ofp << "Thickness(um) Edep(MeV) Error(%) KinecticEnergy(MeV) ElectronsInSi ElectronsInMetal ElectronHolePairs" << G4endl;
    } else {
        ofp.open(resultFileName.str().c_str(),ios_base::app);
    }

    Run* theRun = (Run*)aRun;
    //Calculate mean Edep and error

    G4double eDep = theRun->GetEDep()/(MeV);
    G4double nEvent = theRun->GetNEvent();
    G4double squaredEdep = theRun->GetSquaredEDep();
    G4int electronsInSensitive = theRun->GetNumberOfElectronsInSensitive();
    G4int electronsInMetal = theRun->GetNumberOfElectronsInMetal();
    G4double meanEDep = eDep/nEvent;
    G4int electronHolePairs = theRun->GetNumberOfElectronHolePairs();
    //G4double electronsInSensitivePerEvent = electronsInSensitive/nEvent;
    //G4double electronsInMetalPerEvent = electronsInMetal/nEvent;
    G4double varianceEdep = (squaredEdep/nEvent - (meanEDep*meanEDep)/nEvent)/(nEvent-1);
    G4double primaryEnergy = theRun->GetKineticEnergy()/(MeV);

    ofp << detectorConstruction->GetSensitiveThickness()/(um) << " " << meanEDep/(MeV) << " " 
        << sqrt(sqrt(varianceEdep*varianceEdep)) << " " << primaryEnergy << " " << electronsInSensitive << " " << electronsInMetal << " " << electronHolePairs << " " << G4endl;

    ofp.close();

}

void RunAction::BookHisto(const G4Run* aRun) {
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);  // enable inactivation of histograms
  G4int nbins = 100;
  analysisManager->CreateH1("Edep","Edep in sensitive volume", nbins,(2*detectorConstruction->GetMetalThickness())/(um),(2*detectorConstruction->GetMetalThickness() + 2*detectorConstruction->GetSensitiveThickness())/(um));
  
  analysisManager->CreateNtuple("SiDetector", "physics");
  analysisManager->SetNtupleMerging(true); //So that all is joined in one file
  analysisManager->CreateNtupleDColumn("flagParticle");
  analysisManager->CreateNtupleDColumn("flagProcess");
  analysisManager->FinishNtuple();
  G4int runId = aRun->GetRunID();
  std::stringstream strRunId;
  strRunId << runId;
  G4String fileName =  "output_" + strRunId.str() + ".root"; 
  analysisManager->OpenFile(fileName);
  
}


