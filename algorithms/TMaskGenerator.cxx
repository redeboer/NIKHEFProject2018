// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	WARNING: TTimepixLoader has to be initialised and run before this algorithm!!
	This algorithm makes a file that lists all pixels that have hits and puts the coordinates of those pixels to a txt file. This file can then serve as a mask file. The mask file is loaded with TMaskLoader. (Make sure to handle these classes smartly in the algorithm list in Steering.cxx.)
	Note that the convention for coordinates is (row,column), as in a matrix, and for the dimension it is (nrows,ncols) = (height,width).
*/

// === INCLUDES =======
	#include "TMaskGenerator.h"
	#include "TPixel.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: makes timepix mask object
	void TMaskGenerator::Initialise() {
		// If pixel mask already exists in clipboard, we will expand on it
		fPixelMask = (TPixelMask*)fClipboard->GetFirst("pixelmask");
		if(!fPixelMask) {
			fPixelMask = new TPixelMask(pNRows,pNCols);
			fClipboard->Put(fPixelMask);
		}
		// Open file stream
		fFilestream.open(pMaskFileName);
		cout << "Writing pixel mask to \"" << pMaskFileName << "\"" << endl;
		// Reset counter
		fCount = 0;
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
			// Get timepix
			fTimepix = *fTimepixIter;
			// Loop over pixels and mask them if not yet done so
			pit = fTimepix->GetPixels()->begin();
			while( pit!=fTimepix->GetPixels()->end() ) {
				// get coordinates
				fRow = (*pit)->GetRow();
				fCol = (*pit)->GetColumn();
				// store if necessary
				if( !fPixelMask->IsMasked(fRow,fCol) ) {
					fPixelMask->MaskPixel(fRow,fCol);
					fFilestream << fRow << " " << fCol << endl;
					if(fDebug) cout << "  added pixel (" << setw(3) << fRow << "," << setw(3) << fCol << ") of \"" << fTimepix->GetName() << "\"" << endl;
					++fCount;
				}
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

	// FINALISE FUNCTION: closes file stream
	void TMaskGenerator::Finalise() {
		fFilestream.close();
		if(fDebug) cout << "Writen " << fCount << " masked pixels to file \"" << pMaskFileName << "\"" << endl;
	}