#ifndef HIT_h
#define HIT_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4Threading.hh"

class Hits: public G4VHit 
{
    public:
    Hits();
    virtual ~Hits();
    
    // Operators
    inline void *operator new(size_t);
    inline void operator delete(void*);

    inline void AddEnergyDeposit(double e) { energyDeposit += e; }
    inline void AddNumberOfElectrons() {numberOfElectrons += 1;}
    inline void AddNumberOfElectronsInMetal() {numberOfElectronsInMetal += 1;}
    inline void AddNumberOfElectronHolePairs(int NElectronHolePairs) {numberOfElectronHolePairs += NElectronHolePairs;}
    inline void AddEventID() { eventID += 1; }
    inline double GetEnergyDeposit() {return energyDeposit;}
    inline int GetNumberOfElectrons() {return numberOfElectrons;}
    inline int GetNumberOfElectronsInMetal() {return numberOfElectronsInMetal;}
    inline int GetNumberOfEvent() { return eventID; }
    inline int GetNumberOfElectronHolePairs() {return numberOfElectronHolePairs;}

    private:
    int numberOfElectrons;
    int numberOfElectronsInMetal;
    int numberOfElectronHolePairs;
    double energyDeposit;
    int eventID;
    
};

typedef G4THitsCollection<Hits> HitsCollection;
 
extern G4ThreadLocal G4Allocator<Hits>* HitAllocator;

inline void* Hits::operator new(size_t)
{
  if (!HitAllocator) {
    HitAllocator = new G4Allocator<Hits>;
  }
  void *hit;
  hit = (void *) HitAllocator->MallocSingle();
  return hit;
}

inline void Hits::operator delete(void *aHit)
{
  if (!HitAllocator) {
    HitAllocator = new G4Allocator<Hits>;
  }
  HitAllocator->FreeSingle((Hits*) aHit);
}

#endif
