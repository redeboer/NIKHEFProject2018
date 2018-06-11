// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the information during the event.
  It puts the energy and tracklength data from SteppingAction into RunAction.
  It couts the entry, exit positions and energy deposit at the end of the event.
*/

// === INCLUDES =======

#include "pbmEventAction.hh"

#include "pbmRunAction.hh"
#include "pbmEventActionMessenger.hh"

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4VTrajectory.hh"
#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

pbmEventAction::pbmEventAction(pbmRunAction* run)
:runAct(run),printModulo(1),eventMessenger(0)
{
  //make a messenger
  eventMessenger = new pbmEventActionMessenger(this);
}

pbmEventAction::~pbmEventAction()
{
  //delete messenger
  delete eventMessenger;
}

void pbmEventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
 
 // initialisation per event
  
 EnergyAbs = EnergyPhantom = EnergyWallF1 = EnergyDICE1 = EnergyWallB1 = 0.;
 TrackLAbs = TrackLPhantom = TrackLWallF1 = TrackLDICE1 = TrackLWallB1 = 0.;
 EnergyScint = TrackLScint = EnergyWallF2 = EnergyWallB2 = EnergyDICE2 = 0.;
 TrackLWallF2 = TrackLWallB2 = TrackLDICE2 = EnergyAir = TrackLAir =  0.;
 EnterScint = ExitScint = EnterPhantom = ExitPhantom = EnterDICE1 = ExitDICE1 = EnterDICE2 = ExitDICE2 = {0,0,0};
 EnterCal = ExitCal = {0,0,0};
 
}

void pbmEventAction::EndOfEventAction(const G4Event* evt)
{
  //puts energy deposition ans tracklengths into RunAction
  //
  runAct->fillPerEvent(EnergyAbs, EnergyPhantom, EnergyWallF1, EnergyDICE1, EnergyWallB1, EnergyWallF2, EnergyDICE2, EnergyWallB2, EnergyScint, EnergyAir,
		       TrackLAbs, TrackLPhantom, TrackLWallF1, TrackLDICE1, TrackLWallB1, TrackLWallF2, TrackLDICE2, TrackLWallB2, TrackLScint, TrackLAir,
					  ExitScint, EnterPhantom, ExitPhantom, EnterDICE1, ExitDICE1, EnterDICE2, ExitDICE2);
  
  //print per event (modulo n)
  //
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) {
    
    G4cout  <<" "<< evtNb <<" "<< EnterScint.x()<<" "<< EnterScint.y() <<" "<< EnterScint.z()
            <<" "<< ExitScint.x()<<" "<< ExitScint.y() <<" "<< ExitScint.z()
            <<" "<< EnterDICE1.x()<<" "<< EnterDICE1.y() <<" "<< EnterDICE1.z()
   				  <<" "<< ExitDICE1.x()<<" "<< ExitDICE1.y() <<" "<< ExitDICE1.z()
            <<" "<< EnterPhantom.x() <<" "<< EnterPhantom.y() <<" "<< EnterPhantom.z()
            <<" "<< ExitPhantom.x() <<" "<< ExitPhantom.y() <<" "<< ExitPhantom.z() 
            <<" "<< EnterDICE2.x()<<" "<< EnterDICE2.y() <<" "<< EnterDICE2.z()
   				  <<" "<<ExitDICE2.x()<<" "<< ExitDICE2.y() <<" "<< ExitDICE2.z()
             <<" "<< EnterCal.x()<<" "<< EnterCal.y() <<" "<< EnterCal.z()
   				  <<" "<<ExitCal.x()<<" "<< ExitCal.y() <<" "<< ExitCal.z()
            <<" "<<EnergyScint <<" "<<EnergyWallF1+EnergyDICE1+EnergyWallB1 
            <<" "<<EnergyPhantom
            <<" "<<EnergyWallF2 + EnergyDICE2 + EnergyWallB2
            <<" "<<EnergyAbs
   					<<G4endl;
   	  
  }
}  
