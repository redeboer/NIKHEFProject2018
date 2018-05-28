// Author: Remco de Boer
// Date: May 22nd, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm writes all timepixes on the clipboard to the current (ROOT file) directory in the form of a 2D graph.
*/

#ifndef TWRITETIMEPIXGRAPH_H
#define TWRITETIMEPIXGRAPH_H 1

// === INCLUDES =======
	#include "TGraph2D.h"
	#include "TAlgorithm.h"
	#include "TTimepix.h"

// === CLASS DEFINITION =======
class TWriteTimepixGraph : public TAlgorithm {

public:
	// Constructors and destructors
	// has writing operation, so writing bit set to true
	TWriteTimepixGraph(TClipboard* cp)
		: TAlgorithm(cp,"TWriteTimepixGraph",true) {}
	TWriteTimepixGraph(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TWriteTimepixGraph",true,debug) {}
	~TWriteTimepixGraph() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	// Private functions
	void MakeGraph(TTimepix*);
	// Data members
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;
	TTimepixList_t* fClusterList;
	TTimepixIter_t fClusterIter;
	TGraph2D* fGraph;

};

#endif // TWRITETIMEPIXGRAPH_H
