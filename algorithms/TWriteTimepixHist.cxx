// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes all timepixes on the clipboard to the current (ROOT file) directory in the form of a 2D histogram.
*/

// === INCLUDES =======
	#include "TWriteTimepixHist.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TWriteTimepixHist::Initialise() {}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TWriteTimepixHist::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Loop over timepixes
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		while( fTimepixIter!=fTimepixList->end() ) {
			// Write timepix
			MakeHistogram(*fTimepixIter);
			fHistogram->Write();
			delete fHistogram; // not written to clipboard so deleted asap
			// Get list of clusters and loop over it to write them (if available)
			fClusterList = (TTimepixList_t*)fClipboard->Get((*fTimepixIter)->GetName()+"clusters");
			fClusterIter = fClusterList->begin();
			while( fClusterIter!=fClusterList->end() ) {
				MakeHistogram(*fClusterIter);
				fHistogram->Write();
				delete fHistogram;
				++fClusterIter;
			}
			// on succesfull, set nodata bit
			if(nodata) nodata = false;
			++fTimepixIter;
		}

		// NODATA: if no timepix in clipboard
		if(nodata) {
			if(fDebug) cout << "No timepix in clipboard" << endl;
			return NoData;
		}

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise here
	void TWriteTimepixHist::Finalise() {}

// === PRIVATE FUNCTIONS =======
	void TWriteTimepixHist::MakeHistogram(TTimepix* timepix)
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
	}