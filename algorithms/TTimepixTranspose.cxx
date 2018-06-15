// Author: Remco de Boer
// Date: June 13th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This algorithm transposes al timepixes on the clipboard. Columns become rows.
*/

// === INCLUDES =======
	#include "TTimepixTranspose.h"
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: does nothing
	void TTimepixTranspose::Initialise() {}

	// RUN FUNCTION: gets pixels and timepixes from the clipboard and flips their (n)cols and (n)rows
	StatusCode TTimepixTranspose::Run()
	{
		// Flip dimensions on clipboard
		fRow = pNRows;
		fCol = pNCols;
		pNRows = fCol;
		pNCols = fRow;
		// Flip dimension of all timepixes
		fTimepixList = (TTimepixList_t*)fClipboard->Get("timepix");
		fTimepixIter = fTimepixList->begin();
		if( fTimepixIter != fTimepixList->end()) {
			(*fTimepixIter)->GetSize(fRow,fCol);
			(*fTimepixIter)->SetSize(fCol,fRow);
		}
		// Flip row and column of all pixels
		fPixelList = (TPixelList_t*)fClipboard->Get("pixels");
		fPixelIter = fPixelList->begin();
		if( fPixelIter != fPixelList->end()) {
			fRow = (*fPixelIter)->GetRow();
			fCol = (*fPixelIter)->GetColumn();
			(*fPixelIter)->SetRow(fCol);
			(*fPixelIter)->SetColumn(fCol);
		}
	}

	// FINALISE FUNCTION: does nothing
	void TTimepixTranspose::Finalise() {}