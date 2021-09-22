/////////////////////////////////////////////////
/*
   this version only do the n2b1 first
*/
////////////////////////////////////////////////

#include "N2_study.C"
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
//#define MinN2 -0.2
#define MinN2 0
#define MaxN2 0.5
#define Nrho 20
#define Minrho -6
#define Maxrho -1
//
using namespace std;

string filesdir = "/afs/cern.ch/work/k/kuchen/public/v06_00_05/";
string sig_root= filesdir+"crab_EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV_0000_1.root";
string tt_semi= filesdir+"crab_TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root";
string tt_LL = filesdir+"crab_TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8.root";
string tt_had = filesdir+"crab_TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.root";

string s1 = "h_pt_rho_26";

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9
#define Lumi 41000.0 // 1/pb

void load_to_hist(string s, TH1D* h, TH2D* h_cut, double& count, double& N, double xsbkg, bool isQCD){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
	TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
	TTreeReaderValue< Double_t > nj(myRead,"nJets");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	double width = (double) (Maxrho -Minrho) / (double) Nrho;
	while (myRead.Next()){  // loop in one root file
		/*if (*dphi < 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		if (*ddb < 0.86 ) continue;
		if (*nj > 2) continue;*/
		N+=1;
		if (*rho < Minrho ||*rho > Maxrho) continue;
		int x = ceil((double)(*rho - Minrho) / d );
		int y;
		if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
		else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
		else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
		else if (*pt >= pt_r4) y = 4;
		else continue;
		double n_cut = h_cut->GetBinContent(x,y);
		if (n_cut == 0) continue;
		//h->Fill(*n2b1-n_cut);
		h->Fill(*n2b2);
		if ( *n2b1-n_cut < 0 ) count+=1;

	}
	cout << "N " <<N << " count " << count << endl;
	
	if (xsbkg != 0){	
		TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
		double totalevent = h_event->Integral();
		count = (double)count* (double)Lumi* (double)xsbkg/ (double)totalevent;
		N = (double)N* (double)Lumi* (double)xsbkg/ (double)totalevent;
		if (! isQCD ) h->Scale(1.0/totalevent);
	}
}
void plot_3(){
	TH1D* h_sig = new TH1D("signal","signal",NN2,MinN2,MaxN2);
	TH1D* h_top[3];
	h_top[0] = new TH1D("top_1","top_1",NN2,MinN2,MaxN2);
	h_top[1] = (TH1D*) h_top[0]->Clone("top_2");
	h_top[2] = (TH1D*) h_top[0]->Clone("top_3");
	TH1D* h_QCD = new TH1D("QCD","QCD",NN2,MinN2,MaxN2);
	
	TFile* myfile = new TFile("TH3_output.root","READ");
	TH2D* h_pt_rho = (TH2D*) myfile->Get(s1.c_str());
	
	double N=0, count=0;
	double eff_s, count_QCD=0,count_top=0,count_bkg,N_bkg=0;
	// QCD //
	ifstream infile("QCD_xs_list.txt");
	string line,name;
	double xs;
	stringstream ss;
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist(name,h_QCD,h_pt_rho,count=0,N=0,xs,true);
		N_bkg += N;
		count_QCD += count;
	}
	cout << "the selection eff. of QCD " << (double)count_QCD / (double) N_bkg * 100 << " (%) " << endl;
	// signal //
	load_to_hist(sig_root,h_sig,h_pt_rho,count=0,N=0,0,false); // xs = 0, used to show it is signal in code
	eff_s = (double) count / (double) N;	
	cout << "the selection eff. of signal " << eff_s * 100 << " (%) " << endl;
	// top //
	double eff=0;
	load_to_hist(tt_semi,h_top[0],h_pt_rho,count=0,N=0,semi,false);
	eff += (float) count/ (float) N *semi;
	count_top += count;
	
	load_to_hist(tt_LL,h_top[1],h_pt_rho,count=0,N=0,LL,false);
	eff += (float) count/ (float) N *LL;
	count_top += count;
	
	load_to_hist(tt_had,h_top[2],h_pt_rho,count=0,N=0,hadron,false);
	eff += (float) count/ (float) N *hadron;
	count_top += count;
	cout << "the selection eff. of top " << (float)eff * 100.0 /(semi+LL+hadron) << " (%) " << endl;
	
	// punzi significance //
	double puzi_QCD = eff_s / (1+TMath::Sqrt(count_QCD) ); 
	double puzi_top = eff_s / (1+TMath::Sqrt(count_top) );
	count_bkg = count_QCD +count_top;
	double puzi = eff_s / (1+TMath::Sqrt(count_bkg) );
	cout << "QCD puzi significance = " << puzi_QCD << " top " << puzi_top << " total " << puzi << endl;
	// output plot //
	h_top[0]->Scale(semi);
	h_top[1]->Scale(LL);
	h_top[2]->Scale(hadron);

	gStyle->SetOptStat("");	
	h_top[0]->Add(h_top[1]);
	h_top[0]->Add(h_top[2]);
	
	auto c1 = new TCanvas("c1","c1");
	h_sig->Scale(1.0/h_sig->Integral());
	h_sig->SetXTitle("N_2(beta=2))");
	h_sig->SetTitle("");
	h_sig->SetLineColor(kRed);
	h_sig->GetXaxis()->SetTitleSize(0.04);
	h_sig->GetXaxis()->SetLabelSize(0.05);
	h_sig->GetYaxis()->SetLabelSize(0.05);
	h_sig->SetTitleOffset(1.0,"X");
	
	h_sig->Draw("HIST E");
	h_QCD->Scale(1.0/h_QCD->Integral());
	h_QCD->Draw("SAME HIST E");
	h_top[0]->Scale(1.0/h_top[0]->Integral());
	h_top[0]->SetLineColor(kBlack);
	h_top[0]->Draw("SAME HIST E");
	
	//TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	TLegend* legend = new TLegend(0.8,0.7,1.0,0.9);
	legend->AddEntry(h_sig,"signal","l");
	legend->AddEntry(h_QCD,"QCD bkg","l");
	legend->AddEntry(h_top[0],"top bkg","l");
	legend->Draw();
	
	c1->SaveAs("n2b2_new.png");
}
