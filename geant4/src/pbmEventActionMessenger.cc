// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the Qt messages for EventAction.
*/

// === INCLUDES =======

#include "pbmEventActionMessenger.hh"

#include "pbmEventAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "globals.hh"

pbmEventActionMessenger::pbmEventActionMessenger(pbmEventAction* EvAct)
:eventAction(EvAct)
{
  eventDir = new G4UIdirectory("/pbm/event/");
  eventDir->SetGuidance("event control");
   
  PrintCmd = new G4UIcmdWithAnInteger("/pbm/event/printModulo",this);
  PrintCmd->SetGuidance("Print events modulo n");
  PrintCmd->SetParameterName("EventNb",false);
  PrintCmd->SetRange("EventNb>0");
}

pbmEventActionMessenger::~pbmEventActionMessenger()
{
  delete PrintCmd;
  delete eventDir;   
}

void pbmEventActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if(command == PrintCmd)
    {eventAction->SetPrintModulo(PrintCmd->GetNewIntValue(newValue));}
}
