// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	Performs fits over the data points of a calo event. The fits are stored to the histogram of the calo event. Fit information is stored to a TTree.
*/

#ifndef TCALOFITTER_T
#define TCALOFITTER_T 1

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
		fHist(NULL), fFitLSP(NULL), fLandau(NULL) {}
	TCaloFitter(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TCaloFitter",false,debug),
		fHist(NULL), fFitLSP(NULL), fLandau(NULL) {}
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
	TH1D* fHist;  // pointer to calo event data histogram
	TF1* fFitLSP; // longitudinal shower profile
	TF1* fLandau; // Landau function
	// Fit results
	Double_t fEnergySum;
	Double_t fEnergyLSP;
	Double_t fEnergyLandau;
	Double_t fChi2LSP;
	Double_t fChi2Landau;
	Double_t fChi2LSP_norm;
	Double_t fChi2Landau_norm;
	Int_t fNDoF_LSP;
	Int_t fNDoF_Landau;

};

#endif // TCALOFITTER_T