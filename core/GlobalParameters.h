// Author: Remco de Boer
// Date: June 1st, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This file defines a namespace with functions and parameters that can be loaded anywhere so that it is accessible to any object. It is best practice to include this header in the header files of base the classes (TBeamObject.h and TAlgorithm) and in the remaining core classes (TAnalysis and TClipboard).
	Any functions or parameters that you want to be accessible for any other object should be defined here.
*/

#ifndef GLOBALPARS_H
#define GLOBALPARS_H 1

// === INCLUDES =======
	#include <fstream>
	#include <string>
	#include <sstream>
	#include "TString.h"
	#include "TRegexp.h"
	#include "TVector3.h"

namespace NIKHEFProject {

// === PARAMETERS =======
	// Character buffers
	extern const Int_t pBufferSize;
	extern Char_t pBuffer[];
	// Hough transform
	extern Double_t pDeltaR;
	extern Double_t pPStep;
	extern Int_t pPSteps;
	extern Int_t pRSteps;
	// Default timepix settings
	extern UShort_t pNCols;
	extern UShort_t pNRows;
	// Fit parameters
	extern UChar_t pMaxNFits;
	extern UChar_t pMinClusterPixels;
	// IO names
	extern Bool_t pSimulationData; // determined from input file name
	extern TString pInput;
	extern TString pOutput;
	extern const TString pTempZipFolderFull;
	extern const Char_t* pSupportedZipExts[];
	extern const UChar_t pNSupportedZipExts;
	// Structural names
	extern const Char_t* pTPC1id;
	extern const Char_t* pTPC2id;
	extern const Char_t* pTreeName;
	extern const Char_t* pTreeTitle;
	// Draw options
	extern const Char_t* pDrawHistoOption;
	extern const Char_t* pDrawGraphOption;
	extern Int_t pShowStats;
	// Reconstruction parameters
	extern const UShort_t pResolution;
	extern const Double_t pXmin;
	extern const Double_t pYmin;
	extern const Double_t pZmin;
	extern const Double_t pXmax;
	extern const Double_t pYmax;
	extern const Double_t pZmax;
	// Event counters
	extern UInt_t pFileNumber;
	extern UInt_t pEventNumber;
	extern UInt_t pTotalFiles;
	extern Double_t pETARenewTime;
	// Regular expressions
	extern TRegexp pDigit;
	extern TRegexp pNonDigit;

// === DETECTOR PARAMETERS =======
	namespace Detector {
		extern TVector3 pTPC2pos;
		extern Double_t pBeamEnergy;
	}

// === FILE FUNCTIONS =======
	extern void OpenFile(std::ifstream&,const char*,Bool_t debug=false);

// === STRING FUNCTIONS =======
	extern Bool_t SortFileNames(std::string,std::string);
	extern ULong64_t GetTimestamp(TString);
	extern ULong64_t GetTimestamp(std::string);
	extern TString FormatInputString(const char*);
	extern TString FormatOutputString(const char*);
	extern TString GetMotherPath(TString);
	extern TString GetMotherPath(const char*);
	extern TString GetFileName(TString);
	extern TString GetFileName(const char*);
	extern TString GetFullPath(TString);
	extern void RemoveExtension(TString&);

// === INFO FUNCTIONS =======
	extern void ReadCommandLineOptions(Int_t,char**);
	extern void Help();
	extern std::string GetTimeFormat(UInt_t);
	extern void PrintTimeFormat(UInt_t);
	extern Bool_t IsZipFile(TString&);

// === MATHEMATICAL FUNCTIONS =======
	extern Double_t HoughTransform(Double_t,Double_t,Double_t);
	extern Int_t PowerOfTwo(Int_t);
}

#endif // NIKHEFPROJECTGLOBALPARS_H