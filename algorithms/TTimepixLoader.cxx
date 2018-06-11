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

		// As a favor, set the timepix format so other algorithms can use it
		UChar_t i, imax=5;
		while( i<imax && fFileIterator!=fInputFilenames.end() ) {
			DetermineFileFormat( (*fFileIterator).c_str() );
			++fFileIterator;
			++i;
		}

		// Reset file iterator
		fFileIterator = fInputFilenames.begin();

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
		// Load pixel mask
		if(!fPixelMask) fPixelMask = (TPixelMask*)fClipboard->Get("pixelmask");
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
	// Function that adds pixel to the clipboard
	void TTimepixLoader::AddPixel(UShort_t col, UShort_t row, UShort_t adc)
	{
		// Check if pixels falls in pixel mask
		if(fPixelMask) if(fPixelMask->IsMasked(col,row)) return;
		// Create pixel and at to clipboard
		TPixel* pixel = new TPixel(col,row,adc);
		fTimepix->AddPixel(pixel);
		fClipboard->Put(pixel);
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
		fTimepix = new TTimepix(
			timepixname.Data(), GetTimestamp(timepixname),
			fNCols, fNRows, fMpxClock, fAcqTime, fStartTime );
		// Open file stream
		ifstream filestream;
		OpenFile(filestream,filename);
		// Read lines
		UShort_t row, col, adc;
		if(pMatrixFormat) { // if in matrix format
			for( row=0; row<fTimepix->GetNRows(); row++ ) {
				for( col=0; col<fTimepix->GetNColumns(); col++ ) {
					filestream >> adc;
					if(adc) AddPixel(col,row,adc);
				}
			}
		} else { // if in 3xN format
			while(filestream.getline(pBuffer,pBufferSize)) {
				istringstream sstream(pBuffer);
				sstream >> row >> col >> adc;
				if(adc) AddPixel(col,row,adc);
			}
		}
		// Put the timpix on the clipboard and close file stream
		fClipboard->Put(fTimepix);
		filestream.close();
		// Warning message
		if( !fTimepix->GetNHits() ) if(fDebug) cout << "  \"" << timepixname << "\" has no hits" << endl;
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
			if(!name.Contains(pTPC2id)) fInputFilenames.push_back(name.Data());
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