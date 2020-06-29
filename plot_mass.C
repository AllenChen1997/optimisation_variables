/////////////////////////////////////////////////
/*
   this version only do the n2b1 first
*/
////////////////////////////////////////////////

#include "N2_study.C"
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
#include <TLine.h>
// make sure the variables are the same in the N2_study.C
#define Maxpt 2000
#define NN2 28
#define MinN2 -0.2
#define MaxN2 0.5
#define Nrho 20
#define Minrho -6
#define Maxrho -1

#define NMass 40
#define MinMass 100
#define MaxMass 500

#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800
//
using namespace std;
vector<double> v_cut;
TH2D* h_5;
TH2D* h_20;
TH2D* h_26;
TH2D* h_50;
TH1D* h_n2[4];
TH1D* h_n2ddt[4];
TH1D* h_total = new TH1D("","h_total",NMass,MinMass,MaxMass);
void load_to_hist(string s){
	TFile* fin = new TFile(s.c_str(),"READ");
	TTreeReader finRead("monoHbb_SR_boosted",fin);
	TTreeReaderValue< Double_t > n2b1(finRead,"FJetN2b1");
	TTreeReaderValue< Double_t > rho(finRead,"FJetrho");
	TTreeReaderValue< Double_t > mass(finRead,"FJetMass");
	TTreeReaderValue< Double_t > pt(finRead,"FJetPt");
	while(finRead.Next()){
		// for N2 cut //
		int x = -1;
		int y = -1;
		if (*rho > Maxrho || *rho < Minrho) {
			//for(int i=0;i<4;i++) h_n2ddt[i]->Fill(*mass);
			continue;
		} else {
			x = ceil((double)(*rho - Minrho) / d );
			if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
			else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
			else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
			else if (*pt >= pt_r4) y = 4;
			else continue;
			if (*n2b1 - h_5->GetBinContent(x,y) < 0) h_n2ddt[0]->Fill(*mass);
			if (*n2b1 - h_20->GetBinContent(x,y) < 0) h_n2ddt[1]->Fill(*mass);
			if (*n2b1 - h_26->GetBinContent(x,y) < 0) h_n2ddt[2]->Fill(*mass);
			if (*n2b1 - h_50->GetBinContent(x,y) < 0) h_n2ddt[3]->Fill(*mass);
		}
		h_total->Fill(*mass);
		for (int i=0;i<4;i++){
			if (*n2b1<v_cut[i]) h_n2[i]->Fill(*mass);
		}
	}
}

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9

void plot_mass(){
	string per[4] = {"5% select.","20% select.","26% select.","50% select."};
	
	TFile* myfile = new TFile("TH3_output.root","READ");
	TTreeReader myRead("tree",myfile);  
	TTreeReaderValue<vector<double>> n2b1_cut(myRead,"n2b1_cut");
	h_5 = (TH2D*) myfile->Get("h_pt_rho_5");
	h_20 = (TH2D*) myfile->Get("h_pt_rho_20");
	h_26 = (TH2D*) myfile->Get("h_pt_rho_26");
	h_50 = (TH2D*) myfile->Get("h_pt_rho_50");
	while(myRead.Next()){		
		for (auto x : *n2b1_cut) v_cut.push_back(x);
	}

	
	//load signal and do the plot

	for (int i=0;i<4;i++){
		h_n2[i] = new TH1D("",Form("h_n2_%i",i),NMass,MinMass,MaxMass);
		h_n2ddt[i] = new TH1D("",Form("h_n2ddt_%i",i),NMass,MinMass,MaxMass);
	}

	
	ifstream infile("QCD_list.txt");
	string line;
	while(getline(infile,line)){
		load_to_hist(line);
	}
	
	gStyle->SetOptStat("");	
	
	auto c1 = new TCanvas("c1","c1");
	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	h_total->SetLineColor(kBlack);
	h_total->Scale(1.0/h_total->Integral());
	h_total->SetTitle("N2 cut");
	h_total->SetTitleSize(0.07);
	h_total->SetXTitle("FJetMass[GeV]");
	h_total->GetXaxis()->SetTitleSize(0.05);
	h_total->GetXaxis()->SetLabelSize(0.05);
	h_total->GetYaxis()->SetLabelSize(0.05);
	
	//h_total->SetAxisRange(0, 0.2, "Y");
	h_total->Draw("SAME HIST");
	legend->AddEntry(h_total,"inclusive");
	for (int i=3;i>=0;i--){
		h_n2[i]->SetLineColor(kRed+i);
		h_n2[i]->Scale(1.0/h_n2[i]->Integral());
		h_n2[i]->Draw("SAME HIST");
		legend->AddEntry(h_n2[i],Form("%s",per[i].c_str()),"l");
	}

	legend->Draw();
	c1->SaveAs("n2cut_mass.png");
	
	auto c2 = new TCanvas("c2","c2");
	h_total->SetTitle("N2DDT cut");
	h_total->Draw("SAME HIST");
	for (int i=3;i>=0;i--){
		h_n2ddt[i]->SetLineColor(kRed+i);
		h_n2ddt[i]->Scale(1.0/h_n2ddt[i]->Integral());
		h_n2ddt[i]->Draw("SAME HIST");
	}
	
	legend->Draw();
	c2->SaveAs("n2ddt_mass.png");
}
