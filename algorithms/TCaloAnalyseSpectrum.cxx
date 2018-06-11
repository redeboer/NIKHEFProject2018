// Author: Remco de Boer
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This class opens the TTree from TCaloWriter and analyses the energy spectra in there. This can only happen in Finalise (and after TCaloWriter has finilised). Output is written to the TFile (energy is put into the title of the histograms).
*/

// === INCLUDES =======
	#include "TCaloAnalyseSpectrum.h"
	#include "TObject.h"
	#include "TTree.h"
	#include "TF1.h"
	#include <iostream>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: nothing to initialise
	void TCaloAnalyseSpectrum::Initialise() {}

	// RUN FUNCTION: nothin to run
	StatusCode TCaloAnalyseSpectrum::Run()
	{
		// return Finished; // uncomment if this is the only algorithm
	}

	// FINALISE FUNCTION: open TTree from TCaloWriter and analyse it. 
	void TCaloAnalyseSpectrum::Finalise()
	{
		// Get access to output file
		if(!pCaloOutputFile) pCaloOutputFile = new TFile(pOutputCalo,"UPDATE");
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
			200, 0, tree->GetMaximum("energy_sum") );
		TH1I* hist_lsp = new TH1I("energy_lsp",
			"Enery spectrum computed with LSP function",
			200, 0, tree->GetMaximum("energy_lsp") );
		TH1I* hist_lan = new TH1I("energy_lan",
			"Enery spectrum computed with LSP function",
			200, 0, tree->GetMaximum("energy_lan") );
		// Loop over Tree to make histograms
		for( Long64_t i=0; i<nentries; ++i ) {
			tree->GetEntry(i);
			hist_sum->Fill(energy_sum);
			hist_lsp->Fill(energy_lsp);
			hist_lan->Fill(energy_lan);
		}
		// Fit functions
		TF1* fit_sum = new TF1("fit_sum","[0]*TMath::Landau(x,[1],[2])");
		TF1* fit_lsp = new TF1("fit_lsp","[0]*TMath::Landau(x,[1],[2])");
		TF1* fit_lan = new TF1("fit_lan","[0]*TMath::Landau(x,[1],[2])");
		// Helpers for parameters
		Int_t maxbin;
		Double_t maxloc;
		TString title;
		// Set parameters for fit over sum energy
		maxbin = hist_sum->GetMaximumBin();
		maxloc = hist_sum->GetBinCenter(maxbin);
		title.Form("Energy sum spectrum. Peak at: %f",maxloc);
		hist_sum->SetTitle(title);
		fit_sum->SetParameters( // Landau curve
			5.2*hist_sum->GetBinContent(maxbin),
			1.1*maxloc, // location of peak
			0.3*maxloc  // sigma
		);
		// Set parameters for fit over LSP energy
		maxbin = hist_lsp->GetMaximumBin();
		maxloc = hist_lsp->GetBinCenter(maxbin);
		title.Form("Energy LSP spectrum. Peak at: %f",maxloc);
		hist_lsp->SetTitle(title);
		fit_lsp->SetParameters( // Landau curve
			5.2*hist_lsp->GetBinContent(maxbin),
			1.1*maxloc, // location of peak
			0.3*maxloc  // sigma
		);
		// Set parameters for fit over Landau energy
		maxbin = hist_lan->GetMaximumBin();
		maxloc = hist_lan->GetBinCenter(maxbin);
		title.Form("Energy Landau spectrum. Peak at: %f",maxloc);
		hist_lan->SetTitle(title);
		fit_lan->SetParameters( // Landau curve
			5.2*hist_lan->GetBinContent(maxbin),
			1.1*maxloc, // location of peak
			0.3*maxloc  // sigma
		);
		// Fit histograms
		hist_sum->Fit(fit_sum,"QN","",0,hist_sum->GetXaxis()->GetXmax());
		hist_lsp->Fit(fit_lsp,"QN","",0,hist_lsp->GetXaxis()->GetXmax());
		hist_lan->Fit(fit_lan,"QN","",0,hist_lan->GetXaxis()->GetXmax());
		hist_sum->GetListOfFunctions()->Add(fit_sum);
		hist_lsp->GetListOfFunctions()->Add(fit_lsp);
		hist_lan->GetListOfFunctions()->Add(fit_sum);
		// Gain ownership
		hist_sum->GetListOfFunctions()->SetOwner();
		hist_lsp->GetListOfFunctions()->SetOwner();
		hist_lan->GetListOfFunctions()->SetOwner();
		// Remove old histograms from TFile
		// pCaloOutputFile->Delete(hist_sum->GetName());
		// pCaloOutputFile->Delete(hist_lsp->GetName());
		// pCaloOutputFile->Delete(hist_lan->GetName());
		// Write histograms
		pCaloOutputFile->WriteObject(hist_sum,hist_sum->GetName());
		pCaloOutputFile->WriteObject(hist_lsp,hist_lsp->GetName());
		pCaloOutputFile->WriteObject(hist_lan,hist_lan->GetName());
	}