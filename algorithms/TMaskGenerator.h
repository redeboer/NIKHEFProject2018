// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	WARNING: TTimepixLoader has to be initialised and run before this algorithm!!
	This algorithm makes a file that lists all pixels that have hits and puts the coordinates of those pixels to a txt file. This file can then serve as a mask file. The mask file is loaded with TMaskLoader. (Make sure to handle these classes smartly in the algorithm list in Steering.cxx.)
*/

#ifndef TMASKGENERATOR_H
#define TMASKGENERATOR_H 1

// === INCLUDES =======
	#include "TAlgorithm.h"
	#include "TPixelMask.h"
	#include "TTimepix.h"

// === CLASS DEFINITION =======
class TMaskGenerator : public TAlgorithm {

public:
	// Constructors and destructors
	// has no writing operation (writes to a txt file)
	TMaskGenerator(TClipboard* cp)
		: TAlgorithm(cp,"TMaskGenerator",false), fPixelMask(NULL) {}
	TMaskGenerator(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TMaskGenerator",false,debug), fPixelMask(NULL) {}
	~TMaskGenerator() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	TPixelMask* fPixelMask;
	TTimepixList_t* fTimepixList;
	TTimepixIter_t fTimepixIter;

};

#endif // TRECOGNISETRACKS_H