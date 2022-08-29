#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;
class G4UIcmdWithABool;

class DetectorMessenger: public G4UImessenger
{
public:
  DetectorMessenger(DetectorConstruction*);
  ~DetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:

  DetectorConstruction* detector;
  G4UIdirectory* changeTargetDir;
  G4UIcmdWithAString* changeTarget;
  G4UIcmdWithADoubleAndUnit* changeSensitiveThickness;
  G4UIcmdWithAString* changeSensitiveMaterial;

};
#endif
