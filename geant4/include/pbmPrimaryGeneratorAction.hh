// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script makes the particle gun.
  The particle type, energy and positionof the gun is defined here.
*/

// === INCLUDES =======

#ifndef pbmPrimaryGeneratorAction_h
#define pbmPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class pbmDetectorConstruction;
class pbmPrimaryGeneratorMessenger;

class pbmPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  pbmPrimaryGeneratorAction(pbmDetectorConstruction*);    
  virtual ~pbmPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);
  void SetRndmFlag(G4String val) { rndmFlag = val;}

private:
  G4ParticleGun*                particleGun;	  //pointer a to G4  class
  pbmDetectorConstruction*      pbmDetector;    //pointer to the geometry
    
  pbmPrimaryGeneratorMessenger* gunMessenger;   //messenger of this class
  G4String                      rndmFlag;	      //flag for a rndm impact point
};

#endif


