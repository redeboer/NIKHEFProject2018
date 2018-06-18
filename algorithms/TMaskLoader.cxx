// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm makes a file that lists all pixels that have hits and puts the coordinates of those pixels to a txt file. This file can then serve as a mask file. The mask file is loaded with TMaskLoader. (Make sure to handle these classes smartly in the algorithm list in Steering.cxx.)
	Note that the convention for coordinates is (row,column), as in a matrix, and for the dimension it is (nrows,ncols) = (height,width).
*/

// === INCLUDES =======
	#include "TMaskLoader.h"
	#include <iostream>
	#include <fstream>
	#include <sstream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: creates 
	void TMaskLoader::Initialise() {
		// Check if timepix object is in clipboard
		fPixelMask = (TPixelMask*)fClipboard->GetFirst("pixelmask");
		if(fPixelMask) {
			if(fDebug) cout << "Pixel mask object already exists on clipboard" << endl;
			return;
		}
		// Create timepix new, empty timepix object (do NOT store to clipboard)
		fPixelMask = new TPixelMask(pNRows,pNCols);
		// Attempt to open mask file
		ifstream file(pMaskFileName.Data());
		if(!file.is_open()) {
			cout << "Mask file \"" << pMaskFileName << "\" does not exist!" << endl;
			return;
		}
		cout << "Reading pixel mask from file \"" << pMaskFileName << "\"" << endl;
		// Loop over mask file
		Bool_t fail = false;
		UShort_t row, col, dummy;
		UInt_t npixels=0;
		while( file.getline(pBuffer,pBufferSize) ) {
			stringstream sstr(pBuffer);
			if(!(sstr>>row)) {
				if(fDebug) cout << "Error: Mask file contains empty line" << endl;
				fail = true;
				break;
			}
			if(!(sstr>>col)) {
				if(fDebug) cout << "Error: Mask file contains line with one value only" << endl;
				fail = true;
				break;
			}
			if(sstr>>dummy) {
				if(fDebug) cout << "Error: Mask file contains line with three values so aborted" << endl;
				fail = true;
				break;
			}
			fPixelMask->MaskPixel(row,col);
			++npixels;
		}
		// Put file pixel mask on clipboard if successfull
		if(fail) delete fPixelMask;
		else {
			if(fDebug) cout << "Successfully loaded " << npixels << " masked pixels" << endl;
			fClipboard->Put(fPixelMask);
		}
		// Close file stream
		file.close();
	}

	// RUN FUNCTION: nothing to run
	StatusCode TMaskLoader::Run() { return Success; }

	// FINALISE FUNCTION: nothing to finalise
	void TMaskLoader::Finalise() {}