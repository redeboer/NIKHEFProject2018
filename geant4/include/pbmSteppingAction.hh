// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script deals with taking the particle trough the world iteratively.
  With each ieration a step is done, from this step you can extract particle information.
  So in general this script is the place to be to extract data from the simulation.
*/

#ifndef pbmSteppingAction_h
#define pbmSteppingAction_h 1

// === INCLUDES =======

#include "G4UserSteppingAction.hh"

class pbmDetectorConstruction;
class pbmEventAction;

class pbmSteppingAction : public G4UserSteppingAction
{
public:
  pbmSteppingAction(pbmDetectorConstruction*, pbmEventAction*);
  virtual ~pbmSteppingAction();

  void UserSteppingAction(const G4Step*);
    
private:
  pbmDetectorConstruction* detector;
  pbmEventAction*          eventaction;  
};

#endif
