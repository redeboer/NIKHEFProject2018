// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	WARNING: TTimepixLoader has to be initialised and run before this algorithm!!
	This algorithm makes a file that lists all pixels that have hits and puts the coordinates of those pixels to a txt file. This file can then serve as a mask file. The mask file is loaded with TMaskLoader. (Make sure to handle these classes smartly in the algorithm list in Steering.cxx.)
*/

// === INCLUDES =======
	#include "TMaskGenerator.h"
	#include "TPixel.h"
	#include <fstream>
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: makes timepix mask object
	void TMaskGenerator::Initialise() {
		fPixelMask = (TPixelMask*)fClipboard->Get("pixelmask");
		if(!fPixelMask) fPixelMask = new TPixelMask(pNCols,pNRows);
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TMaskGenerator::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;
		// Get list of timepixes and loop over it
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		TPixelIter_t pit;
		while( fTimepixIter!=fTimepixList->end() ) {
			// Loop over pixels and and mask them
			pit = (*fTimepixIter)->GetPixels()->begin();
			while( pit!=(*fTimepixIter)->GetPixels()->end() ) {
				fPixelMask->MaskPixel( (*pit)->GetColumn(), (*pit)->GetRow() );
				++pit;
			}
			if(nodata) nodata = false;
			++fTimepixIter;
		}
		// NODATA: if no timepix in clipboard
		if(nodata) {
			if(fDebug) cout << endl << "  --> no timepix to mask pixels" << endl;
			return NoData;
		}
		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: write masked pixels and delete pixel mask object
	void TMaskGenerator::Finalise() {
		// Write lines
		ofstream file(pMaskFileName.Data());
		for( UShort_t i=0; i<fPixelMask->GetMask().size(); ++i )
			for( UShort_t j=0; j<fPixelMask->GetMask()[i].size(); ++j )
				if(fPixelMask->GetMask()[i][j])
					file << i << " " << j << endl;
		// Delete pixel mask object
		delete fPixelMask;
	}