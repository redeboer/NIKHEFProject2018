// Author: Remco de Boer
// Date: May 23rd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from a text file generated with a Geant simulation (text format to be determined by yourself).
	In the current form, Geant generates tracks data in the form of vectors. When using the simulated eventloader, the recognise-and-fit-tracks algorithms can therefore be skipped.
*/
// === INCLUDES =======
	#include "TSimulatedEventLoader.h"
	#include <stdio.h>
	#include <sstream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: reads number of lines in file fInputFileName and use that as total number of events. Also prepares fFilestream for the Run.
	void TSimulatedEventLoader::Initialise()
	{
		// Open filestream
		string filename( pInput.Data() );
		fFilestream.open(filename);
		if(!fFilestream.is_open()) {
			cout << "Simulation event file \"" << filename << "\" does not exist\n" << endl;
			return;
		}
		cout << "Reading simulation event file \"" << pInput << "\"";
		fFilestream.getline(pBuffer,pBufferSize);
		if(fDebug) cout <<  " with format:" << endl << pBuffer << endl;
		else cout << endl;
		// Count number of lines after header
		while(fFilestream.getline(pBuffer,pBufferSize)) ++pTotalFiles;
		if(fDebug) cout << "(contains " << pTotalFiles << " events)" << endl;
		// Reopen file stream
		fFilestream.close();
		fFilestream.open(filename);
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TSimulatedEventLoader::Run()
	{
		/* Read line. Presumed format:
			event # ||
			point x TPC1 [pixel] || point y TPC1 [pixel] || point z TPC1 [pixel] ||
			dir x TPC1 || dir y TPC1 || dir z TPC1 ||
			point x TPC2 [pixel] || point y TPC2 [pixel] || point z TPC2 [pixel] ||
			dir x TPC2 || dir y TPC2 || dir z TPC2 ||
			energy Calo [MeV] */
		if(!fFilestream.getline(pBuffer,pBufferSize)) {
			if(fDebug) printf("\nEnd of file reached (event %d/%d)\n",pEventNumber,pTotalFiles);
			return Finished;
		}
		istringstream sstr(pBuffer);
		// Create two timepixes and put on clipboard (pixels won't be filled)
		ULong64_t timestamp;
		sstr >> timestamp; // event number
		TTimepix* timepix1 = new TTimepix("cam1",timestamp);
		TTimepix* timepix2 = new TTimepix("cam2",timestamp);
		fClipboard->Put(timepix1);
		fClipboard->Put(timepix2);
		// Create two tracks attached to these timepixes
		TTrack* track1 = new TTrack(timepix1);
		TTrack* track2 = new TTrack(timepix2);
		fClipboard->Put(track1);
		fClipboard->Put(track2);
		// Read point of entry in TPC1
		Double_t x,y,z;
		if( !(sstr >> x >> y >> z) ) return NoData;
		track1->SetState(x,y,z);
		// Read direction in TPC1
		if( !(sstr >> x >> y >> z) ) return NoData;
		track1->SetDirection(x,y,z);
		// Read point of entry in TPC2
		if( !(sstr >> x >> y >> z) ) return NoData;
		track2->SetState(x,y,z);
		// Read direction in TPC2
		if( !(sstr >> x >> y >> z) ) return NoData;
		track2->SetDirection(x,y,z);
		// Read energy for calorimeter
		if( !(sstr >> x) ) return NoData;
		TCalo* calo = new TCalo("calorimeter",timestamp,x);
		fClipboard->Put(calo);

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: only closes file stream
	void TSimulatedEventLoader::Finalise() { fFilestream.close(); }