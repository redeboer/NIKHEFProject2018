// Author: Remco de Boer
// Date: May 20th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	An event loader is necessary for any analysis sequence. This event loader loads data from each detecter in each event and places that data on the clipboard for further analysis.
	(For now, this means it only reads data files from two timepix detectors.)
*/

// === INCLUDES =======
	#include "TEventLoader.h"
	#include <stdio.h>
	#include <dirent.h>
	#include <iostream>
	#include <sstream>
	#include "TPixel.h"
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: generate a list of filenames in a directory (fInputFilenames), sorted by the timestamp in these filenames. Note that only files from cam_1 are included, but they will be linked to cam_2 later.
	void TEventLoader::Initialise()
	{

		// Open the directory. This directory contains several txt files that contain the timepix data in either matrix or 3xN format
		DIR* directory = opendir(pInputDirectory.c_str());
		if( directory==NULL ) {
			if(fDebug) printf("Error: Directory \"%s\" does not exist",pInputDirectory.c_str());
			return;
		} else printf("Reading events from directory \"%s\"\n",pInputDirectory.c_str());

		// Read the entries in the folder
		dirent* entry;
		while( entry=readdir(directory) ) {
			// get the name of this entry
			TString filename = entry->d_name;
			// check if it is a .txt file
			if( filename.EndsWith(".txt") && filename.Contains("_cam_1_") ) {
				// push back this filename to the list of files to be included
				fInputFilenames.push_back(filename.Data());
				if(fDebug) cout<<"Added file: "<<filename<<endl;
			}
		}

		// Now, sort the list of filenames
		fFileIterator = fInputFilenames.begin();
		// sort(fFileIterator,fInputFilenames.end(),SortString);

		// Set total number of events
		// (this should be done in any algorithm that is loaded in TAnalysis first)
		pTotalFiles = fInputFilenames.size();
		pTotalFiles = fInputFilenames.size();
	}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TEventLoader::Run()
	{
		// Open file if end of filename list has not been reached
		ifstream file1, file2;
		TString timepixname1, timepixname2;
		if(fFileIterator != fInputFilenames.end()) {
			// Get timepix names (for generating histograms/graphs)
			timepixname1 = *fFileIterator;
			timepixname2 = *fFileIterator;
			TString filename1(pInputDirectory+"/"+*fFileIterator);
			++fFileIterator;
			// Open file for cam 1
			file1.open(filename1.Data());
			if( !file1.is_open() ) {
				if(fDebug) cout << "Warning: File \"" << timepixname1 << "\" not found" << endl;
				return NoData;
			}
			// Open file for cam 2
			TString filename2 = filename1;
			filename2   .ReplaceAll("_cam_1_","_cam_2_");
			timepixname2.ReplaceAll("_cam_1_","_cam_2_");
			file2.open(filename2.Data());
			if( !file2.is_open() ) {
				if(fDebug) cout << "Warning: File \"" << timepixname2 << "\" not found" << endl;
				return NoData;
			}
			// Debug message
			if(fDebug)
				cout << endl << "Opened files" << endl
				<< "  \"" << filename1 << "\"" << endl
				<< "  \"" << filename2 << "\"" << endl;
			// Remove file extensions from timepix name
			timepixname1.Remove(timepixname1.Last('.'),4);
			timepixname2.Remove(timepixname2.Last('.'),4);
		} else { // return FINISHED if all files have been analysed
			return Finished;
		}

		// Create two timepix objects (for cam1 and cam2)
		ULong64_t timestamp = GetTimestamp(timepixname1);
		TTimepix* timepix1 = new TTimepix("cam1",timestamp);
		TTimepix* timepix2 = new TTimepix("cam2",timestamp);

		// Import Timepix data for cam 1
			LoadTimepix(file1,timepix1);
			if( !timepix1->GetNHits() ) {
				delete timepix1;
				if(fDebug) cout << endl << "File \"" << timepixname1 << "\" is empty" << endl;
				return NoData;
			}
			file1.close();

		// Import Timepix data for cam 1
			LoadTimepix(file2,timepix2);
			if( !timepix2->GetNHits() ) {
				delete timepix2;
				if(fDebug) cout << endl << "File \"" << timepixname2 << "\" is empty" << endl;
				return NoData;
			}
			file2.close();

		// Put the timpixes on the clipboard
		fClipboard->Put(timepix1);
		fClipboard->Put(timepix2);

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: only prints number of events analysed
	void TEventLoader::Finalise() {}

// === PRIVATE FUNCTIONS =======
	void TEventLoader::LoadTimepix(ifstream& filestream, TTimepix* timepix) {
		UShort_t row, col, adc;
		if(pMatrixFormat) { // if in matrix format
			for( row=0; row<pNRows; row++ ) {
				for( col=0; col<pNCols; col++ ) {
					filestream >> adc;
					if(adc) {
						TPixel* pixel = new TPixel(col,row,adc);
						timepix->AddPixel(pixel);
						fClipboard->Put(pixel);
					}
				}
			}
		} else { // if in 3xN format
			string data;
			while(getline(filestream,data)) {
				istringstream sstream(data);
				sstream >> row >> col >> adc;
				if(adc) {
					TPixel* pixel = new TPixel(col,row,adc);
					timepix->AddPixel(pixel);
					fClipboard->Put(pixel);
				}
			}
		} }