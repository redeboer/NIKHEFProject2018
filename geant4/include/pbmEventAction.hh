// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the information during the event.
  It puts the energy and tracklength data from SteppingAction into RunAction.
  It couts the entry, exit positions and energy deposit at the end of the event.
*/

#ifndef pbmEventAction_h
#define pbmEventAction_h 1

// === INCLUDES =======

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

class pbmRunAction;
class pbmEventActionMessenger;

class pbmEventAction : public G4UserEventAction
{
public:
  pbmEventAction(pbmRunAction*);
  virtual ~pbmEventAction();

  void    BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void VecEnterSci(G4ThreeVector EnSc)      {EnterScint = EnSc;};
  void VecExitSci(G4ThreeVector ExSc)       {ExitScint = ExSc;};
  void VecEnterPha(G4ThreeVector EnPh)      {EnterPhantom = EnPh;}; 
  void VecExitPha(G4ThreeVector ExPh)       {ExitPhantom = ExPh;};
  void VecEnterDI1(G4ThreeVector EnDI1)     {EnterDICE1 = EnDI1;};
  void VecExitDI1(G4ThreeVector ExDI1)      {ExitDICE1 = ExDI1;};
  void VecEnterDI2(G4ThreeVector EnDI2)     {EnterDICE2 = EnDI2;};
  void VecExitDI2(G4ThreeVector ExDI2)      {ExitDICE2 = ExDI2;};
  void VecEnterCal(G4ThreeVector EnCal)     {EnterCal = EnCal;};
  void VecExitCal(G4ThreeVector ExCal)      {ExitCal = ExCal;};
  void AddPhantom(G4double de, G4double dl) {EnergyPhantom += de; TrackLPhantom += dl;};
  void AddScint(G4double de, G4double dl)   {EnergyScint += de; TrackLScint += dl;};
  void AddWallF1(G4double de, G4double dl)  {EnergyWallF1 += de; TrackLWallF1 += dl;};
  void AddDICE1(G4double de, G4double dl)   {EnergyDICE1 += de; TrackLDICE1 += dl;};
  void AddWallB1(G4double de, G4double dl)  {EnergyWallB1 += de; TrackLWallB1 += dl;};
  void AddWallF2(G4double de, G4double dl)  {EnergyWallF2 += de; TrackLWallF2 += dl;};
  void AddDICE2(G4double de, G4double dl)   {EnergyDICE2 += de; TrackLDICE2 += dl;};
  void AddWallB2(G4double de, G4double dl)  {EnergyWallB2 += de; TrackLWallB2 += dl;};
  void AddAbs(G4double de, G4double dl)     {EnergyAbs += de; TrackLAbs += dl;};
  void AddAir(G4double de, G4double dl)     {EnergyAir += de; TrackLAir += dl;};
  
  
  void SetPrintModulo(G4int    val)  {printModulo = val;};
    
private:
   pbmRunAction*  runAct;
   
   G4double  EnergyAbs, EnergyPhantom, EnergyWallF1, EnergyDICE1, EnergyWallB1;
   G4double  TrackLAbs, TrackLPhantom, TrackLWallF1, TrackLDICE1, TrackLWallB1;
   G4double  EnergyScint, TrackLScint, EnergyWallF2, EnergyDICE2, EnergyWallB2;
   G4double  TrackLWallB2, TrackLWallF2, TrackLDICE2, EnergyAir, TrackLAir;

   G4ThreeVector EnterScint;
   G4ThreeVector ExitScint;
   G4ThreeVector EnterDICE1;
   G4ThreeVector ExitDICE1;  
   G4ThreeVector EnterPhantom;
   G4ThreeVector ExitPhantom;
   G4ThreeVector EnterDICE2;
   G4ThreeVector ExitDICE2;
   G4ThreeVector EnterCal;
   G4ThreeVector ExitCal;
   
   G4int     printModulo;
                             
   pbmEventActionMessenger*  eventMessenger;
};

#endif

    
