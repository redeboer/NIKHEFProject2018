// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	The analysis class is the core class which allows the event processing to run. It contains a list of pointers to algorithms, each of which is (1) initialised, (2) run on each event, and (3) finalised. This class does not define what an event is, but merely runs each algorithm sequentially and passes the clipboard between them (erasing it at the end of each Run sequence).
	An example: you can start with (a) some load algorithm, follow up with (b) an algorithm that writes the data loaded to some histograms, and end with (c) some track fitting. The cool thing is that you can now easily cut out the writing step (b) by commenting it in Steering, or replace it with some other algorithm.
	When an algorithm returns NoData (see TAlgorithm), it will simply jump to the next event. When an algorithm returns Finished, the event processing will stop at the end of run.
*/

#ifndef TANALYSIS_H
#define TANALYSIS_H 1

// === INCLUDES =======
	#include <string>
	#include <list>
	#include "TAlgorithm.h"
	#include "TClipboard.h"
	#include "TStopwatch.h"

// === CLASS DECLARATIONS =======
class TAnalysis {

public:

	// Constructors and destructors
	TAnalysis(TClipboard* cp) : fClipboard(cp) {}
	~TAnalysis();

	// Member functions
	void Add(TAlgorithm*);
	void InitialiseAll();
	void RunAll();
	void FinaliseAll();
	void Timing();

protected:
	TClipboard* fClipboard;
	TStopwatch* fEventStopwatch;
	TStopwatch* fLocalStopwatch;
	Double_t fTimingInitial;
	Double_t fTimingFinal;
	Double_t fETA;
	UShort_t fETAcount;
	std::string fETAString;
	TAlgorithmList_t fAlgorithms;

};

#endif // TANALYSIS_H
