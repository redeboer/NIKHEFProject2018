// Author: Remco de Boer
// Date: June 21st, 2018
// For NIKHEF Project 2018

/* === SCRIPT DESCRIPTION =======
	Put this script in the output folder with output ROOT files from TCaloAnalyseSpectrum and compile+run with
		root -l calibration.cxx+
	Note that output ROOT files from this algorithm should end in "_calo.root" and that, the energies are preset (with 5 values).
*/

// === INCLUDES =======
	#include <iostream>
	#include <sstream>
	#include "TString.h"
	#include "TFile.h"
	#include "TH1D.h"
	#include "TF1.h"
	#include "TList.h"
	#include "TGraphErrors.h"
	#include "TSystem.h"
	#include "TSystemDirectory.h"
	#include "TSystemFile.h"
	#include "TCanvas.h"
	using namespace std;

// === MAIN FUNCTION =======
void calibration() {
	// Get directory and list of files 
	TSystemDirectory dir(".",".");
	TList* files = dir.GetListOfFiles();
	// Count number of TCaloAnalyseSpectrum output ROOT files
	Int_t n=0;
	TIter countit(files);
	TSystemFile* entry = NULL;
	while( (entry=(TSystemFile*)countit()) ) {
		TString name(entry->GetName());
		if(name.EndsWith("_calo.root")) ++n;
	}
	// Create TGraphError and x-values (actual energies)
	TGraphErrors* gr_sum = new TGraphErrors(n);
	TGraphErrors* gr_fit = new TGraphErrors(n);
	gr_sum->SetTitle("Beam energy vs calorimeter spectrum output from sum;beam energy (MeV);energy from spectrum fit (a.u.)");
	gr_fit->SetTitle("Beam energy vs calorimeter spectrum output from fit;beam energy (MeV);energy from spectrum fit (a.u.)");
	gr_sum->SetMarkerColor(4); gr_sum->SetMarkerStyle(21);
	gr_fit->SetMarkerColor(4); gr_fit->SetMarkerStyle(21);
	// Add points from TFiles
	Int_t i=0;
	TIter next(files);
	while( (entry=(TSystemFile*)next()) ) {
		TString name(entry->GetName());
		if(name.EndsWith("_calo.root")) {
			// Get fit from histogram in TFile
			TFile* file = new TFile(name.Data());
			TH1D* hist_sum = (TH1D*)file->Get("energy_sum");
			TH1D* hist_fit = (TH1D*)file->Get("energy_fit");
			TF1* fit_sum = (TF1*)hist_sum->GetListOfFunctions()->At(0);
			TF1* fit_fit = (TF1*)hist_fit->GetListOfFunctions()->At(0);
			// Write spectra without rescaling
			TCanvas* c = new TCanvas();
			hist_sum->Draw(); c->Update();
			gSystem->mkdir("sum_pdf"); gSystem->cd("sum_pdf");
			name.ReplaceAll("_calo.root","_sum.pdf"); c->SaveAs(name.Data());
			gSystem->cd(".."); gSystem->mkdir("sum_png"); gSystem->cd("sum_png");
			name.ReplaceAll("_sum.pdf",  "_sum.png"); c->SaveAs(name.Data());
			hist_fit->Draw(); c->Update();
			gSystem->cd(".."); gSystem->mkdir("fit_pdf"); gSystem->cd("fit_pdf");
			name.ReplaceAll("_sum.png","_fit.pdf"); c->SaveAs(name.Data());
			gSystem->cd(".."); gSystem->mkdir("fit_png"); gSystem->cd("fit_png");
			name.ReplaceAll("_fit.pdf","_fit.png"); c->SaveAs(name.Data());
			// Write 'normalised' spectra
			hist_sum->SetAxisRange(0,25,"Y");
			hist_fit->SetAxisRange(0,25,"Y");
			hist_sum->Draw(); c->Update();
			gSystem->cd(".."); gSystem->mkdir("sum_pdf25"); gSystem->cd("sum_pdf25");
			name.ReplaceAll("_fit.png",  "_sum25.pdf"); c->SaveAs(name.Data());
			gSystem->cd(".."); gSystem->mkdir("sum_png25"); gSystem->cd("sum_png25");
			name.ReplaceAll("_sum25.pdf","_sum25.png"); c->SaveAs(name.Data());
			hist_fit->Draw(); c->Update();
			gSystem->cd(".."); gSystem->mkdir("fit_pdf25"); gSystem->cd("fit_pdf25");
			name.ReplaceAll("_sum25.png","_fit25.pdf"); c->SaveAs(name.Data());
			gSystem->cd(".."); gSystem->mkdir("fit_png25"); gSystem->cd("fit_png25");
			name.ReplaceAll("_fit25.pdf","_fit25.png"); c->SaveAs(name.Data());
			gSystem->cd("..");
			delete c;
			// Get energy from file name
			name.ReplaceAll("CaloCalibration_","");
			name.ReplaceAll("MeV_fit25.png","");
			stringstream sstr(name.Data());
			Double_t energy;
			sstr >> energy;
			energy /= 10.;
			// Set graph point with error from parameters
			gr_sum->SetPoint(i,energy,fit_sum->GetParameter(1));
			gr_sum->SetPointError(i,0,fit_sum->GetParameter(2));
			gr_fit->SetPoint(i,energy,fit_fit->GetParameter(1));
			gr_fit->SetPointError(i,0,fit_fit->GetParameter(2));
			// Close input ROOT file
			file->Close();
			// Increase counter
			++i;
		}
	}
	// Fit functions
	gr_sum->Fit("pol2","","",0,150);
	gr_fit->Fit("pol2","","",0,150);
	// Draw graph in Canvas and save as png
	TCanvas* c = new TCanvas();
	gr_sum->Draw("AP");
	gr_sum->GetXaxis()->SetLimits(0,150);
	gr_sum->GetYaxis()->SetLimits(0,7e5);
	gr_sum->GetXaxis()->SetRangeUser(0,150);
	gr_sum->GetYaxis()->SetRangeUser(0,7e5);
	gr_sum->Draw("AP");
	c->Update();
	c->SaveAs("calibration_sum.png");
	c->SaveAs("calibration_sum.pdf");
	gr_fit->Draw("AP");
	gr_fit->GetXaxis()->SetLimits(0,150);
	gr_fit->GetYaxis()->SetLimits(0,7e5);
	gr_fit->GetXaxis()->SetRangeUser(0,150);
	gr_fit->GetYaxis()->SetRangeUser(0,7e5);
	gr_fit->Draw("AP");
	c->Update();
	c->SaveAs("calibration_fit.png");
	c->SaveAs("calibration_fit.pdf");
	delete c;
	// Release memory
	delete gr_sum;
	delete gr_fit;
}