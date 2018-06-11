// Author: Maarten Hammer
// Date: June 11th 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm intersects takes two linear tracks, one from each TPC, and intersects those by extrapolating them and taking the point that is closest to both lines. This intersectin point is stored to the clipboard.
*/

#ifndef TSINGLETRACK_H
#define TSINGLETRACK_H 1

// === INCLUDES =======
	#include "TTrack.h"
	#include "TAlgorithm.h"
	#include "TCaloEvent.h"
	#include "TRecPoint.h"
	#include "TTrack.h"

// === CLASS DEFINITION =======
class TSingleTracks : public TAlgorithm {

public:
	// Constructors and destructors
	// set writing bit to true if you want to see 2D Hough transforms
	TSingleTracks(TClipboard* cp)
		: TAlgorithm(cp,"TSingleTracks",false) {}
	TSingleTracks(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TSingleTracks",false,debug) {}
	~TSingleTracks() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	Bool_t Reconstruct();
	// Temporary pointers
	TTrackList_t* fTrackList;
	TTrackIter_t  fTrackIter;
	TCaloEventList_t* fCaloList;
	TCaloEvent* fCaloEvent;
	TRecPoint* fRecPointObj;
	TTrack* fTrack1;
	TVector3 fRecPoint;
	TVector3 fP1, fD1;
	Double_t fD1sq, fD2sq;

};

#endif // TINTERSECTTRACKS_H
