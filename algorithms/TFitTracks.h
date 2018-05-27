// Author: Remco de Boer
// Date: May 22nd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm creates track objects for all clusters available in the clipboard. This is done through applying a linear fit (TLinearFitter) on the pixel data points associated to each cluster. See TTrack for how this information is stored.
*/

#ifndef TFITTRACKS_H
#define TFITTRACKS_H 1

// === INCLUDES =======
	#include "TLinearFitter.h"
	#include "TAlgorithm.h"
	#include "TTimepix.h"
	#include "TTrack.h"

// === CLASS DEFINITION =======
class TFitTracks : public TAlgorithm {

public:
	// Constructors and destructors
	// set writing bit to true if you want to see 2D Hough transforms
	TFitTracks(TClipboard* cp)
		: TAlgorithm(cp,"TFitTracks",false) {}
	TFitTracks(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TFitTracks",false,debug) {}
	~TFitTracks() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	Bool_t FitTrack(TTimepix*);
	// Temporary pointers
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TTimepixList_t* fClusterList;
	TTimepixIter_t fClusterIter;
	TTrack* fTrack;
	TLinearFitter* fFitterXY;
	TLinearFitter* fFitterXZ;

};

#endif // TFITTRACKS_H
