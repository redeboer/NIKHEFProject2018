// Author: Lizette Lamers
// Date: June 7th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads calo data from a text file generated with a Geant simulation (text format to be determined by yourself).
	In the current form, Geant generates calo data in a list of energies. When using the simulated calo-eventloader, the normal calo loader can therefore be skipped.
*/
// === INCLUDES =======
	#include "TString.h"
	#include "TSimulatedCaloLoader.h"
	#include <stdio.h>
	#include <sstream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: reads number of lines in file fInputFileName and use that as total number of events. Also prepares fFilestream for the Run.
	void TSimulatedCaloLoader::Initialise()
	{
		// Open filestream
		string filename( pCaloFileName );
		fFilestream.open(filename);
		if(!fFilestream.is_open()) {
			cout << "Simulation calo event file \"" << pCaloFileName << "\" does not exist\n" << endl;
			return;
		}
		cout << "Reading simulation calo event file \"" << pCaloFileName << "\"";
		fFilestream.getline(pBuffer,pBufferSize);
		if(fDebug) cout <<  " with format:" << endl << pBuffer << endl;
		else cout << endl;
		// Count number of lines after header
		pTotalFiles = 0;
		while(fFilestream.getline(pBuffer,pBufferSize)){ 
			++pTotalFiles;
		}
		if(fDebug) cout << "(contains " << pTotalFiles << " events)" << endl;
		// Reopen file stream
		fFilestream.close();
		fFilestream.open(filename);
		//read in initializing line
		fFilestream.getline(pBuffer,pBufferSize);
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TSimulatedCaloLoader::Run()
	{
		if(!fFilestream.getline(pBuffer,pBufferSize)) {
			if(fDebug) printf("\nEnd of file reached (event %d/%d)\n",pEventNumber,pTotalFiles);
			return Finished;
		}
		istringstream sstr(pBuffer);
		// Create two timepixes and put on clipboard (pixels won't be filled)
		ULong64_t timestamp;
		sstr >> timestamp; // event number
        Double_t En;
		// Read energy for calorimeter
		if( !(sstr >> En) ) return NoData;
		TCaloEvent* calo = new TCaloEvent(pEventNumber,timestamp);
		calo->SetEnergy(En);
		fClipboard->Put(calo);

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: only closes file stream
	void TSimulatedCaloLoader::Finalise() { fFilestream.close(); }