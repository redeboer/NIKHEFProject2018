// Author: Remco de Boer
// Date: May 27th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes combined data of point reconstruction, the calorimeter, and the two TPCs to the ROOT output file in the form of histograms, a 3D graph, and a TTree. The TTree can be reused later in TReadReconstruction.
*/


#ifndef TWRITERECONSTRUCTION_H
#define TWRITERECONSTRUCTION_H 1

// === INCLUDES =======
	#include "TAlgorithm.h"
	#include "TRecPoint.h"
	#include "TGraph2D.h"
	#include "TH2I.h"
	#include "TH2D.h"
	#include "TH3D.h"
	#include "TTree.h"

// === CLASS DEFINITION =======
class TWriteReconstruction : public TAlgorithm {

public:
	// Constructors and destructors
	// has writing operation, so writing bit set to true
	TWriteReconstruction(TClipboard* cp)
		: TAlgorithm(cp,"TWriteReconstruction",true) {}
	TWriteReconstruction(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TWriteReconstruction",true,debug) {}
	~TWriteReconstruction() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	void MakeGraph(TTimepix*);
	// Pointers and iterators
	TRecPointList_t* fPointList;
	TRecPointIter_t fPointIter;
	TTrack* fTrack1;
	TTrack* fTrack2;
	// TTree
	TTree* fTree;
	Double_t fChiSqXY1,fChiSqXZ1;
	Double_t fChiSqXY2,fChiSqXZ2;
	Double_t fPy1,fPz1,fVy1,fVz1;
	Double_t fPy2,fPz2,fVy2,fVz2;
	Double_t fX, fY, fZ, fEloss;
	// Graph and histograms
	TH3D* fRecPointDist3D;
	TH2D* fRecPointDistXY;
	TH2D* fRecPointDistXZ;
	TH2D* fRecPointDistYZ;
	TH2I* fTPC1pointsYZ; // histogram of fState
	TH2I* fTPC2pointsYZ; // histogram of fState
	TH2D* fTPC2energyYZ; // histogram of fState with energy loss attributed
	TGraph2D* fGraph;

};

#endif // TWRITERECONSTRUCTION_H