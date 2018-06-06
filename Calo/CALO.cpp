#include <iostream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <cmath>

#include "TObject.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TList.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TF1.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TImage.h"


using namespace std;

static const int Npoint = 1024;				//global variable: number of points per event
string trigger_time = "Trigger Time Stamp: ";   //global variable: trigger time stamp header
const string filename = "wave2_calotrig.txt";     //global variable: file name
		
//define structure to store information from file
struct File{
	vector<int> values; //voltage
	vector<long> timesteps;
        
	Int_t count;        //number of points
	Int_t nevents;      //number of triggered events
	
	//Int_t trigger[nevents];
};



//function to open a file and read all needed information
File File_open(){

	ifstream in;
	in.open(filename.c_str());

	if (!in.is_open()){					//check if file is open
	   cout << "Couldn't find a file!" << endl;
	   }
        else{
            cout<< "File found! :D "<<endl;
        }
   

	Int_t c;

 //expected number of points per event

	File file_in;
	file_in.count=0;
	file_in.nevents=0;
        string line;
        
        Int_t read_lines = 5;

   	while(in.good()){   
            for(Int_t s = 0; s < read_lines; s++){ 
                getline (in,line);
            }
                long stamp;
                in >> line;
                in >> line;
                in >> line;
                
		in >> stamp;
		file_in.timesteps.push_back(stamp);
                
                getline (in,line);
                getline (in,line);

		for(int i=0; i<Npoint; i++){
   			in >> c;
                        file_in.values.push_back(c); //puts value in vector
                        
                        file_in.count++;             //ups count
		}
		file_in.nevents++;                   //ups number of events
                read_lines = 6;

   	}
   	
   	in.close();
   	cout<<"number of events: "<<file_in.nevents<<endl;
	
	return file_in;
};



//Fitting as longitudinal shower profile: E_0*t^a*e^(-bt)
Double_t Fit_shower(const double *xx, const double *par){       
	const Double_t x = xx[0];
	return par[0]*TMath::Power(x,par[1])*TMath::Exp(-par[2]*x);
};


int CALO(){
	File result = File_open();
        
//Check if last event is fine
        if(result.count%Npoint!=0){
		cout << "The last event is not full! Throw it away!" << endl;
		result.nevents=result.nevents-1;
	}
	
	ifstream in2;
	in2.open(filename.c_str());
        
        static const int NEvents = result.nevents;
	
	int values_new[NEvents][Npoint];
        int c;
        string line;
        
        Int_t read_lines = 7;
        //for(Int_t j = 0; j < result.nevents; j++){
        //    for(Int_t s = 0; s < read_lines; s++){ 
        //        getline (in2,line);
        //    }
            
        //    for(Int_t i = 0; i <Npoint; i++){
        //        in2 >> c;
                values_new[1][1] = 1;
                
                read_lines = 8;
        //    }
        //}

        in2.close();
	cout<<"Did this please please please please work?"<<endl;
	
	


	//TH2D *histo[result.nevents];
	//Double_t timesteps[Npoint];
	
	//Double_t Energy[result.nevents];

	//vector<int>::iterator max_value = max_element(result.values.begin(), result.values.end()); //find maximum in vector
        
	//cout << "Value max: "<< result.values[distance(result.values.begin(), max_value)] << endl;
	/*
	vector<int>::iterator min_value = min_element(result.values.begin(), result.values.end());
	cout << "Value min"<< result.values[distance(result.values.begin(), min_value)] << endl;
	*/


	//Int_t Max = result.values[distance(result.values.begin(), max_value)]; //maximum value of voltage from all event - use for histogram
	
	
	

	//TList *l = new TList();



	//TCanvas *c1 = new TCanvas();
	//c1->Divide(result.nevents);
        
        
        
        

	//char *histname = new char[20];
        

	//fitting
	//const Int_t npar = 3;
	//Double_t f1params[npar] = {1, 0, 0};			//fit parameters
	//TF1 *f1 = new TF1("f1", Fit_shower, 0, Npoint, npar);   //define fitting function
	//f1->SetParameters(f1params);				//set fit parameters

	//int k=0;		//iterator for vector
	

	//for(int j =0; j<result.nevents; j++){
        
        //for(int j =0; j<1; j++){
            
		//sprintf(histname, "event_%d", j);
                
		//c1->cd(j);
		//histo[j] = new TH2D(histname, "", Npoint, 0, Npoint, Max, 0, Max );
                //cout<<"------REACHED HERE!!------"<<endl;

		//for(int i =0; i<Npoint; i++){
                //for(int i = 0; i< 1; i++){
                //	values_new[j][i]=result.values[k];
		//	cout << values_new[j][i] << " " << result.values[k] << endl;
		//	histo[j] -> Fill(i + 1,(-1)*result.values[k]+values_new[j][0]);			//fill with flipped around time axis wave
		//	k++;
                //}

	//	cout << histname << " is filled" << endl;
		//cout << "Iteration number: " << j << endl;
		//cout << "Number of points rewritten: "<< k << endl;
		//histo[j] -> GetXaxis()->SetTitle("timesteps");
		//histo[j] -> GetYaxis()->SetTitleOffset(1.3);
		//histo[j] -> GetYaxis()->SetTitle("Counts");
	//	histo[j] -> Draw();
	//	histo[j] -> Fit("f1");
		//histo[j] -> Fit("gaus");
		//histo[j] -> Fit("landau");
		
		//histo[j] -> Fit("gaus");  //overfitting
		//TF1 *f = histo[j]->GetFunction("pol6");
		//double NDF = f->GetNDF();
	//	double Chi = f1 ->GetChisquare(); //histo[j]->GetFunction("gaus")
	//	cout << Chi << endl;
		//double prob = f->GetProb();
 		//TLegend *leg = new TLegend(0.1, 0.5, 0.4, 0.9);
		//char *fitname = new char[20];
		//sprintf(fitname,"#chi^{2}=%f", Chi);
		
		//leg -> AddEntry(histo[j], fitname, "l");
		//leg -> Draw();
		//cout << f1->GetParameter(0) << endl;
		//cout << f1->GetParameter(1) << endl;
		//cout << f1->GetParameter(2) << endl;
	

	//	Energy[j] = f1->Integral(0, Npoint);		//integrate over fitted wave to get energy in a.u.
        //        cout<<Energy[j]<<endl;
	//	delete histo[j];				//delete histogram to prevent memory leak
		//delete f;
		//sprintf(histname, "event_%d.png", j);
		//histo[j] -> SaveAs(histname);
		//l -> Add(histo[j]);

	//}


/*TImage *img = TImage::Create();
img -> FromPad(c1);
img -> WriteImage("results.png");

delete img;
delete c1;

TFile *CALO_results = new TFile("Calo_results.root", "recreate");
CALO_results-> WriteObject(l,"Calo_results", TObject::kSingleKey);
CALO_results -> ls();
*/

	//delete canvas and clear vector
	//c1->Close();
	//delete c1;
//	result.values.clear();
	
	//find min,max energy for spectra
//	Int_t min_energy=FindMinArr(Energy, result.nevents);
	//cout <<"Minimum Energy: " << min_energy << endl;
//	Int_t max_energy=FindMaxArr(Energy, result.nevents);
	//cout <<"Maximum Energy: " << max_energy << endl;

//	TCanvas *c2 = new TCanvas();
//	c2->cd();

//	char spectrum_name[20];
//	sprintf(spectrum_name, "noevents_%d", result.nevents);

//	TH1D *spectra = new TH1D(spectrum_name, "", 100, min_energy-1, max_energy+1);
 
//	for(int i =0; i < result.nevents; i++){
//		spectra -> Fill(Energy[i]);

//	}
//	spectra -> GetXaxis() -> SetTitle("Energy, a.u.");
//	spectra -> GetYaxis() -> SetTitle("counts");
//	spectra->SetMarkerStyle(33);
//	spectra->Draw();

        //delete c1;
        //delete histname;


	return 0;

}


