// Author: Remco de Boer
// Date: June 6th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION ======
	This object contains all relevant information of a calorimeter. It can be used both as a simple container for measured energy in an event (for instance, when useing TSimulatedEventLoader) or as a complete calorimeter data container, containing all 1024 data points from which the energy is computed.
*/

// === INCLUDE =======
	#include "TCaloEvent.h"
	#include "TF1.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === GETTERS =======
	UInt_t TCaloEvent::GetEventNumber() const { return fEventNumber; }
	ULong64_t TCaloEvent::GetTimestamp() const { return fTimestamp; }
	Double_t TCaloEvent::GetValue(Int_t i) const
	{
		if(fHist) {
			++i;
			return fHist->GetBinContent(i);
		} else {
			cout << "No histogram available" << endl;
			return 0;
		}
	}
	TH1D* TCaloEvent::GetHistogram() const { return fHist; }
	TF1* TCaloEvent::GetFit(UChar_t i) const
	{
		if(!fHist) return NULL;
		if(!fHist->GetListOfFunctions()) return NULL;
		if(i>=fHist->GetListOfFunctions()->GetSize()) return NULL;
		return (TF1*)fHist->GetListOfFunctions()->At(i);
	}
	Double_t TCaloEvent::GetEnergy() const { return fEnergy; }
	Double_t TCaloEvent::GetEnergyFit() const { return fEnergyFit; }

// === MODIFY MAJOR VALUES =======
	void TCaloEvent::SetEventNumber(UInt_t en) { fEventNumber = en; }
	void TCaloEvent::SetTimestamp(ULong64_t ts) { fTimestamp = ts; }
	void TCaloEvent::SetEnergy(Double_t e) { fEnergy = e; }
	void TCaloEvent::SetEnergyFit(Double_t e) { fEnergyFit = e; }

// === MODIFY MAJOR VALUES =======
	void TCaloEvent::SetValue(Int_t i, Double_t val)
	{
		if(fHist) {
			++i;
			fHist->SetBinContent(i,val);
		} else {
			cout << "No histogram available" << endl;
		}
	}

// === INFORMATION FUNCTIONS =======
	void TCaloEvent::Print()
	{
		cout << "Calorimeter data for event " << fEventNumber << "(" << fTimestamp << ")" << endl;
		cout << "  Energy: " << fEnergy << endl;
		cout << "  Values: " << fEnergy << endl;
		for( UShort_t i=0; i<fHist->GetNbinsX(); ++i ) {
			cout << "  [" << i << "]:  " << GetValue(i) << endl;
		}
	}
	// Note that the path to write to should be set first
	void TCaloEvent::WriteHistogram()
	{
		if(fHist) fHist->Write();
	}