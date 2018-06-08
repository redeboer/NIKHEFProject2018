// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class writes histograms the histograms of the TCaloEvents. This can result in a large output ROOT file!
*/

#ifndef TCALOWRITER_T
#define TCALOWRITER_T 1

// === INCLUDES =======
	#include <fstream>
	#include "TAlgorithm.h"
	#include "TCaloEvent.h"

// === CLASS DEFINITION =======
class TCaloWriter : public TAlgorithm {

public:
	// Constructors and destructors
	// has writing operation, so writing bit set to true
	TCaloWriter(TClipboard* cp)
		: TAlgorithm(cp,"TCaloWriter",true) {}
	TCaloWriter(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TCaloWriter",true,debug) {}
	~TCaloWriter() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	TCaloEventList_t* fCaloEventList;
	TCaloEventIter_t  fCaloEventIter;

};

#endif // TCALOWRITER_T