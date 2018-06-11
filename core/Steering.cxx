// Author: Remco de Boer
// Date: June 1st, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	The Steering is effectively the executable of this analysis software frame. It can read command line parameters (through a function in GlobalParameters.h) and pass these on to the parameters class.
	Steering initialises the two core objects: one clipboard object and one analysis object. User algorithms are added to the analysis object so they will be applied to each event during the run loop.
	For parameter tweaking, see GlobalParameters.cxx or parse arguments when executing.
*/

// === INCLUDES =======
	#include "TBrowser.h"
	#include "TStyle.h"
	#include <signal.h>
	#include <iostream>
	#include "GlobalParameters.h"
	#include "TAnalysis.h"
	#include "TClipboard.h"

// === INCLUDED ALGORITHMS =======
	#include "TCaloLoader.h"
	#include "TCaloFitter.h"
	#include "TCaloWriter.h"
	#include "TCaloAnalyseSpectrum.h"
	#include "TTimepixLoader.h"
	#include "TSimulatedCaloLoader.h"
	#include "TSimulatedEventLoader.h"
	#include "TWriteTimepixHist.h"
	#include "TWriteTimepixGraph.h"
	#include "TRecogniseTracks.h"
	#include "TFitTracks.h"
	#include "TIntersectTracks.h"
	#include "TSingleTracks.h"
	#include "TWriteReconstruction.h"
	#include "TWriteSingle.h"
	using namespace std;
	using namespace NIKHEFProject;

/* === USER INTERUPTION HANDLER =======
	Handle user interruption: This allows you to ^\ to exit at any point in a controlled way */
	TAnalysis* analysis;
	void userException(Int_t sig) {
		cout << endl << "User interrupted: process killed" << endl;
		analysis->FinaliseAll();
		exit(1);
	}

// === MAIN FUNCTION =======
Int_t main(Int_t argc, char *argv[]) {

	// Do not draw pave stats
	gStyle->SetOptStat(0);
	
	// Define analysis object, with its parameters
	if(argc>1) ReadCommandLineOptions(argc,argv);
	TClipboard* clipboard = new TClipboard();
	analysis = new TAnalysis(clipboard);

	// Register escape behaviour (^\)
	signal(SIGQUIT, userException);

	/* -------------| ALGORITHMS |----------- */
	if(pSimulationData) {
		// analysis->Add( new TSimulatedEventLoader(clipboard,false) );
		// analysis->Add( new TSimulatedCaloLoader(clipboard,false) );
		// analysis->Add( new TIntersectTracks(clipboard,false) );
		// analysis->Add( new TSingleTracks(clipboard,true) ); // Never run both Tracks and TracksSingle
		// analysis->Add( new TWriteReconstruction(clipboard,false) );
		// analysis->Add( new TWriteSingle(clipboard,false) ); // Never run both single and reconstruction
	} else {
		 analysis->Add( new TCaloLoader(clipboard,false) );
		 analysis->Add( new TCaloFitter(clipboard,false) );
		 analysis->Add( new TCaloWriter(clipboard,false) );
		 analysis->Add( new TCaloAnalyseSpectrum(clipboard,false) );
		//  analysis->Add( new TTimepixLoader(clipboard,true) );
		//  analysis->Add( new TRecogniseTracks(clipboard,false) );
		//  analysis->Add( new TWriteTimepixHist(clipboard,false) );
		//  analysis->Add( new TFitTracks(clipboard,false) );
		//  analysis->Add( new TWriteTimepixGraph(clipboard,false) );
		//  analysis->Add( new TIntersectTracks(clipboard,false) );
		//  analysis->Add( new TWriteReconstruction(clipboard,false) );
	}
	/* -------------------------------------- */

	// Run the analysis
	analysis->RunAll();

	// Delete pointers
		delete analysis;
		delete clipboard;

	return 0;
}