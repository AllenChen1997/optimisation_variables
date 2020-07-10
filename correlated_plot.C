/////////////////////////////////////////////////
/*
   this code is used to generate 2D correlated plots
*/
////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TMath.h>
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
#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800
#define Nddb 40
#define Minddb 0
#define Maxddb 4
//
using namespace std;
string filesdir = "/afs/cern.ch/work/k/kuchen/public/samples/";
string sig_root= filesdir+"EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV.root";
string tt_semi= filesdir+"crab_TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root";
string tt_LL = filesdir+"crab_TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8.root";
string tt_had = filesdir+"crab_TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.root";
string s1 = "h_pt_rho_20";
double d = (double)(Maxrho-Minrho)/(double)Nrho; 

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9
#define Lumi 41000 // 1/pb

void load_to_hist_bkg(string s , double xsbkg, TH2D* h_n2, TH2D* h_n2ddt){
	cout << "Reading " << s << endl;
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
	TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
	TTreeReaderValue< Double_t > nj(myRead,"nJets");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	TTreeReaderValue< Double_t > N2DDT(myRead,"N2DDT");
	TH2D* h_n2_tmp = (TH2D*) h_n2->Clone("");
	TH2D* h_n2ddt_tmp = (TH2D*) h_n2ddt->Clone("");
	h_n2_tmp->Reset(); h_n2ddt_tmp->Reset();
	int n = 0;
	while (myRead.Next()){  // loop in one root file
		if (*dphi < 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		if (*ddb < 0.86 ) continue;
		//if (*nj > 2) continue;
		h_n2_tmp->Fill(*n2b1, *nj);
		h_n2ddt_tmp->Fill(*N2DDT, *nj);
		n++;
		
	}
	cout << "num " << n << endl;
	TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
	double totalevent = h_event->Integral();
	h_n2_tmp->Scale(xsbkg/(double)totalevent);
	h_n2ddt_tmp->Scale(xsbkg/(double)totalevent);
	h_n2->Add(h_n2_tmp);
	h_n2ddt->Add(h_n2ddt_tmp);
}

void correlated_plot(){
	double N=0, count=0;
	double eff_s,eff_s_origin,N_origin, count_QCD=0,count_top=0,count_bkg=0,N_bkg=0;
	TH2D* h_top_n2 = new TH2D("h_top","top",NN2,0,MaxN2,Nddb,Minddb,Maxddb);
		h_top_n2->SetYTitle("nj"); h_top_n2->SetXTitle("N2");
	TH2D* h_top_n2ddt = new TH2D("h_top2","top",NN2,MinN2,MaxN2,Nddb,Minddb,Maxddb);
		h_top_n2ddt->SetYTitle("nj"); h_top_n2ddt->SetXTitle("N2DDT");
	TH2D* h_w_n2 = (TH2D*) h_top_n2->Clone("h_w"); h_w_n2->SetTitle("W+jet");
	TH2D* h_w_n2ddt = (TH2D*) h_top_n2ddt->Clone("h_w2"); h_w_n2ddt->SetTitle("W+jet");
	TH2D* h_z_n2 = (TH2D*) h_top_n2->Clone("h_z"); h_z_n2->SetTitle("Z+jet");
	TH2D* h_z_n2ddt = (TH2D*) h_top_n2ddt->Clone("h_z2"); h_z_n2ddt->SetTitle("Z+jet");
	
	// top //
	double eff=0;
	load_to_hist_bkg(tt_semi,semi,h_top_n2,h_top_n2ddt);

	load_to_hist_bkg(tt_LL,LL,h_top_n2,h_top_n2ddt);
	
	load_to_hist_bkg(tt_had,hadron,h_top_n2,h_top_n2ddt);
	
	gStyle->SetOptStat("");
	auto c1 = new TCanvas("c1","c1");
	h_top_n2->Draw("CANDLE");
	c1->SaveAs("top_n2.png");
	h_top_n2ddt->Draw("CANDLE");
	c1->SaveAs("top_n2ddt.png");
	
	// w+jet  //
	ifstream infile("wjet_list.txt");
	stringstream ss;
	string line,name;
	double xs;
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist_bkg(filesdir+name,xs,h_w_n2,h_w_n2ddt);
	}
	h_w_n2->Draw("CANDLE");
	c1->SaveAs("w_n2.png");
	h_w_n2ddt->Draw("CANDLE");
	c1->SaveAs("w_n2ddt.png");
	infile.close();

	// z+jet //
	infile.open("zjet_list.txt");
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist_bkg(filesdir+name,xs,h_z_n2,h_z_n2ddt);
	}
	h_z_n2->Draw("CANDLE");
	c1->SaveAs("z_n2.png");
	h_z_n2ddt->Draw("CANDLE");
	c1->SaveAs("z_n2ddt.png");	
}		