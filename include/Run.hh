#ifndef RUN_h
#define RUN_h 1

#include "G4Run.hh"

class G4Event;
class G4TransportationManager;
class G4ParticleDefinition;

class Run: public G4Run {
    public:
    Run();
    virtual ~Run();

    virtual void RecordEvent(const G4Event*);
    virtual void Merge(const G4Run*);

    //void SetPrimaryInformation(G4ParticleDefinition* particle, G4double eKin,  G4double angle);
    void SetPrimaryInformation(G4ParticleDefinition* particle, G4double eKin);

    inline G4double GetEDep(){return energyDeposit;};
    inline G4double GetSquaredEDep(){return squaredEnergyDeposit;};
    inline G4int GetNEvent(){return nEvent;};
    inline G4ParticleDefinition* GetPrimaryParticle() { return particle; };
    inline G4double GetKineticEnergy() {return kineticEnergy;};
 
    private:
    G4double energyDeposit;
    G4double squaredEnergyDeposit;
    G4int nEvent;

    G4int hid;

    G4ParticleDefinition* particle;
    G4double kineticEnergy;
    //G4double beamAngle;

};

#endif