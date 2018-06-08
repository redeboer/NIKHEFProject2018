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
	void TCaloEvent::SetValue(UShort_t i, Short_t val)
	{
		if(fHist) {
			++i;
			fHist->SetBinContent(i,val);
		} else {
			cout << "No histogram available" << endl;
		}
	}
	void TCaloEvent::Fit()
	{
		// Check if data is available
		if(!fHist) {
			cout << "Fit error: no calo data available!" << endl;
			return;
		}
		// Compute starting point of fit range
		Int_t i=1;
		while( fHist->GetBinContent(i) < pTriggerCaloFit ) i++;
		Double_t startval = fHist->GetBinCenter(i);
		// Difine fitting function:
		// (Lera's function: "A x^a e^(-bx)")
		TF1* fit1 = new TF1("lsp","[0]*TMath::Power(x,[1])*TMath::Exp(-[2]*x)",
			startval, fHist->GetXaxis()->GetXmax() );
		// (Landau function: "[0]*TMath::Landau(x,[1],[2])")
		TF1* fit2 = new TF1("landau","[0]*TMath::Landau(x,[1],[2])",
			startval, fHist->GetXaxis()->GetXmax() );
		fit2->SetLineColor(kBlue);
		// Set estimated fit parameters
		Int_t maxbin = fHist->GetMaximumBin();
		Double_t maxloc = fHist->GetBinCenter(maxbin);
		fit1->SetParameters( // Lera's function
			3.5e-9*fHist->GetBinContent(maxbin),
			4., 1.14e-2 );
		fit2->SetParameters( // Landau curve
			5.2*fHist->GetBinContent(maxbin),
			1.1*maxloc,    // location of peak
			0.3*maxloc // sigma
		);
		// Fit histogram with fit function and compute energy
		fHist->Fit(fit1,"QN","",startval,fHist->GetXaxis()->GetXmax()); // comment this line if you want to see if your fit parameter estimates are close
		fHist->Fit(fit2,"QN","",startval,fHist->GetXaxis()->GetXmax()); // comment this line if you want to see if your fit parameter estimates are close
		fHist->GetListOfFunctions()->Add(fit1);
		fHist->GetListOfFunctions()->Add(fit2);
		fHist->GetListOfFunctions()->SetOwner(); // so fit will be deleted along with histogram
		// Compute integral of function
		fEnergyFit = fit1->Integral(
			fHist->GetXaxis()->GetXmin(),
			fHist->GetXaxis()->GetXmax() );
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