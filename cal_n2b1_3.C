/////////////////////////////////////////////////
/*
   this code is used to calculate the punzi significance for before/after applying N2DDT
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
double d = (double) (Maxrho-Minrho) / (double) Nrho;
#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800
// there are two ways to use this macro
using namespace std;
string filesdir = "/afs/cern.ch/work/k/kuchen/public/samples/";
string sig_root= filesdir+"EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV.root";
string n2ddtweight = "h_pt_rho_26";
bool isN2DDTintuple = false; string fN2weight = "TH3_output.root";
bool isScanSignal = true;  string scanfileName = "signal_list.txt";
void run_();
void cal_n2b1_3(){
	if (isScanSignal){
		ifstream infile(scanfileName.c_str());
		while (getline(infile,sig_root)){
			run_();
		}
	} else {
			run_();
	}
}

// the top sample xs:
#define semi 687.1*0.438
#define LL 687.1*0.105
#define hadron 687.1*0.457
#define Lumi 41000 // 1/pb

void load_to_hist_bkg(string s, TH2D* h , double& count, double& N, double xsbkg){
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
	TTreeReaderValue< Double_t > N2DDT(myRead,"N2DDT");
	while (myRead.Next()){  // loop in one root file
		if (*dphi < 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		//if (*ddb < 0.86 ) continue;
		if (*nj > 2) continue;
		N+=1;
		if (isN2DDTintuple) {
			if (*N2DDT < 0 ) count+=1;
		} else {
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
	TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
	TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
	TTreeReaderValue< Double_t > nj(myRead,"nJets");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	TTreeReaderValue< Double_t > N2DDT(myRead,"N2DDT");
	while (myRead.Next()){  // loop in one root file
		if (*dphi < 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		//if (*ddb < 0.86 ) continue;
		if (*nj > 2) continue;
		if (*rho < Minrho ||*rho > Maxrho) continue;
		N+=1;
		if (isN2DDTintuple) {
			if (*N2DDT < 0) count+=1;
		} else{
			int x = ceil((double)(*rho - Minrho) / d );
			int y;
			if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
			else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
			else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
			else if (*pt >= pt_r4) y = 4;
			else continue;
			if ( *n2b1 - h->GetBinContent(x,y) < 0 ) count+=1;
		}
	}
	TH1F* h_event = (TH1F*) myfile->Get("h_total");
	N_origin = h_event->Integral();
}
void run_(){
	TFile* myfile = new TFile((TString)fN2weight,"READ");
	TH2D* h_pt_rho = (TH2D*) myfile->Get(n2ddtweight.c_str());

	double N=0, count=0;
	double eff_s,eff_s_origin,N_origin ,count_bkg=0,N_bkg=0;

	// QCD //
		// there is no QCD because there is few event after selection
	// signal //
	load_to_hist_signal(sig_root,h_pt_rho,count=0,N=0,N_origin); // xs = 0, used to show it is signal in code
	eff_s = (double) count / (double) N_origin;	
	eff_s_origin = (double) N / (double) N_origin;
	// top //
	string line,name;
	double xs;
	stringstream ss;
	ifstream infile("top_xs_list.txt");
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist_bkg(name,h_pt_rho,count=0,N=0,xs);
		count_bkg += count;
		N_bkg += N;
	}	
	// w+jet and z+jet //
	ifstream infile2("Wjet_Zjet_list.txt");
	while(getline(infile2,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist_bkg(name,h_pt_rho,count=0,N=0,xs);
		N_bkg += N;
		count_bkg += count;
	}
	// punzi significance //

	double puzi = eff_s / (double)(1+TMath::Sqrt(count_bkg) );
	double puzi_origin = eff_s_origin / (double)(1+TMath::Sqrt(N_bkg) ) ;
	ofstream ofile("punzi_result.txt",ios::app);
	ofile << "signal is: " << sig_root << endl;
	ofile << "puzi significance: " <<endl;
	ofile << "befor N2DDT " << puzi_origin << endl;
	ofile << "after N2DDT " << puzi <<endl;
	if (puzi_origin > puzi) ofile << "worse" << endl;
	else ofile << "better" << endl;
}		
