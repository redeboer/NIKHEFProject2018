// Author: Remco de Boer
// Date: June 8th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	Performs fits over the data points of a calo event. The fits are stored to the histogram of the calo event. Fit information is stored to a TTree.
*/

// === INCLUDES =======
	#include "TCaloFitter.h"
	#include "TList.h"
	#include <iostream>
	#include <stdio.h>
	using namespace std;
	using namespace NIKHEFProject;

// === ALGORITHM STEP FUNCTIONS =======

	// INITIALISE FUNCTION: creates TTree to store energies fitting parametesr
	void TCaloFitter::Initialise() {
		fTree = new TTree("calo_fits","fit results of calo events");
		fTree->Branch("energy_sum",   &fEnergySum,   "energy_sum/D");
		fTree->Branch("energy_lsp",   &fEnergyLSP,   "energy_lsp/D");
		fTree->Branch("energy_landau",&fEnergyLandau,"energy_landau/D");
		fTree->Branch("chi2_lsp",     &fChi2LSP,     "chi2_lsp/D");
		fTree->Branch("chi2_landau",  &fChi2Landau,  "chi2_landau/D");
		fTree->Branch("ndof_lsp",     &fNDoF_LSP,    "ndof_lsp/I");
		fTree->Branch("ndof_landau",  &fNDoF_Landau, "ndof_landau/I");
		fTree->Branch("chi2norm_lsp",   &fChi2LSP_norm,   "chi2norm_lsp/D");
		fTree->Branch("chi2norm_landau",&fChi2Landau_norm,"chi2norm_landau/D");
	}

	// RUN FUNCTION: read the next calo event in the text file and create a TCaloEvent from it
	StatusCode TCaloFitter::Run()
	{
		// Get calo event from clipboard
		fCaloEventList = (TCaloEventList_t*)fClipboard->Get("caloevents");
		fCaloEventIter = fCaloEventList->begin();
		if( fCaloEventIter == fCaloEventList->end() ) {
			if(fDebug) cout << endl << "Calofitter: No calodata in event " << pEventNumber << endl;
			return NoData;
		}
		// Run over calo events in clipboard
		while( fCaloEventIter != fCaloEventList->end()) {
			// Get histogram and sum energy
			fHist = (*fCaloEventIter)->GetHistogram();
			fEnergySum = (*fCaloEventIter)->GetEnergy();
			// debugging output
			if(fDebug) {
				printf("\n  Fit of calo event %u (%u)\n",
					(*fCaloEventIter)->GetEventNumber(),
					(*fCaloEventIter)->GetTimestamp() );
				printf("    Energy (sum): %.0f\n", fEnergySum );
			}
			// Fit calo data
			Fit(); // (UN)COMMENT THIS IF NO FIT COMPUTATION
			// output if fit has been performed
			if( fFitLSP ) {
				// Fill fit energy spectrum
				// Compute tree values: longitudinal shower profile
				fChi2LSP   = fFitLSP->GetChisquare();
				fNDoF_LSP  = fFitLSP->GetNDF();
				fChi2LSP_norm = fChi2LSP/fNDoF_LSP;
				fEnergyLSP = fFitLSP->Integral(
					fHist->GetXaxis()->GetXmin(), // or integrate from start value?
					fHist->GetXaxis()->GetXmax()
				);
				// Compute tree values: Landau profile
				fChi2Landau  = fLandau->GetChisquare();
				fNDoF_Landau    = fLandau->GetNDF();
				fChi2Landau_norm = fChi2Landau/fNDoF_Landau;
				fEnergyLandau = fLandau->Integral(
					fHist->GetXaxis()->GetXmin(), // or integrate from start value?
					fHist->GetXaxis()->GetXmax()
				);
				// Fill TTree
				fTree->Fill();
				// Set fit energy: we choose the longitudinal shower profile for this
				(*fCaloEventIter)->SetEnergyFit(fEnergyLSP);
				// Debugging output: print fit parameters
				if(fDebug) {
					printf("  Energy (LSP): %.0f (%.1f%%)\n", fEnergyLSP,
						100.*(1.-fEnergySum/fEnergyLSP) );
					printf("  Energy (Landau): %.0f\n", fEnergyLandau,
						100.*(1.-fEnergySum/fEnergyLandau) );
					// run over list of functions and print fit info
					TList* list = fHist->GetListOfFunctions();
					TIter next(list);
					while( (fFitLSP = (TF1*)next()) ) {
						if(fDebug) {
							printf("  Fit function \"%s\"\n", fFitLSP->GetName() );
							printf("    NDoF:        %.3f\n", fFitLSP->GetNDF() );
							printf("    Chi squared: %.3f\n", fFitLSP->GetChisquare() ); 
							printf("    Parameter 0: %.7f +/- %.7f\n",
								fFitLSP->GetParameter(0),
								fFitLSP->GetParError(0) );
							printf("    Parameter 1: %.5f +/- %.5f\n",
								fFitLSP->GetParameter(1),
								fFitLSP->GetParError(1) );
							printf("    Parameter 2: %.5f +/- %.5f\n",
								fFitLSP->GetParameter(2),
								fFitLSP->GetParError(2) );
						}
					}
				}
			}
			// Return SUCCESS if finished
			return Success;
		}
		// Return NODATA if failure at position in file stream
		return NoData;
	}

	// FINALISE FUNCTION: only writes TTree
	void TCaloFitter::Finalise() {
		if(fTree) {
			fTree->Write();
			delete fTree;
		}
	}


// === FIT FUNCTIONS =======
	void TCaloFitter::Fit()
	{
		// Check if data is available
		if(!fHist) {
			cout << "Fit error: no calo data (histogram) available!" << endl;
			return;
		}
		// Compute starting point of fit range
		Int_t i=1;
		while( fHist->GetBinContent(i) < pTriggerCaloFit ) i++;
		Double_t startval = fHist->GetBinCenter(i);
		// Difine fitting function:
		// (Longitudinal shower profile: "A x^a e^(-bx)")
		fFitLSP = new TF1("lsp","[0]*TMath::Power(x,[1])*TMath::Exp(-[2]*x)",
			startval, fHist->GetXaxis()->GetXmax() );
		// (Landau function: "[0]*TMath::Landau(x,[1],[2])")
		fLandau = new TF1("landau","[0]*TMath::Landau(x,[1],[2])",
			startval, fHist->GetXaxis()->GetXmax() );
		fLandau->SetLineColor(kBlue);
		// Set estimated fit parameters
		Int_t maxbin = fHist->GetMaximumBin();
		Double_t maxloc = fHist->GetBinCenter(maxbin);
		fFitLSP->SetParameters( // longitudinal shower profile
			3.5e-9*fHist->GetBinContent(maxbin),
			4., 1.14e-2
		);
		fLandau->SetParameters( // Landau curve
			5.2*fHist->GetBinContent(maxbin),
			1.1*maxloc, // location of peak
			0.3*maxloc  // sigma
		);
		// Fit histogram with fit function and compute energy
		fHist->Fit(fFitLSP,"QN","",startval,fHist->GetXaxis()->GetXmax()); // comment this line if you want to see if your fit parameter estimates are close
		fHist->Fit(fLandau,"QN","",startval,fHist->GetXaxis()->GetXmax()); // comment this line if you want to see if your fit parameter estimates are close
		fHist->GetListOfFunctions()->Add(fFitLSP);
		fHist->GetListOfFunctions()->Add(fLandau);
		fHist->GetListOfFunctions()->SetOwner(); // so fit will be deleted along with histogram
	}