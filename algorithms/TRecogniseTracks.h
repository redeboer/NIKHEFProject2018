// Author: Remco de Boer
// Date: May 22nd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm recognises tracks in all timepixes currently on the clipboard. The makes new timepixes (now called "clusters", but still of TTimepix object type) that contain only those pixels that lie in a straight line. Tracks are recognised using a 2D Hough transform.
	All clusters recognised in the original timepix (maximum number of tracks defined in TParametesr) are stored to the clipboard.
*/

#ifndef TRECOGNISETRACKS_H
#define TRECOGNISETRACKS_H 1

// === INCLUDES =======
	#include "TAlgorithm.h"
	#include "TTimepix.h"
	#include "TH2I.h"

// === CLASS DEFINITION =======
class TRecogniseTracks : public TAlgorithm {

public:
	// Constructors and destructors
	// set writing bit to true if you want to store the 2D Hough transforms
	TRecogniseTracks(TClipboard* cp)
		: TAlgorithm(cp,"TRecogniseTracks",true) {}
	TRecogniseTracks(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TRecogniseTracks",true,debug) {}
	~TRecogniseTracks() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	UChar_t RecogniseTracks(TTimepix*);
	Double_t ComputeHoughR(Double_t,Double_t,Double_t);
	TH2I* MakeHough2D(TTimepix*);
	// Local data members
	UChar_t fNTracks;
	// Temporary pointers
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TTimepix* fCluster;
	TTimepix* fRemainder;

};

#endif // TRECOGNISETRACKS_H
