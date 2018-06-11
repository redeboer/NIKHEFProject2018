// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the information of the whole run (all events combined).
  It couts the sizes of the objects at the begin or the run
  and the average energy depositions and tracklengths in the objects at the end of the run.
*/

#ifndef pbmRunAction_h
#define pbmRunAction_h 1

// === INCLUDES =======

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class G4Run;
class pbmDetectorConstruction;

class pbmRunAction : public G4UserRunAction
{
public:
  pbmRunAction(pbmDetectorConstruction*);
  virtual ~pbmRunAction();

  void   BeginOfRunAction(const G4Run*);
  void   EndOfRunAction(const G4Run*);
    
  void fillPerEvent(G4double, G4double, G4double, G4double, G4double, G4double,  G4double, G4double, G4double, G4double,
		    G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, G4double, 
		    G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector, G4ThreeVector); 

private:
  pbmDetectorConstruction* detector;

  G4double sumEAbs, sum2EAbs;
  G4double sumLAbs, sum2LAbs; 
  
  G4double sumEPh, sum2EPh;
  G4double sumESc, sum2ESc;
  G4double sumEAir, sum2EAir;
  G4double sumEDi1, sum2EDi1;
  G4double sumEDi2, sum2EDi2;
    
  G4double sumLPh, sum2LPh;
  G4double sumLSc, sum2LSc;
  G4double sumLAir, sum2LAir;
  G4double sumLDi1, sum2LDi1;
  G4double sumLDi2, sum2LDi2; 
};

#endif

