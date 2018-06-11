// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the Qt messages for EventAction.
*/

#ifndef pbmEventActionMessenger_h
#define pbmEventActionMessenger_h 1

// === INCLUDES =======

#include "globals.hh"
#include "G4UImessenger.hh"

class pbmEventAction;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

class pbmEventActionMessenger: public G4UImessenger
{
public:
  pbmEventActionMessenger(pbmEventAction*);
  virtual ~pbmEventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:
  pbmEventAction*     eventAction;
  G4UIdirectory*        eventDir;   
  G4UIcmdWithAnInteger* PrintCmd;    
};

#endif
