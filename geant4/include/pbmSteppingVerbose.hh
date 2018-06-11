// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script makes the iteration steps of the particles used by SteppingAction
  It is advised not to mess around here without knowing what you are doing
*/

class pbmSteppingVerbose;

#ifndef pbmSteppingVerbose_h
#define pbmSteppingVerbose_h 1

// === INCLUDES =======

#include "G4SteppingVerbose.hh"

class pbmSteppingVerbose : public G4SteppingVerbose
{
 public:   

   pbmSteppingVerbose();
  ~pbmSteppingVerbose();

   void StepInfo();
   void TrackingStarted();

};

#endif
