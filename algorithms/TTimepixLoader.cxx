// Author: Remco de Boer
// Latest update: Lizette Lamers
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from each detector in each event and places that data on the clipboard for further analysis.
	(For now, this means it only reads data files from timepix detectors.)
*/

// === INCLUDES =======
	#include "TTimepixLoader.h"
	#include "TSystem.h"
	#include <fstream>
	#include <stdio.h>
	#include <string.h>
	#include <iostream>
	#include <sstream>
	#include "TPixel.h"
	#include "TTimepix.h"
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: generate a list of filenames in a directory (fInputFilenames), including compressed directories. The files will be read during Run.
	void TTimepixLoader::Initialise()
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
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TTimepixLoader::Run()
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
		// Attempt to create timepix from txt file
		if(!LoadTimepix(filename.c_str())) return NoData;
		// SUCCESS if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: remove all extracted zip files
	void TTimepixLoader::Finalise() {
		fFileIterator = fZipFolderNames.begin();
		while( fFileIterator!=fZipFolderNames.end() ) {
			cout << "Removing extracted files \"" << GetFileName(*fFileIterator) << "\"" << endl;
			gSystem->Exec(("rm -rf \""+*fFileIterator+"\"").c_str());
			++fFileIterator;
		}
	}

// === PRINT FUNCTIONS =======
	void TTimepixLoader::PrintFileNames() {
		fFileIterator = fInputFilenames.begin();
		while( fFileIterator!=fInputFilenames.end() ) {
			cout << GetFileName(*fFileIterator) << endl;
			++fFileIterator;
		}
		cout << fInputFilenames.size() << " FILES IN TOTAL" << endl;
	}
// === PRIVATE FUNCTIONS =======
	// Function that attempts to reads information from a .dsc file generated by the timepix
	Bool_t TTimepixLoader::ReadDSC(const char* filename)
	{
		// Open file stream of dsc file
		TString dscname(filename);
		RemoveExtension(dscname);
		dscname += ".dsc";
		ifstream filestream;
		OpenFile(filestream,dscname.Data(),fDebug);
		if(!filestream.is_open()) return false;
		// Read line by line
		while(filestream.getline(pBuffer,pBufferSize)) {
			TString tData(pBuffer);
			// Get width+height
			if( tData.Contains(" width=") && tData.Contains(" height=") ) {
				TString line(pBuffer);
				stringstream str1( line.Remove(0,line.Index(" width=")+7).Data() );
				str1 >> fNCols;
				stringstream str2( line.Remove(0,line.Index(" height=")+8).Data() );
				str2 >> fNRows;
				if(fDebug) cout << "  --> dimensions: " << fNCols << "x" << fNRows << endl;
			}
			// Get clock frequency [MHz]
			else if(tData.Contains("\"Mpx clock\"")) {
				filestream.getline(pBuffer,pBufferSize); // skip line "double[1]"
				filestream.getline(pBuffer,pBufferSize); // get data line
				istringstream sstream(pBuffer);
				sstream >> fMpxClock;
				if(fDebug) cout << "  --> medipix clock [MHz]: " << fMpxClock << endl;
			}
			// Get "acquisition time [s]"
			else if(tData.Contains("\"Acq time\"")) {
				filestream.getline(pBuffer,pBufferSize); // skip line "double[1]"
				filestream.getline(pBuffer,pBufferSize); // get data line
				istringstream sstream(pBuffer);
				sstream >> fAcqTime;
				if(fDebug) cout << "  --> acquisition time [s]: " << fAcqTime << endl;
			}
			// Get "acquisition start time"
			else if(tData.Contains("\"Start time\"")) {
				filestream.getline(pBuffer,pBufferSize); // skip line "double[1]"
				filestream.getline(pBuffer,pBufferSize); // get data line
				istringstream sstream(pBuffer);
				sstream >> fStartTime;
				if(fDebug) cout << "    start time: " << fStartTime << endl;
			}
		}
		filestream.close();
		return true;
	}
	// Function that determines timepix size and file format of txt file
	Bool_t TTimepixLoader::DetermineFileFormat(const char* filename)
	{
		if(fDebug) cout << "  Determining format of \"" << filename <<  "\"" << endl;
		// Generate file stream
		ifstream filestream;
		OpenFile(filestream,filename);
		// Some counters and read dumps
		UInt_t nlines = 0;
		UInt_t nvals  = 0;
		Int_t val;
		fNCols=pNCols, fNRows=pNRows;
		// fNCols=1, fNRows=1; // chose this if you want to reset find maximum at each run
		// Read lines
		while( filestream.getline(pBuffer,pBufferSize) ) {
			istringstream sstream(pBuffer); // create line stream from filestream
			// get 1st two values in line and store if maximum
			sstream >> val; if( val>fNCols ) fNCols = val;
			sstream >> val; if( val>fNRows ) fNRows = val;
			nvals+=2; // count number of values read
			while(sstream >> val) ++nvals; // count remaining number of values
			++nlines; // count number of lines
		}
		// Abort if empty
		if(nvals==0) {
			if(fDebug) cout << "  --> file is empty" << endl;
			return false;
		}
		// Verify if text file is a rectangular block of values
		if( nvals%nlines!=0 ) {
			if(fDebug) cout << "  --> not a rectangular block of values." << endl;
			return false;
		}
		// Set file and timepix dimensions and matrix format bit
		nvals /= nlines;
		if(nvals==3) { // maximum value if 3xN format (rounded to next pwer of 2)
			fNCols = PowerOfTwo(fNCols);
			fNRows = PowerOfTwo(fNRows);
			fMatrixFormat = false;
		} else { // just the matrix size if in matrix format
			fNCols = nvals;
			fNRows = nlines;
			fMatrixFormat = true;
		}
		// Set global values
		pNCols=fNCols, pNRows=fNRows;
		// Close file stream and return
		filestream.close();
		if(fDebug) {
			cout << "  --> file dimensions:    " << nvals << "x" << nlines << endl;
			cout << "  --> timepix dimensions: " << fNCols << "x" << fNRows << endl;
		}
		return true;
	}
	// Function that determines timepix size and file format of txt file
	Bool_t TTimepixLoader::IsMatrixFormat(const char* filename)
	{
		// Get first line and create string stream
		ifstream filestream;
		OpenFile(filestream,filename);
		filestream.getline(pBuffer,pBufferSize);
		istringstream sstream(pBuffer);
		// Read first three values
		UShort_t val;
		sstream >> val >> val >> val;
		// Is matrix format if 4th value exist
		if(sstream >> val) fMatrixFormat = true;
		else               fMatrixFormat = false;
		// Close file stream and return
		filestream.close();
		if(fDebug) {
			if(fMatrixFormat) cout << "  --> matrix format" << endl;
			else              cout << "  --> 3xN format" << endl;
		}
		return fMatrixFormat;
	}
	// Function that loads a timepix with hit pixels as read from a txt data file. If the input filename contains the TPC1 identifier (pTPC1id), at attempt is made to load the corresponding TPC2 data as well (identified by pTPC2id).
	Bool_t TTimepixLoader::LoadTimepix(const char* filename)
	{
		// Attempt to open dsc file
		// if dsc file exists, determine format from first line only
		if( ReadDSC(filename) ) IsMatrixFormat(filename);
		// if no dsc file exists, determine format with full method
		else if(!DetermineFileFormat(filename)) return false;
		// Initiate Timepix data
		TString timepixname = GetFileName(filename);
		RemoveExtension(timepixname);
		TTimepix* timepix = new TTimepix(
			timepixname.Data(), GetTimestamp(timepixname),
			fNCols, fNRows, fMpxClock, fAcqTime, fStartTime );
		// Open file stream
		ifstream filestream;
		OpenFile(filestream,filename);
		// Read lines
		UShort_t row, col, adc;
		if(fMatrixFormat) { // if in matrix format
			for( row=0; row<timepix->GetNRows(); row++ ) {
				for( col=0; col<timepix->GetNColumns(); col++ ) {
					filestream >> adc;
					if(adc) {
						TPixel* pixel = new TPixel(col,row,adc);
						timepix->AddPixel(pixel);
						fClipboard->Put(pixel);
					}
				}
			}
		} else { // if in 3xN format
			while(filestream.getline(pBuffer,pBufferSize)) {
				istringstream sstream(pBuffer);
				sstream >> row >> col >> adc;
				if(adc) {
					TPixel* pixel = new TPixel(col,row,adc);
					timepix->AddPixel(pixel);
					fClipboard->Put(pixel);
				}
			}
		}
		// Put the timpix on the clipboard and close file stream
		fClipboard->Put(timepix);
		filestream.close();
		// Warning message
		if( !timepix->GetNHits() ) if(fDebug) cout << "  \"" << timepixname << "\" has no hits" << endl;
		// See if file defines TPC1. If so, attempt to load corresponding TPC2
		timepixname = filename;
		if(timepixname.Contains(pTPC1id)) {
			timepixname.ReplaceAll(pTPC1id,pTPC2id);
			LoadTimepix(timepixname.Data());
		}
		return true;
	}
	// Function that adds a filename to fInputFilenames if extension is txt. If the file name contains the identifier for TPC 2, it is skipped, so that it can be simulateaously loaded in one event Run when its corresponding TPC 1 file is loaded.
	void TTimepixLoader::AddFileName(TString name)
	{
		if( name.EndsWith(".txt") ) {
			fInputFilenames.push_back(name.Data());
			if(fDebug) cout << "  Added file: \"" << name << "\"" << endl;
		}
	}
	// Function that recursively adds all txt files in a directory or extracts them if they are compressed files
	void TTimepixLoader::AddFileNames(TString input)
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
				if(entry->IsDirectory()) AddFileNames(entry->GetName());
				else if(!ExtractZipFile(entry->GetName())) AddFileName(fCurrentDir+entry->GetName());
			}
			gSystem->cd("..");
			fCurrentDir.Chop(); // remove last '/'
			fCurrentDir.Resize(fCurrentDir.Last('/')+1);
		} else // if file
			if(!ExtractZipFile(input)) AddFileName(fCurrentDir+input);
	}
	// If input is a zipfile, extract it, store its output folder name, and apply AddFileNames
	Bool_t TTimepixLoader::ExtractZipFile(TString input)
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