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

    void AddEnergyDeposit(double e) { energyDeposit += e; }
    void AddNumberOfElectrons() {numberOfElectrons += 1;}
    void AddNumberOfElectronsinMetal() {numberOfElectronsInMetal += 1;}
    double GetEnergyDeposit() {return energyDeposit;}
    int GetNumberOfElectrons() {return numberOfElectrons;}
    int GetNumberOfElectronsInMetal() {return numberOfElectronsInMetal;}

    private:
    int numberOfElectrons;
    int numberOfElectronsInMetal;
    double energyDeposit;
    
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