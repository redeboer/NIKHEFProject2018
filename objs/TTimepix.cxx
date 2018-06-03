// Author: Remco de Boer
// Date: June 1st, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class describes all features of a timepix, like dimensions and clock frequency. Hit information is contained in the list of pixels (fPixels). This object can therefore also serve as a definition for a cluster of pixel data in a timepix (like a linear track).
*/

// === INCLUDE =======
	#include <iostream>
	#include "TTimepix.h"
	#include "TMath.h"
	using namespace std;

// === CONSTRUCTORS =======
	TTimepix* TTimepix::Clone()
	{
		TTimepix* clone = new TTimepix(this);
		// copy pixels
		TPixelIter_t it = fPixels.begin();
		while( it!=fPixels.end() ) {
			clone->AddPixel( *it );
			++it;
		}
		return clone;
	}

// === GETTERS =======
	TString& TTimepix::GetName() { return fName; }
	ULong64_t TTimepix::GetTimestamp() const { return fTimestamp; }
	TPixelList_t* TTimepix::GetPixels() { return &fPixels; }
	UInt_t TTimepix::GetNHits() const { return fPixels.size(); }
	UShort_t TTimepix::GetNColumns() const { return fNCols; }
	UShort_t TTimepix::GetNRows() const { return fNRows; }
	Double_t TTimepix::GetFrequency() const { return fMpxClock; }
	Double_t TTimepix::GetAcquisitionTime() const { return fAcqTime; }
	Double_t TTimepix::GetStartTime() const { return fStartTime; }
	void TTimepix::GetSize(UShort_t& ncols, UShort_t& nrows)
	{
		ncols = fNCols;
		nrows = fNRows;
	}

// === MODIFIERS =======
	// Set detector ID (string) to which the timepix data is related
	void TTimepix::SetName(TString& name) { fName = name; }
	void TTimepix::SetName(const char* name) { fName = name; }
	// Set cluster parameters
	void TTimepix::SetTimestamp(ULong64_t timestamp) { fTimestamp = timestamp; }
	void TTimepix::SetSize(UShort_t ncols, UShort_t nrows)
		{ fNCols = ncols; fNRows = nrows; }
	void TTimepix::SetFrequency(Double_t freq) { fMpxClock = freq; }
	void TTimepix::SetAcquisitionTime(Double_t t) { fAcqTime = t; }
	void TTimepix::SetStartTime(Double_t t) { fStartTime = t; }
	// Add a new pixel to the cluster
	void TTimepix::AddPixel(TPixel* pixel) {
		fPixels.push_back(pixel); }

// === INFORMATION FUNCTIONS =======
	// Print content
	void TTimepix::Print()
	{
		if(fPixels.size()) {
			cout << "Timepix \"" << fName << "\" contains " << fPixels.size() << " pixels:" << endl;
			cout << "  column\trow\tadc" << endl;
			TPixelIter_t it = fPixels.begin();
			while( it!=fPixels.end() ) {
				(*it)->Print();
				++it;
			}
		} else cout << "Timepix \"" << fName << "\" is empty" << endl;
	}
	// Function that converts the pixel list into three double arrays (coordinates of points)
	void TTimepix::GetArrays(Double_t*& x,Double_t*& y,Double_t*& z)
	{
		// Create new double arrays
		x = new Double_t[fPixels.size()];
		y = new Double_t[fPixels.size()];
		z = new Double_t[fPixels.size()];
		// Fill them
		UInt_t i=0;
		TPixelIter_t it = fPixels.begin();
		while( it!=fPixels.end() ) {
			x[i] = (*it)->GetColumn();
			x[i] = (*it)->GetRow();
			y[i] = (*it)->GetADC();
			++it;
			++i;
		}
	}
	// Function that geometrically adds ncols and nrows (for Rmax in Hough transform)
	Double_t TTimepix::GetDiagonal()
	{
		return TMath::Sqrt( fNCols*fNCols + fNRows*fNRows );
	}