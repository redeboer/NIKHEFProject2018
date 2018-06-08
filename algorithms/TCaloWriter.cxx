// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class writes histograms the histograms of the TCaloEvents. This can result in a large output ROOT file!
*/

// === INCLUDES =======
	#include "TCaloWriter.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise
	void TCaloWriter::Initialise() {}

	// RUN FUNCTION: read the next calo event in the text file and create a TCaloEvent from it
	StatusCode TCaloWriter::Run()
	{
		// Get calo events from clipboard
		fCaloEventList = (TCaloEventList_t*)fClipboard->Get("caloevents");
		fCaloEventIter = fCaloEventList->begin();
		if( fCaloEventIter == fCaloEventList->end() ) {
			if(fDebug) cout << endl << "Calowriter: No calodata in event " << pEventNumber << endl;
			return NoData;
		}
		// Write histograms of all calo event objects in clipboard
		while( fCaloEventIter != fCaloEventList->end() ) {
			(*fCaloEventIter)->WriteHistogram();
			++fCaloEventIter;
		}
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise
	void TCaloWriter::Finalise() {}