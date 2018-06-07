// Author: Remco de Boer
// Date: June 6th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object contains all relevant information of a calorimeter. It can be used both as a simple container for measured energy in an event (for instance, when useing TSimulatedEventLoader) or as a complete calorimeter data container, containing all 1024 data points from which the energy is computed.
*/

#ifndef TCALOEVENT_H
#define TCALOEVENT_H 1

// === INCLUDE =======
	#include <iostream>
	#include "TBeamObject.h"
	#include "TTimepix.h"
	#include "TString.h"
	#include "TH1S.h"

// === CLASS DECLARATION =======
class TCaloEvent : public TBeamObject {
  
public:
	// Constructors and destructors
	TCaloEvent(UInt_t en, ULong64_t ts) // for simulator or no calo
		: fEventNumber(en), fTimestamp(ts), fHist(NULL) {}
	TCaloEvent(UInt_t en, ULong64_t ts, UShort_t npoints)
		: fEventNumber(en), fTimestamp(ts) { // for calo data
			TString name, title;
			name .Form("event_%u", fEventNumber);
			title.Form("Calorimeter data for event %u;Timesteps;Counts", fEventNumber);
			fHist = new TH1S( name, title, npoints, 0, npoints );
		}
	~TCaloEvent() { if(fHist) delete fHist; }
	// Getters for major values
	UInt_t GetEventNumber() const;
	ULong64_t GetTimestamp() const;
	Double_t GetEnergy() const;
	// Getters for measurement data
	Double_t GetEnergyFit() const;
	Short_t GetValue(UShort_t) const;
	TH1S* GetHistogram() const;
	TF1* GetFit() const;
	// Modify major values
	void SetEventNumber(UInt_t);
	void SetTimestamp(ULong64_t);
	void SetEnergy(Double_t);
	void SetEnergyFit(Double_t);
	// Modify measurement data
	void AddValue(Short_t); 
	void SetValue(UShort_t, Short_t);
	TF1* Fit();
	// Information
	void Print();
	void WriteHistograms();

private:
	// Major info
	UInt_t fEventNumber;
	ULong64_t fTimestamp;
	Double_t fEnergy;
	// Measurement data (if available)
	Double_t fEnergyFit;
	TH1S* fHist;

};

// === TYPE DEFINITIONS =======
	typedef std::list<TCaloEvent*> TCaloEventList_t;
	typedef std::list<TCaloEvent*>::iterator TCaloIter_t;

#endif // TCALOEVENT_H