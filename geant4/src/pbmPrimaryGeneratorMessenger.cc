// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the Qt gun messages and adds /pbm/gun.
*/

// === INCLUDES =======

#include "pbmPrimaryGeneratorMessenger.hh"

#include "pbmPrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

pbmPrimaryGeneratorMessenger::pbmPrimaryGeneratorMessenger(pbmPrimaryGeneratorAction* pbmGun)
:pbmAction(pbmGun)
{
  gunDir = new G4UIdirectory("/pbm/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/pbm/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance("  Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

pbmPrimaryGeneratorMessenger::~pbmPrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete gunDir;
}

void pbmPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == RndmCmd )
   { pbmAction->SetRndmFlag(newValue);}
}

