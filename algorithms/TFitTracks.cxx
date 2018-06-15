// Author: Remco de Boer
// Date: June 13th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm creates track objects for all clusters available in the clipboard. This is done through applying a linear fit (TLinearFitter) on the pixel data points associated to each cluster. See TTrack for how this information is stored.
	All fit parameters are stored to a TTree.
*/

// === INCLUDES =======
	#include "TFitTracks.h"
	#include <iostream>
	#include "TTrack.h"
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: makes a ttree
	void TFitTracks::Initialise() {
		// Create tree
		fTree = new TTree("fits_tracks","Fit parameters of tracks");
		// Set branches
		fTree->Branch("XYpar0",&fXYp0, "XYpar0/D");
		fTree->Branch("XYpar1",&fXYp1, "XYpar1/D");
		fTree->Branch("XYerr0",&fXYe0, "XYerr0/D");
		fTree->Branch("XYerr1",&fXYe1, "XYerr1/D");
		fTree->Branch("XYchi2",&fXYchi,"XYchi2/D");
		fTree->Branch("XYndof",&fXYndf,"XYndof/D");
		fTree->Branch("XZpar0",&fXZp0, "XZpar0/D");
		fTree->Branch("XZpar1",&fXZp1, "XZpar1/D");
		fTree->Branch("XZerr0",&fXZe0, "XZerr0/D");
		fTree->Branch("XZerr1",&fXZe1, "XZerr1/D");
		fTree->Branch("XZchi2",&fXZchi,"XZchi2/D");
		fTree->Branch("XZndof",&fXZndf,"XZndof/D");
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TFitTracks::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Loop over timepixes
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		while( fTimepixIter!=fTimepixList->end() ) {
			// Counter for number of fits
			UChar_t nfits = 0;
			// Get list of clusters and loop over it
			fClusterList = (TTimepixList_t*)fClipboard->Get((*fTimepixIter)->GetName()+"clusters");
			fClusterIter = fClusterList->begin();
			while( fClusterIter!=fClusterList->end() ) {
				nfits += FitTrack(*fClusterIter);
				MakeHistogramFit(*fClusterIter);
				fHistogram->Write();
				++fClusterIter;
			}
			// Check if there was at least one track fit for each timepix
			nodata = !nfits && nodata;
			++fTimepixIter;
		}

		// NODATA: if no timepix in clipboard
		if(nodata) {
			if(fDebug) cout << endl << "One of the timepixes has no clusters" << endl;
			return NoData;
		}

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: store TTree
	void TFitTracks::Finalise() {
		fTree->Write();
	}

// === PRIVATE FUNCTIONS =======
	Bool_t TFitTracks::FitTrack(TTimepix* cluster)
	{
		// Make track object from cluster
		fTrack = new TTrack(cluster);
		fFitterXY = fTrack->GetFitterXY();
		fFitterXZ = fTrack->GetFitterXZ();

		// Fill its two fitters with data from its pixels
		// (not sure if this is the best way, but Corryvreckan also uses AddPoint in this way)
		Double_t* x = new Double_t[1];
		TPixelIter_t it = cluster->GetPixels()->begin();
		while( it!=cluster->GetPixels()->end() ) {
			// Add it to the fitter
			x[0] = (*it)->GetColumn();
			fFitterXY->AddPoint(x,(*it)->GetColumn()/*, <implement errors here>*/);
			fFitterXZ->AddPoint(x,(*it)->GetADC()/*, <implement errors here>*/);
			++it;
		}
		delete x;

		// Fit the two lines
		fFitterXY->Eval();
		fFitterXZ->Eval();

		// Store fit parameters
		fXYp0  = fFitterXY->GetParameter(0);
		fXYp1  = fFitterXY->GetParameter(1);
		fXYe0  = fFitterXY->GetParError(0);
		fXYe1  = fFitterXY->GetParError(1);
		fXYchi = fFitterXY->GetChisquare();
		fXYndf = fFitterXY->GetNpoints()-fFitterXY->GetNumberFreeParameters();
		fXZp0  = fFitterXZ->GetParameter(0);
		fXZp1  = fFitterXZ->GetParameter(1);
		fXZe0  = fFitterXZ->GetParError(0);
		fXZe1  = fFitterXZ->GetParError(1);
		fXZchi = fFitterXZ->GetChisquare();
		fXYndf = fFitterXZ->GetNpoints()-fFitterXZ->GetNumberFreeParameters();
		fTree->Fill();

		// Set the track state and direction and their errors
		fTrack->SetState( 0., fXYp0, fXZp0 );
		fTrack->SetDirection( 1., fXYp1, fXZp1 );
		fTrack->SetStateError( 0., fXYe0, fXZe0 );
		fTrack->SetDirectionError( 0., fXYe1, fXZe1 );

		// Store track object on clipboard
		fClipboard->Put(fTrack);
		if(fDebug) fTrack->Print();
		return true;
	}
	// Function that is adopted from TWriteTimepixHist, but now adds a fit
	void TFitTracks::MakeHistogramFit(TTimepix* timepix)
	{
		// Generate appropriate name and title
		TString name(timepix->GetName());
		// Create histogram of correct number of pixels
		fHistogram = new TH2D( name.Data(), name.Data(), timepix->GetNColumns(), 0, timepix->GetNColumns(), timepix->GetNRows(), 0, timepix->GetNRows() );
		fHistogram->SetOption(pDrawHistoOption);
		// Fill bins by looping over pixels in timepix
		TPixelIter_t it = timepix->GetPixels()->begin();
		while( it!=timepix->GetPixels()->end() ) {
			fHistogram->SetBinContent( (*it)->GetColumn(), (*it)->GetRow(), (*it)->GetADC() );
			++it;
		}
		// Add tracks
	}