

#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "Hits.hh"

class G4String;
class G4Step;
class G4TouchableHistory;
class G4HCofThisEvent;

class SensitiveDetector: public G4VSensitiveDetector 
{
    public:
    //constructor
    SensitiveDetector(const G4String& SDname, const G4String& hitsCollectionName);
    //destructor
    ~SensitiveDetector();

    G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    void Initialize(G4HCofThisEvent* hce);
    void EndOfEvent(G4HCofThisEvent* hce);

    private:
    HitsCollection* hitsCollection;
    int hcid;
    int eventID;

};

#endif