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
#define Nrho 16
#define Minrho -5
#define Maxrho -1

//double d = (double) (Maxrho - Minrho) / (double)Nrho;  // this is the width of the rho bin in N2_study.C
//
using namespace std;
//string sig_root="/eos/cms/store/group/phys_exotica/monoHiggs/monoHbb/Analyser_Outputs/N2_N3_Study/EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1600_MH4_150_MH2_1600_MHC_1600_CP3Tune_13TeV_0000_0.root";

string sig_root="/eos/cms/store/group/phys_exotica/monoHiggs/monoHbb/Analyser_Outputs/N2_N3_Study/EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV_0000_0.root";

//string s1 = "n2b1_v26";
//string s2 = "n2b2_v26";

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9

void plot_mass(){
	TH3D* h_sig = new TH3D("signal_3D","signal",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt);
	//TH3D* h_sig_b2 = (TH3D*) h_sig_b1->Clone("signal_n2b2");
	string per[4] = {"5% select.","20% select.","26% select.","50% select."};
	vector<double> v5,v20,v26,v50,v_cut;
	
	TFile* myfile = new TFile("TH3_output.root","READ");
	TTreeReader myRead("tree",myfile);  
	TTreeReaderValue<vector<double>> n2b1_5(myRead,"n2b1_v5");
	TTreeReaderValue<vector<double>> n2b1_20(myRead,"n2b1_v20");
	TTreeReaderValue<vector<double>> n2b1_26(myRead,"n2b1_v26");
	TTreeReaderValue<vector<double>> n2b1_50(myRead,"n2b1_v50");
	TTreeReaderValue<vector<double>> n2b1_cut(myRead,"n2b1_cut");
	while(myRead.Next()){		
		for (auto x : *n2b1_5) v5.push_back(x);
		for (auto x : *n2b1_20) v20.push_back(x);
		for (auto x : *n2b1_26) v26.push_back(x);
		for (auto x : *n2b1_50) v50.push_back(x);
		for (auto x : *n2b1_cut) v_cut.push_back(x);
	}

	int N = 0, count = 0;
	
	//load signal and do the plot
	TH1D* h_n2[4];
	TH1D* h_n2ddt[4];
	for (int i=0;i<4;i++){
		h_n2[i] = new TH1D("",Form("h_n2_%i",i),50,100,200);
		h_n2ddt[i] = new TH1D("",Form("h_n2ddt_%i",i),50,100,200);
	}
	TH1D* h_total = new TH1D("","h_total",50,100,200);
	
	TFile* fin = new TFile(sig_root.c_str(),"READ");
	TTreeReader finRead("monoHbb_SR_boosted",fin);
	TTreeReaderValue< Double_t > n2b1(finRead,"FJetN2b1");
	TTreeReaderValue< Double_t > rho(finRead,"FJetrho");
	TTreeReaderValue< Double_t > mass(finRead,"FJetMass");
	while(finRead.Next()){
		// for N2 cut //
		for (int i=0;i<4;i++){
			if (*n2b1<v_cut[i]) h_n2[i]->Fill(*mass);
		}
		int rho_region = floor((double)(*rho - Minrho) / d );
		if (*n2b1 - v5[rho_region] < 0) h_n2ddt[0]->Fill(*mass);
		if (*n2b1 - v20[rho_region] < 0) h_n2ddt[1]->Fill(*mass);
		if (*n2b1 - v26[rho_region] < 0) h_n2ddt[2]->Fill(*mass);
		if (*n2b1 - v50[rho_region] < 0) h_n2ddt[3]->Fill(*mass);
		h_total->Fill(*mass);
	}

	gStyle->SetOptStat("");	
	
	auto c1 = new TCanvas("c1","c1");
	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	h_total->SetLineColor(kBlack);
	h_total->Scale(1.0/h_total->Integral());
	h_total->SetTitle("N2");
	h_total->SetXTitle("FJetmass[GeV]");
	h_total->SetAxisRange(0, 0.2, "Y");
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
	h_total->SetTitle("N2ddt");
	h_total->Draw("SAME HIST");
	for (int i=3;i>=0;i--){
		h_n2ddt[i]->SetLineColor(kRed+i);
		h_n2ddt[i]->Scale(1.0/h_n2ddt[i]->Integral());
		h_n2ddt[i]->Draw("SAME HIST");
	}
	
	legend->Draw();
	c2->SaveAs("n2ddt_mass.png");
}
