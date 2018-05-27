// Author: Remco de Boer
// Date: May 23rd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This file defines a namespace with functions and parameters that can be loaded anywhere so that it is accessible to any object. It is best practice to include this header in the header files of base the classes (TBeamObject.h and TAlgorithm) and in the remaining core classes (TAnalysis and TClipboard).
*/

#ifndef GLOBALPARS_H
#define GLOBALPARS_H 1

// === INCLUDES =======
	#include <string>
	#include <sstream>
	#include "TString.h"
	#include "TVector3.h"

namespace NIKHEFProject {

// === PARAMETERS =======
	// Character buffers
	extern const Int_t pBufferSize;
	extern Char_t pBuffer[];
	// Simulation data or measurements?
	extern Bool_t pSimulationData;
	// Timepix file format
	extern Bool_t pMatrixFormat;
	extern UShort_t pNCols;
	extern UShort_t pNRows;
	// Hough transform
	extern Double_t pRmax;
	extern Double_t pDeltaR;
	extern Double_t pPStep;
	extern Double_t pRStep;
	extern Int_t pPSteps;
	extern Int_t pRSteps;
	// Fit parameters
	extern UChar_t pMaxNFits;
	extern UChar_t pMinClusterPixels;
	// IO names
	extern std::string pInputDirectory;
	extern std::string pInputFileName;
	extern std::string pOutputFileName;
	extern const char* pTreeName;
	extern const char* pTreeTitle;
	// Draw options
	extern const char* pDrawHistoOption;
	extern const char* pDrawGraphOption;
	extern Int_t pShowStats;
	// Event counters
	extern UInt_t pFileNumber;
	extern UInt_t pEventNumber;
	extern UInt_t pTotalFiles;
	extern Double_t pETARenewTime;

// === DETECTOR PARAMETERS =======
	namespace Detector {
		extern TVector3 pTPC2pos;
		extern Double_t pBeamEnergy;
	}


// === FUNCTIONS =======
	extern void ReadCommandLineOptions(Int_t,char**);
	extern void Help();
	extern Double_t HoughTransform(Double_t,Double_t,Double_t);
	extern ULong64_t GetTimestamp(TString);
	extern ULong64_t GetTimestamp(std::string);
	extern Bool_t SortString(std::string,std::string);
	extern std::string GetTimeFormat(UInt_t);
	extern void PrintTimeFormat(UInt_t);
	// Mathematical
	extern Int_t PowerOfTwo(Int_t);
}

#endif // NIKHEFPROJECTGLOBALPARS_H