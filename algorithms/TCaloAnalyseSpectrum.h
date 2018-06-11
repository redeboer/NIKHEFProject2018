// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class writes histograms the histograms of the TCaloEvents. This can result in a large output ROOT file!
*/

#ifndef TCALOANALYSESPECTRUM_T
#define TCALOANALYSESPECTRUM_T 1

// === INCLUDES =======
	#include <fstream>
	#include "TAlgorithm.h"

// === CLASS DEFINITION =======
class TCaloAnalyseSpectrum : public TAlgorithm {

public:
	// Constructors and destructors
	// does not need writing operation, so writing bit set to true
	// (output written to separate ROOT file)
	TCaloAnalyseSpectrum(TClipboard* cp)
		: TAlgorithm(cp,"TCaloAnalyseSpectrum",false) {}
	TCaloAnalyseSpectrum(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TCaloAnalyseSpectrum",false,debug) {}
	~TCaloAnalyseSpectrum() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	TCaloEventList_t* fCaloEventList;
	TCaloEventIter_t  fCaloEventIter;

};

#endif // TCALOANALYSESPECTRUM_T