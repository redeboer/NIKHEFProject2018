// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of adjusting materials/sizes of detector components from Qt messages.
  It adds a /pbm/ option in the Qt environment.
*/

// === INCLUDES =======

#include "pbmDetectorMessenger.hh"

#include "pbmDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pbmDetectorMessenger::pbmDetectorMessenger(pbmDetectorConstruction* pbmDet)
:pbmDetector(pbmDet)
{ 
  pbmDir = new G4UIdirectory("/pbm/");
  pbmDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/pbm/det/");
  detDir->SetGuidance("detector control");
       
  //This adds a command to adjust the calorimeter material     
  AbsMaterCmd = new G4UIcmdWithAString("/pbm/det/setAbsMat",this);
  AbsMaterCmd->SetGuidance("Select Material of the Absorber.");
  AbsMaterCmd->SetParameterName("choice",false);
  AbsMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  //This adds a command to adjust the calorimenter thickness 
  AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/pbm/det/setAbsThick",this);
  AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
  AbsThickCmd->SetParameterName("Size",false);
  AbsThickCmd->SetRange("Size>=0.");
  AbsThickCmd->SetUnitCategory("Length");
  AbsThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //This adds a command to adjust the calorimeter size in the y and z axis.
  SizeYZCmd = new G4UIcmdWithADoubleAndUnit("/pbm/det/setSizeYZ",this);
  SizeYZCmd->SetGuidance("Set tranverse size of the calorimeter");
  SizeYZCmd->SetParameterName("Size",false);
  SizeYZCmd->SetRange("Size>0.");
  SizeYZCmd->SetUnitCategory("Length");    
  SizeYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //This command updates the geometry
  UpdateCmd = new G4UIcmdWithoutParameter("/pbm/det/update",this);
  UpdateCmd->SetGuidance("Update calorimeter geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/pbm/det/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
}

pbmDetectorMessenger::~pbmDetectorMessenger()
{
  delete AbsMaterCmd; 
  delete AbsThickCmd; 
  delete SizeYZCmd;   delete UpdateCmd;
  delete MagFieldCmd;
  delete detDir;
  delete pbmDir;  
}

void pbmDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == AbsMaterCmd )
   { pbmDetector->SetAbsorberMaterial(newValue);}
  
  if( command == AbsThickCmd )
   { pbmDetector->SetAbsorberThickness(AbsThickCmd->GetNewDoubleValue(newValue));}   
   
  if( command == SizeYZCmd )
   { pbmDetector->SetCalorSizeYZ(SizeYZCmd->GetNewDoubleValue(newValue));}

  if( command == UpdateCmd )
   { pbmDetector->UpdateGeometry(); }

  if( command == MagFieldCmd )
   { pbmDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
}

