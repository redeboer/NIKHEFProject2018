// Author: Remco de Boer
// Date: May 27th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm writes combined data of point reconstruction, the calorimeter, and the two TPCs to the ROOT output file in the form of histograms, a 3D graph, and a TTree. The TTree can be reused later in TReadReconstruction.
*/

// === INCLUDES =======
	// #include "GlobalParameters.h"
	#include "GlobalParameters.h"
	#include "TWriteReconstruction.h"
	#include <iostream>
	using namespace std;
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
		fTree->Branch("eloss",&fEloss,"eloss/D");
		// Create graph and histogram
		fRecPointDist3D = new TH3D(
			"RecPointDistr","Distribution of reconstructed points;x (beam dir);y (transverse);z (drift dir)", // name and title
			100, -1500, 1200, // x bins (beam direction)
			100, 0, 350,   // y bins (transverse direction)
			100, 0, 450 ); // z bins (drift direction)
		fTPC1pointsYZ = new TH2I(
			"TPC1pointsYZ","Points of entry TPC1;y (transverse);z (drift dir);count",
			100, 0, 350,   // y bins (transverse direction)
			100, 0, 450 ); // z bins (drift direction)
		fTPC2pointsYZ = new TH2I(
			"TPC2pointsYZ","Points of entry TPC2;y (transverse);z (drift dir);count",
			100, 0, 350,   // y bins (transverse direction)
			100, 0, 450 ); // z bins (drift direction)
		fTPC2energyYZ = new TH2D(
			"TPC2energyYZ","Energy loss distribution when attributed to points of entry TPC2;y (transverse);z (drift dir);energy loss (GeV)",
			100, 0, 350,   // y bins (transverse direction)
			100, 0, 450 ); // z bins (drift direction)
		fRecPointDist3D->SetOption(pDrawHistoOption);
		fTPC1pointsYZ  ->SetOption(pDrawHistoOption);
		fTPC2pointsYZ  ->SetOption(pDrawHistoOption);
		fTPC2energyYZ  ->SetOption(pDrawHistoOption);
		fGraph = new TGraph2D(pTotalFiles);
		fGraph->SetNameTitle("RecPoints","Reconstructed points;x (beam dir);y (transverse);z (drift dir)");
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
			fX = (*fPointIter)->GetPoint().X();
			fY = (*fPointIter)->GetPoint().Y();
			fZ = (*fPointIter)->GetPoint().Z();
			fEloss = (*fPointIter)->GetEnergyLoss();
			// Fill tree
			fTree->Fill();
			// Fill histogram and graph
			fRecPointDist3D->Fill(fX,fY,fZ);
			fTPC1pointsYZ->Fill(fPy1,fPz1);
			fTPC2pointsYZ->Fill(fPy2,fPz2);
			fTPC2energyYZ->Fill(fPy2,fPz2,fEloss);
			fGraph->SetPoint(pEventNumber,fX,fY,fZ);
			// on succesfull, set nodata bit
			if(nodata) nodata = false;
			++fPointIter;
		}

		// NODATA: if no relevant data in clipboard
		if(nodata) {
			if(fDebug) cout << "No reconstructed poitns in clipboard for event " << pEventNumber << endl;
			return NoData;
		}

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: writes everything and then releases memory
	void TWriteReconstruction::Finalise(){
		// Project to xy plane (ignore drift direction)
		fRecPointDistXY = (TH2D*)fRecPointDist3D->Project3D("yx");
		fRecPointDistXZ = (TH2D*)fRecPointDist3D->Project3D("zx");
		fRecPointDistYZ = (TH2D*)fRecPointDist3D->Project3D("zy");
		fRecPointDistXY->SetOption(pDrawHistoOption);
		fRecPointDistXZ->SetOption(pDrawHistoOption);
		fRecPointDistYZ->SetOption(pDrawHistoOption);
		// Write histograms, graph, and tree
		fRecPointDist3D->Write();
		fRecPointDistXY->Write();
		fRecPointDistXZ->Write();
		fRecPointDistYZ->Write();
		fTPC1pointsYZ->Write();
		fTPC2pointsYZ->Write();
		fTPC2energyYZ->Write();
		fGraph->Write();
		fTree->Write();
		// Delete them
		delete fRecPointDistXY; fRecPointDistXY = NULL;
		delete fRecPointDistXZ; fRecPointDistXZ = NULL;
		delete fRecPointDistYZ; fRecPointDistYZ = NULL;
		delete fRecPointDist3D; fRecPointDist3D = NULL;
		delete fTPC1pointsYZ;   fTPC1pointsYZ   = NULL;
		delete fTPC2pointsYZ;   fTPC2pointsYZ   = NULL;
		delete fTPC2energyYZ;   fTPC2energyYZ   = NULL;
		delete fGraph;          fGraph          = NULL;
	}