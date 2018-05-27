// Author: Remco de Boer
// Date: May 22nd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm creates track objects for all clusters available in the clipboard. This is done through applying a linear fit (TLinearFitter) on the pixel data points associated to each cluster. See TTrack for how this information is stored.
*/

// === INCLUDES =======
	#include "TFitTracks.h"
	#include <iostream>
	#include "TTrack.h"
	using namespace std;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TFitTracks::Initialise() {}

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

	// FINALISE FUNCTION: nothing to finalise here
	void TFitTracks::Finalise() {}

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
			fFitterXY->AddPoint(x,(*it)->GetRow()/*, <implement errors here>*/);
			fFitterXZ->AddPoint(x,(*it)->GetADC()/*, <implement errors here>*/);
			++it;
		}
		delete x;

		// Fit the two lines
		fFitterXY->Eval();
		fFitterXZ->Eval();

		// Set the track state and direction and their errors
		fTrack->SetState( 0.,
			fFitterXY->GetParameter(0),
			fFitterXZ->GetParameter(0) );
		fTrack->SetDirection( 1.,
			fFitterXY->GetParameter(1),
			fFitterXZ->GetParameter(1) );
		fTrack->SetStateError( 0.,
			fFitterXY->GetParError(0),
			fFitterXZ->GetParError(0) );
		fTrack->SetDirectionError( 0.,
			fFitterXY->GetParError(1),
			fFitterXZ->GetParError(1) );

		// Store track object on clipboard
		fClipboard->Put(fTrack);
		if(fDebug) fTrack->Print();
		return true;
	}