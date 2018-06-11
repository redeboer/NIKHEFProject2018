// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script makes the physical objects in the simulation.
  Here, the sizes and materials are defined.
*/

#ifndef pbmDetectorConstruction_h
#define pbmDetectorConstruction_h 1

// === INCLUDES =======

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

class G4Box;
class G4Polyhedra;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4UniformMagField;
class pbmDetectorMessenger;


class pbmDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    pbmDetectorConstruction();
   ~pbmDetectorConstruction();

     void SetPhantomMaterial (G4String); 
     void SetBoneMaterial    (G4String);    
     void SetPinMaterial     (G4String); 
     void SetMuscleMaterial  (G4String);
     void SetTissueMaterial  (G4String);
     void SetFatMaterial     (G4String);
     void SetProteseMaterial (G4String);

     void SetPhantomThickness(G4double); 
    
     void SetWallMaterial (G4String);     
     void SetWallThickness(G4double);
     
     void SetDICEMaterial (G4String);     
     void SetDICEThickness(G4double); 
     
     void SetPMTMaterial (G4String);     
     void SetPMTThickness(G4double); 
     
     void SetAbsorberMaterial (G4String);     
     void SetAbsorberThickness(G4double);     
     
     void SetCalorSizeYZ(G4double);            
      
     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
  
     void PrintCalorParameters(); 
                    
     G4double GetWorldSizeX()                   {return WorldSizeX;}; 
     G4double GetWorldSizeYZ()                  {return WorldSizeYZ;};

     G4double GetPhantomThickness()             {return PhantomThickness;}; 
     G4double GetPhantomSizeYZ()                {return PhantomSizeYZ;};
     
     G4double GetWallThickness()                {return WallThickness;};
     
     G4double GetDICEThickness()                {return DICEThickness;}; 
     G4double GetDICESizeY()                    {return DICESizeY;};
     G4double GetDICESizeZ()                    {return DICESizeZ;};
     
     G4double GetPMTThickness()                 {return PMTThickness;}; 
     G4double GetPMTSizeYZ()                    {return PMTSizeYZ;};
     
     G4double GetCalorSizeYZ()                  {return CalorSizeYZ;};
     
     G4Material* GetAbsorberMaterial()          {return AbsorberMaterial;};
     G4double    GetAbsorberThickness()         {return AbsorberThickness;};      
     
     const G4VPhysicalVolume* GetphysiWorld()   {return physiWorld;};          
     const G4VPhysicalVolume* GetAbsorber()     {return physiAbsorber;};
     const G4VPhysicalVolume* GetScint()        {return physiPMT;};
     const G4VPhysicalVolume* GetPhantom()      {return physiPhantom;};
     const G4VPhysicalVolume* GetWallF1()       {return physiWallF1;};
     const G4VPhysicalVolume* GetWallF2()       {return physiWallF2;};
     const G4VPhysicalVolume* GetDICE1()        {return physiDICE1;};
     const G4VPhysicalVolume* GetDICE2()        {return physiDICE2;};
     const G4VPhysicalVolume* GetWallB1()       {return physiWallB1;};
     const G4VPhysicalVolume* GetWallB2()       {return physiWallB2;};
     const G4VPhysicalVolume* GetPMT()          {return physiPMT;};
     
  private:
     
     G4Material*        PhantomMaterial;
     G4double           PhantomThickness;
     G4double           PhantomSizeYZ;

     G4Material*        BoneMaterial;
     G4double           Bonerad;
     G4bool             UseBone;

     G4Material*        PinMaterial;
     G4bool             UsePin;
     G4double           Pinrad;
     G4double           PinZ;

     G4Material*        MuscleMaterial;
     G4bool             UseMuscle;
     G4double           Muscle_rad;

     G4Material*        TissueMaterial;
     G4bool             UseTissue;
     G4double           Tissue_rad;

     G4Material*        FatMaterial;
     G4bool             UseFat;
     G4double           Fat_rad;

     G4Material*        ProteseMaterial;
     G4bool             UseProtese;
     G4double           Protese_rad;
     G4double           ProteseY;
     
     G4Material*        DICEMaterial;
     G4double           DICEThickness;
     G4double           DICESizeY;
     G4double           DICESizeZ;
     
     G4Material*        PMTMaterial;
     G4double           PMTThickness;
     G4double           PMTSizeYZ;
     
     G4Material*	      WallMaterial;
     G4double		        WallThickness;
     
     G4Material*        AbsorberMaterial;
     G4double           AbsorberThickness;
               
     G4double           CalorSizeYZ;
     
     G4Material*        defaultMaterial;
     G4double           WorldSizeYZ;
     G4double           WorldSizeX;
            
     G4Box*             solidWorld;       //pointer to the solid World 
     G4LogicalVolume*   logicWorld;       //pointer to the logical World
     G4VPhysicalVolume* physiWorld;       //pointer to the physical Worlde

     G4Box*             solidPhantom;     //pointer to the solid Phantom 
     G4LogicalVolume*   logicPhantom;     //pointer to the logical Phantom
     G4VPhysicalVolume* physiPhantom;     //pointer to the physical Phantom 

     G4Tubs*            solidBone;        //pointer to the solid Bone 
     G4LogicalVolume*   logicBone;        //pointer to the logical Bone
     G4VPhysicalVolume* physiBone;        //pointer to the physical Bone 

     G4Tubs*            solidPin;         //pointer to the solid metal pin 
     G4LogicalVolume*   logicPin;         //pointer to the logical metal pin
     G4VPhysicalVolume* physiPin;         //pointer to the physical metal pin

     G4Tubs*            solidMuscle;      //pointer to the solid Muscle 
     G4LogicalVolume*   logicMuscle;      //pointer to the logical Muscle
     G4VPhysicalVolume* physiMuscle;      //pointer to the physical Muscle

     G4Tubs*            solidTissue;      //pointer to the solid Tissue
     G4LogicalVolume*   logicTissue;      //pointer to the logical Tissue
     G4VPhysicalVolume* physiTissue;      //pointer to the physical Tissue

     G4Tubs*            solidFat;         //pointer to the solid Fat 
     G4LogicalVolume*   logicFat;         //pointer to the logical Fat
     G4VPhysicalVolume* physiFat;         //pointer to the physical Fat

     G4Tubs*            solidProtese;     //pointer to the solid Prothese 
     G4LogicalVolume*   logicProtese;     //pointer to the logical Prothese
     G4VPhysicalVolume* physiProtese;     //pointer to the physical Prothese
     
     G4Box*             solidWallF1;      //pointer to the solid DICE 
     G4LogicalVolume*   logicWallF1;      //pointer to the logical DICE
     G4VPhysicalVolume* physiWallF1;      //pointer to the physical DICE 

     G4Box*             solidWallF2;      //pointer to the solid DICE 
     G4LogicalVolume*   logicWallF2;      //pointer to the logical DICE
     G4VPhysicalVolume* physiWallF2;      //pointer to the physical DICE 

     G4Box*             solidDICE1;       //pointer to the solid DICE 
     G4LogicalVolume*   logicDICE1;       //pointer to the logical DICE
     G4VPhysicalVolume* physiDICE1;       //pointer to the physical DICE 

     G4Box*             solidDICE2;       //pointer to the solid DICE 
     G4LogicalVolume*   logicDICE2;       //pointer to the logical DICE
     G4VPhysicalVolume* physiDICE2;       //pointer to the physical DICE 
     
     G4Box*             solidWallB1;      //pointer to the solid DICE 
     G4LogicalVolume*   logicWallB1;      //pointer to the logical DICE
     G4VPhysicalVolume* physiWallB1;      //pointer to the physical DICE 

     G4Box*             solidWallB2;      //pointer to the solid DICE 
     G4LogicalVolume*   logicWallB2;      //pointer to the logical DICE
     G4VPhysicalVolume* physiWallB2;      //pointer to the physical DICE 

     G4Box*             solidPMT;         //pointer to the solid DICE 
     G4LogicalVolume*   logicPMT;         //pointer to the logical DICE
     G4VPhysicalVolume* physiPMT;         //pointer to the physical DICE 
         
     G4Polyhedra*       solidAbsorber;    //pointer to the solid Absorber
     G4LogicalVolume*   logicAbsorber;    //pointer to the logical Absorber
     G4VPhysicalVolume* physiAbsorber;    //pointer to the physical Absorber
     
     
     G4UniformMagField* magField;         //pointer to the magnetic field
     
     pbmDetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void DefineMaterials();
     G4VPhysicalVolume* ConstructCalorimeter();     
};

#endif

