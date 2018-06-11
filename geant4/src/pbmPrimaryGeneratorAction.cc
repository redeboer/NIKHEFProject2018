// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script makes the particle gun.
  The particle type, energy and positionof the gun is defined here.
*/

// === INCLUDES =======

#include "pbmPrimaryGeneratorAction.hh"

#include "pbmDetectorConstruction.hh"
#include "pbmPrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

pbmPrimaryGeneratorAction::pbmPrimaryGeneratorAction(
                                             pbmDetectorConstruction* pbmDC)
:pbmDetector(pbmDC),rndmFlag("on")
{
  G4int n_particle = 1;
  particleGun  = new G4ParticleGun(n_particle);
  
  //create a messenger for this class
  gunMessenger = new pbmPrimaryGeneratorMessenger(this);

  // default particle kinematic

  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

  //Set particle type of gun
  //DISCLAIMER: SET PARTICLE TYPE IN QT WITH /gun/particle ......, DO NOT CHANGE IT HERE EVERYTIME
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="proton");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  //Set gun energy 
  //DISCLAIMER: SET GUN ENERGY IN QT WITH /gun/energy ......, DO NOT CHANGE IT HERE EVERYTIME
  particleGun->SetParticleEnergy(150*MeV);
  //gun is always on beginning of world
  G4double position = -0.5*(pbmDetector->GetWorldSizeX());
  particleGun->SetParticlePosition(G4ThreeVector(position, 0.*cm, 0.*cm));

}

pbmPrimaryGeneratorAction::~pbmPrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

void pbmPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  // 
  G4double x0 = -0.5*(pbmDetector->GetWorldSizeX());
  G4double y0 = 0.*cm, z0 = 0.*cm;
  //if randomflag on, the gun position is randomly distributed over the PMT surface (the smallest surface)
  if (rndmFlag == "on")
     {
       G4double size = pbmDetector->GetPMTSizeYZ();
       y0 = size*G4UniformRand()-size/2;
       z0 = size*G4UniformRand()-size/2;
     } 
  particleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  particleGun->GeneratePrimaryVertex(anEvent);
}


