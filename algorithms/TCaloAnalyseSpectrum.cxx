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

	// RUN FUNCTION: nothing to run
	StatusCode TCaloAnalyseSpectrum::Run() { return Success; }

	// FINALISE FUNCTION: open TTree from TCaloWriter and analyse spectrum using a Gaussian fit
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
		Double_t energy_sum; tree->SetBranchAddress("energy_sum", &energy_sum);
		Double_t energy_fit; tree->SetBranchAddress("energy_fit", &energy_fit);
		// Make histograms
		TH1I* hist_sum = new TH1I("energy_sum",
			"Enery spectrum computed with sum",
			200, 0, 1e6 ); // 0, tree->GetMaximum("energy_sum")
		TH1I* hist_fit = new TH1I("energy_fit",
			"Enery spectrum computed with LSP fit",
			200, 0, 1e6 ); // 0, tree->GetMaximum("energy_fit")
		// Loop over Tree to make histograms
		for( Long64_t i=0; i<nentries; ++i ) {
			tree->GetEntry(i);
			hist_sum->Fill(energy_sum);
			hist_fit->Fill(energy_fit);
		}
		// For sum energies: Create Gaussian fit, set parameters, and fit
		Double_t sigma = 1e5;
		Int_t maxbin = hist_sum->GetMaximumBin();
		Double_t maxloc = hist_sum->GetBinCenter(maxbin);
		TF1* fit_sum = new TF1("fit_sum","gaus",maxloc-2*sigma,maxloc+2*sigma);
		fit_sum->SetParameters(
			hist_sum->GetBinContent(maxbin),
			maxloc, // location of peak
			sigma // sigma
		);
		hist_sum->Fit(fit_sum,"QN","",maxloc-2*sigma,maxloc+2*sigma);
		// For fit energies: Create Gaussian fit, set parameters, and fit
		maxbin = hist_fit->GetMaximumBin();
		maxloc = hist_fit->GetBinCenter(maxbin);
		TF1* fit_fit = new TF1("fit_fit","gaus",maxloc-2*sigma,maxloc+2*sigma);
		fit_fit->SetParameters(
			hist_fit->GetBinContent(maxbin),
			maxloc, // location of peak
			sigma // sigma
		);
		hist_fit->Fit(fit_fit,"QN","",maxloc-2*sigma,maxloc+2*sigma);
		// Fit histograms, add functions, gain ownership, and set options
		hist_sum->GetListOfFunctions()->Add(fit_sum);
		hist_fit->GetListOfFunctions()->Add(fit_fit);
		hist_sum->GetListOfFunctions()->SetOwner();
		hist_fit->GetListOfFunctions()->SetOwner();
		hist_sum->SetOption("err");
		hist_fit->SetOption("err");
		// Set new titles for histograms
		name.Form("Energy spectrum computed with sum (peak: %.2e +/- %.2e, valid entries: %.0f/%u);calo event sum (a.u.);counts",
			fit_sum->GetParameter(1),
			fit_sum->GetParameter(2),
			hist_sum->GetEntries(),
			pTotalFiles );
		hist_sum->SetTitle(name.Data());
		name.Form("Energy spectrum computed with LSP fit (peak: %.2e +/- %.2e, valid entries: %.0f/%u);calo event sum (a.u.);counts",
			fit_fit->GetParameter(1),
			fit_fit->GetParameter(2),
			hist_fit->GetEntries(),
			pTotalFiles );
		hist_fit->SetTitle(name.Data());
		// Write histogram and delete
		hist_sum->Write(); delete hist_sum;
		hist_fit->Write(); delete hist_fit;
		// Close input/output ROOT files
		pCaloOutputFile->Close();
		pCaloOutputFile = NULL;
	}