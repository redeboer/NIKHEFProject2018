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
	Short_t TCaloEvent::GetValue(UShort_t i) const
	{
		if(fHist) {
			++i;
			return fHist->GetBinContent(i);
		} else {
			cout << "No histogram available" << endl;
			return NULL;
		}
	}
	TH1S* TCaloEvent::GetHistogram() const { return fHist; }
	TF1* TCaloEvent::GetFit() const
	{
		if(!fHist) return NULL;
		if(!fHist->GetListOfFunctions()) return NULL;
		return (TF1*)fHist->GetListOfFunctions()->At(0);
	}
	Double_t TCaloEvent::GetEnergy() const { return fEnergy; }
	Double_t TCaloEvent::GetEnergyFit() const { return fEnergyFit; }

// === MODIFY MAJOR VALUES =======
	void TCaloEvent::SetEventNumber(UInt_t en) { fEventNumber = en; }
	void TCaloEvent::SetTimestamp(ULong64_t ts) { fTimestamp = ts; }
	void TCaloEvent::SetEnergy(Double_t e) { fEnergy = e; }
	void TCaloEvent::SetEnergyFit(Double_t e) { fEnergyFit = e; }

// === MODIFY MAJOR VALUES =======
	void TCaloEvent::SetValue(UShort_t i, Short_t val)
	{
		if(fHist) {
			++i;
			fHist->SetBinContent(i,val);
		} else {
			cout << "No histogram available" << endl;
		}
	}
	TF1* TCaloEvent::Fit()
	{
		// Check if data is available
		if(!fHist) {
			cout << "Fit error: no calo data available!" << endl;
			return NULL;
		}
		// Difine fitting function: E_0*t^a*e^(-bt)
		TF1* fit = new TF1("f1",
			"[0]*TMath::Power(x,[1])*TMath::Exp(-[2]*x)",
			fHist->GetXaxis()->GetXmin(),
			fHist->GetXaxis()->GetXmax() );
		fit->SetParameter(0,1);
		fit->SetParameter(1,0);
		fit->SetParameter(1,0);
		// Fit histogram with fit function and compute energy
		fHist->Fit(fit,"Q0");
		fHist->GetListOfFunctions()->SetOwner(); // so fit will be deleted along with histogram
		// Compute integral of function
		fEnergyFit = fit->Integral(
			fHist->GetXaxis()->GetXmin(),
			fHist->GetXaxis()->GetXmax() );
		// Return fit result upon success
		return fit;
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
	void TCaloEvent::WriteHistograms()
	{
		if(fHist) fHist->Write();
	}