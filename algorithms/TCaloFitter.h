// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	Performs fits over the data points of a calo event. The fits are stored to the histogram of the calo event. Fit information is stored to a TTree.
*/

#ifndef TCALOFITTER_H
#define TCALOFITTER_H 1

// === INCLUDES =======
	#include <fstream>
	#include "TAlgorithm.h"
	#include "TCaloEvent.h"
	#include "TTree.h"
	#include "TH1D.h"
	#include "TF1.h"

// === CLASS DEFINITION =======
class TCaloFitter : public TAlgorithm {

public:
	// Constructors and destructors
	// does not need writing operation, so writing bit set to true
	// (output written to separate ROOT file)
	TCaloFitter(TClipboard* cp)
		: TAlgorithm(cp,"TCaloFitter",false),
		fHist(NULL), fFit(NULL) {}
	TCaloFitter(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TCaloFitter",false,debug),
		fHist(NULL), fFit(NULL) {}
	~TCaloFitter() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

	// Fit function
	void Fit();

private:
	// Data members
	TTree* fTree;
	TCaloEventList_t* fCaloEventList;
	TCaloEventIter_t  fCaloEventIter;
	// Histogram and fit pointers
	TH1D* fHist; // pointer to calo event data histogram
	TF1* fFit; // longitudinal shower profile
	// Fit results
	Double_t fEnergySum;
	Double_t fEnergyFit;
	Double_t fChi2;
	Double_t fChi2_norm;
	Int_t fNDoF;

};

#endif // TCALOFITTER_H