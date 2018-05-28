// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from each detecter in each event and places that data on the clipboard for further analysis.
	(For now, this means it only reads data files from two timepix detectors.)
*/

#ifndef TTIMEPIXEVENTLOADER_H
#define TTIMEPIXEVENTLOADER_H 1

// === INCLUDES =======
	#include <string>
	#include <fstream>
	#include <list>
	#include "TAlgorithm.h"
	#include "TTimepix.h"

// === CLASS DEFINITION =======
class TEventLoader : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TEventLoader(TClipboard* cp)
		: TAlgorithm(cp,"TEventLoader",false) {}
	TEventLoader(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TEventLoader",false,debug) {}
	~TEventLoader() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	void LoadTimepix(std::ifstream&,TTimepix*);
	// Data members
	std::list<std::string> fInputFilenames;
	std::list<std::string>::iterator fFileIterator;

};

#endif // TTIMEPIXEVENTLOADER_H
