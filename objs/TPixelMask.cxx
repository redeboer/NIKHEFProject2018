// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class contains a list (actually a map) of all pixels that should be ignored.
*/

// === INCLUDE =======
	#include <iostream>
	#include "TPixelMask.h"
	using namespace std;

// === CLASS DECLARATION =======
	TPixelMask::~TPixelMask() {}

// === INFORMATION =======
	vector<vector<bool> >& TPixelMask::GetMask() { return fMaskedPixels; }
	Bool_t TPixelMask::IsMasked( UShort_t col, UShort_t row ) const
	{
		return fMaskedPixels[col][row];
	}
	void TPixelMask::MaskPixel(UShort_t col, UShort_t row)
	{
		fMaskedPixels[col][row] == true;
	}
	void TPixelMask::UnmaskPixel(UShort_t col, UShort_t row)
	{
		fMaskedPixels[col][row] == false;
	}
	void TPixelMask::Print()
	{
		if(!fMaskedPixels.size()) {
			cout << "Pixel mask is empty" << endl;
			return;
		}
		else {
			cout << "Pixel mask contains" << endl;
			for( UShort_t i=0; i<fMaskedPixels.size(); ++i )
				for( UShort_t j=0; j<fMaskedPixels[i].size(); ++j )
					if(fMaskedPixels[i][j]) cout << "  pixel: " << i << " x " << j << endl;
		}
	}