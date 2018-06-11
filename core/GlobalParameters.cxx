// Author: Remco de Boer
// Date: June 6th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This file defines a namespace with functions and parameters that can be loaded anywhere so that it is accessible to any object. It is best practice to include this header in the header files of base the classes (TBeamObject.h and TAlgorithm) and in the remaining core classes (TAnalysis and TClipboard).
	Any functions or parameters that you want to be accessible for any other object should be defined here.
*/

// === INCLUDES =======
	#include <iostream>
	#include <stdio.h>
	#include "GlobalParameters.h"
	#include "TMath.h"
	#include "TSystem.h"
	using namespace std;
	using namespace TMath;

namespace NIKHEFProject {
// === PARAMETERS =======
	// Character buffers
	const Int_t pBufferSize = 1024;
	Char_t pBuffer[pBufferSize];
	// Hough transform
	Int_t pRSteps = 200;   // resolution of Hough 2D r values
	Double_t pDeltaR = 30; // cutout size for Hough transform
	Double_t pPStep = 2;   // angle phi stepsize for Hough transform
	Int_t pPSteps = 180/pPStep; // do not change!
	// Calorimeter information
	const Double_t pEnergyConvFactor = 5.0e-4; // determine this using TCaloAnalyseSpectrum
	TFile* pCaloOutputFile = NULL;
	// Default timepix settings (will be used as minimum)
	UShort_t pNCols = 256;
	UShort_t pNRows = 256;
	// Fit parameters
	UChar_t pMaxNFits = 3; // maximum number of identified tracks
	UChar_t pMinClusterPixels = 5; // minimal number of points that a cluster (linear track)
	const Double_t pTriggerCaloFit = 5; // bin content value that determines starting point of fit range
	// IO names
	// (it is possibly to only provide an input file or directory name)
	Bool_t pSimulationData = false;
	TString pInput(FormatInputString("data"));
	TString pCaloFileName(FormatCaloFileString(pInput.Data())); // requires pInput to be set
	TString pOutputCalo(FormatOutputCaloString(pInput.Data())); // requires pInput to be set
	TString pOutput(FormatOutputString(pInput.Data())); // requires pInput to be set
	TString pMaskFileName(FormatMaskFileString(pInput.Data())); // requires pInput to be set
	const Char_t* pSupportedZipExts[] = {"tar","tar.gz","tgz","zip"};
	const UChar_t pNSupportedZipExts = sizeof(pSupportedZipExts)/sizeof(*pSupportedZipExts);
	// Structural names
	const Char_t* pTPC1id = "cam_1"; // substring in txt filename to identify TPC 1
	const Char_t* pTPC2id = "cam_2"; // substring in txt filename to identify TPC 2
	const Char_t* pTreeName = "fitparameters";
	const Char_t* pTreeTitle = "Linear fit parameters of dominant tracks";
	// Draw options
	const Char_t* pDrawHistoOption = "colz"; // draw option for historgrams
	const Char_t* pDrawGraphOption = "pcol"; // draw option for graphs
	Int_t pShowStats = 1<<9; // means: display no statistics pad
	// Reconstruction parameters
	const UShort_t pResolution = 100;
	const Double_t	
		pXmin = 0, pYmin = 0  , pZmin = 0,
		pXmax =  3000, pYmax = 1000, pZmax = 1000;
	// Event counters
	UInt_t pFileNumber = 0;
	UInt_t pEventNumber = 0;
	UInt_t pTotalFiles = 0;
	Double_t pETARenewTime = 1.;
	// Regular expressions
	TRegexp pDigit("[0-9]");
	TRegexp pNonDigit("^[0-9]");

// === DETECTOR PARAMETERS =======
	namespace Detector
	{
		TVector3 pTPC2pos = TVector3(1000.,0.,0.);
		Double_t pBeamEnergy = 150; // GeV
	}

// === FILE FUNCTIONS =======
	// Function that makes a filestream based on some Open file stream
	void OpenFile(ifstream& file, const char* filename, Bool_t debug) {
		file.open(filename);
		if(debug) {
			if(!file.is_open()) cout << "  File \"" << filename << "\" does not exist" << endl;
			else cout << "  Opening file \"" << filename << "\"" << endl;
		}
	}

// === STRING FUNCTIONS =======
	// Sort function that sorts correctly with regard to numbers in file names
	Bool_t SortFileNames(string n1, string n2)
	{
		// Return simple string sort function if no digits in string
		TString s1 = GetFileName(n1);
		TString s2 = GetFileName(n2);
		if( s1.EqualTo(s2) ) return false;
		if( !s1.Contains(pDigit) || !s1.Contains(pDigit) ) return (s1.CompareTo(s2)<0);
		// Remove all characters from the start of the string that are the same, as long as the last are no digits
		Ssiz_t i = 0;
		while(s1[i]==s2[i]) ++i;
		while(s1[i]<='9'&&s1[i]>='0'&&i>=0) --i;
		++i;
		s1.Remove(0,i);
		s2.Remove(0,i);
		// Extract first group of digits
		s1.Remove(0,s1.Index(pDigit)); istringstream sstr1(s1.Data());
		s2.Remove(0,s2.Index(pDigit)); istringstream sstr2(s2.Data());
		Double_t d1; sstr1 >> d1;
		Double_t d2; sstr2 >> d2;
		// Return compare value
		return (d1<d2);

	}
	// This function gets a timestamp from a string (like a file name). Edit this function such that it gets a correct (unique!) timestamp from your data files.
	ULong64_t GetTimestamp(TString name)
	{
		// SIMPLE PROCEDURE
			// // Remove all non-numerical characters
			// for( Char_t c='9'; c!='0'; c++ ) name.ReplaceAll(c,"");
			// ULong64_t timestamp;
			// stringstream ss(name.Data());
			// ss >> timestamp;
		// PROCEDURE FOR NAMES OF TYPE 4-21:14:39_cam_1_0.txt
			// Remove extension
			name.ReplaceAll(".txt","");
			// name.Remove(name.Last('.'),name.Sizeof());
			// Remove date
			name.Remove(0,name.First('-')+1);
			name.ReplaceAll(":","");
			// Get counter
			TString counter(name);
			counter.Remove(0,counter.Last('_')+1);
			UShort_t i;
			stringstream cs(counter.Data());
			cs >> i;
			// Get time
			ULong64_t timestamp;
			stringstream ss(name.Data());
			ss >> timestamp;
			// Append counter
			timestamp *= 1e4;
			timestamp += i;
		return timestamp;
	}
	ULong64_t GetTimestamp(string str)
	{
		TString name(str);
		GetTimestamp(name);
	}
	// Functions used internally in this namespace to get properly formatted input/output file or directory name
	TString FormatInputString(const char* name)
	{
		TString str(name);
		if(str.EndsWith("/")) str.Chop();
		return str;
	}
	TString FormatOutputString(const char* name)
	{
		// Strip input string of path
		TString str(GetFileName(name));
		str.Prepend("output/");
		if( str.Contains(".txt") || str.Contains(".dat") ) {
			// supported simulation file extensions
			pSimulationData = true;
			str.Remove( str.Last('.'), str.Sizeof() );
		}
		str.Append(".root");
		return str;
	}
	TString FormatOutputCaloString(const char* name)
	{
		// Strip input string of path
		TString str(GetFileName(name));
		str.Prepend("output/");
		if( str.Contains(".txt") || str.Contains(".dat") )
			str.Remove( str.Last('.'), str.Sizeof() );
		str.Append("_calo.root");
		return str;
	}
	// File that adds "_calo.txt" into input file string (so only works for an input DIRECTORY)
	TString FormatCaloFileString(const char* name)
	{
		TString str(name);
		str += "_calo.txt";
		return str;
	}
	// File that adds "_mask.txt" into input file string (so only works for an input DIRECTORY)
	TString FormatMaskFileString(const char* name)
	{
		TString str(name);
		str += "_calo.txt";
		return str;
	}
	// Function that returns all characters before the last '/'
	TString GetMotherPath(TString name)
	{
		return GetMotherPath(name.Data());
	}
	TString GetMotherPath(const char* name)
	{
		TString path(name);
		if(path.Contains('/')) path.Resize(path.Last('/'));
		else path = "./";
		return path;
	}
	// Function that returns all characters after the last '/'
	TString GetFileName(TString name)
	{
		return GetFileName(name.Data());
	}
	TString GetFileName(const char* name)
	{
		TString path(name);
		if(path.Contains('/')) path.Remove(0,path.Last('/')+1);
		return path;
	}
	// Function that returns the entire pwd
	TString GetFullPath(TString input)
	{
		TString pwd(gSystem->pwd());
		gSystem->cd(input);
		TString fullpath(gSystem->pwd());
		gSystem->cd(pwd);
		return fullpath;
	}
	// Function that removes everything after and including the last dot (.)
	void RemoveExtension(TString& input)
	{
		if(input.Contains('.')) input.Resize(input.Last('.'));
	}

// === INFO FUNCTIONS =======
	void ReadCommandLineOptions(Int_t argc, char *argv[])
	{
		// If there are no input parameters then display the help function
		if(argc == 1) {
			Help();
			return;
		}
		// Output
		cout<<endl;
		cout<<"===================| Reading parameters  |===================="<<endl<<endl;
		Int_t option;
		while ( (option=getopt(argc,argv,"esi:o:c:")) != -1) switch (option) {
			case 's':
				pSimulationData = true;
				cout<<"Analysing simulation data from text file"<<endl;
				break;
			case 'e':
				sscanf( optarg, "%f", &Detector::pBeamEnergy);
				cout<<"Beam energy set to: "<<Detector::pBeamEnergy<<endl;
				break;
			case 'i':
				pInput = FormatInputString(optarg);
				if(pSimulationData) cout<<"Reading simulation from: "<<pInput<<endl;
				else cout<<"Reading measurement data from: "<<pInput<<endl;
				pOutput = FormatOutputString(pInput.Data());
				break;
			case 'o':
				pOutput = optarg;
				// cout<<"Output file name set to: "<<pOutput<<endl; // already printed in TClipboard constructor
				break;
			case 'c':
				pCaloFileName = optarg;
				pOutputCalo = FormatOutputCaloString(optarg);
				cout << "Calorimeter data file set to: \"" << pCaloFileName << "\"" << endl;
				break;
			case 'm':
				pMaskFileName = optarg;
				cout << "Mask filename set to: \"" << pMaskFileName << "\"" << endl;
				break;
		}
	}
	void Help()
	{
		cout << "********************************************************************" << endl;
		cout << "Typical executions are:" << endl;
		cout << " => ./execute -i <input directory or file name>" << endl;
		cout << " => ./execute -o <out ROOT file name>" << endl;
		cout << endl;
	}
	string GetTimeFormat(UInt_t t)
	{
		if(t>3600) sprintf(pBuffer,"%uh%02um%02us",t/3600,t%3600/60,t%60);
		else if(t>60) sprintf(pBuffer,"%2um%02us",t/60,t%60);
		else sprintf(pBuffer,"%2us",t);
		return (string)pBuffer;
	}
	void PrintTimeFormat(UInt_t t)
	{
		cout << GetTimeFormat(t);
	}
	// Function that checks if file ends in any of the zip extensions
	Bool_t IsZipFile(TString& filename) {
		for(UChar_t i=0; i<pNSupportedZipExts; i++)
			if(filename.EndsWith(pSupportedZipExts[i])) return true;
		return false;
	}

// === MATHEMATICAL FUNCTIONS =======
	// Functino that returns 2D Hough transform value: r = x*cos(phi) + y*sin(phi)
	Double_t HoughTransform( Double_t x, Double_t y, Double_t phi )
	{
		return
			x*Cos(DegToRad()*phi) +
			y*Sin(DegToRad()*phi);
	}
	// Function that returns the next power of 2 for some number
	Int_t PowerOfTwo(Int_t x) { return Power( 2, Ceil(Log2(x)) ); }
} // end of namespace GlobalPars