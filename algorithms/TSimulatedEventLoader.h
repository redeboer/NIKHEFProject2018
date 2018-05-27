// Author: Remco de Boer
// Date: May 23rd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from a text file generated with a Geant simulation (text format to be determined by yourself).
	In the current form, Geant generates tracks data in the form of vectors. When using the simulated eventloader, the recognise-and-fit-tracks algorithms can therefore be skipped.
*/

#ifndef TSIMULATEDEVENTLOADER_H
#define TSIMULATEDEVENTLOADER_H 1

// === INCLUDES =======
	#include <string>
	#include <fstream>
	#include <list>
	#include "TAlgorithm.h"
	#include "TTimepix.h"

// === CLASS DEFINITION =======
class TSimulatedEventLoader : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TSimulatedEventLoader(TClipboard* cp)
		: TAlgorithm(cp,"TSimulatedEventLoader",false) {}
	TSimulatedEventLoader(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TSimulatedEventLoader",false,debug) {}
	~TSimulatedEventLoader() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	std::ifstream fFilestream;
	void LoadTimepix(std::ifstream&,TTimepix*);
	// Data members
	std::list<std::string> fInputFilenames;
	std::list<std::string>::iterator fFileIterator;

};

#endif // TSIMULATEDEVENTLOADER_H
