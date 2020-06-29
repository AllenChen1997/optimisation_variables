/////////////////////////////////////////////////
/*
   this code is used to calculate the punzi significance for before/after applying N2DDT
*/
////////////////////////////////////////////////

//#include "N2_study.C"
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

void load_to_hist_bkg(string s, TH2D* h , double& count, double& N, double xsbkg){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	while (myRead.Next()){  // loop in one root file
		N+=1;
		if (*rho < Minrho ||*rho > Maxrho) continue;
		int x = ceil((double)(*rho - Minrho) / d );
		int y;
		if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
		else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
		else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
		else if (*pt >= pt_r4) y = 4;
		else continue;
		double n_cut = h->GetBinContent(x,y);
		if (n_cut == 0) continue;
		if ( *n2b1 - n_cut < 0 ) count+=1;
	}
	TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
	double totalevent = h_event->Integral();
	count = (double)count* (double)Lumi* (double)xsbkg/ (double)totalevent;
	N = (double)N* (double)Lumi* (double)xsbkg/ (double)totalevent;
}
void load_to_hist_signal(string s, TH2D* h , double& count, double& N, double& N_origin){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	while (myRead.Next()){  // loop in one root file
		if (*rho < Minrho ||*rho > Maxrho) continue;
		int x = ceil((double)(*rho - Minrho) / d );
		int y;
		if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
		else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
		else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
		else if (*pt >= pt_r4) y = 4;
		else continue;
		N+=1;
		if ( *n2b1 - h->GetBinContent(x,y) < 0 ) count+=1;
	}
	TH1F* h_event = (TH1F*) myfile->Get("h_total");
	N_origin = h_event->Integral();
}
void cal_n2b1_3(){
	TFile* myfile = new TFile("TH3_output.root","READ");
	TH2D* h_pt_rho = (TH2D*) myfile->Get(s1.c_str());

	double N=0, count=0;
	double eff_s,eff_s_origin,N_origin, count_QCD=0,count_top=0,count_bkg=0,N_bkg=0;
	// QCD //
	ifstream infile("QCD_xs_list.txt");
	string line,name;
	double xs;
	stringstream ss;
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist_bkg(name,h_pt_rho,count=0,N=0,xs);
		N_bkg += N;
		count_QCD += count;
	}

	// signal //
	load_to_hist_signal(sig_root,h_pt_rho,count=0,N=0,N_origin); // xs = 0, used to show it is signal in code
	eff_s = (double) count / (double) N_origin;	
	eff_s_origin = (double) N / (double) N_origin;
	
	// top //
	double eff=0;
	load_to_hist_bkg(tt_semi,h_pt_rho,count=0,N=0,semi);
	count_top += count;
	N_bkg += N;
	
	load_to_hist_bkg(tt_LL,h_pt_rho,count=0,N=0,LL);
	count_top += count;
	N_bkg += N;
	
	load_to_hist_bkg(tt_had,h_pt_rho,count=0,N=0,hadron);
	count_top += count;
	N_bkg += N;
	
	// punzi significance //
	count_bkg = count_QCD +count_top;
	double puzi = eff_s / (double)(1+TMath::Sqrt(count_bkg) );
	double puzi_origin = eff_s_origin / (double)(1+TMath::Sqrt(N_bkg) ) ;
	cout << "puzi significance: " <<endl;
	cout << "befor N2DDT " << puzi_origin << endl;
	cout << "after N2DDT " << puzi <<endl;
}
