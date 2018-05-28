// Author: Remco de Boer
// Date: May 20th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	The Steering is effectively the executable of this analysis software frame. It can read command line parameters that will be passed to the parameters class.
	Steering initialises the three core objects: one parameters object, one clipboard object, and one analysis object. User algorithms are added to the analysis object so can be applied to each event during the run loop.
	For parameter tweaking, see Global.cxx or parse arguments when executing.
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
	#include "TEventLoader.h"
	#include "TSimulatedEventLoader.h"
	#include "TWriteTimepixHist.h"
	#include "TWriteTimepixGraph.h"
	#include "TRecogniseTracks.h"
	#include "TFitTracks.h"
	#include "TIntersectTracks.h"
	#include "TWriteReconstruction.h"
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
		analysis->Add( new TSimulatedEventLoader(clipboard,false) );
	} else {
		analysis->Add( new TEventLoader(clipboard,false) );
		analysis->Add( new TRecogniseTracks(clipboard,false) );
		// analysis->Add( new TWriteTimepixHist(clipboard,false) );
		analysis->Add( new TWriteTimepixGraph(clipboard,false) );
		analysis->Add( new TFitTracks(clipboard,false) );
	}
	analysis->Add( new TIntersectTracks(clipboard,false) );
	analysis->Add( new TWriteReconstruction(clipboard,true) );
	/* -------------------------------------- */

	// Run the analysis
	analysis->RunAll();

	// Delete pointers
		delete analysis;
		delete clipboard;

	return 0;
}