// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of adjusting materials/sizes of detector components from Qt messages.
  It adds a /pbm/ option in the Qt environment.
*/

#ifndef pbmDetectorMessenger_h
#define pbmDetectorMessenger_h 1

// === INCLUDES =======

#include "globals.hh"
#include "G4UImessenger.hh"

class pbmDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class pbmDetectorMessenger: public G4UImessenger
{
  public:
    pbmDetectorMessenger(pbmDetectorConstruction* );
   ~pbmDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    pbmDetectorConstruction* pbmDetector;
    
    G4UIdirectory*             pbmDir;
    G4UIdirectory*             detDir;
    G4UIcmdWithAString*        AbsMaterCmd;
    G4UIcmdWithADoubleAndUnit* AbsThickCmd;
    G4UIcmdWithADoubleAndUnit* SizeYZCmd;   
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;
};

#endif

