// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	The TClipboard class is used to transfer information between algorithms during the event processing.
	Any object inheriting from TBeamObject can be placed on the clipboard and retrieved by its name. At the end of each event (each algorithm run), the clipboard is wiped clean. It is therefore best practice to add any dynamically created object immediately to the clipboard using Put.
*/

#ifndef TCLIPBOARD_H 
#define TCLIPBOARD_H 1

// === INCLUDES =======
	#include "TFile.h"
	#include "TSystem.h"
	#include "TString.h"
	#include <map>
	#include <list>
	#include <string>
	#include <iostream>
	// Beam objects
	#include "GlobalParameters.h"
	#include "TBeamObject.h"
	#include "TCalo.h"
	#include "TPixel.h"
	#include "TRecPoint.h"
	#include "TTimepix.h"
	#include "TTrack.h"

// === TYPE DEFINITIONS =======
typedef std::map<std::string,TBeamObjectList_t> TBeamObjectMap_t;
typedef std::map<std::string,TBeamObjectList_t>::iterator TBeamObjectMapIter_t;


// === CLASS DECLARATION =======
class TClipboard {
  
public:
	
	// Constructors and destructors
		TClipboard() {
			gSystem->mkdir(NIKHEFProject::GetMotherPath(NIKHEFProject::pOutput));
			fFile = new TFile(NIKHEFProject::pOutput.Data(),"RECREATE");
			std::cout << "Output will be written to \"" << NIKHEFProject::pOutput.Data() << "\"" << std::endl; }
		virtual ~TClipboard() { fFile->Close(); };
	// Modifiers
		void Put(std::string, TBeamObjectList_t*);
		void Put(std::string, TBeamObject*);
		void Clear();
	// Specific put functions (MODIFY IF objs CLASSES CHANGE)
		void Put(TCalo*);
		void Put(TPixel*);
		void Put(TRecPoint*);
		void Put(TTrack*);
		void Put(TTimepix*);
	// Getters
		TBeamObjectList_t* Get(const char*);
		TBeamObjectList_t* Get(TString&);
		TBeamObjectList_t* Get(std::string&);
		TFile* GetOutputFile();
	// Information
		void CheckCollections();

private:
	// Output ROOT file (temporary?)
	TFile* fFile;
	// Map of (a list of) beam objects
	TBeamObjectMap_t fObjectsMap;

};

#endif // TCLIPBOARD_H
