// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm recognises tracks in all timepixes currently on the clipboard. The makes new timepixes (now called "clusters", but still of TTimepix object type) that contain only those pixels that lie in a straight line. Tracks are recognised using a 2D Hough transform.
	All clusters recognised in the original timepix (maximum number of tracks defined in TParametesr) are stored to the clipboard.
*/

// === INCLUDES =======
	#include "TRecogniseTracks.h"
	#include <iostream>
	#include "TMath.h"
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise in this algorithm
	void TRecogniseTracks::Initialise() {}

	// RUN FUNCTION: in each event, load one frame of data from all devices
	StatusCode TRecogniseTracks::Run()
	{
		// Bit to check if there is no data
		Bool_t nodata = true;

		// Get list of timepixes and loop over it
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		while( fTimepixIter!=fTimepixList->end() ) {
			nodata = !RecogniseTracks(*fTimepixIter) && nodata;
			if(fDebug) cout << endl << "  --> recognised " << (Int_t)fNTracks << " track(s) in timepix \"" << (*fTimepixIter)->GetName() << "\" (" << (*fTimepixIter)->GetNHits() << ")" << endl;
			++fTimepixIter;
		}
		// NODATA: if no timepix in clipboard
		if(nodata) {
			if(fDebug) cout << endl << "  --> no timepix to search for tracks" << endl;
			return NoData;
		}
		// SUCCESS: if entire procedure has been run
		return Success;
	}

	// FINALISE FUNCTION: nothing to finalise here
	void TRecogniseTracks::Finalise() {}

// === PRIVATE FUNCTIONS =======
	UChar_t TRecogniseTracks::RecogniseTracks(TTimepix* timepix)
	{
		// Tracks will be recognised recursively from a 'remainder timepix'
		fRemainder = timepix->Clone();
		// Recognise tracks recursively until remainder is (almost?) empty
		fNTracks = 0;
		while(
			fNTracks < pMaxNFits  &&
			fRemainder->GetNHits() > pMinClusterPixels  )
		{
			// Compute 2D Hough transform of remainder and compute location of max
			TH2I* hough = MakeHough2D(fRemainder);
			Int_t maxPbin, maxRbin, zdummy;
			hough->GetMaximumBin(maxPbin,maxRbin,zdummy);
			Double_t maxP = hough->GetXaxis()->GetBinCenter(maxPbin);
			Double_t maxR = hough->GetYaxis()->GetBinCenter(maxRbin);
			Double_t tR1 = maxR - pDeltaR;
			Double_t tR2 = maxR + pDeltaR;
			if(fDebug) printf("\n  TRACK %u\n    Hough cutout: phi = %.0f deg, r = %.1f +/- %.1f",
				fNTracks, maxP, maxR, pDeltaR );
			// Make a new remainder and track holder
			fCluster = new TTimepix(fRemainder);
			TTimepix* tRemainder = new TTimepix(fRemainder);
			// Get new name for track
			TString name(fCluster->GetName());
			name.Append("_cluster");
			name.Append(fNTracks+'0');
			fCluster->SetName(name);
			// Add tracks to remainder or to track
			Double_t r;
			TPixelIter_t it = fRemainder->GetPixels()->begin();
			while( it!=fRemainder->GetPixels()->end() ) {
				r = ComputeHoughR( (*it)->GetColumn(), (*it)->GetRow(), maxP );
				if( r>tR1 && r<tR2 ) fCluster->GetPixels()->push_back(*it); // add to track
				else               tRemainder->GetPixels()->push_back(*it); // or to new remainder
				++it;
			}
			if(fDebug) printf("\n    grouped %u/%u pixels as new track",
				tRemainder->GetNHits(), fRemainder->GetNHits() );
			// Set new remainder and delete old one
			delete fRemainder;
			fRemainder = tRemainder;
			// Put cluster to clipboard and add new number of tracks
			if(fCluster->GetNHits()) { // if there is data
				if(fCanWrite) hough->Write();
				fClipboard->Put((timepix->GetName()+"clusters").Data(),fCluster);
				++fNTracks;
			} else { // but not if no data
				delete fCluster;
			}
			// Delete Hough
			delete hough;
		}
		// Return number of recognised tracks when finished while loop
		return fNTracks;
	}
	// Function that computes the R in a hough transform for some point and a phi
	Double_t TRecogniseTracks::ComputeHoughR( Double_t x, Double_t y, Double_t phi )
	{
		// r = x*cos(phi) + y*sin(phi)
		return
			x*TMath::Cos( TMath::DegToRad() * phi ) +
			y*TMath::Sin( TMath::DegToRad() * phi );
	}
	// Function that generates the 2D Hough transform for some timepix (column vs row)
	TH2I* TRecogniseTracks::MakeHough2D(TTimepix* timepix)
	{
		// generate name and histogram for Hough transform
		TString name;
		name.Form("%s_HT%c", timepix->GetName().Data(), fNTracks+'0' );
		TH2I* hough = new TH2I(name,
			name+";#phi (#circ);r = x cos(#phi) + y sin(#phi)", // axis labels
			pPSteps, 0, 180,                                    // bin definitions
			pRSteps, -timepix->GetDiagonal(), timepix->GetDiagonal() ); // bin definitions
		hough->SetOption(pDrawHistoOption);
		// loop over pixels in remainder
		Double_t x, y, r, phi;
		TPixelIter_t it = timepix->GetPixels()->begin();
		while( it!=timepix->GetPixels()->end() ) {
			x = (*it)->GetColumn();
			y = (*it)->GetRow();
			// fill bins of Hough transform for each phi
			for( UShort_t p=0; p<pPSteps; ++p ) {
				phi = p*pPStep;
				r = ComputeHoughR(x,y,phi);
				hough->Fill(phi,r);
			}
			++it;
		}
		return hough;
	}