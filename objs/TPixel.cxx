// Author: Remco de Boer
// Date: May 20th, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This class describes all information of a pixel in a timepix.
*/

// === INCLUDE =======
	#include <iostream>
	#include "TPixel.h"
	using namespace std;

// === CONSTRUCTORS =======
	// Constructors and destructors
	TPixel::TPixel(UShort_t col, UShort_t row, UShort_t adc) {
		fCol = col;
		fRow = row;
		fADC = adc;
	}
	TPixel::~TPixel() {}
// === GETTERS =======
	UShort_t TPixel::GetColumn() const { return fCol; };
	UShort_t TPixel::GetRow() const { return fRow; };
	UShort_t TPixel::GetADC() const { return fADC; };
	void TPixel::Get( UShort_t& col, UShort_t& row, UShort_t& adc ) const
	{
		col = fCol;
		row = fRow;
		adc = fADC;
	}

// === INFORMATION =======
	void TPixel::Print() {
		cout << "  " << fCol << "\t" << fRow << "\t" << fADC << endl; }