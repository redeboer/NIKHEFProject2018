// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm makes a file that lists all pixels that have hits and puts the coordinates of those pixels to a txt file. This file can then serve as a mask file. The mask file is loaded with TMaskLoader. (Make sure to handle these classes smartly in the algorithm list in Steering.cxx.)
	Note that the convention for coordinates is (row,column), as in a matrix, and for the dimension it is (nrows,ncols) = (height,width).
*/

#ifndef TMASKLOADER_H
#define TMASKLOADER_H 1

// === INCLUDES =======
	#include "TAlgorithm.h"
	#include "TPixelMask.h"

// === CLASS DEFINITION =======
class TMaskLoader : public TAlgorithm {

public:
	// Constructors and destructors
	// has no writing operation (writes to a txt file)
	TMaskLoader(TClipboard* cp)
		: TAlgorithm(cp,"TMaskLoader",false), fPixelMask(NULL) {}
	TMaskLoader(TClipboard* cp, Bool_t debug)
		: TAlgorithm(cp,"TMaskLoader",false,debug), fPixelMask(NULL) {}
	~TMaskLoader() {}

	// Algorithm step functions
	void Initialise();
	StatusCode Run();
	void Finalise();

private:
	TPixelMask* fPixelMask;

};

#endif // TMASKLOADER_H