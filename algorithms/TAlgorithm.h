// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	The algorithm class is the base class that all user algorithms are built on. It allows the analysis class to hold algorithms of different types without knowing what they are and forces the user algorithms to have member functions Initialise, Run and Finalise. It also holds some basic tools, like the stopwatch for timing measurements.
	On construction, any derived algorithm should be given a pointer to the clipboard and a name.
	Note it is best to store any dynamically created object immediately to the clipboard using Put. At the end of each run, the clipboard is wiped, so there is no need to delete this memory in the algorithm.
*/

#ifndef TALGORITHM_H 
#define TALGORITHM_H 1

// === INCLUDES =======
	#include "GlobalParameters.h"
	#include "TDirectory.h"
	#include "TStopwatch.h"
	#include <string>
	#include "TClipboard.h"

// === INFORMATION OBJECT =======
	enum StatusCode {
		Success,  // returned when algorithm run has been run completely
		NoData,   // return if algorithm run cannot find a data file
		Finished, // returned if algorithm has reached end of data files
	};

// === CLASS DEFINITION =======
class TAlgorithm {

public:

	// Constructors and destructors
	TAlgorithm(TClipboard* cp, const char* name, Bool_t write=true, Bool_t debug=false)
		: fClipboard(cp), fName(name),
		fCanWrite(write), fDebug(debug), fDirectory(NULL)
	{
		fStopwatch = new TStopwatch();
	}
	virtual ~TAlgorithm() {}

	// Three main functions that are called for every algorithm:
	// (1) Initialise, (2) Run, and (3) Finalise 
	virtual void Initialise() {}
	virtual StatusCode Run() {}
	virtual void Finalise() {}

	// Setters
	void SetDirectory(TDirectory* dir) { fDirectory = dir; }

	// Getters
	TDirectory* GetDirectory() const { return fDirectory; }
	std::string GetName() { return fName; }
	TStopwatch* GetStopwatch() { return fStopwatch; }
	void PrintStopwatch() { fStopwatch->Print(); fStopwatch->Continue(); }
	Bool_t CanWrite() { return fCanWrite; }
	
protected:
	TClipboard* fClipboard; // clipboard is created at the beginning of Steering. This is just a pointer to it.
	TStopwatch* fStopwatch;
	TDirectory* fDirectory;
	std::string fName;
	Bool_t fCanWrite; // causes TAnalyse to create a directory in the output ROOT file
	Bool_t fDebug;
	
};

typedef std::list<TAlgorithm*> TAlgorithmList_t;
typedef std::list<TAlgorithm*>::iterator TAlgoritmIter_t;


#endif // TALGORITHM_H
