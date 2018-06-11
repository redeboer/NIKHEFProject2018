// Author: Lizette Lamers
// Date: June 11th, 2018
// For NIKHEF Project 2018

/* === CLASS DESCRIPTION =======
	This script reads in the Geant4 output file and makes a data file which can be put in to the data analysis framework.
    There are two options,

    Errors = true, means tracks in the TPC's are reconstructed. A number of points is uniformly distributed on the track and random x,y and z errors are put on the TPC data points
    For each TPC and event a .txt is made where all the points are stored
    A seperate txt file is made that lists all the calorimeter energies for the whole run

    Errors = false, means data is perfect and has no measurement uncertainties.
    A track is reconstred for each TPC which consists of three entry points (x,y,z) and a unit vector giving the direction of the track
    A .txt file is made that stores the tracks of the two TPC's and the corresponding calorimeter energies.
*/

// === INCLUDES =======

#include <fstream>
#include <string>
#include <iostream>
#include "TRandom3.h"


void Fake_data_protons(){

    bool Errors = true;
    
    Double_t event; 
    Double_t ScThck, ScYZ, DiceThck, DiceY, DiceZ, PhThck, PhYZ, CalThck, CalYZ;
    Double_t pointx_1, pointy_1, pointz_1, dirx_1, diry_1, dirz_1, pointx_2, pointy_2, pointz_2, dirx_2, diry_2, dirz_2, eng_Cal;
    Double_t countEnSc, countExSc, countEnD1, countExD1, countEnPh, countExPh, countEnD2, countExD2, countEnCal, write_count;
    
    //Output Geant4 file to be read
    string file = "Output_Geant_water.txt";

    //If Errors = true, this file will be the calorimeter output file
    string Calofile = "Error_data_water/Calo_energy.txt";
    
    countEnSc = countExSc = countEnD1 = countExD1 = countEnPh = countExPh = countEnD2 = countExD2 = countEnCal = write_count = 0;
        
    ifstream iofile(file, ios::in|ios::out);
        
    if(Errors == false){
        //initialise desired output file and state format
        ofstream outfile ("test.txt");
        outfile << " event # || point x TPC1 [pixel] || point y TPC1 [pixel] || point z TPC1 [pixel] || dir x TPC1 || dir y TPC1 || dir z TPC1 || point x TPC2 [pixel] || point y TPC2 [pixel] || point z TPC2 [pixel] || dir x TPC2 || dir y TPC2 || dir z TPC2 || energy Calo [MeV]" << endl;
        outfile.close();
    }
    if(Errors == true){
        //initialise desired output calo file and state format
        ofstream outfile (Calofile);
        outfile << " event # || energy Calo [MeV]" << endl;
        outfile.close();
    }
    
    if (!iofile) cout<<"File does not exist"<<endl;	
    
    if (iofile)  cout << "Found file "  << endl;
        string line;  
        
    //first read in extra information from Geant4 file untill sizes decleration
    while (getline (iofile,line)){
        if(line == "----- Sizes declaration -----"){
            iofile >> ScThck >> ScYZ >> DiceThck >> DiceY >> DiceZ >> PhThck >> PhYZ >> CalThck >> CalYZ;
            break;
        }
    }   
    Double_t pixel_x = DiceThck/256; //size pixel in x-direction
    Double_t pixel_z = DiceZ/512;    //size pixel in z-direction
    Double_t number;                 
    Double_t total = 0; //total data points to be read
            
    for(Int_t i = 0; i < total; i++) {
        pointx_1 = pointy_1 = pointz_1 = dirx_1 = diry_1 = dirz_1 = pointx_2, pointy_2, pointz_2 = dirx_2 = diry_2 = dirz_2 = eng_Cal = 0.;
                    
        Double_t numbpoints_1 = int(gRandom->Gaus(125,10));
        Double_t Error_Array_1[int(numbpoints_1*3)];
        Double_t numbpoints_2 = int(gRandom->Gaus(125,10));
        Double_t Error_Array_2[int(numbpoints_2*3)];
                    
        iofile >> event;

        //somtimes when the particle does not hit the volume, no geometry boundary condition is satisfied and no value will be put in the output file
        //to make sure the file will still be read correctly it is checked if each new number is equal to next event number
        Double_t Array[35] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
                    
        for(Int_t k = 0; k < 35; k++){
            iofile>>number;
            if(number != event + 1.){
                Array[k] = number;
            }else{
                if(number < 10){
                    iofile.unget();
                }else if(number < 100){
                    iofile.unget();
                    iofile.unget();
                }else if(number < 1000){
                    iofile.unget();
                    iofile.unget();
                    iofile.unget();
                }else{
                    iofile.unget();
                    iofile.unget();
                    iofile.unget();
                    iofile.unget();
                }
                break;
            }
        }
                    
        //Sometimes when the particle does not hit the volume values are put in the output file but are equal to zero
        //All the if statements check if the particle goes trough all components and counts how many particle did not go through each component
        //count entered means it did not enter
        //count exited means it did enter but it did not hit the back surface

        if(Array[0] == 0 ){ //Scint not entering
            countEnSc += 1;
            countEnD1 += 1;
            countEnPh += 1;
            countEnD2 += 1;
            countEnCal += 1;
        }else if(Array[3] == 0 ){//Scint not exiting
            countEnSc += 1;
            countEnD1 += 1;
            countEnPh += 1;
            countEnD2 += 1;
            countEnCal += 1;
        }else{                        
            if(Array[6] == 0 ){ // TPC1 not entering
                    countEnD1 += 1;
                    countEnPh += 1;
                    countEnD2 += 1;
                    countEnCal += 1;
            }else if(Array[9] == 0 ){// TPC1 not exiting
                    countExD1 += 1;
                    countEnPh += 1;
                    countEnD2 += 1;
                    countEnCal += 1;
            }else{  
                    if(Errors == false){
                        Double_t dx = DiceThck;
                        Double_t dy = fabs(Array[10] - Array[7]);
                        Double_t dz = fabs(Array[11] - Array[8]);
                        dirx_1 = dx/(dx + dy + dz);
                        diry_1 = dy/(dx + dy + dz);
                        dirz_1 = dz/(dx + dy + dz);
                                
                        Array[7]  = int((Array[7]  + DiceY/2)/(pixel_z));   //y-in
                        Array[10] = int((Array[10] + DiceY/2)/(pixel_z));   //y-out
                        Array[8]  = int((Array[8]  + DiceZ/2)/(pixel_z));   //z-in
                        Array[11] = int((Array[11] + DiceZ/2)/(pixel_z));   //z-out
                                    
                        pointx_1 = 0.;
                        pointy_1 = Array[7];
                        pointz_1 = Array[8]; 
                                    
                    } else{
                                
                        Array[7]  = Array[7]  + DiceY/2;   //y-in
                        Array[10] = Array[10] + DiceY/2;   //y-out
                        Array[8]  = Array[8]  + DiceZ/2;   //z-in
                        Array[11] = Array[11] + DiceZ/2;   //z-out
                                
                        Double_t p0[4] = {Array[7],(Array[10]-Array[7])/(DiceThck),Array[8],(Array[11] - Array[8])/(DiceThck)};
                
                        Int_t count = 0;
                                    
                        // generate graph with the 3d points
                        for (Int_t N=0; N<numbpoints_1; N++) {
                                        
                            Double_t t = gRandom->Uniform(0,DiceThck);
                                    
                            Double_t y = p0[0] + p0[1]*t;
                            Double_t z = p0[2] + p0[3]*t;
                            Double_t y_cm = (p0[0] + p0[1]*t)*10.;
                            Double_t x = t; 
                                    
                            Double_t Dl = 83.187; //micro meter/square root(cm)
                            Double_t Dt = 71.9395; // micro meter/square root(cm)
                                    
                            Double_t sigma_l = (Dl*sqrt(y_cm))/1000.; //mili meters
                            Double_t sigma_T = (Dt*sqrt(y_cm))/1000.; //mili meters
                                    
                            // do a gaussian smearing around the points in all coordinates
                            x += gRandom->Gaus(0,sigma_T);
                            y += gRandom->Gaus(0,sigma_l);
                            z += gRandom->Gaus(0,sigma_T);
                                        
                            if(x < 0) x = 0.0;
                            if(y < 0) y = 0.0;
                            if(z < 0) z = 0.0;
                                    
                            x = int(x/pixel_x);
                            y = int(y/pixel_z);
                            z = int(z/pixel_z);
                                        
                            Error_Array_1[count] = x;
                            count += 1;
                            Error_Array_1[count] = y;
                            count += 1;
                            Error_Array_1[count] = z;
                            count += 1;
                        }
                                    
                                    
                    }
                                
                                
                if(Array[12] == 0 ){// Phantom not entering
                        countEnPh += 1;
                        countEnD2 += 1;
                        countEnCal += 1;
                }else if(Array[15] == 0 ){ // Phantom not exiting
                        countExPh += 1;
                        countEnD2 += 1;
                        countEnCal += 1;
                }else{                                
                    if(Array[18]== 0 ){// TPC2 not entering
                            countEnD2 += 1;
                            countEnCal += 1;
                    }else if(Array[21] == 0 ){ // TPC2 not exiting
                            countExD2 += 1;
                            countEnCal += 1;
                    }else{  
                        if(Errors == false){
                                        
                            Double_t dx = DiceThck;
                            Double_t dy = fabs(Array[22] - Array[19]);
                            Double_t dz = fabs(Array[23] - Array[20]);
                            dirx_2 = dx/(dx + dy + dz);
                            diry_2 = dy/(dx + dy + dz);
                            dirz_2 = dz/(dx + dy + dz);
                                    
                            Array[19] = int((Array[19] + DiceY/2)/(pixel_z));   //y-in
                            Array[22] = int((Array[22] + DiceY/2)/(pixel_z));   //y-out
                            Array[20] = int((Array[20] + DiceZ/2)/(pixel_z));   //z-in
                            Array[23] = int((Array[23] + DiceZ/2)/(pixel_z));   //z-out
                                
                            pointx_2 = int((DiceThck + PhThck + 50)/(pixel_x));
                            pointy_2 = Array[19];
                            pointz_2 = Array[20];
                                        
                        } else{
                                        
                            Array[19]  = Array[19]  + DiceY/2;   //y-in
                            Array[22]  = Array[22] + DiceY/2;    //y-out
                            Array[20]  = Array[20]  + DiceZ/2;   //z-in
                            Array[23]  = Array[23] + DiceZ/2;    //z-out
                                
                            Double_t p0[4] = {Array[19],(Array[22]-Array[19])/(DiceThck),Array[20],(Array[23] - Array[20])/(DiceThck)};
                    
                            Int_t count = 0;
                                        
                            // generate graph with the 3d points
                            for (Int_t N=0; N<numbpoints_2; N++) {
                                            
                                Double_t t = gRandom->Uniform(0,DiceThck);
                                    
                                Double_t y = p0[0] + p0[1]*t;
                                Double_t z = p0[2] + p0[3]*t;
                                Double_t y_cm = (p0[0] + p0[1]*t)*10.;
                                Double_t x = t; 
                                    
                                Double_t Dl = 83.187; //micro meter/square root(cm)
                                Double_t Dt = 71.9395; // micro meter/square root(cm)
                                    
                                Double_t sigma_l = (Dl*sqrt(y_cm))/1000.; //mili meters
                                Double_t sigma_T = (Dt*sqrt(y_cm))/1000.; //mili meters
                                    
                                // do a gaussian smearing around the points in all coordinates
                                x += gRandom->Gaus(0,sigma_T);
                                y += gRandom->Gaus(0,sigma_l);
                                z += gRandom->Gaus(0,sigma_T);
                                            
                                if(x < 0) x = 0.0;
                                if(y < 0) y = 0.0;
                                if(z < 0) z = 0.0;
                                    
                                x = int(x/pixel_x);
                                y = int(y/pixel_z);
                                z = int(z/pixel_z);
                                            
                                Error_Array_2[count] = x;
                                count += 1;
                                Error_Array_2[count] = y;
                                count += 1;
                                Error_Array_2[count] = z;
                                count += 1;
                            }
                        }
                                    
                        if(Array[24] == 0 ){ // Calo not entering
                                countEnCal += 1;
                        }else{
                                eng_Cal = Array[34];
                                if(Errors == false){
                                    fstream filestr;
                                    filestr.open ("test.txt", fstream::in | fstream::out  | fstream::app);
                                                
                                    filestr <<write_count<<" "<<pointx_1<<" "<<pointy_1<<" "<<pointz_1<<" "<<dirx_1<<" "<<diry_1<<" "<<dirz_1<<" "<<pointx_2<<" "<<pointy_2<<" "<<pointz_2<<" "<<dirx_2<<" "<<diry_2<<" "<<dirz_2<<" "<<eng_Cal<<endl;
                                    write_count += 1;
                                    filestr.close();
                                                
                                } else{
                                    string text_1 = "Error_data_water/Event_";
                                    text_1 += to_string(int(write_count));
                                    text_1.append("_cam_1.txt");
                                                
                                    ofstream outfile (text_1);
                                                
                                    Int_t count = 0;
                                                
                                    for(Int_t N = 0; N < numbpoints_1; N++){
                                                
                                        outfile << Error_Array_1[count] << " ";
                                        count += 1;
                                        outfile << Error_Array_1[count] << " ";
                                        count += 1;
                                        outfile << Error_Array_1[count] <<endl;
                                        count += 1;
                                    }
                                                
                                    outfile.close();
                                                
                                    string text_2 = "Error_data_water/Event_";
                                    text_2 += to_string(int(write_count));
                                    text_2.append("_cam_2.txt");
                                                
                                    ofstream outfile_2 (text_2);
                                                
                                    count = 0;
                                                
                                    for(Int_t N = 0; N < numbpoints_2; N++){
                                                
                                        outfile_2 << Error_Array_2[count] << " ";
                                        count += 1;
                                        outfile_2 << Error_Array_2[count] << " ";
                                        count += 1;
                                        outfile_2 << Error_Array_2[count] <<endl;;
                                        count += 1;
                                    }
                                                
                                    outfile_2.close();
                                                
                                    fstream filestr_2;
                                    filestr_2.open (Calofile, fstream::in | fstream::out  | fstream::app);
                                                
                                    filestr_2 <<write_count<<" "<<eng_Cal<<endl;
                                    filestr_2.close();
                                                
                                                
                                    write_count += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
            
        cout<<"Percentage not entering  Scint: "<<(countEnSc/total)*100<<" %"<<endl;
        cout<<"Percentage not exiting   Scint:  "<<(countExSc/total)*100<<" %"<<endl;
        cout<<"Percentage not entering  TPC1: "<<(countEnD1/total)*100<<" %"<<endl;
        cout<<"Percentage not exiting   TPC1:  "<<(countExD1/total)*100<<" %"<<endl;
        cout<<"Percentage not entering  Phan: "<<(countEnPh/total)*100<<" %"<<endl;
        cout<<"Percentage not exiting   Phan:  "<<(countExPh/total)*100<<" %"<<endl;
        cout<<"Percentage not entering  TPC2: "<<(countEnD2/total)*100<<" %"<<endl;
        cout<<"Percentage not exiting   TPC2:  "<<(countExD2/total)*100<<" %"<<endl;
        cout<<"Percentage not entering  Calo: "<<(countEnCal/total)*100<<" %"<<endl;
            
            
            
             
    
    

    
 
 
 
 
 
 
 
 

 
 
 
 
 
 
 
 


}