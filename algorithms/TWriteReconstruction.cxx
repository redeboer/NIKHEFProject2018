// Author: Remco de Boer
// Date: May 27th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes combined data of point reconstruction, the calorimeter, and the two TPCs to the ROOT output file in the form of histograms, a 3D graph, and a TTree. The TTree can be reused later in TReadReconstruction.
*/

// === INCLUDES =======
	#include "TWriteReconstruction.h"
	#include <iostream>
	#include "TMath.h"
	using namespace std;
	using namespace TMath;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: create TTree with addresses and histograms
	void TWriteReconstruction::Initialise() {
		// Create TTree// Define TTree and branches
		fTree = new TTree(pTreeName,pTreeTitle);
		// Event number
		fTree->Branch("id",&pEventNumber,"id/l");
		// Parameters track 1
		fTree->Branch("chisqXY1",&fChiSqXY1,"chisq1XY/D");
		fTree->Branch("chisqXZ1",&fChiSqXZ1,"chisq1xZ/D");
		fTree->Branch("py1",     &fPy1,     "py1/D"     );
		fTree->Branch("pz1",     &fPz1,     "pz1/D"     );
		fTree->Branch("vy1",     &fVy1,     "vy1/D"     );
		fTree->Branch("vz1",     &fVz1,     "vz1/D"     );
		// Parameters track 2
		fTree->Branch("chisqXY2",&fChiSqXY2,"chisqXY2/D");
		fTree->Branch("chisqXZ2",&fChiSqXZ2,"chisqXZ2/D");
		fTree->Branch("py2",     &fPy2,     "py2/D"     );
		fTree->Branch("pz2",     &fPz2,     "pz2/D"     );
		fTree->Branch("vy2",     &fVy2,     "vy2/D"     );
		fTree->Branch("vz2",     &fVz2,     "vz2/D"     );
		// Parameters of reconstructed point
		fTree->Branch("x",    &fX,    "x/D"   );
		fTree->Branch("y",    &fY,    "y/D"   );
		fTree->Branch("z",    &fZ,    "z/D"   );
		fTree->Branch("angle",&fAngle,"angle/D");
		fTree->Branch("eloss",&fEloss,"eloss/D");
		// Create graphs and histograms
		fTPC1pointsYZ = new TH2I(
			"TPC1pointsYZ","Points of entry TPC1;y (transverse);z (drift dir);count",
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fTPC2pointsYZ = new TH2I(
			"TPC2pointsYZ","Points of entry TPC2;y (transverse);z (drift dir);count",
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fRecPoints3D = new TH3S(
			"RecPoints","Distribution of reconstructed points;x (beam dir);y (transverse);z (drift dir)", // name and title
			pResolution, pXmin, pXmax,   // x bins (beam direction)
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fRecEnergy3D = new TH3D(
			"RecEnergies","Average energy losses of reconstructed points;x (beam dir);y (transverse);z (drift dir)", // name and title
			pResolution, pXmin, pXmax,   // x bins (beam direction)
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fRecAngles2D = new TH2D(
			"RecAngles","Distribution of reconstructed angles;y (transverse);z (drift dir)", // name and title
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fRecAngles = new TH1I(
			"RecAngles","Scattering angles;Angle (degrees);count", // name and title
			200, -30, 30);
		if(pTotalFiles>0) fGraph = new TGraph2D(pTotalFiles);
		else              fGraph = new TGraph2D();
		fGraph->SetNameTitle("RecPoints","Reconstructed points;x (beam dir);y (transverse);z (drift dir)");
		// Set drawing options
		fTPC1pointsYZ->SetOption(pDrawHistoOption);
		fTPC2pointsYZ->SetOption(pDrawHistoOption);
		fRecPoints3D ->SetOption(pDrawHistoOption);
		fRecEnergy3D ->SetOption(pDrawHistoOption);
		fRecAngles2D ->SetOption(pDrawHistoOption);
		fRecAngles   ->SetOption(pDrawHistoOption);
	}

	// RUN FUNCTION: load reconstructed points and corresponding tracks and write infor to histograms, the graph and the TTree
	StatusCode TWriteReconstruction::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Loop over timepixes
		fPointList = (TRecPointList_t*)fClipboard->Get("recpoint");
		fPointIter = fPointList->begin();
		while( fPointIter!=fPointList->end() ) {
			// Extract tracks
			fTrack1 = (*fPointIter)->GetTrack1();
			fTrack2 = (*fPointIter)->GetTrack2();
			// Abort if one of the tracks is missing
			if(!fTrack1) {
				if(fDebug) cout << "Track 1 is missing!"<< endl;
				return NoData;
			}
			if(!fTrack2) {
				if(fDebug) cout << "Track 2 is missing!"<< endl;
				return NoData;
			}
			// Get track 1 info
			fTrack1->GetState();
			TVector3 temp(fTrack1->GetState());
			fPy1 = temp.y();
			fPz1 = fTrack1->GetState().Z();
			fVy1 = fTrack1->GetDirection().Y();
			fVz1 = fTrack1->GetDirection().Z();
			// Get track 2 info
			fPy2 = fTrack2->GetState().Y();
			fPz2 = fTrack2->GetState().Z();
			fVy2 = fTrack2->GetDirection().Y();
			fVz2 = fTrack2->GetDirection().Z();
			// If fit from cluster, add chi square and ndof
			if( fTrack1->GetFitterXY() && fTrack2->GetFitterXY() ) {
				fChiSqXY1 = fTrack1->GetFitterXY()->GetChisquare();
				fChiSqXZ2 = fTrack2->GetFitterXY()->GetChisquare();
			}
			if( fTrack1->GetFitterXZ() && fTrack2->GetFitterXZ() ) {
				fChiSqXY1 = fTrack1->GetFitterXZ()->GetChisquare();
				fChiSqXZ2 = fTrack2->GetFitterXZ()->GetChisquare();
			}
			// Get reconstructed point info
			fX = (*fPointIter)->GetPointAlt().X();
			fY = (*fPointIter)->GetPointAlt().Y();
			fZ = (*fPointIter)->GetPointAlt().Z();
			fAngle = TMath::RadToDeg()*ACos( fTrack1->GetDirection().Dot(fTrack2->GetDirection()) );
			fEloss = (*fPointIter)->GetEnergyLoss();
			// Fill tree
			fTree->Fill();
			// Fill histograms and graph
			fTPC1pointsYZ->Fill(fPy1,fPz1);
			fTPC2pointsYZ->Fill(fPy2,fPz2);
			fRecPoints3D->Fill(fX,fY,fZ);
			fRecEnergy3D->Fill(fX,fY,fZ,fEloss);
			fRecAngles2D->Fill(fY,fZ,fAngle);
			fRecAngles->Fill(fAngle);
			fGraph->SetPoint(pEventNumber,fX,fY,fZ);
			// on succesfull, set nodata bit
			if(nodata) nodata = false;
			++fPointIter;
		}

		// NODATA: if no relevant data in clipboard
		if(nodata) {
			if(fDebug) cout << "No reconstructed points in clipboard for event " << pEventNumber << endl;
			return NoData;
		}

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: writes everything and then releases memory
	void TWriteReconstruction::Finalise(){
		// Project to xy plane (ignore drift direction)
		fRecPoints3Dxy = (TH2S*)fRecPoints3D->Project3D("yx");
		fRecPoints3Dxz = (TH2S*)fRecPoints3D->Project3D("zx");
		fRecPoints3Dyz = (TH2S*)fRecPoints3D->Project3D("zy");
		fRecEnergy3Dxy = (TH2D*)fRecEnergy3D->Project3D("yx");
		fRecEnergy3Dxz = (TH2D*)fRecEnergy3D->Project3D("zx");
		fRecEnergy3Dyz = (TH2D*)fRecEnergy3D->Project3D("zy");
		fRecEnergy3Dxy->Divide(fRecPoints3Dxy);
		fRecEnergy3Dxz->Divide(fRecPoints3Dxz);
		fRecEnergy3Dyz->Divide(fRecPoints3Dyz);
		fRecAngles2D  ->Divide(fRecPoints3Dyz);
		// Set drawing options
		fRecPoints3Dxy->SetOption(pDrawHistoOption);
		fRecPoints3Dxz->SetOption(pDrawHistoOption);
		fRecPoints3Dyz->SetOption(pDrawHistoOption);
		fRecEnergy3Dxy->SetOption(pDrawHistoOption);
		fRecEnergy3Dxz->SetOption(pDrawHistoOption);
		fRecEnergy3Dyz->SetOption(pDrawHistoOption);
		// Write histograms, graph, and tree
		fTPC1pointsYZ ->Write(0,TObject::kOverwrite);
		fTPC2pointsYZ ->Write(0,TObject::kOverwrite);
		fRecPoints3D  ->Write(0,TObject::kOverwrite);
		fRecPoints3Dxy->Write(0,TObject::kOverwrite);
		fRecPoints3Dxz->Write(0,TObject::kOverwrite);
		fRecPoints3Dyz->Write(0,TObject::kOverwrite);
		fRecEnergy3D  ->Write(0,TObject::kOverwrite);
		fRecEnergy3Dxy->Write(0,TObject::kOverwrite);
		fRecEnergy3Dxz->Write(0,TObject::kOverwrite);
		fRecEnergy3Dyz->Write(0,TObject::kOverwrite);
		fRecAngles2D  ->Write(0,TObject::kOverwrite);
		fRecAngles    ->Write(0,TObject::kOverwrite);
		fGraph        ->Write(0,TObject::kOverwrite);
		fTree         ->Write(0,TObject::kOverwrite);
		// Delete them
		delete fTPC1pointsYZ; fTPC1pointsYZ = NULL;
		delete fTPC2pointsYZ; fTPC2pointsYZ = NULL;
		delete fRecPoints3D;  fRecPoints3D  = NULL;
		delete fRecEnergy3D;  fRecEnergy3D  = NULL;
		delete fRecAngles2D;  fRecAngles2D  = NULL;
		delete fRecAngles;    fRecAngles    = NULL;
		delete fGraph;        fGraph        = NULL;
	}