// Author: Remco de Boer
// Date: June 13th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm transposes al timepixes on the clipboard. Columns become rows.
*/

#ifndef TTIMEPIXTRANSPOSE_H
#define TTIMEPIXTRANSPOSE_H 1

// === INCLUDES =======
	#include "TAlgorithm.h"
	#include "TTimepix.h"
	#include "TPixel.h"

// === CLASS DEFINITION =======
class TTimepixTranspose : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TTimepixTranspose(TClipboard* cp, Bool_t debug=false)
		: TAlgorithm(cp,"TTimepixTranspose",false,debug), fTimepixList(NULL) {}
	~TTimepixTranspose() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TPixelList_t* fPixelList;
	TPixelIter_t fPixelIter;
	UShort_t fRow;
	UShort_t fCol;

};

#endif // TTIMEPIXTRANSPOSE_H