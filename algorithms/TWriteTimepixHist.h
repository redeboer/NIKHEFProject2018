// Author: Remco de Boer
// Date: May 22nd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This algorithm writes all timepixes on the clipboard to the current (ROOT file) directory in the form of a 2D histogram.
*/

#ifndef TWRITETIMEPIXHIST_H
#define TWRITETIMEPIXHIST_H 1

// === INCLUDES =======
	#include "TH2D.h"
	#include "TAlgorithm.h"
	#include "TTimepix.h"

// === CLASS DEFINITION =======
class TWriteTimepixHist : public TAlgorithm {

public:
	// Constructors and destructors
	// has writing operation, so writing bit set to true
	TWriteTimepixHist(TClipboard* cp)
		: TAlgorithm(cp,"TWriteTimepixHist",true) {}
	TWriteTimepixHist(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TWriteTimepixHist",true,debug) {}
	~TWriteTimepixHist() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	void MakeHistogram(TTimepix*);
	// Data members
	TTimepixList_t* fClusterList;
	TTimepixIter_t fClusterIter;
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TH2D* fHistogram;

};

#endif // TWRITETIMEPIXHIST_H
