#ifndef RUNACTION_h
#define RUNACTION_h 1


#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"

class DetectorConstruction;
class Run;

class RunAction: public G4UserRunAction
{
    public:
        RunAction();
        ~RunAction();

        virtual G4Run* GenerateRun();

        void BeginOfRunAction(const G4Run*);
        void EndOfRunAction(const G4Run*);
    
    private:
        void BookHisto();
  
        const DetectorConstruction* detectorConstruction;
        G4AnalysisManager* analysisManager;
        Run* run;
}; 

#endif
