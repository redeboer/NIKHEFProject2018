// Author: Remco de Boer
// Date: May 26th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm intersects takes two linear tracks, one from each TPC, and intersects those by extrapolating them and taking the point that is closest to both lines. This intersectin point is stored to the clipboard.
*/

// === INCLUDES =======
	#include "GlobalParameters.h"
	#include "TIntersectTracks.h"
	#include <iostream>
	#include "TMath.h"
	#include "TTrack.h"
	using namespace std;
	using namespace TMath;
	using namespace NIKHEFProject::Detector;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TIntersectTracks::Initialise() {}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TIntersectTracks::Run()
	{
		// Works only if there are two tracks
		fTrackList = (TTrackList_t*)fClipboard->Get("tracks");
		if(fTrackList->size()!=2) {
			if(fDebug) cout << endl << "There are " << fTrackList->size() << " tracks while we want 2 tracks only" << endl;
			return NoData;
		}
		// Extract tracks
		fTrackIter = fTrackList->begin();
		fTrack1 = *fTrackIter;
		if(!fTrack1->GetCluster()->GetName().Contains("cam1")) {
			if(fDebug) cout << endl << "Track 1 does not belong to \"cam1\", but to \"" << fTrack1->GetCluster()->GetName() << "\"" << endl;
			return NoData;
		}
		fTrackIter++;
		fTrack2 = *fTrackIter;
		while( !fTrack2->GetCluster()->GetName().Contains("cam2") && fTrackIter!=fTrackList->end() ) {
			if(fDebug) cout << endl << "Track 2 does not belong to \"cam2\", but to \"" << fTrack2->GetCluster()->GetName() << "\"" << endl;
			fTrackIter++;
		}
		if( fTrackIter==fTrackList->end() ) return NoData;
		// Reconstruct point of intersection and put to clipboard
		if(!Intersect()) return NoData;
		fRecPointObj = new TRecPoint(fTrack1,fTrack2,fRecPoint.X(),fRecPoint.Y(),fRecPoint.Z());
		fClipboard->Put(fRecPointObj);
		// Add energy if available
		fCaloList = (TCaloList_t*)fClipboard->Get("calorimeter");
		if(fCaloList->size()==1) {
			if(fDebug && fCaloList->size()!=1 ) cout << "Warning: two calorimeters in clipboard" << endl;
			fCalo = (TCalo*)*fCaloList->begin();
			fRecPointObj->SetEnergyLoss(pBeamEnergy-fCalo->GetEnergy());
		}
		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise here
	void TIntersectTracks::Finalise() {}

// === PRIVATE FUNCTIONS =======
Bool_t TIntersectTracks::Intersect() {

	if(!fTrack1||!fTrack2) {
		if(fDebug) cout << endl << "No vectors!" << endl;
		return false;
	}

	// Some handy variables
	// see https://en.m.wikipedia.org/wiki/Skew_lines for the maths
	fP1 = fTrack1->GetState();
	fP2 = fTrack2->GetState();
	fD1 = fTrack1->GetDirection();
	fD2 = fTrack2->GetDirection();
	fPdiff = fP1 - fP2;
	fDcross = fD1.Cross(fD2);
	fD1sq = fD1.Mag2();
	fD2sq = fD2.Mag2();
	fD1D2 = fD1.Dot(fD2);
	fN1 = fD1sq*fD2 - fD1D2*fD1;
	fN2 = fD2sq*fD1 - fD1D2*fD2;
	fDenominator  = fD1sq;
	fDenominator *= fD2sq;
	fDenominator -= fD1D2*fD1D2;
	fDenominator = 1/fDenominator;

	// Find distance between the two nearest points
	fDistance  = Abs(fDcross.Dot(fPdiff));
	fDistance /= fDcross.Mag();

	// Find middle of the two neares points
	fRecPoint  = fPdiff.Dot(fN2)*fD1;
	fRecPoint -= fPdiff.Dot(fN1)*fD2;
	fRecPoint *= fDenominator; // denominator
	fRecPoint += fP1;
	fRecPoint += fP2;
	fRecPoint *= .5;
	return true;
}