// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script connects all the scripts and runs them in the correct order
*/

// === INCLUDES =======

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "pbmDetectorConstruction.hh"
#include "pbmPhysicsList.hh"
#include "pbmPrimaryGeneratorAction.hh"
#include "pbmRunAction.hh"
#include "pbmEventAction.hh"
#include "pbmSteppingAction.hh"
#include "pbmSteppingVerbose.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc,char** argv)
{
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
  
  // User Verbose output class
  //
  G4VSteppingVerbose::SetInstance(new pbmSteppingVerbose);
     
  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  pbmDetectorConstruction* detector = new pbmDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new pbmPhysicsList;
  runManager->SetUserInitialization(physics);
    
  // Set user action classes
  //
  G4VUserPrimaryGeneratorAction* gen_action = 
                          new pbmPrimaryGeneratorAction(detector);
  runManager->SetUserAction(gen_action);
  //
  pbmRunAction* run_action = new pbmRunAction(detector);  
  runManager->SetUserAction(run_action);
  //
  pbmEventAction* event_action = new pbmEventAction(run_action);
  runManager->SetUserAction(event_action);
  //
  G4UserSteppingAction* stepping_action =
                    new pbmSteppingAction(detector, event_action);
  runManager->SetUserAction(stepping_action);
  
  // Initialize G4 kernel
  //
  runManager->Initialize();
  
  
#ifdef G4VIS_USE
  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
  
  // Get the pointer to the User Interface manager
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);    
    }
    else{  // interactive mode : define UI session
      #ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
      #ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
      #endif
      if (ui->IsGUI())
	    UImanager->ApplyCommand("/control/execute gui.mac");     
      ui->SessionStart();
      delete ui;
#endif
    }
    
  delete runManager;

  return 0;
}