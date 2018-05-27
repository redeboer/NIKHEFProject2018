// Author: Remco de Boer
// Date: May 23rd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This file defines a namespace with functions and parameters that can be loaded anywhere so that it is accessible to any object. It is best practice to include this header in the header files of base the classes (TBeamObject.h and TAlgorithm) and in the remaining core classes (TAnalysis and TClipboard).
*/

// === INCLUDES =======
	#include <iostream>
	#include <stdio.h>
	#include "GlobalParameters.h"
	#include "TMath.h"
	using namespace std;
	using namespace TMath;

namespace NIKHEFProject {
// === PARAMETERS =======
	// Character buffers
	const Int_t pBufferSize = 1024;
	Char_t pBuffer[pBufferSize];
	// Simulation or measurements?
	Bool_t pSimulationData = false;
	// Timepix file format
	Bool_t pMatrixFormat = true;
	UShort_t pNCols = 256;
	UShort_t pNRows = 512;
	// Hough transform
	Int_t pRSteps = 200;   // resolution of Hough 2D r values
	Double_t pDeltaR = 30; // cutout size for Hough transform
	Double_t pPStep = 2;   // angle phi stepsize for Hough transform
	Double_t pRmax = Sqrt(pNCols*pNCols+pNRows*pNRows); // do not change!
	Double_t pRStep = 2*pRmax/pRSteps; // do not change!
	Int_t pPSteps = 180/pPStep;        // do not change!
	// Fit parameters
	UChar_t pMaxNFits = 3; // maximum number of identified tracks
	UChar_t pMinClusterPixels = 5; // minimal number of points that a cluster (linear track) may have
	// IO names
	string pInputDirectory = "data";
	string pInputFileName = "data/Fake_data_protons_1.txt";
	string pOutputFileName = "output.root";
	const char* pTreeName = "fitparameters";
	const char* pTreeTitle = "Linear fit parameters of dominant tracks";
	// Draw options
	const char* pDrawHistoOption = "colz"; // draw option for historgrams
	const char* pDrawGraphOption = "pcol"; // draw option for graphs
	Int_t pShowStats = 1<<9; // means: display no statistics pad
	// Event counters
	UInt_t pFileNumber = 0;
	UInt_t pEventNumber = 0;
	UInt_t pTotalFiles = 0;
	Double_t pETARenewTime = 1.;

// === DETECTOR PARAMETERS =======
	namespace Detector {
		TVector3 pTPC2pos = TVector3(1000.,0.,0.);
		Double_t pBeamEnergy = 150; // GeV
	}


// === FUNCTIONS =======
	Double_t HoughTransform( Double_t x, Double_t y, Double_t phi ) {
		// r = x*cos(phi) + y*sin(phi)
		return
			x*Cos(DegToRad()*phi) +
			y*Sin(DegToRad()*phi); }
	void Help() {
		cout << "********************************************************************" << endl;
		cout << "Typical executions are:" << endl;
		cout << " => ./execute -d directory" << endl;
		cout << endl; }
	void ReadCommandLineOptions(Int_t argc, char *argv[]) {
		// If there are no input parameters then display the help function
		if(argc == 1) {
			Help();
			return;
		}
		// Output
		cout<<endl;
		cout<<"===================| Reading parameters  |===================="<<endl<<endl;
		Int_t option;
		while ( (option=getopt(argc,argv,"esd:i:o:")) != -1) switch (option) {
			case 's':
				pSimulationData = true;
				cout<<"Analysing simulation data from text file"<<endl;
				break;
			case 'e':
				sscanf( optarg, "%f", &Detector::pBeamEnergy);
				cout<<"Beam energy set to: "<<Detector::pBeamEnergy<<endl;
				break;
			case 'd':
				sscanf( optarg, "%s", &pBuffer);
				pInputDirectory = (string)pBuffer;
				cout<<"Taking data from: "<<pInputDirectory<<endl;
				break;
			case 'i':
				sscanf( optarg, "%s", &pBuffer);
				pInputFileName = (string)pBuffer;
				cout<<"Reading simulation from: "<<pInputFileName<<endl;
				break;
			case 'o':
				sscanf( optarg, "%s", &pBuffer);
				pOutputFileName = (string)pBuffer;
				cout<<"Output written to: "<<pOutputFileName<<endl;
				break;
		} }
	// This function gets a timestamp from a string (like a file name). Edit this function such that it gets a correct (unique!) timestamp from your data files.
	ULong64_t GetTimestamp(TString name) {
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
		return timestamp; }
	ULong64_t GetTimestamp(string str) {
		TString name(str);
		GetTimestamp(name); }
	Bool_t SortString(string n1, string n2) {
		return (GetTimestamp(n1)>GetTimestamp(n2)); }
	string GetTimeFormat(UInt_t t) {
		if(t>3600) sprintf(pBuffer,"%uh%2um%2us",t/3600,t/60%3600,t%60);
		else if(t>60) sprintf(pBuffer,"%2um%2us",t/60%3600,t%60);
		else sprintf(pBuffer,"%2us",t);
		return (string)pBuffer; }
	void PrintTimeFormat(UInt_t t) {
		cout << GetTimeFormat(t); }
	// Mathematical functions
	Int_t PowerOfTwo(Int_t x) { return Power( 2, Ceil(Log2(x)) ); }
} // end of namespace GlobalPars