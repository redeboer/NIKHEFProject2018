// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from each detector in each event and places that data on the clipboard for further analysis.
	(For now, this means it only reads data files from timepix detectors.)
*/

#ifndef TTIMEPIXEVENTLOADER_H
#define TTIMEPIXEVENTLOADER_H 1

// === INCLUDES =======
	#include "TString.h"
	#include "TSystemDirectory.h"
	#include <string>
	#include <list>
	#include "TAlgorithm.h"

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

	// Print functions
	void PrintFileNames();

private:
	// Private functions
	void AddFileNames(TString);
	Bool_t ExtractZipFile(TString);
	void AddFileName(TString);
	Bool_t ReadDSC(const char*);
	Bool_t DetermineFileFormat(const char*);
	Bool_t IsMatrixFormat(const char*);
	Bool_t LoadTimepix(const char*);
	// Data members
	Bool_t fMatrixFormat;
	Bool_t fHasDSC;
	TString fCurrentDir;
	std::list<std::string> fInputFilenames;
	std::list<std::string> fZipFolderNames;
	std::list<std::string>::iterator fFileIterator;
	// Read dump
	std::string fDummy;
	UShort_t fNCols; // "width" of timepix
	UShort_t fNRows; // "height" of timepix
	Double_t fMpxClock; // medipix clock frequency [MHz]
	Double_t fAcqTime;  // acquisition time [s]
	Double_t fStartTime; // recorded start time [s?]


};

#endif // TTIMEPIXEVENTLOADER_H