// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class contains a list (actually a map) of all pixels that should be ignored.
*/

#ifndef TPIXELMASK_H
#define TPIXELMASK_H 1

// === INCLUDE =======
	#include <vector>
	#include "TBeamObject.h"

// === CLASS DECLARATION =======
class TPixelMask : public TBeamObject {

public:
	// Constructors and destructors
	TPixelMask(UShort_t nrows, UShort_t ncols)
		: fMaskedPixels(nrows,std::vector<bool>(ncols)) {}
	~TPixelMask();
	// Functions
	std::vector<std::vector<bool> >& GetMask(); // nrow x ncol matrix
	Bool_t IsMasked(UShort_t,UShort_t) const;
	void MaskPixel(UShort_t,UShort_t);
	void UnmaskPixel(UShort_t,UShort_t);
	void Print();

private:
	std::vector<std::vector<bool> > fMaskedPixels;

};

#endif // TPIXELMASK_H
