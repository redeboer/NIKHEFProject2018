// Author: Remco de Boer
// Date: June 13th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm creates track objects for all clusters available in the clipboard. This is done through applying a linear fit (TLinearFitter) on the pixel data points associated to each cluster. See TTrack for how this information is stored.
	All fit parameters are stored to a TTree.
*/

#ifndef TFITTRACKS_H
#define TFITTRACKS_H 1

// === INCLUDES =======
	#include "TLinearFitter.h"
	#include "TAlgorithm.h"
	#include "TTimepix.h"
	#include "TTrack.h"
	#include "TH2D.h"
	#include "TTree.h"

// === CLASS DEFINITION =======
class TFitTracks : public TAlgorithm {

public:
	// Constructors and destructors
	// has writing writing: write tree of fit parameters
	TFitTracks(TClipboard* cp, Bool_t debug=false)
		: TAlgorithm(cp,"TFitTracks",true,debug) {}
	~TFitTracks() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	Bool_t FitTrack(TTimepix*);
	void MakeHistogramFit(TTimepix*);
	// Histogram
	TH2D* fHistogram;
	// Fit parameters
	TTree* fTree;
	Double_t fXYp0;  // XY fit parameter 0
	Double_t fXYp1;  // XY fit parameter 1
	Double_t fXYe0;  // XY fit error 0
	Double_t fXYe1;  // XY fit error 1
	Double_t fXYchi; // CY fit chi squared
	Double_t fXYndf; // XY fit number of degreees of freedom
	Double_t fXZp0;  // XZ fit parameter 0
	Double_t fXZp1;  // XZ fit parameter 1
	Double_t fXZe0;  // XY fit error 0
	Double_t fXZe1;  // XY fit error 1
	Double_t fXZchi; // XZ fit chi squared
	Double_t fXZndf; // XZ fit number of degreees of freedom
	// Temporary pointers
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TTimepixList_t* fClusterList;
	TTimepixIter_t fClusterIter;
	TTrack* fTrack;
	TLinearFitter* fFitterXY;
	TLinearFitter* fFitterXZ;

};

#endif // TFITTRACKS_H
