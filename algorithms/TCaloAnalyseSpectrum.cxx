// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class opens the TTree from TCaloWriter and analyses the energy spectra in there. This can only happen in Finalise (and after TCaloWriter has finilised). Output is written to the TFile (energy is put into the title of the histograms).
*/

// === INCLUDES =======
	#include "TCaloAnalyseSpectrum.h"
	#include "TFile.h"
	#include "TObject.h"
	#include "TString.h"
	#include "TTree.h"
	#include "TF1.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise
	void TCaloAnalyseSpectrum::Initialise() {}

	// RUN FUNCTION: nothin to run
	StatusCode TCaloAnalyseSpectrum::Run() { return Success; }

	// FINALISE FUNCTION: open TTree from TCaloWriter and analyse it. 
	void TCaloAnalyseSpectrum::Finalise()
	{
		// Get access to the calo output file
		if(pCaloOutputFile) if(pCaloOutputFile->IsOpen()) pCaloOutputFile->Close();
		pCaloOutputFile = new TFile(pOutputCalo,"UPDATE");
		if(!pCaloOutputFile) {
			cout << "  ERROR: Calo output file \"" << pOutputCalo << "\" not loaded" << endl;return;
		}
		// Create new output file
		TString name(pOutputCalo);
		name.Insert(name.Last('.'),"_analysed");
		cout << "  Writing spectrum analysis to file \"" << name << "\"" << endl;
		// Attempt to load TTree
		TTree* tree = (TTree*)pCaloOutputFile->Get("calo_fits");
		if(!tree) {
			cout << "No TTree (\"calo_fits\") in \"" << pOutput << "\"" << endl;
			return;
		}
		// Get relevant info TTree
		Long64_t nentries = tree->GetEntries();
		Double_t energy_sum; tree->SetBranchAddress("energy_sum",   &energy_sum);
		Double_t energy_lsp; tree->SetBranchAddress("energy_lsp",   &energy_lsp);
		Double_t energy_lan; tree->SetBranchAddress("energy_landau",&energy_lan);
		// Make histograms
		TH1I* hist_sum = new TH1I("energy_sum",
			"Enery spectrum computed with sum",
			200, 0, 12e5 ); // 0, tree->GetMaximum("energy_sum")
		TH1I* hist_lsp = new TH1I("energy_lsp",
			"Enery spectrum computed with LSP function",
			200, 0, 12e5 ); // 0, tree->GetMaximum("energy_lsp")
		TH1I* hist_lan = new TH1I("energy_lan",
			"Enery spectrum computed with LSP function",
			200, 0, 12e5 ); // 0, tree->GetMaximum("energy_sum")
		// Loop over Tree to make histograms
		for( Long64_t i=0; i<nentries; ++i ) {
			tree->GetEntry(i);
			hist_sum->Fill(energy_sum);
			hist_lsp->Fill(energy_lsp);
			hist_lan->Fill(energy_lan);
		}
		if(true) { // Fit or no fit?
			// Fit functions
			TF1* fit_sum = new TF1("fit_sum","gaus");
			TF1* fit_lsp = new TF1("fit_lsp","gaus");
			TF1* fit_lan = new TF1("fit_lan","gaus");
			// Helpers for parameters
			Int_t maxbin;
			Double_t maxloc;
			TString title;
			// Set sigma
			Double_t sigma = 100;
			// Set parameters for fit over sum energy
			maxbin = hist_sum->GetMaximumBin();
			maxloc = hist_sum->GetBinCenter(maxbin);
			title.Form("Energy sum spectrum. Peak at: %f;energy (a.u.);counts",maxloc);
			hist_sum->SetTitle(title.Data());
			fit_sum->SetParameters(
				hist_sum->GetBinContent(maxbin),
				maxloc, // location of peak
				sigma // sigma
			);
			// Set parameters for fit over LSP energy
			maxbin = hist_lsp->GetMaximumBin();
			maxloc = hist_lsp->GetBinCenter(maxbin);
			title.Form("Energy LSP spectrum. Peak at: %f;energy (a.u.);counts",maxloc);
			hist_lsp->SetTitle(title.Data());
			fit_lsp->SetParameters(
				hist_lsp->GetBinContent(maxbin),
				maxloc, // location of peak
				sigma // sigma
			);
			// Set parameters for fit over Landau energy
			maxbin = hist_lan->GetMaximumBin();
			maxloc = hist_lan->GetBinCenter(maxbin);
			title.Form("Energy Landau spectrum. Peak at: %f;energy (a.u.);counts",maxloc);
			hist_lan->SetTitle(title.Data());
			fit_lan->SetParameters(
				hist_lan->GetBinContent(maxbin),
				maxloc, // location of peak
				sigma // sigma
			);
			// Fit histograms
			hist_sum->Fit(fit_sum,"QN","",maxloc-sigma,maxloc+sigma);
			hist_lsp->Fit(fit_lsp,"QN","",maxloc-sigma,maxloc+sigma);
			hist_lan->Fit(fit_lan,"QN","",maxloc-sigma,maxloc+sigma);
			hist_sum->GetListOfFunctions()->Add(fit_sum);
			hist_lsp->GetListOfFunctions()->Add(fit_lsp);
			hist_lan->GetListOfFunctions()->Add(fit_sum);
			// Gain ownership
			hist_sum->GetListOfFunctions()->SetOwner();
			hist_lsp->GetListOfFunctions()->SetOwner();
			hist_lan->GetListOfFunctions()->SetOwner();
		} // end of fit or no fit
		// Write histograms
		hist_sum->Write();
		hist_lsp->Write();
		hist_lan->Write();
		// Close input/output ROOT files
		pCaloOutputFile->Close();
		pCaloOutputFile = NULL;
	}