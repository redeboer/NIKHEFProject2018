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
        int** values_new = new int*[Npoint];
        
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
        
        //Check if last event is fine
        if(file_in.count%Npoint!=0){
		cout << "The last event is not full! Throw it away!" << endl;
		file_in.nevents=file_in.nevents-1;
	}
	
	ifstream in2;
	in2.open(filename.c_str());
        
        static const int NEvents = file_in.nevents;
        
        for(int i = 0; i < Npoint; ++i)
            file_in.values_new[i] = new int[NEvents];
        
        read_lines = 7;
        for(Int_t j = 0; j < file_in.nevents; j++){
            for(Int_t s = 0; s < read_lines; s++){ 
                getline (in2,line);
            }
            
            for(Int_t i = 0; i <Npoint; i++){
                in2 >> c;
                file_in.values_new[i][j] = c;
                
                read_lines = 8;
            }
        }

        in2.close();
	
	return file_in;
};



//Fitting as longitudinal shower profile: E_0*t^a*e^(-bt)
Double_t Fit_shower(const double *xx, const double *par){       
	const Double_t x = xx[0];
	return par[0]*TMath::Power(x,par[1])*TMath::Exp(-par[2]*x);
};

//function for finding minimum in array
Double_t FindMinArr(Double_t Array[], int size){
    Double_t small = Array[0];
    for(int i=1; i<size; i++){
        if(Array[i]<small) small=Array[i];
    }
    return small;
};
 
//function for finding maximum in array
Double_t FindMaxArr(Double_t Array[], int size){
    Double_t big = Array[0];
    for(int i=1; i<size; i++){
        if(Array[i]>big) big=Array[i];
    }
    return big;
};


int CALO(){
	File result = File_open();
        

	TH2D *histo[result.nevents];
	Double_t Energy_fit[result.nevents];
        Double_t Energy_add[result.nevents];

	vector<int>::iterator max_value = max_element(result.values.begin(), result.values.end()); //find maximum in vector

	Int_t Max = result.values[distance(result.values.begin(), max_value)]; //maximum value of voltage from all event - use for histogram



	TCanvas *c1 = new TCanvas();
	c1->Divide(result.nevents);
        char *histname = new char[20];
        

	//fitting
	const Int_t npar = 3;
	Double_t f1params[npar] = {1, 0, 0};			//fit parameters
	TF1 *f1 = new TF1("f1", Fit_shower, 0, Npoint, npar);   //define fitting function
	f1->SetParameters(f1params);				//set fit parameters
        
	

	for(int j =0; j<result.nevents; j++){
                Energy_add[j] = 0;
                sprintf(histname, "event_%d", j);
                
		c1->cd(j);
		histo[j] = new TH2D("histname", "", Npoint, 0, Npoint, Max, 0, Max );
		for(int i =0; i<Npoint; i++){
                        int start_value = result.values_new[0][j];
                        histo[j] -> Fill(i+1,(result.values_new[i][j]- start_value)*-1);			//fill with flipped around time axis wave
                        
                        if((result.values_new[i][j]- start_value)*-1 > 5){
                            Energy_add[j] += (result.values_new[i][j]- start_value)*-1;
                            
                        }
                }

		//histo[j] -> GetXaxis()->SetTitle("timesteps");
		//histo[j] -> GetYaxis()->SetTitleOffset(1.3);
		//histo[j] -> GetYaxis()->SetTitle("Counts");
		//histo[j] -> Draw();
		histo[j] -> Fit("f1","Q");
		
		double NDF = f1->GetNDF();
		double Chi = f1 ->GetChisquare(); 
                
                double par1 = f1->GetParameter(0);
                double par2 = f1->GetParameter(1);
                double par3 = f1->GetParameter(2);	

		Energy_fit[j] = f1->Integral(0, Npoint);		//integrate over fitted wave to get energy in a.u.
                cout<<"Energy_fit: "<<Energy_fit[j]<<endl;
                cout<<"Energy_add: "<<Energy_add[j]<<endl;
                //cout<<"Percentage difference: "<<(1-Energy_add[j]/Energy_fit[j])*100<<" %"<<endl;
                
                //cout<<j<<" out of "<<result.nevents<<endl;
                
		delete histo[j];				//delete histogram to prevent memory leak

	}
	
	
	//find min,max energy for spectra
	Int_t min_energy=FindMinArr(Energy_fit, result.nevents);
	//cout <<"Minimum Energy: " << min_energy << endl;
	Int_t max_energy=FindMaxArr(Energy_fit, result.nevents);
	//cout <<"Maximum Energy: " << max_energy << endl;

	TCanvas *c2 = new TCanvas();
	c2->cd();

	char spectrum_name[20];
	sprintf(spectrum_name, "noevents_%d", result.nevents);

	TH1D *spectra = new TH1D(spectrum_name, "", 100, min_energy-1, max_energy+1);
 
	for(int i =0; i < result.nevents; i++){
		spectra -> Fill(Energy_fit[i]);

	}
	spectra -> GetXaxis() -> SetTitle("Energy, a.u.");
	spectra -> GetYaxis() -> SetTitle("counts");
	spectra->SetMarkerStyle(33);
	spectra->Draw();
        
        result.values.clear();
        delete c1;
        delete histname;
        delete result.values_new;

	return 0;

}


