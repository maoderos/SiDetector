#include "Hits.hh"

G4ThreadLocal G4Allocator<Hits>* HitAllocator = 0;

Hits::Hits(): G4VHit(), numberOfElectrons(0), numberOfElectronsInMetal(0), numberOfElectronHolePairs(0), energyDeposit(0), eventID(0) 
{}

Hits::~Hits()
{ }
