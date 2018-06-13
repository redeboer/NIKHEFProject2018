// Author: Remco de Boer
// Modified by: Maarten Hammer
// Date: June 8th 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm intersects takes two linear tracks, one from each TPC, and intersects those by extrapolating them and taking the point that is closest to both lines. This intersectin point is stored to the clipboard.
*/

#ifndef TINTERSECTTRACKS_H
#define TINTERSECTTRACKS_H 1

// === INCLUDES =======
	#include "TTrack.h"
	#include "TAlgorithm.h"
	#include "TCaloEvent.h"
	#include "TRecPoint.h"
	#include "TTrack.h"

// === CLASS DEFINITION =======
class TIntersectTracks : public TAlgorithm {

public:
	// Constructors and destructors
	// has no writing operation
	TIntersectTracks(TClipboard* cp)
		: TAlgorithm(cp,"TIntersectTracks",false) {}
	TIntersectTracks(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TIntersectTracks",false,debug) {}
	~TIntersectTracks() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	Bool_t Intersect();
	// Temporary pointers
	TTrackList_t* fTrackList;
	TTrackIter_t  fTrackIter;
	TCaloEventList_t* fCaloList;
	TCaloEvent* fCaloEvent;
	TRecPoint* fRecPointObj;
	TTrack* fTrack1;
	TTrack* fTrack2;
	TVector3 fRecPoint, fRecPointAlt;
	TVector3 fP1, fP2, fD1, fD2;
	TVector3 fN1, fN2;
	TVector3 fA1, fA2;
	TVector3 fPdiff, fDcross;
	Double_t fD1sq, fD2sq, fD1D2;
	Double_t fDistance;
	Double_t fDenominator;
	Double_t fTime1, fTime2;

};

#endif // TINTERSECTTRACKS_H
