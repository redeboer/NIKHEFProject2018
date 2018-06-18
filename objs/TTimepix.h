// Author: Remco de Boer
// Date: June 1st, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class describes all features of a timepix, like dimensions and clock frequency. Hit information is contained in the list of pixels (fPixels). This object can therefore also serve as a definition for a cluster of pixel data in a timepix (like a linear track).
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
			: fName(name), fTimestamp(timestamp), fNRows(0), fNCols(0), fMpxClock(0), fAcqTime(0), fStartTime(0) {}
		TTimepix(const char* name, ULong_t timestamp, UShort_t ncols, UShort_t nrows, Double_t freq, Double_t acqtime, Double_t stime)
			: fName(name), fTimestamp(timestamp), fNRows(nrows), fNCols(ncols), fMpxClock(freq), fAcqTime(acqtime), fStartTime(stime) {}
		TTimepix(TTimepix* tp) // copy constructor only copies, not pixels!
			: fName(tp->GetName()), fTimestamp(tp->GetTimestamp()), fNRows(tp->GetNRows()), fNCols(tp->GetNColumns()), fMpxClock(tp->GetFrequency()), fAcqTime(tp->GetAcquisitionTime()), fStartTime(tp->GetStartTime()) {}
		~TTimepix() {}
		TTimepix* Clone();
	// Getters
		TString& GetName();
		ULong64_t GetTimestamp() const;
		TPixelList_t* GetPixels();
		UInt_t GetNHits() const;
		UShort_t GetNRows() const;
		UShort_t GetNColumns() const;
		Double_t GetFrequency() const;
		Double_t GetAcquisitionTime() const;
		Double_t GetStartTime() const;
		void GetSize(UShort_t&,UShort_t&);
	// Modifiers
		void SetName(TString&);
		void SetName(const char*);
		void SetTimestamp(ULong64_t);
		void SetSize(UShort_t,UShort_t);
		void SetFrequency(Double_t);
		void SetAcquisitionTime(Double_t);
		void SetStartTime(Double_t);
		void AddPixel(TPixel*);
	// Information functions
		void Print();
		void GetArrays(Double_t*&,Double_t*&,Double_t*&);
		Double_t GetDiagonal();

private:
	TString fName; // file name without extension
	ULong64_t fTimestamp;
	TPixelList_t fPixels; // list of hit pixels
	UShort_t fNRows; // "height" of timepix
	UShort_t fNCols; // "width" of timepix
	Double_t fMpxClock; // medipix clock frequency [MHz]
	Double_t fAcqTime;  // acquisition time [s]
	Double_t fStartTime; // recorded start time [s?]

};

// === TYPE DEFINITIONS =======
	typedef std::list<TTimepix*> TTimepixList_t;
	typedef std::list<TTimepix*>::iterator TTimepixIter_t;

#endif // TTIMEPIX_H
