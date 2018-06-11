// Author: Maarten Hammer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes combined data of point reconstruction, the calorimeter, and the two TPCs to the ROOT output file in the form of histograms, a 3D graph, and a TTree. The TTree can be reused later in TReadReconstruction.
*/

// === INCLUDES =======
	#include "TWriteSingle.h"
	#include <iostream>
	#include "TMath.h"
	using namespace std;
	using namespace TMath;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: create TTree with addresses and histograms
	void TWriteSingle::Initialise() {
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
		// Parameters of reconstructed point
		fTree->Branch("x",    &fX,    "x/D"   );
		fTree->Branch("y",    &fY,    "y/D"   );
		fTree->Branch("z",    &fZ,    "z/D"   );
		fTree->Branch("eloss",&fEloss,"eloss/D");
		// Create graphs and histograms
		fTPC1pointsYZ = new TH2I(
			"TPC1pointsYZ","Points of entry TPC1;y (transverse);z (drift dir);count",
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)

		fRecPoints2D = new TH2S(
			"RecPoints","Distribution of reconstructed points;y (transverse);z (drift dir)", // name and title
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)
		fRecEnergy2D = new TH2D(
			"RecEnergies","Average energy losses of reconstructed points;y (transverse);z (drift dir)", // name and title
			pResolution, pYmin, pYmax,   // y bins (transverse direction)
			pResolution, pZmin, pZmax ); // z bins (drift direction)

		// Set drawing options
		fTPC1pointsYZ->SetOption(pDrawHistoOption);
		fRecPoints2D ->SetOption(pDrawHistoOption);
		fRecEnergy2D ->SetOption(pDrawHistoOption);
	}

	// RUN FUNCTION: load reconstructed points and corresponding tracks and write infor to histograms, the graph and the TTree
	StatusCode TWriteSingle::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Loop over timepixes
		fPointList = (TRecPointList_t*)fClipboard->Get("recpoint");
		fPointIter = fPointList->begin();
		while( fPointIter!=fPointList->end() ) {
			// Extract tracks
			fTrack1 = (*fPointIter)->GetTrack1();
			// Get track 1 info
			fTrack1->GetState();
			TVector3 temp(fTrack1->GetState());
			fPy1 = temp.y();
			fPz1 = fTrack1->GetState().Z();
			fVy1 = fTrack1->GetDirection().Y();
			fVz1 = fTrack1->GetDirection().Z();
			// If fit from cluster, add chi square and ndof
			if( fTrack1->GetFitterXY()) {
				fChiSqXY1 = fTrack1->GetFitterXY()->GetChisquare();
			}
			if( fTrack1->GetFitterXZ()) {
				fChiSqXY1 = fTrack1->GetFitterXZ()->GetChisquare();
			}
			// Get reconstructed point info
			fX = (*fPointIter)->GetPointAlt().X();
			fY = (*fPointIter)->GetPointAlt().Y();
			fZ = (*fPointIter)->GetPointAlt().Z();
			fEloss = (*fPointIter)->GetEnergyLoss();
			// Fill tree
			fTree->Fill();
			// Fill histograms and graph
			fTPC1pointsYZ->Fill(fPy1,fPz1);
			fRecPoints2D->Fill(fY,fZ);
			fRecEnergy2D->Fill(fY,fZ,fEloss);
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
	void TWriteSingle::Finalise(){
		// Project to xy plane (ignore drift direction)
		fRecPoints1D = (TH1S*)fRecPoints2D->ProjectionY();
		fRecEnergy1D = (TH1D*)fRecEnergy2D->ProjectionY();
		fRecEnergy2D->Divide(fRecPoints2D);
		// Set drawing options
		fRecPoints1D->SetOption(pDrawHistoOption);
		fRecEnergy1D->SetOption(pDrawHistoOption);
		// Write histograms, graph, and tree
		fTPC1pointsYZ ->Write();
		fRecEnergy1D->Write();
		fRecPoints1D->Write();
		fRecEnergy2D->Write();
		fRecPoints2D->Write();
		fTree         ->Write();
		// Delete them
		delete fTPC1pointsYZ; fTPC1pointsYZ = NULL;
		delete fRecPoints2D;  fRecPoints2D  = NULL;
		delete fRecEnergy2D;  fRecEnergy2D  = NULL;
	}