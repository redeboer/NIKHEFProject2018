// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the Qt gun messages and adds /pbm/gun.
*/

#ifndef pbmPrimaryGeneratorMessenger_h
#define pbmPrimaryGeneratorMessenger_h 1

// === INCLUDES =======

#include "G4UImessenger.hh"
#include "globals.hh"

class pbmPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithAString;

class pbmPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  pbmPrimaryGeneratorMessenger(pbmPrimaryGeneratorAction*);
  virtual ~pbmPrimaryGeneratorMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  pbmPrimaryGeneratorAction* pbmAction;
  G4UIdirectory*               gunDir; 
  G4UIcmdWithAString*          RndmCmd;
};

#endif

