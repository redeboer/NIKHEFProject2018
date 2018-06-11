// Author: Maarten Hammer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm intersects takes two linear tracks, one from each TPC, and intersects those by extrapolating them and taking the point that is closest to both lines. This intersectin point is stored to the clipboard.
*/

// === INCLUDES =======
	#include "TSingleTracks.h"
	#include <iostream>
	#include "TMath.h"
	#include "TTrack.h"
	using namespace std;
	using namespace TMath;
	using namespace NIKHEFProject;
	using namespace NIKHEFProject::Detector;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TSingleTracks::Initialise() {}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TSingleTracks::Run()
	{
		// Works only if there are two tracks
		fTrackList = (TTrackList_t*)fClipboard->Get("tracks");
		if(fTrackList->size()==0) {
			if(fDebug) cout << endl << "  There are " << fTrackList->size() << " tracks while we want 1 tracks only" << endl;
			return NoData;
		}
		// Extract tracks
		fTrackIter = fTrackList->begin();
		fTrack1 = *fTrackIter;
		if(!fTrack1->GetCluster()->GetName().Contains(pTPC1id)) {
			if(fDebug) cout << endl << "  Track 1 does not belong to \"" << pTPC1id << ", but to \"" << fTrack1->GetCluster()->GetName() << "\"" << endl;
			return NoData;
		}

		if( fTrackIter==fTrackList->end() ) return NoData;
		// Reconstruct point of intersection and put to clipboard
		if(!Reconstruct()) return NoData;
		fRecPointObj = new TRecPoint(fTrack1, fRecPoint);
		fClipboard->Put(fRecPointObj);
		// Add energy if available
		fCaloList = (TCaloEventList_t*)fClipboard->Get("caloevents");
		
		if(fCaloList->size()==1) {
			if(fDebug && fCaloList->size()!=1 ) cout << "Warning: two calorimeters in clipboard" << endl;
			fCaloEvent = (TCaloEvent*)*fCaloList->begin();
			fRecPointObj->SetEnergyLoss(pBeamEnergy-fCaloEvent->GetEnergy());
		}
		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise here
	void TSingleTracks::Finalise() {}

// === PRIVATE FUNCTIONS =======
Bool_t TSingleTracks::Reconstruct() {

	if(!fTrack1) {
		if(fDebug) cout << endl << "No vector!" << endl;
		return false;
	}

	// Some handy variables
	fP1 = fTrack1->GetState();
	fD1 = fTrack1->GetDirection();
	// Find middle of the two nearest points
	fRecPoint  = fP1;

	return true;
}