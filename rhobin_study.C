///////////////////////////
/*
	this is used to study the effect of the Nrho bins

*/
//////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH3D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>

#define Maxpt 2000
#define NN2 20
#define MinN2 0
#define MaxN2 0.5
#define Nrho 16
#define Minrho -5
#define Maxrho -1
// pt bin region [200, 350, 480,1000]
#define pt_r1 200
#define pt_r2 350
#define pt_r3 480
#define pt_r4 1000
#define Npt 3
using namespace std;

double d = (double) (Maxrho - Minrho);  // this is the width of the rho bin

void rhobin_study(){
	// input file data
	int i=0; // lowerN is the number of which pt lower than 0, overN is the number of which pt > Maxpt
	TFile* myfile = new TFile("nbin_10.root","READ");
	TTreeReader myRead("tree",myfile);  
	TTreeReaderValue<vector<double>> n2b1(myRead,"noPt_n2b1");
	TTreeReaderValue<vector<double>> n2b2(myRead,"noPt_n2b2");
	double num1[10] = {0.0}; // use this to decide the data in which rho region 
	double num2[10] = {0.0};
	double rho[10] = {0.0};
	while (myRead.Next()){  // loop in one root file
		int i=0;
		for (auto x : *n2b1){
			num1[i] = x;
			rho[i] = -5+(1+2*i)*(double)2/(int)n2b1->size() ;
			i++;
		}
		i=0;
		
		for (auto x : *n2b2){
			num2[i] = x;
			i++;
		}

	}
	
	TFile* myfile2 = new TFile("nbin_16.root","READ");
	TTreeReader myRead2("tree",myfile2);  
	TTreeReaderValue<vector<double>> n2b1_2(myRead2,"noPt_n2b1");
	TTreeReaderValue<vector<double>> n2b2_2(myRead2,"noPt_n2b2");
	double num1_2[16] = {0.0}; // use this to decide the data in which rho region 
	double num2_2[16] = {0.0};
	double rho_2[16] = {0.0};
	while (myRead2.Next()){  // loop in one root file
		int i=0;
		for (auto x : *n2b1_2){
			num1_2[i] = x;
			rho_2[i] = -5+(1+2*i)*(double)2/(int)n2b1_2->size() ;
			i++;
		}
		i=0;
		
		for (auto x : *n2b2_2){
			num2_2[i] = x;
			i++;
		}

	}
	
	TFile* myfile3 = new TFile("nbin_22.root","READ");
	TTreeReader myRead3("tree",myfile3);  
	TTreeReaderValue<vector<double>> n2b1_3(myRead3,"noPt_n2b1");
	TTreeReaderValue<vector<double>> n2b2_3(myRead3,"noPt_n2b2");
	double num1_3[22] = {0.0}; // use this to decide the data in which rho region 
	double num2_3[22] = {0.0};
	double rho_3[22] = {0.0};
	while (myRead3.Next()){  // loop in one root file
		int i=0;
		for (auto x : *n2b1_3){
			num1_3[i] = x;
			rho_3[i] = -5+(1+2*i)*(double)2/(int)n2b1_3->size() ;
			i++;
		}
		i=0;
		
		for (auto x : *n2b2_3){
			num2_3[i] = x;
			i++;
		}

	}

	// output some plots
	TGraph* g1 = new TGraph( (int)n2b1->size(),rho, num1	);
	TGraph* g2 = new TGraph( (int)n2b1->size(),rho, num2	);
	TGraph* g3 = new TGraph( (int)n2b1_2->size(),rho_2, num1_2);
	g3->SetMarkerColor(4);
	TGraph* g4 = new TGraph( (int)n2b1_2->size(),rho_2, num2_2);
	g4->SetMarkerColor(4);
	TGraph* g5 = new TGraph( (int)n2b1_3->size(),rho_3, num1_3);
	g5->SetMarkerColor(6);
	TGraph* g6 = new TGraph( (int)n2b1_3->size(),rho_3, num2_3);
	g6->SetMarkerColor(6);
	TMultiGraph  *mg  = new TMultiGraph();
	mg->Add(g1);
	mg->Add(g3);
	mg->Add(g5);
	mg->Draw("ap *");
	TLegend* legend = new TLegend(0.8,0.8,1.0,1.0);
	legend->AddEntry(g1,"bins = 10","ap");
	legend->AddEntry(g3,"bins = 16","ap *");
	legend->AddEntry(g5,"bins = 22","ap *");
	legend->Draw();
	auto c2 = new TCanvas("c2","c2");
	TMultiGraph *mg2 = new TMultiGraph();
	mg2->Add(g2);
	mg2->Add(g4);
	mg2->Add(g6);
	mg2->Draw("ap * ");
	legend->Draw();
}
