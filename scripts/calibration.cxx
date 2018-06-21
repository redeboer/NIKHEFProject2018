// Author: Remco de Boer
// Date: June 21st, 2018
// For NIKHEF Project 2018

/* === SCRIPT DESCRIPTION =======
	Put this script in the output folder with output ROOT files from TCaloAnalyseSpectrum and compile+run with
		root -l calibration.cxx+
	Note that output ROOT files from this algorithm should end in "_calo.root" and that, the energies are preset (with 5 values).
*/

#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"
#include "TList.h"
#include "TGraphErrors.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TCanvas.h"
using namespace std;

void calibration() {
	// Walk through 
	TSystemDirectory dir(".",".");
	TList* files = dir.GetListOfFiles();
	TIter next(files);
	next(); // skip folder "."
	next(); // skip folder ".."
	TSystemFile* entry = NULL;
	// Create TGraphError and x-values (actual energies)
	TGraphErrors* gr = new TGraphErrors(5);
   gr->SetTitle("Beam energy versus calorimeter spectrum output;beam energy (MeV);energy from spectrum fit (a.u.)");
   gr->SetMarkerColor(4);
   gr->SetMarkerStyle(21);
	Double_t energies[] = {103.6,143.4,29.4,45.7,78.8}; // MeV
	// Add points from TFiles
	Int_t i=0;
	while( (entry=(TSystemFile*)next()) ) if(!entry->IsDirectory()) {
		TString name = entry->GetName();
		if(name.EndsWith("_calo.root")) {
			cout << name << endl;
			// Get fit from histogram in TFile
			TFile* file = new TFile(name.Data());
			TH1D* hist = (TH1D*)file->Get("energy_sum");
			TF1* fit = (TF1*)hist->GetListOfFunctions()->At(0);
			// Set graph point with error from parameters
			gr->SetPoint(i,energies[i],fit->GetParameter(1));
			gr->SetPointError(i,0,fit->GetParameter(2));
			// Close input ROOT file
			file->Close();
			// Increase counter
			++i;
		}
	}
	// Write graph to new ROOT file
	TFile* file = new TFile("calibration.root","RECREATE");
	gr->Write();
	file->Close();
	// Draw graph in Canvas and save as png
	TCanvas* c = new TCanvas();
	gr->Draw("AP");
	gr->GetXaxis()->SetLimits(0,150);
	gr->GetYaxis()->SetLimits(0,7e5);
	// c->Update();
	c->SaveAs("calibration.png");
	c->Close();
	// Release memory
	// delete htemp;
	delete gr;
}