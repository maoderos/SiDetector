#include "Run.hh"
#include "Hits.hh"
#include "G4MTRunManager.hh"
#include "G4SDManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

using namespace std;

//Run::Run():G4Run(), energyDeposit(0), squaredEnergyDeposit(0), hid(-1), particle(0), kineticEnergy(0), beamAngle(0) {
Run::Run():G4Run(), energyDeposit(0), squaredEnergyDeposit(0), electronsInSensitive(0), electronsInMetal(0), nEvent(0), hid(-1), particle(0), kineticEnergy(0) {
    G4SDManager* SDman = G4SDManager::GetSDMpointer();
    hid = SDman->GetCollectionID("DetectorCollection");

}

Run::~Run() {

}

void Run::RecordEvent(const G4Event* evt) {
    G4HCofThisEvent* hce = evt->GetHCofThisEvent();
    if (!hce) return;
    HitsCollection* hitsColl = static_cast<HitsCollection*>(hce->GetHC(hid));
    // If hit collection is not found
    if(!hitsColl) {
    G4ExceptionDescription msg;
    msg << "Some hits collections of this event not found.\n";
    G4Exception("Run::RecordEvent()", "Code001", JustWarning, msg);
    return;
    }

    for(size_t i=0;i<hitsColl->GetSize();i++) {
        energyDeposit += ((*hitsColl)[i]->GetEnergyDeposit());
        //cout << "Edep(RUN) (MeV): " << ((*hitsColl)[i]->GetEnergyDeposit())/MeV << endl;
        squaredEnergyDeposit += ((*hitsColl)[i]->GetEnergyDeposit())*((*hitsColl)[i]->GetEnergyDeposit());
        electronsInSensitive += ((*hitsColl)[i]->GetNumberOfElectrons());
        electronsInMetal += ((*hitsColl)[i]->GetNumberOfElectronsInMetal());
        nEvent += ((*hitsColl)[i]->GetNumberOfEvent());
    
    }
}

void Run::Merge(const G4Run* aRun){
    const Run* localRun = static_cast<const Run*>(aRun);

    energyDeposit += localRun->energyDeposit;
    squaredEnergyDeposit += localRun->squaredEnergyDeposit;
    electronsInSensitive += localRun->electronsInSensitive;
    electronsInMetal += localRun->electronsInMetal;
    nEvent += localRun->nEvent;
    
    // transfer primary information in local to master
    particle= localRun->particle;
    kineticEnergy = localRun->kineticEnergy;
    //beamAngle = localRun->beamAngle;
    G4Run::Merge(aRun);
}

//void Run::SetPrimaryInformation(G4ParticleDefinition* primaryParticle, G4double eKin,  G4double angle) {
void Run::SetPrimaryInformation(G4ParticleDefinition* primaryParticle, G4double eKin) {

    particle  = primaryParticle;
    kineticEnergy = eKin;
    //beamAngle = angle;
}