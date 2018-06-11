// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script takes care of the information of the whole run (all events combined).
  It couts the sizes of the objects at the begin or the run
  and the average energy depositions and tracklengths in the objects at the end of the run.
*/

// === INCLUDES =======

#include "pbmRunAction.hh"
#include "pbmDetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4ThreeVector.hh"

pbmRunAction::pbmRunAction(pbmDetectorConstruction* det)
:detector(det)
{}

pbmRunAction::~pbmRunAction()
{}

//This gets all the sizes from detector construction and couts them at the begin of the run.
void pbmRunAction::BeginOfRunAction(const G4Run* aRun)
{ 
   G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

   G4double ScThck = detector->GetPMTThickness();
   G4double ScYZ = detector->GetPMTSizeYZ(); 

   G4double PhThck = detector->GetPhantomThickness();
   G4double PhYZ = detector->GetPhantomSizeYZ();

   G4double DiceThck = detector->GetDICEThickness();
   G4double DiceY = detector->GetDICESizeY();
   G4double DiceZ = detector->GetDICESizeZ();

   G4double WallThickness = detector->GetWallThickness();

   G4double CalThck = detector->GetAbsorberThickness();
   G4double CalYZ = detector->GetCalorSizeYZ();

   G4cout<<"----- Sizes declaration -----"<<G4endl;
   G4cout <<ScThck<<" "<<ScYZ<<" "<<DiceThck + WallThickness*2<<" "<<DiceY<<" "<<DiceZ<<" "<<PhThck<<" "<<PhYZ<<" "<<CalThck<<" "<<CalYZ<<G4endl;

									   

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
  //initialize cumulative quantities
  //
  sumEAbs = sum2EAbs = sumEPh = sum2EPh = sumEDi1 = sum2EDi1 = 0.;
  sumLAbs = sum2LAbs = sumLPh = sum2LPh = sumLDi1 = sum2LDi1 = 0.; 
  sumEAir = sum2EAir = sumESc = sum2ESc = sumEDi2 = sum2EDi2 = 0.; 
  sumLAir = sum2LAir = sumLSc = sum2LSc = sumLDi2 = sum2LDi2 = 0.; 
}

//This function is filled in EventAction per event and is used to determine average energy depositions and tracklengths for all the objects.
void pbmRunAction::fillPerEvent(G4double EAbs, G4double EPh, G4double EWF1, G4double EDi1, G4double EWB1, G4double EWF2, G4double EDi2, G4double EWB2, G4double ESc, G4double EAir,
                                G4double LAbs, G4double LPh, G4double LWF1, G4double LDi1, G4double LWB1, G4double LWF2, G4double LDi2, G4double LWB2, G4double LSc, G4double LAir,
				 G4ThreeVector ExitSc, G4ThreeVector EnterPh, G4ThreeVector ExitPh, G4ThreeVector EnterDI1, G4ThreeVector ExitDI1, G4ThreeVector EnterDI2, G4ThreeVector ExitDI2)
{
  //accumulate statistic
  //
  sumEAbs += EAbs;  sum2EAbs += EAbs*EAbs;
  sumEPh += EPh;    sum2EPh += EPh*EPh;
  sumESc += ESc;    sum2ESc += ESc*ESc;
  sumEAir += EAir;  sum2EAir += EAir*EAir;
  
  sumEDi1 = EDi1 + sumEDi1 + EWF1 + EWB1;  sum2EDi1 += EDi1*EDi1;
  sumEDi2 = EDi2 + sumEDi2 + EWF2 + EWB2;  sum2EDi2 += EDi2*EDi2;
  
  sumLAbs += LAbs;  sum2LAbs += LAbs*LAbs;
  sumLPh += LPh;  sum2LPh += LPh*LPh;
  sumLSc += LSc;  sum2LSc += LSc*LSc;
  sumLAir += LAir;  sum2LAir += LAir*LAir;
  
  sumLDi1 = LDi1 + sumLDi1 + LWF1 + LWB1;  sum2LDi1 += LDi1*LDi1;
  sumLDi2 = LDi2 + sumLDi2 + LWF2 + LWB2;  sum2LDi2 += LDi2*LDi2;
}

void pbmRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;
  
  //compute statistics: mean and rms
  //
  sumEAbs /= NbOfEvents; sum2EAbs /= NbOfEvents;
  G4double rmsEAbs = sum2EAbs - sumEAbs*sumEAbs;
  if (rmsEAbs >0.) rmsEAbs = std::sqrt(rmsEAbs); else rmsEAbs = 0.;
  
  sumLAbs /= NbOfEvents; sum2LAbs /= NbOfEvents;
  G4double rmsLAbs = sum2LAbs - sumLAbs*sumLAbs;
  if (rmsLAbs >0.) rmsLAbs = std::sqrt(rmsLAbs); else rmsLAbs = 0.; 
  
  sumEPh /= NbOfEvents; sum2EPh /= NbOfEvents;
  G4double rmsEPh = sum2EPh - sumEPh*sumEPh;
  if (rmsEPh >0.) rmsEPh = std::sqrt(rmsEPh); else rmsEPh = 0.;

  sumESc /= NbOfEvents; sum2ESc /= NbOfEvents;
  G4double rmsESc = sum2ESc - sumESc*sumESc;
  if (rmsESc >0.) rmsESc = std::sqrt(rmsESc); else rmsESc = 0.;

  sumEAir /= NbOfEvents; sum2EAir /= NbOfEvents;
  G4double rmsEAir = sum2EAir - sumEAir*sumEAir;
  if (rmsEAir >0.) rmsEAir = std::sqrt(rmsEAir); else rmsEAir = 0.;
  
  sumEDi1 /= NbOfEvents; sum2EDi1 /= NbOfEvents;
  G4double rmsEDi1 = sum2EDi1 - sumEDi1*sumEDi1;
  if (rmsEDi1 >0.) rmsEDi1 = std::sqrt(rmsEDi1); else rmsEDi1 = 0.;

  sumEDi2 /= NbOfEvents; sum2EDi2 /= NbOfEvents;
  G4double rmsEDi2 = sum2EDi2 - sumEDi2*sumEDi2;
  if (rmsEDi2 >0.) rmsEDi2 = std::sqrt(rmsEDi2); else rmsEDi2 = 0.;
  
  sumLPh /= NbOfEvents; sum2LPh/= NbOfEvents;
  G4double rmsLPh= sum2LPh - sumLPh*sumLPh;
  if (rmsLPh >0.) rmsLPh = std::sqrt(rmsLPh); else rmsLPh = 0.;

  sumLSc /= NbOfEvents; sum2LSc/= NbOfEvents;
  G4double rmsLSc= sum2LSc - sumLSc*sumLSc;
  if (rmsLSc >0.) rmsLSc = std::sqrt(rmsLSc); else rmsLSc = 0.;

  sumLAir /= NbOfEvents; sum2LAir/= NbOfEvents;
  G4double rmsLAir= sum2LAir - sumLAir*sumLAir;
  if (rmsLAir >0.) rmsLAir = std::sqrt(rmsLAir); else rmsLAir = 0.;
  
  sumLDi1 /= NbOfEvents; sum2LDi1 /= NbOfEvents;
  G4double rmsLDi1 = sum2LDi1 - sumLDi1*sumLDi1;
  if (rmsLDi1 >0.) rmsLDi1 = std::sqrt(rmsLDi1); else rmsLDi1 = 0.;

  sumLDi2 /= NbOfEvents; sum2LDi2 /= NbOfEvents;
  G4double rmsLDi2 = sum2LDi2 - sumLDi2*sumLDi2;
  if (rmsLDi2 >0.) rmsLDi2 = std::sqrt(rmsLDi2); else rmsLDi2 = 0.;
  
  //print
  //
  G4cout
     << "\n--------------------End of Run------------------------------\n"
     <<"\n mean Energy in Scintill : "  << G4BestUnit(sumESc,"Energy")
     << " +- "                          << G4BestUnit(rmsESc,"Energy") 
     << "\n mean Energy in DICE1    : " << G4BestUnit(sumEDi1,"Energy")
     << " +- "                          << G4BestUnit(rmsEDi1,"Energy")
     << "\n mean Energy in Phantom : "  << G4BestUnit(sumEPh,"Energy")
     << " +- "                          << G4BestUnit(rmsEPh,"Energy")  
     << "\n mean Energy in DICE2    : " << G4BestUnit(sumEDi2,"Energy")
     << " +- "                          << G4BestUnit(rmsEDi2,"Energy")
     << "\n mean Energy in Absorber : " << G4BestUnit(sumEAbs,"Energy")
     << " +- "                          << G4BestUnit(rmsEAbs,"Energy")  
     << "\n mean Energy in Air      : " << G4BestUnit(sumEAir,"Energy")
     << " +- "                          << G4BestUnit(rmsEAir,"Energy")
     << "\n------------------------------------------------------------\n"
     << G4endl;
     
    G4cout << G4endl; 
    
  G4cout
     << "\n  mean trackLength in Scintill: " << G4BestUnit(sumLSc,"Length")
     << " +- "                               << G4BestUnit(rmsLSc,"Length") 
     << "\n  mean trackLength in DICE1   : " << G4BestUnit(sumLDi1,"Length")
     << " +- "                               << G4BestUnit(rmsLDi1,"Length")
     << "\n  mean trackLength in Phantom  : "<< G4BestUnit(sumLPh,"Length")
     << " +- "                               << G4BestUnit(rmsLPh,"Length") 
     << "\n  mean trackLength in DICE2   : " << G4BestUnit(sumLDi2,"Length")
     << " +- "                               << G4BestUnit(rmsLDi2,"Length") 
     << "\n  mean trackLength in Absorber : "<< G4BestUnit(sumLAbs,"Length")
     << " +- "                               << G4BestUnit(rmsLAbs,"Length")  
     << "\n  mean trackLength in Air      : "<< G4BestUnit(sumLAir,"Length")
     << " +- "                               << G4BestUnit(rmsLAir,"Length")
     << "\n------------------------------------------------------------\n"
     << G4endl;
}
