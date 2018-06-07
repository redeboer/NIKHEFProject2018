// Author: Lizette Lamers
// Date: June 7th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads calo data from a text file generated with a Geant simulation (text format to be determined by yourself).
	In the current form, Geant generates calo data in a list of energies. When using the simulated calo-eventloader, the normal calo loader can therefore be skipped.
*/

#ifndef TSIMULATEDCALOLOADER_H
#define TSIMULATEDCALOLOADER_H 1

// === INCLUDES =======
	#include <string>
	#include <fstream>
	#include <list>
	#include "TAlgorithm.h"

// === CLASS DEFINITION =======
class TSimulatedCaloLoader : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TSimulatedCaloLoader(TClipboard* cp)
		: TAlgorithm(cp,"TSimulatedCaloLoader",false) {}
	TSimulatedCaloLoader(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TSimulatedCaloLoader",false,debug) {}
	~TSimulatedCaloLoader() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	std::ifstream fFilestream;
	// Data members
	std::list<std::string> fInputFilenames;
	std::list<std::string>::iterator fFileIterator;

};

#endif // TSIMULATEDEVENTLOADER_H