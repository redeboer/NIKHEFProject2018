// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script deals with taking the particle trough the world iteratively.
  With each ieration a step is done, from this step you can extract particle information.
  So in general this script is the place to be to extract data from the simulation.
*/

// === INCLUDES =======

#include "pbmSteppingAction.hh"

#include "pbmDetectorConstruction.hh"
#include "pbmEventAction.hh"

#include "G4Step.hh"

pbmSteppingAction::pbmSteppingAction(pbmDetectorConstruction* det, pbmEventAction* evt)
:detector(det), eventaction(evt)					 
{ }

pbmSteppingAction::~pbmSteppingAction()
{ }

void pbmSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  //See what type of particle the current step takes care of
  G4String particleName = aStep->GetTrack()->GetDefinition()->GetParticleName();

  
  //Create 2 points
  G4StepPoint* point1 = aStep->GetPreStepPoint();
  G4StepPoint* point2 = aStep->GetPostStepPoint();
  G4String name = point1->GetPhysicalVolume()->GetLogicalVolume()->GetName(); 
  
  G4double stepl = 0.;
  if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.)
    stepl = aStep->GetStepLength();

    //if the step enters the PMT volume, add x,y and z position of track to vector
    if(point1->GetStepStatus() == fGeomBoundary && name == "PMT" && particleName=="proton"){ 
      G4ThreeVector pos1 = point1->GetPosition();
      eventaction-> VecEnterSci(pos1);
    }

    //if the step exits the PMT volume, add x,y and z position of track to vector
    if(point2->GetStepStatus() == fGeomBoundary && name == "PMT" && particleName=="proton"){ 
      G4ThreeVector pos2 = point2->GetPosition();
      eventaction-> VecExitSci(pos2);
    }
  
    //if the step enters the phantom volume, add x,y and z position of track to vector
    if(point1->GetStepStatus() == fGeomBoundary && name == "Phantom" && particleName=="proton"){ 
      G4ThreeVector pos1 = point1->GetPosition();
      eventaction-> VecEnterPha(pos1);  
    }

    //if the step exits the phantom volume, add x,y and z position of track to vector
    if(point2->GetStepStatus() == fGeomBoundary && name == "Phantom" && particleName=="proton"){ 
      G4ThreeVector pos2 = point2->GetPosition();
      eventaction-> VecExitPha(pos2);
    }

  //if step is in a detector volume, add steplength and step energy   
  if (volume == detector->GetScint())      eventaction->AddScint(edep,stepl); 
  if (volume == detector->GetPhantom())    eventaction->AddPhantom(edep,stepl);

  if (volume == detector->GetWallF1())     eventaction->AddWallF1(edep,stepl);  
  if (volume == detector->GetDICE1())      eventaction->AddDICE1(edep,stepl);
  if (volume == detector->GetWallB1())     eventaction->AddWallB1(edep,stepl);

  if (volume == detector->GetWallF2())     eventaction->AddWallF2(edep,stepl);  
  if (volume == detector->GetDICE2())      eventaction->AddDICE2(edep,stepl);
  if (volume == detector->GetWallB2())     eventaction->AddWallB2(edep,stepl);

  if (volume == detector->GetAbsorber()) eventaction->AddAbs(edep,stepl);

  if (volume != detector->GetScint() && volume != detector->GetWallF1() && volume != detector->GetDICE1() &&volume != detector->GetWallB1() && volume != detector->GetPhantom() && volume != detector->GetWallF2() && volume != detector->GetDICE2() && volume != detector->GetWallB2() && volume != detector->GetAbsorber() && particleName=="proton"){      
    eventaction->AddAir(edep,stepl);
  }
    //if the step enters the first front wall volume, add x,y and z position of track to vector
    if(point1->GetStepStatus() == fGeomBoundary && name == "WallF1" && particleName=="proton"){ 
      G4ThreeVector pos1 = point1->GetPosition();
      eventaction-> VecEnterDI1(pos1);
    }

    //if the step enters the second front wall volume, add x,y and z position of track to vector
    if(point1->GetStepStatus() == fGeomBoundary && name == "WallF2" && particleName=="proton"){ 
      G4ThreeVector pos1 = point1->GetPosition();
      eventaction-> VecEnterDI2(pos1);
    }

    //if the step exits the first back wall volume, add x,y and z position of track to vector
    if(point2->GetStepStatus() == fGeomBoundary && name == "WallB1" && particleName=="proton"){ 
      G4ThreeVector pos2 = point2->GetPosition();
      eventaction-> VecExitDI1(pos2);
    }

    //if the step exits the second back wall volume, add x,y and z position of track to vector
    if(point2->GetStepStatus() == fGeomBoundary && name == "WallB2" && particleName=="proton"){ 
      G4ThreeVector pos2 = point2->GetPosition();
      eventaction-> VecExitDI2(pos2);
    }

    //if the step enters the calorimeter volume, add x,y and z position of track to vector
    if(point1->GetStepStatus() == fGeomBoundary && name == "Absorber" && particleName=="proton"){ 
      G4ThreeVector pos1 = point1->GetPosition();
      eventaction-> VecEnterCal(pos1);  
    }

    //if the step exits the calorimeter volume, add x,y and z position of track to vector
    if(point2->GetStepStatus() == fGeomBoundary && name == "Absorber" && particleName=="proton"){ 
      G4ThreeVector pos2 = point2->GetPosition();
      eventaction-> VecExitCal(pos2);
    }
}

