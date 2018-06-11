// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script decides which kind of physics processes are used.
  It is advised to not mess around here without knowing what you are doing.
*/

#ifndef pbmPhysicsList_h
#define pbmPhysicsList_h 1

// === INCLUDES =======

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class pbmPhysicsList: public G4VUserPhysicsList
{
public:
  pbmPhysicsList();
  virtual ~pbmPhysicsList();

  // Construct particle and physics
  void ConstructParticle();
  void ConstructProcess();
 
  void SetCuts();
   
private:
  // these methods Construct particles 
  void ConstructBosons();
  void ConstructLeptons();
  void ConstructMesons();
  void ConstructBaryons();

  // these methods Construct physics processes and register them
  void ConstructDecay();
  void ConstructEM();
};

#endif



