// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script makes the physical objects in the simulation.
  Here, the sizes and materials are defined.
*/

// === INCLUDES =======

#include "pbmDetectorConstruction.hh"
#include "pbmDetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4UniformMagField.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4RunManager.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"

pbmDetectorConstruction::pbmDetectorConstruction()
:AbsorberMaterial(0),defaultMaterial(0), PhantomMaterial(0), DICEMaterial(0),
 solidWorld(0),logicWorld(0),physiWorld(0),
 solidPhantom(0),logicPhantom(0),physiPhantom(0),
 solidBone(0), logicBone(0), physiBone(0),
 solidPin(0), logicPin(0), physiPin(0),
 solidTissue(0), logicTissue(0), physiTissue(0),
 solidFat(0), logicFat(0), physiFat(0),
 solidProtese(0), logicProtese(0), physiProtese(0),
 solidMuscle(0), logicMuscle(0), physiMuscle(0),
 solidWallF1(0), logicWallF1(0), physiWallF1(0),
 solidWallF2(0), logicWallF2(0), physiWallF2(0),
 solidDICE1(0),logicDICE1(0),physiDICE1(0),
 solidDICE2(0),logicDICE2(0),physiDICE2(0),
 solidPMT(0), logicPMT(0), physiPMT(0),
 solidWallB1(0), logicWallB1(0), physiWallB1(0),
 solidWallB2(0), logicWallB2(0), physiWallB2(0),
 solidAbsorber(0),logicAbsorber(0),physiAbsorber(0),
 magField(0)
{
  
  // === SIZES DECLERATION =======
  PhantomThickness  = 2.*cm;//8.*cm;
  PhantomSizeYZ	    = 6.*cm;

  UseBone           = true;
  Bonerad           = 1.2*cm;

  UsePin            = false;
  Pinrad            = 1*cm;
  PinZ              = PhantomSizeYZ*0.4;

  UseMuscle         = true;
  Muscle_rad        = 1.2*cm;

  UseTissue         = true;
  Tissue_rad        = 1.2*cm;

  UseFat            = true;
  Fat_rad           = 1.2*cm;

  UseProtese        = false;
  Protese_rad       = 1*cm;
  ProteseY          = PhantomSizeYZ/5;
  
  // parameters for DICE
  DICEThickness  = 33.34*mm; //33.44 - Walls
  DICESizeY	     = 40.50*mm;
  DICESizeZ      = 54.74*mm;
  
  // walls of DICE
  WallThickness = 0.050*mm;
  
  // parameters for PMT
  PMTThickness  = 4.55*mm;
  PMTSizeYZ     = 32.62*mm;
  
  // default parameter values of the calorimeter
  AbsorberThickness  = 25.*cm;
  CalorSizeYZ        = 4.*cm;
     
  WorldSizeX = 0.7*m; 
  WorldSizeYZ = 0.7*m;
  
  // materials
  DefineMaterials();
  SetPhantomMaterial("G4_ADIPOSE_TISSUE_ICRP");
  SetDICEMaterial("Aerio");
  SetPMTMaterial("Scintillator");
  SetWallMaterial("G4_KAPTON");
  SetAbsorberMaterial("G4_BARIUM_FLUORIDE");
  SetBoneMaterial("G4_BONE_COMPACT_ICRU");
  SetPinMaterial("G4_STAINLESS-STEEL");
  SetMuscleMaterial("G4_BONE_COMPACT_ICRU");
  SetTissueMaterial("G4_BONE_COMPACT_ICRU");
  SetFatMaterial("G4_BONE_COMPACT_ICRU");
  SetProteseMaterial("G4_POLYETHYLENE");
  
  // create commands for interactive definition of the calorimeter
  detectorMessenger = new pbmDetectorMessenger(this);
}

pbmDetectorConstruction::~pbmDetectorConstruction()
{ delete detectorMessenger;}

G4VPhysicalVolume* pbmDetectorConstruction::Construct()
{
  return ConstructCalorimeter();
}

// === MATERIALS =======

void pbmDetectorConstruction::DefineMaterials()
{ 
 //This function illustrates the possible ways to define materials
 
G4String symbol;             //a=mass of a mole;
G4double a, z, density;      //z=mean number of protons;  
G4int iz, n;                 //iz=number of protons  in an isotope; 
                             // n=number of nucleons in an isotope;

G4int ncomponents, natoms;
G4double abundance, fractionmass;

G4NistManager* man = G4NistManager::Instance();
man->SetVerbose(1);

//
// define Elements
//

G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
G4Element* Si = new G4Element("Silicon",symbol="Si" , z= 14., a= 28.09*g/mole);

//
// define an Element from isotopes, by relative abundance 
//

G4Isotope* U5 = new G4Isotope("U235", iz=92, n=235, a=235.01*g/mole);
G4Isotope* U8 = new G4Isotope("U238", iz=92, n=238, a=238.03*g/mole);

G4Element* U  = new G4Element("enriched Uranium",symbol="U",ncomponents=2);
U->AddIsotope(U5, abundance= 90.*perCent);
U->AddIsotope(U8, abundance= 10.*perCent);

//
// define simple materials
//

new G4Material("Copper" , z= 29., a= 63.5*g/mole, density=8.960*g/cm3);
new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);
new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
new G4Material("Lead"     , z=82., a= 207.19*g/mole, density= 11.35*g/cm3);

//
// define a material from elements.   case 1: chemical molecule
//

G4Material* H2O = 
new G4Material("Water", density= 1.000*g/cm3, ncomponents=2);
H2O->AddElement(H, natoms=2);
H2O->AddElement(O, natoms=1);
// overwrite computed meanExcitationEnergy with ICRU recommended value 
H2O->GetIonisation()->SetMeanExcitationEnergy(75.0*eV);

G4Material* Sci = 
new G4Material("Scintillator", density= 1.032*g/cm3, ncomponents=2);
Sci->AddElement(C, natoms=9);
Sci->AddElement(H, natoms=10);

G4Material* Myl = 
new G4Material("Mylar", density= 1.397*g/cm3, ncomponents=3);
Myl->AddElement(C, natoms=10);
Myl->AddElement(H, natoms= 8);
Myl->AddElement(O, natoms= 4);

G4Material* SiO2 = 
new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
SiO2->AddElement(Si, natoms=1);
SiO2->AddElement(O , natoms=2);

G4Material* DME = 
new G4Material("dimethyl",density= 2.2*0.001*g/cm3, ncomponents=3);
DME->AddElement(C,  natoms=2);
DME->AddElement(H , natoms=6);
DME->AddElement(O , natoms=1);

// define NIST materials

  G4Material* BaF2 = man->FindOrBuildMaterial("G4_BARIUM_FLUORIDE");
  G4Material* kapton = man->FindOrBuildMaterial("G4_KAPTON");
  G4Material* Bone = man->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  G4Material* Steel = man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* Tissue = man->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP");
  G4Material* Kunststof = man->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* Muscle = man->FindOrBuildMaterial("G4_MUSCLE_SKELETAL_ICRP");
  G4Material* Fat = man->FindOrBuildMaterial("G4_ADIPOSE_TISSUE_ICRP");
//
// define a material from elements.   case 2: mixture by fractional mass
//

G4Material* Air = 
new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
Air->AddElement(N, fractionmass=0.7);
Air->AddElement(O, fractionmass=0.3);


//
// examples of gas in non STP conditions
//

G4Material* CO2 = 
new G4Material("CarbonicGas", density= 1.842*mg/cm3, ncomponents=2,
                              kStateGas, 325.*kelvin, 50.*atmosphere);
CO2->AddElement(C, natoms=1);
CO2->AddElement(O, natoms=2);
 
G4Material* steam = 
new G4Material("WaterSteam", density= 0.3*mg/cm3, ncomponents=1,
                             kStateGas, 500.*kelvin, 2.*atmosphere);
steam->AddMaterial(H2O, fractionmass=1.);

//
// define a material from elements and/or others materials (mixture of mixtures)
//

G4Material* Aerog = 
new G4Material("Aerogel", density= 0.200*g/cm3, ncomponents=3);
Aerog->AddMaterial(SiO2, fractionmass=62.5*perCent);
Aerog->AddMaterial(H2O , fractionmass=37.4*perCent);
Aerog->AddElement (C   , fractionmass= 0.1*perCent);

G4Material* Aer = 
new G4Material("Aerio", density= 0.002*g/cm3, ncomponents=2);	// density might be wrong !!! consider also 0.2*mg/cm3
Aer->AddMaterial(DME, fractionmass=10*perCent);
Aer->AddMaterial(CO2 , fractionmass=90*perCent);

//
// examples of vacuum
//

G4Material* Vacuum =
new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                           kStateGas, 2.73*kelvin, 3.e-18*pascal);

G4Material* beam = 
new G4Material("Beam", density= 1.e-5*g/cm3, ncomponents=1,
                       kStateGas, STP_Temperature, 2.e-2*bar);
beam->AddMaterial(Air, fractionmass=1.);

G4cout << *(G4Material::GetMaterialTable()) << G4endl;

//default materials of the World
defaultMaterial  = Air;
}


G4VPhysicalVolume* pbmDetectorConstruction::ConstructCalorimeter()
{

  // Clean old geometry, if any
  //
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();
   
  //     
  // World
  //
  solidWorld = new G4Box("World",				                                //its name
                   WorldSizeX/2,WorldSizeYZ/2,WorldSizeYZ/2);	          //its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,		                      //its solid
                                   defaultMaterial,	                    //its material
                                   "World");		                        //its name
                                   
  physiWorld = new G4PVPlacement(0,			                                //no rotation
  				 G4ThreeVector(),	                                            //World is always at (0,0,0)
                                 logicWorld,		                        //its logical volume				 
                                 "World",		                            //its name
                                 0,			                                //its mother  volume
                                 false,			                            //no boolean operation
                                 0);			                              //copy number

   //     
   //Phantom
  
  solidPhantom=0; logicPhantom=0; physiPhantom=0;  
  
  if (PhantomThickness > 0.)  
  {solidPhantom = new G4Box("Phantom",				                          //its name
                   PhantomThickness/2,PhantomSizeYZ/2,PhantomSizeYZ/2);	//its size
                         
  logicPhantom = new G4LogicalVolume(solidPhantom,		                  //its solid
                                   PhantomMaterial,		                  //its material
                                   "Phantom");                          //its name
                                   
  physiPhantom = new G4PVPlacement(0,			                              //no rotation
  				G4ThreeVector(-DICEThickness/2 - WallThickness - PhantomThickness/2 - 2.5*cm,0.*cm,0.*cm),	
                                 logicPhantom,		                      //its logical volume				 
                                 "Phantom",		                          //its name
                                 logicWorld,		                        //its mother  volume
                                 false,			                            //no boolean operation
                                 0);			                              //copy number
  } 

  //Rotationmatrices to put the special components like cylinders and the hexagon in the right position
  G4RotationMatrix rm;
  G4RotationMatrix Tr;
  G4double gonia = 90*deg;
  rm.rotateY(gonia);
  Tr.rotateX(gonia);

    //Muscle volume in phantom

  solidMuscle=0; logicMuscle=0; physiMuscle=0;

  if(UseMuscle == true){
  solidMuscle = new G4Tubs("Muscle",                                      //its name
                          0.,                                             //Rin
                          Muscle_rad/2.,                                  //Rout
                          PhantomSizeYZ/2,                                //Z
                          0.*deg,                                         //min phi
                          360.*deg );                                     //max phi
                         
  logicMuscle = new G4LogicalVolume(solidMuscle,	  	                    //its solid
                                  MuscleMaterial,		                      //its material
                                  "Muscle");                              //its name
                                   
  physiMuscle = new G4PVPlacement(G4Transform3D(Tr,			                  //90 deg rotation
  				G4ThreeVector(0,0,  -PhantomSizeYZ/2 + 3.5*Muscle_rad + 4*(PhantomSizeYZ - Muscle_rad*4)/5)),		
                                 logicMuscle,		                          //its logical volume				 
                                 "Muscle",		                            //its name
                                 logicPhantom,		                        //its mother  volume
                                 false,			                              //no boolean operation
                                 0);			                                //copy number
  }

  // Bone volume in phantom

  solidBone=0; logicBone=0; physiBone=0;

  if(UseBone == true){
  solidBone = new G4Tubs("Bone",                                           //its name
                          0.,                                              //Rin
                          Bonerad/2.,                                      //Rout
                          PhantomSizeYZ/2,                                 //Z
                          0.*deg,                                          //min phi
                          360.*deg );                                      //max phi
                         
  logicBone = new G4LogicalVolume(solidBone,		                           //its solid
                                  BoneMaterial,		                         //its material
                                  "Bone");                                 //its name
                                   
  physiBone =  new G4PVPlacement(G4Transform3D(Tr,			                   //90 deg rotation
  				G4ThreeVector(0,0,  -PhantomSizeYZ/2 + 2.5*Muscle_rad + 3*(PhantomSizeYZ - Muscle_rad*4)/5)),	
                                 logicBone,		                             //its logical volume				 
                                 "Bone",		                               //its name
                                 logicPhantom,		                         //its mother  volume
                                 false,			                               //no boolean operation
                                 0);			                                 //copy number
  }

    //Tissue volume in phantom

  solidTissue=0; logicTissue=0; physiTissue=0;

  if(UseTissue == true){
  solidTissue = new G4Tubs("Tissue",                                        //its name
                          0.,                                               //Rin
                          Tissue_rad/2.,                                    //Rout
                          PhantomSizeYZ/2,                                  //Z
                          0.*deg,                                           //min phi
                          360.*deg );                                       //max phi
                         
  logicTissue = new G4LogicalVolume(solidTissue,		                        //its solid
                                  TissueMaterial,		                        //its material
                                  "Tissue");                                //its name
                                   
  physiTissue = new G4PVPlacement(G4Transform3D(Tr,			                    //90 deg rotation
  				G4ThreeVector(0,0,  -PhantomSizeYZ/2 + 1.5*Muscle_rad + 2*(PhantomSizeYZ - Muscle_rad*4)/5)),		
                                 logicTissue,		                            //its logical volume				 
                                 "Tissue",		                              //its name
                                 logicPhantom,                          		//its mother  volume
                                 false,			                                //no boolean operation
                                 0);		                                  	//copy number
  }

      //Fat volume in phantom

  solidFat=0; logicFat=0; physiFat=0;

  if(UseFat == true){
  solidFat = new G4Tubs("Fat",                                              //its name
                          0.,                                               //Rin
                          Fat_rad/2.,                                       //Rout
                          PhantomSizeYZ/2,                                  //Z
                          0.*deg,                                           //min phi
                          360.*deg );                                       //max phi
                         
  logicFat = new G4LogicalVolume(solidFat,		                              //its solid
                                  FatMaterial,		                          //its material
                                  "Fat");                                   //its name
                                   
  physiFat = new G4PVPlacement(G4Transform3D(Tr,			                      //90 deg rotation
  				G4ThreeVector(0,0,  -PhantomSizeYZ/2 + Muscle_rad/2 + (PhantomSizeYZ - Muscle_rad*4)/5)),	
                                 logicFat,		                              //its logical volume				 
                                 "Fat",		                                  //its name
                                 logicPhantom,		                          //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  }

      //Prothese volume in phantom

  solidProtese=0; logicProtese=0; physiProtese=0;

  if(UseProtese == true){
  solidProtese = new G4Tubs("Protese",                                       //its name
                          0.,                                                //Rin
                          Protese_rad/2.,                                    //Rout
                          ProteseY/2,                                        //Z
                          0.*deg,                                            //min phi
                          360.*deg );                                        //max phi
                         
  logicProtese = new G4LogicalVolume(solidProtese,		                       //its solid
                                  ProteseMaterial,		                       //its material
                                  "Protese");                                //its name
                                   
  physiProtese = new G4PVPlacement(G4Transform3D(Tr,			                   //90 deg rotation
  				G4ThreeVector(-PhantomThickness/4,0,-PMTSizeYZ/2+Tissue_rad/2)),	
                                 logicProtese,	                           	 //its logical volume				 
                                 "Protese",		                               //its name
                                 logicPhantom,		                           //its mother  volume
                                 false,			                                 //no boolean operation
                                 0);			                                   //copy number
  }

    //Metal pin in Bone

  solidPin=0; logicPin=0; physiPin=0;

  if(UsePin == true){
  solidPin = new G4Tubs("Pin",                                               //its name
                          0.,                                                //Rin
                          Pinrad/2.,                                         //Rout
                          PinZ/2,                                            //Z
                          0.*deg,                                            //min phi
                          360.*deg );                                        //max phi
                         
  logicPin = new G4LogicalVolume(solidPin,		                               //its solid
                                  PinMaterial,		                           //its material
                                  "Pin");                                    //its name
                                   
  physiPin =  new G4PVPlacement(0,			                                     //no rotation
  				G4ThreeVector(0,PMTSizeYZ/6,0),	
                                 logicPin,                               		 //its logical volume				 
                                 "Pin",		                                   //its name
                                 logicPhantom,		                           //its mother  volume
                                 false,			                                 //no boolean operation
                                 0);			                                   //copy number
  }

  
  //     
  // WallF_1
  //
  solidWallF1=0; logicWallF1=0; physiWallF1=0;  
  
  if (WallThickness > 0.)  {
  solidWallF1 = new G4Box("WallF1",				                                  //its name
                   WallThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicWallF1 = new G4LogicalVolume(solidWallF1,		                        //its solid
                                   WallMaterial,		                        //its material
                                   "WallF1");                               //its name
                                   
  physiWallF1 = new G4PVPlacement(0,			                                  //no rotation
  				 G4ThreeVector(-DICEThickness*1.5 - WallThickness*2.5 - PhantomThickness - 5.*cm,0.*cm,0.*cm),	
                                 logicWallF1,		                            //its logical volume				 
                                 "WallF1",		                              //its name
                                 logicWorld,	                            	//its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  }
  
  //     
  // DICE_1
  //
  solidDICE1=0; logicDICE1=0; physiDICE1=0;  
  
  if (DICEThickness > 0.)  {
  solidDICE1 = new G4Box("DICE1",				                                    //its name
                   DICEThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicDICE1 = new G4LogicalVolume(solidDICE1,		                          //its solid
                                   DICEMaterial,		                        //its material
                                   "DICE1");                                //its name
                                   
  physiDICE1 = new G4PVPlacement(0,			                                    //no rotation
  				 G4ThreeVector(-DICEThickness - WallThickness*2 - PhantomThickness - 5.*cm, 0*cm, 0*cm),	
                                 logicDICE1,	  	                          //its logical volume				 
                                 "DICE1",		                                //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  
  }
  
  
  //     
  // WallB_1
  //
  solidWallB1=0; logicWallB1=0; physiWallB1=0;  
  
  if (WallThickness > 0.)  {
  solidWallB1 = new G4Box("WallB1",				                                  //its name
                   WallThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicWallB1 = new G4LogicalVolume(solidWallB1,		                        //its solid
                                   WallMaterial,		                        //its material
                                   "WallB1");                               //its name
                                   
  physiWallB1 = new G4PVPlacement(0,			                                  //no rotation
  				 G4ThreeVector(-DICEThickness/2 - WallThickness*1.5 - PhantomThickness - 5.*cm,0.*cm,0.*cm),	
                                 logicWallB1,		                            //its logical volume				 
                                 "WallB1",		                              //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  }

  //     
  // WallF_2
  //
  solidWallF2=0; logicWallF2=0; physiWallF2=0;  
  
  if (WallThickness > 0.)  {
  solidWallF2 = new G4Box("WallF2",				                                  //its name
                   WallThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicWallF2 = new G4LogicalVolume(solidWallF2,		                        //its solid
                                   WallMaterial,		                        //its material
                                   "WallF2");                               //its name
                                   
  physiWallF2 = new G4PVPlacement(0,			                                  //no rotation
  				 G4ThreeVector(-DICEThickness/2 + -WallThickness/2,0.*cm,0.*cm),
                                 logicWallF2,	  	                          //its logical volume				 
                                 "WallF2",		                              //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  }
  
  //     
  // DICE_2
  //

  //This object is chosen to be at the center of the world (0,0,0)
  solidDICE2=0; logicDICE2=0; physiDICE2=0;  
  
  if (DICEThickness > 0.)  {
  solidDICE2 = new G4Box("DICE2",				                                    //its name
                   DICEThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicDICE2 = new G4LogicalVolume(solidDICE2,		                          //its solid
                                   DICEMaterial,		                        //its material
                                   "DICE2");                                //its name
                                   
  physiDICE2 = new G4PVPlacement(0,			                                    //no rotation
  				 G4ThreeVector(0*cm, 0*cm, 0*cm),	                                //at (0,0,0)
                                 logicDICE2,		                            //its logical volume				 
                                 "DICE2",		                                //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  
  }
  
  
  //     
  // WallB_2
  //
  solidWallB2=0; logicWallB2=0; physiWallB2=0;  
  
  if (WallThickness > 0.)  {
  solidWallB2 = new G4Box("WallB2",				                                  //its name
                   WallThickness/2,DICESizeY/2,DICESizeZ/2);	              //its size
                         
  logicWallB2 = new G4LogicalVolume(solidWallB2,		                        //its solid
                                   WallMaterial,	                        	//its material
                                   "WallB2");                               //its name
                                   
  physiWallB2 = new G4PVPlacement(0,			                                  //no rotation
  				 G4ThreeVector(DICEThickness/2 + WallThickness/2,0.*cm,0.*cm),	
                                 logicWallB2,	  	                          //its logical volume				 
                                 "WallB2",		                              //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  }
  
  
  //     
  // PMT
  //
  solidPMT=0; logicPMT=0; physiPMT=0;  
  
  if (PMTThickness > 0.)  {
  solidPMT = new G4Box("PMT",				                                        //its name
                   PMTThickness/2,PMTSizeYZ/2,PMTSizeYZ/2);	                //its size
                         
  logicPMT = new G4LogicalVolume(solidPMT,		                              //its solid
                                   PMTMaterial,		                          //its material
                                   "PMT");                                  //its name
                                   
  physiPMT = new G4PVPlacement(0,			                                      //no rotation
  				 G4ThreeVector(-DICEThickness*1.5 - WallThickness*3 - PhantomThickness - PMTThickness/2 - 7.5*cm, 0, 0),	
                                 logicPMT,		                              //its logical volume				 
                                 "PMT",		                                  //its name
                                 logicWorld,		                            //its mother  volume
                                 false,			                                //no boolean operation
                                 0);			                                  //copy number
  
  }

G4double* zPA = new G4double[2];
zPA[0] = 0.*cm;			// z start
zPA[1] = AbsorberThickness;// z end

G4double* rI = new G4double[2];	 
rI[0] = 0.*cm;			// radius inner
rI[1] = 0.*cm;			// radius inner
G4double* rO = new G4double[2];
rO[0] = CalorSizeYZ; //radius outer
rO[1] = CalorSizeYZ; //radius outer

  //                               
  // Calorimeter
  //   
  
  solidAbsorber=0; logicAbsorber=0; physiAbsorber=0;  
  
  if (AbsorberThickness > 0.) 
    { solidAbsorber = new G4Polyhedra("Absorber",		                    //its name
			       0.*deg,				                                            //phi start
			       360.*deg, 			                                            //phi total
			       6, 				                                                //num of sides
			       2,				                                                  //num of z planes
			       zPA,				                                                //z plane
			       rI,				                                                //r inner
			       rO);				                                                //r outer
                         
      logicAbsorber = new G4LogicalVolume(solidAbsorber,                //its solid
      			                  AbsorberMaterial,                         //its material
                              "Absorber");                              //name
      			                  
      physiAbsorber = new G4PVPlacement(G4Transform3D(rm, G4ThreeVector(DICEThickness/2 + WallThickness+ 2.5*cm,0.,0.)),
                                        logicAbsorber,                   //its logical volume		    
                                        "Absorber",                      //its name
                                        logicWorld,                      //its mother
                                        false,                           //no boulean operation
                                        0);                              //copy number
                                        
    }
      
  
  //                                        
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  logicAbsorber->SetVisAttributes(simpleBoxVisAtt);


  //
  //always return the physical World
  //
  return physiWorld;
}

// === SET MATERIAL AND SIZES FUNCTIONS =======

void pbmDetectorConstruction::SetPhantomMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) PhantomMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetWallMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) WallMaterial = pttoMaterial;
}


void pbmDetectorConstruction::SetDICEMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) DICEMaterial = pttoMaterial;
}


void pbmDetectorConstruction::SetPMTMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) PMTMaterial = pttoMaterial;
}


void pbmDetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) AbsorberMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetBoneMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) BoneMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetPinMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) PinMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetMuscleMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) MuscleMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetTissueMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) TissueMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetFatMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) FatMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetProteseMaterial(G4String materialChoice)
{
  // search the material by its name   
  G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  if (pttoMaterial) ProteseMaterial = pttoMaterial;
}

void pbmDetectorConstruction::SetAbsorberThickness(G4double val)
{
  AbsorberThickness = val;
}

void pbmDetectorConstruction::SetCalorSizeYZ(G4double val)
{
  CalorSizeYZ  = val;
}


void pbmDetectorConstruction::SetMagField(G4double fieldValue)
{
  //apply a global uniform magnetic field along Z axis
  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if(magField) delete magField;		//delete the existing magn field

  if(fieldValue!=0.)			// create a new one if non nul
  { magField = new G4UniformMagField(G4ThreeVector(0.,0.,fieldValue));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  } else {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }
}


void pbmDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructCalorimeter());
}
