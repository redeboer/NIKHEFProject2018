// Author: Remco de Boer
// Date: May 22nd, 2018
// Inspired by Corryvreckan

/* === CLASS DESCRIPTION =======
	This class describes all features of a timepix (there are two at the moment of writing). Hit information is contained in the list of pixels (fPixels). This object can therefore also serve as a definition for a cluster of pixel data in a timepix (like a linear track).
*/

#ifndef TTIMEPIX_H
#define TTIMEPIX_H 1

// === INCLUDE =======
	#include "TString.h"
	#include "TPixel.h"

// === CLASS DECLARATION =======
class TTimepix : public TBeamObject {

public:
	// Constructors and destructors
		TTimepix(const char* name, ULong_t timestamp)
			: fName(name), fTimestamp(timestamp) {}
		TTimepix(TTimepix* tp) // copy constructor only copies, not pixels!
			: fName(tp->GetName()), fTimestamp(tp->GetTimestamp()) {}
		~TTimepix() {}
		TTimepix* Clone();
	// Getters
		TString& GetName();
		ULong64_t GetTimestamp() const;
		TPixelList_t* GetPixels();
		UInt_t GetNHits() const;
	// Modifiers
		void SetName(TString&);
		void SetName(const char*);
		void SetTimestamp(ULong64_t);
		void AddPixel(TPixel*);
	// Information functions
		void Print();
		void GetArrays(Double_t*&,Double_t*&,Double_t*&);

private:
	TString fName;
	ULong64_t fTimestamp;
	TPixelList_t fPixels;

};

// === TYPE DEFINITIONS =======
	typedef std::list<TTimepix*> TTimepixList_t;
	typedef std::list<TTimepix*>::iterator TTimepixIter_t;

#endif // TTIMEPIX_H
