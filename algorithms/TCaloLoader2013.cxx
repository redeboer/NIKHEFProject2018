// STILL UNDER DEVELOPMENT

// Author: Remco de Boer
// Date: June 18th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm is a version of the TCaloLoader2013 for KVI data from 2013. This data was stored in binary (.bin) files and converted to one-per-event-txt files. These txt files (probably) contain data points along a longitudinal shower profile that needs to be integrated to get the accumulated energy in the calorimeter in the event that the txt file describes. These values have to be integrated to obtain the cumulative energy detected by the calorimeter in an event. In this algorithm, this is done by computing the sum of its histogram. There is a seperate algorithm for computing the energy with a fit.
*/

// === INCLUDES =======
	#include "TCaloLoader2013.h"
	#include "TSystem.h"
	#include "TSystemDirectory.h"
	#include <fstream>
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: generate a list of filenames in a directory (fInputFilenames), including compressed directories. The files will be read during Run.
	void TCaloLoader2013::Initialise()
	{
		// Recursively add txt files (including zip files)
		fCurrentDir = "";
		TString pwd = gSystem->pwd();
		AddFileNames(pInput);
		gSystem->cd(pwd);

		// Sort list of file names
		fInputFilenames.sort(SortFileNames);
		if(fDebug) PrintFileNames();

		// Set total number of events
		// (this should be done in any algorithm that is loaded in TAnalysis first)
		fFileIterator = fInputFilenames.begin();
		pTotalFiles = fInputFilenames.size();

		// As a favor, set the timepix format so other algorithms can use it
		UChar_t i, imax=5;
		while( i<imax && fFileIterator!=fInputFilenames.end() ) {
			DetermineFileFormat( (*fFileIterator).c_str() );
			++fFileIterator;
			++i;
		}
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TCaloLoader2013::Run()
	{
		// Open file if end of filename list has not been reached
		string filename;
		if(fFileIterator != fInputFilenames.end()) {
			// Get filestream of txt file
			filename = *fFileIterator;
			++fFileIterator;
			ifstream file;
			if(fDebug) cout << endl << endl;
			OpenFile(file,filename.c_str(),fDebug);
			if( !file.is_open() ) return NoData;
			file.close();
		} else { // return FINISHED if all files have been analysed
			return Finished;
		}
		// Attempt to create caloevent from txt file
		if(!LoadCaloEvent(filename.c_str())) return NoData;
		// SUCCESS if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: remove all extracted zip files
	void TCaloLoader2013::Finalise() {
		fFileIterator = fZipFolderNames.begin();
		while( fFileIterator!=fZipFolderNames.end() ) {
			cout << "Removing extracted files \"" << GetFileName(*fFileIterator) << "\"" << endl;
			gSystem->Exec(("rm -rf \""+*fFileIterator+"\"").c_str());
			++fFileIterator;
		}
	}

// === PRINT FUNCTIONS =======
	void TCaloLoader2013::PrintFileNames() {
		fFileIterator = fInputFilenames.begin();
		cout << "LIST OF FILE NAMES IN TIMEPIX LOADER:" << endl;
		while( fFileIterator!=fInputFilenames.end() ) {
			cout << "  " << GetFileName(*fFileIterator) << endl;
			++fFileIterator;
		}
		cout << fInputFilenames.size() << " FILES IN TOTAL" << endl;
	}

// === PRIVATE FUNCTIONS =======
	// Function that adds a filename to fInputFilenames if extension is txt. If the file name contains the identifier for TPC 2, it is skipped, so that it can be simulateaously loaded in one event Run when its corresponding TPC 1 file is loaded.
	void TCaloLoader2013::AddFileName(TString name)
	{
		if( name.EndsWith(".txt") ) {
			fInputFilenames.push_back(name.Data());
			if(fDebug) cout << "  Added file: \"" << name << "\"" << endl;
		}
	}
	// Function that recursively adds all txt files in a directory or extracts them if they are compressed files
	void TCaloLoader2013::AddFileNames(TString input)
	{
		// Abort if hidden file (starts with '.')
		if(input.Length()>1&&input(0)=='.') return;
		//abort if no other folders but only files
		if(input == '.') return; 
		// Get pwd and replace with relative path as entered by pInput
		if(fDebug) cout << "Applying AddFileNames to \"" << fCurrentDir+input << "\"" << endl;
		// Either continue recursively on directory or add/extract files
		if(gSystem->cd(input)) { // if directory
			fCurrentDir+=input;
			fCurrentDir+="/";
			TSystemDirectory dir(".",".");
			TList* files = dir.GetListOfFiles();
			TIter next(files);
			next(); // skip folder "."
			next(); // skip folder ".."
			TString name = dir.GetName();
			TSystemFile* entry = NULL;
			while( entry=(TSystemFile*)next() ) {
				// If directory: apply this same function (recursive behaviour)
				if(entry->IsDirectory()) AddFileNames(entry->GetName());
				// If file: extract if zip file and apply this same function (recursive behaviour), otherwise add file name to list
				else if(!ExtractZipFile(entry->GetName())) AddFileName(fCurrentDir+entry->GetName());
			}
			gSystem->cd("..");
			fCurrentDir.Chop(); // remove last '/'
			fCurrentDir.Resize(fCurrentDir.Last('/')+1);
		} else // if file
			if(!ExtractZipFile(input)) AddFileName(fCurrentDir+input);
	}
	// If input is a zipfile, extract it, store its output folder name, and apply AddFileNames
	Bool_t TCaloLoader2013::ExtractZipFile(TString input)
	{
		if(IsZipFile(input)) {
			cout << "Extracting \"" << input << "\"" << endl;
			TString outputfolder(input);
			RemoveExtension(outputfolder);
			gSystem->mkdir(outputfolder);
			gSystem->Exec("aunpack \""+input+"\" -qfX\""+outputfolder+"\"");
			fZipFolderNames.push_back((string)(TString(gSystem->pwd())+"/"+outputfolder));
			AddFileNames(outputfolder);
			return true;
		}
		return false;
	}
	// Function that reads a calo event from a text file
	Bool_t TCaloLoader2013::LoadCaloEvent(const char* filename)
	{
		TString buffer;
		// Check if file stream is still ok
		// if(!fFileStream.good()) {
		// 	if(fDebug) cout << "Problem reading \"" << pCaloFileName << "\"" << endl;
		// 	return false;
		// }
		// Read record length
		fFileStream.getline(pBuffer,pBufferSize,':'); // check words "Record Length"
		if(strcmp(pBuffer,"Record Length")) {
			if(fDebug) cout << endl << "Record Length line incorrect format: " << pBuffer << endl;
			return false;
		}
		fFileStream.get(); // read ':'
		fFileStream >> fNCaloPoints;
		fFileStream.getline(pBuffer,pBufferSize); // finish line
		// Check line "BoardID"
		fFileStream.getline(pBuffer,pBufferSize);
		buffer = pBuffer; // added because of some sort of unicode
		if( !buffer.Contains("BoardID: ") ) {
			if(fDebug) cout << endl << "Board ID line incorrect format: " << pBuffer << endl;
			return false;
		}
		// Check line "Channel"
		fFileStream.getline(pBuffer,pBufferSize);
		buffer = pBuffer; // added because of some sort of unicode
		if(!buffer.Contains("Channel: ")) {
			if(fDebug) cout << endl << "Channel line incorrect format: " << pBuffer << endl;
			return false;
		}
		// Read event number
		fFileStream.getline(pBuffer,pBufferSize,':'); // check words "Event Number"
		buffer = pBuffer; // added because of some sort of unicode
		if(!buffer.Contains("Event Number")) {
			if(fDebug) cout << endl << "Event Number line incorrect format: " << pBuffer << endl;
			return false;
		}
		fFileStream.get(); // read ':'
		fFileStream >> fEventNumber;
		fFileStream.getline(pBuffer,pBufferSize); // finish line
		// Check line "Pattern"
		fFileStream.getline(pBuffer,pBufferSize);
		buffer = pBuffer; // added because of some sort of unicode
		if(!buffer.Contains("Pattern: 0x0000")) {
			if(fDebug) cout << endl << "Pattern line incorrect format: " << pBuffer << endl;
			return false;
		}
		// Read timestamp and add to fCalorimeter
		fFileStream.getline(pBuffer,pBufferSize,':'); // check words "Trigger Time Stamp"
		buffer = pBuffer; // added because of some sort of unicode
		if(!buffer.Contains("Trigger Time Stamp")) {
			if(fDebug) cout << endl << "Timestamp line incorrect format: " << pBuffer << endl;
			return false;
		}
		fFileStream.get(); // read ':'
		fFileStream >> fTimestamp;
		fFileStream.getline(pBuffer,pBufferSize); // finish line
		// Check line "DC offset (DAC)"
		fFileStream.getline(pBuffer,pBufferSize);
		buffer = pBuffer; // added because of some sort of unicode
		if(!buffer.Contains("DC offset (DAC): 0x1999")) {
			if(fDebug) cout << endl << "DC offset line incorrect format: " << pBuffer << endl;
			return false;
		}
		// Create TCaloEvent
		fCaloEvent = new TCaloEvent(fEventNumber,fTimestamp,fNCaloPoints);
		fClipboard->Put(fCaloEvent);
		// Read all values of calo event (usually 1024)
		for(Int_t i=0; i<fNCaloPoints; ++i) {
			// read value
			fFileStream >> fValue;
			// flip value if not first value
			if(i) {
				fValue -= fCaloEvent->GetValue(0);
				fValue *= -1;
			}
			// store value
			fCaloEvent->SetValue(i,fValue); // puts value fFileStream list
		}
		fCaloEvent->SetValue(0,0.); // was skipped in previous loop
		fFileStream.getline(pBuffer,pBufferSize); // finish line
		// Set energy
		fCaloEvent->SetEnergy(fCaloEvent->GetHistogram()->Integral(1,fNCaloPoints+1));
		return true;
	}