// Author: Remco de Boer
// Date: June 1st, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	The analysis class is a core class that allows to run algorithms over each event. It contains a list of pointers to algorithms, each of which is (1) initialised, (2) run on each event, and (3) finalised. This class does not define what an event is, but merely runs each algorithm sequentially and passes the clipboard between them (erasing it at the end of each Run sequence).
	An example: you can start with (a) some load algorithm, follow up with (b) an algorithm that writes the data loaded to some histograms, and end with (c) some track fitting. The cool thing is that you can now easily cut out the writing step (b) by commenting it in Steering or by replace it with some other algorithm.
	When an algorithm returns NoData (see TAlgorithm), it will simply jump to the next event. When an algorithm returns Finished, the event processing will stop at the end of run.
*/

// === INCLUDES =======
	#include "TAnalysis.h"
	using namespace std;
	using namespace NIKHEFProject;

// === DESTRUCTOR =======
	TAnalysis::~TAnalysis()
	{
		// Delete all algoritms added!
		TAlgoritmIter_t it = fAlgorithms.begin();
		while( it!=fAlgorithms.end() ) {
			delete *it;
			++it;
		}
	}

// === MODIFIERS =======
	// Add an algorithm to the list of algorithms to run. Note that the algorithm is 'owned' now: it will be removed upon destruction of TAnalysis
	void TAnalysis::Add(TAlgorithm* algorithm) {
		fAlgorithms.push_back(algorithm);
	}

// === ALGORITHM FUNCTIONS =======

	// INITIALISE STEP: Initalise all algorithms
	void TAnalysis::InitialiseAll()
	{
		// Start stopwatch for initialise operation
		fLocalStopwatch = new TStopwatch();
		// Start initialise operation
		cout << endl
			<< "=================| Initialising algorithms |=================="
			<< endl;
		pTotalFiles=0;
		TAlgoritmIter_t it = fAlgorithms.begin();
		while( it!=fAlgorithms.end() ) {
			// Make a new folder in the output file if algorithm needs writing
			(*it)->GetName();
			if((*it)->CanWrite()) {
				(*it)->SetDirectory(fClipboard->GetOutputFile()->mkdir((*it)->GetName().c_str()));
			}
			cout<<"["<<(*it)->GetName()<<"] Initialising"<<endl;
			// Initialise the algorithm
			(*it)->Initialise();
			++it;
		}
		// Get timing for initialise operation
		fTimingInitial = fLocalStopwatch->RealTime();
		delete fLocalStopwatch;
	}

	// RUN STEP: Run the analysis loop. This initialises, runs and finalises all algorithms
	void TAnalysis::RunAll()
	{

		// Loop over all algorithms and initialise each of them
		InitialiseAll();
		// Start stopwatch to compute ETA
		fEventStopwatch = new TStopwatch();
		// Loop over all events, running each algorithm on each "event"
		cout << endl
			<< "========================| Event loop |========================"
			<< endl;
		pEventNumber=0;
		fETAcount = 0;
		fETA = 0;
		Bool_t run = true; // set to zero when last algorithm returns "Finished"
		while(run) {
			// Event info: step info is printed within same line (no newlines)
			cout << "\rRunning over event " << pFileNumber << "/" << pTotalFiles << flush;
			if(fETAString!="") cout << " (ETA:" << fETAString << ")    " << flush;
			Bool_t nodata = false;
			// Run all algorithms
			TAlgoritmIter_t it = fAlgorithms.begin();
			while( it!=fAlgorithms.end() ) {
				// Run the algorithms with Timing enabled
				(*it)->GetStopwatch()->Start(false);
				if((*it)->CanWrite()) (*it)->GetDirectory()->cd(); // change directory
				StatusCode check = (*it)->Run();
				(*it)->GetStopwatch()->Stop();
				// If nothing to be done in this event:
				if(check == NoData) {
					nodata = true;
					break;
				}
				// If any algorithm notifies that this is last event:
				if(check == Finished) run = false;
				++it;
			}
			// Clear objects from this iteration from the clipboard
			fClipboard->Clear();
			// Increment counters
			++fETAcount;
			++pFileNumber;
			if(!nodata) ++pEventNumber;
			// Renew ETA
			fETA = fEventStopwatch->RealTime();
			fEventStopwatch->Continue();
			if(fETA>pETARenewTime) {
				fETA /= fETAcount;
				fETA *= pTotalFiles-pFileNumber;
				// fEventStopwatch->Continue();
				fETAString = GetTimeFormat(fETA);
				fEventStopwatch->Start();
				fETAcount = 0;
			}
		}
		cout << "\r--> FINISHED: Analysed "
			<< pEventNumber << " valid events of "
			<< pTotalFiles << " in total" << endl;
		// Loop over all algorithms and Finalise them
		FinaliseAll();
	
	}

	// FINALISE STEP: Finalise all algorithms
	void TAnalysis::FinaliseAll()
	{
		// Start finalise operation
		cout << endl
			<< "=================| Initialising algorithms |=================="
			<< endl;
		// Start stopwatch for finalise operation
		fLocalStopwatch = new TStopwatch();
		// Perform finalisations
		TAlgoritmIter_t it = fAlgorithms.begin();
		while( it!=fAlgorithms.end() ) {
			// Finalise the algorithm
			if((*it)->CanWrite()) (*it)->GetDirectory()->cd(); // change directory
			(*it)->Finalise();
			++it;
		}
		// Close output ROOT file
		fClipboard->GetOutputFile()->Close();
		// Get timing for finalise operation
		fTimingFinal = fLocalStopwatch->RealTime();
		delete fLocalStopwatch;
		// Check the Timing for all events
		Timing();

	}

// === INFORMATION FUNCTIONS =======
	// Display timing statistics for each algorithm, over all events and per event
	void TAnalysis::Timing()
	{
		cout << endl
			<< "===============| Wall-clock Timing (seconds) |================" << endl;
		if(pTotalFiles) {
			TAlgoritmIter_t it = fAlgorithms.begin();
			// Print reading time if more than 1 s
			if(fTimingInitial>.1) {
				cout.width(22);
				cout << "INITIALISATION" << "  --  ";
				PrintTimeFormat(fTimingInitial);
				cout << endl;
			}
			// Print time per algorithm
			Double_t timer, total = fTimingInitial;
			while( it!=fAlgorithms.end() ) {
				timer = (*it)->GetStopwatch()->RealTime();
				total += timer;
				cout.width(22);
				cout << (*it)->GetName() << "  --  ";
				PrintTimeFormat(timer);
				cout << " = " << timer/pTotalFiles << " s/evt" << endl;
				++it;
			}
			// Print writing time if more than 1 s
			total += fTimingFinal;
			if(fTimingFinal>.1) {
				cout.width(22);
				cout << "FINALISATION" << "  --  ";
				PrintTimeFormat(fTimingFinal);
				cout << endl;
			}
			// Print total compilation time
			cout << "--------------------------------------------------------------" << endl;
			cout.width(22);
			cout << "TOTAL" << "  --  ";
			PrintTimeFormat(total);
			cout << " = " << total/pTotalFiles << " s/evt" << endl;
		} else {
			cout << "  NO EVENTS WERE ANALYSED" << endl;
		}
		cout << "==============================================================\n" << endl;
	}