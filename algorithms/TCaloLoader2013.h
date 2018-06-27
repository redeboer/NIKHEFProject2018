// STILL UNDER DEVELOPMENT

// Author: Remco de Boer
// Date: June 18th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm is a version of the TCaloLoader2013 for KVI data from 2013. This data was stored in binary (.bin) files and converted to one-per-event-txt files. These txt files (probably) contain data points along a longitudinal shower profile that needs to be integrated to get the accumulated energy in the calorimeter in the event that the txt file describes. These values have to be integrated to obtain the cumulative energy detected by the calorimeter in an event. In this algorithm, this is done by computing the sum of its histogram. There is a seperate algorithm for computing the energy with a fit.
	NOTE: calo files are read from input folder (argument -i), not from specified calo file (argument -c).
*/

#ifndef TCALOLOADER2013_H
#define TCALOLOADER2013_H 1

// === INCLUDES =======
	#include "TString.h"
	#include <list>
	#include <string>
	#include <fstream>
	#include "TAlgorithm.h"
	#include "TCaloEvent.h"

// === CLASS DEFINITION =======
class TCaloLoader2013 : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TCaloLoader2013(TClipboard* cp)
		: TAlgorithm(cp,"TCaloLoader2013",false) {}
	TCaloLoader2013(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TCaloLoader2013",false,debug) {}
	~TCaloLoader2013() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

	// Print functions
	void PrintFileNames();

private:
	// Private functions
	void AddFileName(TString);
	void AddFileNames(TString);
	Bool_t ExtractZipFile(TString);
	Bool_t LoadCaloEvent(const char*);
	// Data members
	TCaloEvent* fCaloEvent;
	// Data members
	std::ifstream fFilestream;
	TString fCurrentDir;
	std::list<std::string> fInputFilenames;
	std::list<std::string> fZipFolderNames;
	std::list<std::string>::iterator fFileIterator;
	UInt_t fEventNumber;
	// Read dump
	Int_t fNCaloPoints; // number of data points (lines) in the calorimeter
	Double_t fValue;    // value of calo data point

};

#endif // TCALOLOADER2013_H