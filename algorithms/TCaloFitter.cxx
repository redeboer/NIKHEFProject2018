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

	// INITIALISE FUNCTION: creates TTree to store energies fitting parameters and TFile to store the TTree
	void TCaloFitter::Initialise()
	{
		// Create TTree
		fTree = new TTree("calo_fits","fit results of calo events");
		fTree->Branch("energy_sum",&fEnergySum,"energy_sum/D",1e6);
		fTree->Branch("energy_fit",&fEnergyFit,"energy_fit/D",1e6);
		fTree->Branch("chi2",      &fChi2,     "chi2/D",1e6);
		fTree->Branch("ndof",      &fNDoF,     "ndof/I",1e6);
		fTree->Branch("chi2norm",  &fChi2_norm,"chi2norm/D",1e6);
		// Open TFile for calo analysis output
		pCaloOutputFile = new TFile(pOutputCalo,"RECREATE");
	}

	// RUN FUNCTION: read the next calo event in the text file and create a TCaloEvent from it
	StatusCode TCaloFitter::Run()
	{
		// Get calo event from clipboard
		fCaloEventList = (TCaloEventList_t*)fClipboard->Get("caloevents");
		if( !fCaloEventList ) {
			if(fDebug) cout << endl << "Calofitter: No calodata in event " << pEventNumber << endl;
			return NoData;
		}
		// Run over calo events in clipboard
		fCaloEventIter = fCaloEventList->begin();
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
			Fit(); // COMMENT THIS IF NO FIT COMPUTATION
			// Output if fit has been performed
			if( fFit ) {
				// Fill fit energy spectrum
				// Compute tree values: longitudinal shower profile
				fChi2 = fFit->GetChisquare();
				fNDoF = fFit->GetNDF();
				fChi2_norm = fChi2/fNDoF;
				fEnergyFit = fFit->Integral(
					fHist->GetXaxis()->GetXmin(), // or integrate from start value?
					fHist->GetXaxis()->GetXmax()
				);
				// Set fit energy: we choose the longitudinal shower profile for this
				(*fCaloEventIter)->SetEnergyFit(fEnergyFit);
				// Debugging output: print fit parameters
				if(fDebug) {
					printf("  Energy (fit): %.0f (%.1f%%)\n", fEnergyFit,
						100.*(1.-fEnergySum/fEnergyFit) );
					// run over list of functions and print fit info
					TList* list = fHist->GetListOfFunctions();
					TIter next(list);
					while( (fFit = (TF1*)next()) ) {
						if(fDebug) {
							printf("  Fit function \"%s\"\n", fFit->GetName() );
							printf("    NDoF:        %.3f\n", fFit->GetNDF() );
							printf("    Chi squared: %.3f\n", fFit->GetChisquare() ); 
							printf("    Parameter 0: %.7f +/- %.7f\n",
								fFit->GetParameter(0),
								fFit->GetParError(0) );
							printf("    Parameter 1: %.5f +/- %.5f\n",
								fFit->GetParameter(1),
								fFit->GetParError(1) );
							printf("    Parameter 2: %.5f +/- %.5f\n",
								fFit->GetParameter(2),
								fFit->GetParError(2) );
						}
					}
				}
			}
			// Fill TTree
			// if(pEventNumber>=3e3) return Finished; // temporary fix
			fTree->Fill();
			// Return SUCCESS if finished
			return Success;
		}
		// Return NODATA if failure at position in file stream
		return NoData;
	}

	// FINALISE FUNCTION: only writes TTree
	void TCaloFitter::Finalise() {
		// Write TTree
		if(fTree) {
			pCaloOutputFile->WriteObject(fTree,fTree->GetName());
			delete fTree;
		}
		// Close output file
		pCaloOutputFile->Close();
		pCaloOutputFile = NULL;
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
		// Define longitudinal shower profile (LSP): "A x^a e^(-bx)"
		fFit = new TF1("fit","[0]*TMath::Power(x,[1])*TMath::Exp(-[2]*x)",
			startval, fHist->GetXaxis()->GetXmax() );
		// Set estimated fit parameters
		Int_t maxbin = fHist->GetMaximumBin();
		Double_t maxloc = fHist->GetBinCenter(maxbin);
		fFit->SetParameters(
			3.5e-9*fHist->GetBinContent(maxbin),
			4., 1.14e-2
		);
		// Fit histogram with fit function and compute energy
		fHist->Fit(fFit,"QN","",startval,fHist->GetXaxis()->GetXmax()); // comment this line if you want to see if your fit parameter estimates are close
		fHist->GetListOfFunctions()->Add(fFit);
		fHist->GetListOfFunctions()->SetOwner(); // so fit will be deleted along with histogram
	}