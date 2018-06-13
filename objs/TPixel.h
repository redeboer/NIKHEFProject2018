// Author: Remco de Boer
// Date: May 20th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class describes all information of a pixel in a timepix.
*/

#ifndef TPIXEL_H
#define TPIXEL_H 1

// === INCLUDE =======
	#include <list>
	#include "TBeamObject.h"

// === CLASS DECLARATION =======
class TPixel : public TBeamObject {

public:
	// Constructors and destructors
	TPixel() {}
	~TPixel();
	TPixel(UShort_t,UShort_t,UShort_t);
	// Getters
	UShort_t GetRow() const;
	UShort_t GetColumn() const;
	UShort_t GetADC() const;
	void Get(UShort_t&,UShort_t&,UShort_t&) const;
	// Setters
	void SetRow(UShort_t&);
	void SetColumn(UShort_t&);
	void SetADC(UShort_t&);
	void Set(UShort_t&,UShort_t&,UShort_t&);
	// Information
	void Print();

private:
	UShort_t fRow;
	UShort_t fCol;
	UShort_t fADC;

};

// === TYPE DEFINITIONS =======
	typedef std::list<TPixel*> TPixelList_t;
	typedef std::list<TPixel*>::iterator TPixelIter_t;

#endif // TPIXEL_H
