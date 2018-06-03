// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes all timepixes on the clipboard to the current (ROOT file) directory in the form of a 2D graph.
*/

// === INCLUDES =======
	#include "TWriteTimepixGraph.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TWriteTimepixGraph::Initialise() {}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TWriteTimepixGraph::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Loop over timepixes
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		while( fTimepixIter!=fTimepixList->end() ) {
			// Write timepix
			MakeGraph(*fTimepixIter);
			fGraph->Write();
			delete fGraph; // not written to clipboard so deleted asap
			// Write histograms of all clusters on clipboard
			fClusterList = (TTimepixList_t*)fClipboard->Get((*fTimepixIter)->GetName()+"clusters");
			fClusterIter = fClusterList->begin();
			while( fClusterIter!=fClusterList->end() ) {
				MakeGraph(*fClusterIter);
				fGraph->Write();
				delete fGraph; // not written to clipboard so deleted asap
				++fClusterIter;
			}
			// on succesfull, set nodata bit
			if(nodata) nodata = false;
			++fTimepixIter;
		}

		// NODATA: if no timepix in clipboard
		if(nodata) {
			if(fDebug) cout << "No timepix in clipboard" << endl;
			return NoData;
		}

		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise here
	void TWriteTimepixGraph::Finalise(){}

// === PRIVATE FUNCTIONS =======
	void TWriteTimepixGraph::MakeGraph(TTimepix* timepix) {
		// Generate appropriate name and title
		TString name(timepix->GetName());
		// Create graph of with correct number of points
		if(timepix->GetPixels()->size()>0)
			fGraph = new TGraph2D( timepix->GetPixels()->size() );
		else fGraph = new TGraph2D();
		fGraph->SetNameTitle(name.Data(),name.Data());
		// Set points by looping over pixels in timepix
		Int_t i=0;
		TPixelIter_t it = timepix->GetPixels()->begin();
		while( it!=timepix->GetPixels()->end() ) {
			fGraph->SetPoint( i, (*it)->GetColumn(), (*it)->GetRow(), (*it)->GetADC() );
			++it;
			++i;
		} }