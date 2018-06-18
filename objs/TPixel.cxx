// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class describes all information of a pixel in a timepix.
*/

// === INCLUDE =======
	#include <iostream>
	#include "TPixel.h"
	using namespace std;

// === CONSTRUCTORS =======
	// Constructors and destructors
	TPixel::TPixel(UShort_t row, UShort_t col, UShort_t adc) {
		fRow = row;
		fCol = col;
		fADC = adc;
	}
	TPixel::~TPixel() {}

// === GETTERS =======
	UShort_t TPixel::GetRow() const { return fRow; };
	UShort_t TPixel::GetColumn() const { return fCol; };
	UShort_t TPixel::GetADC() const { return fADC; };
	void TPixel::Get( UShort_t& row, UShort_t& col, UShort_t& adc ) const
	{
		row = fRow;
		col = fCol;
		adc = fADC;
	}

// === SETTERS =======
	void TPixel::SetRow(UShort_t& row) { fRow = row; };
	void TPixel::SetColumn(UShort_t& col) { fCol = col; };
	void TPixel::SetADC(UShort_t& adc) { fADC = adc; };
	void TPixel::Set( UShort_t& row, UShort_t& col, UShort_t& adc )
	{
		fRow = row;
		fCol = col;
		fADC = adc;
	}

// === INFORMATION =======
	void TPixel::Print() {
		cout << "  " << fRow << "\t" << fCol << "\t" << fADC << endl; }