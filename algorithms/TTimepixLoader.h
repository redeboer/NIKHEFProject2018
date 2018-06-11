// Author: Remco de Boer
// Latest update: Lizette Lamers
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from each detector in each event and places that data on the clipboard for further analysis.
	(For now, this means it only reads data files from timepix detectors.)
*/

#ifndef TTIMEPIXLOADER_H
#define TTIMEPIXLOADER_H 1

// === INCLUDES =======
	#include "TString.h"
	#include "TPixelMask.h"
	#include "TSystemDirectory.h"
	#include <string>
	#include <list>
	#include "TTimepix.h"
	#include "TAlgorithm.h"

// === CLASS DEFINITION =======
class TTimepixLoader : public TAlgorithm {

public:
	// Constructors and destructors
	// no writing operation, so writing bit set to false
	TTimepixLoader(TClipboard* cp)
		: TAlgorithm(cp,"TTimepixLoader",false), fPixelMask(NULL) {}
	TTimepixLoader(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TTimepixLoader",false,debug), fPixelMask(NULL) {}
	~TTimepixLoader() {}

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
	void AddPixel(UShort_t,UShort_t,UShort_t);
	Bool_t LoadTimepix(const char*);
	// Data member pointers
	TTimepix* fTimepix;
	TPixelMask* fPixelMask;
	// Data members
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

#endif // TTIMEPIXLOADER_H