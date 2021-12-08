/////////////////////////////////////////////////
/*
   this code is used to calculate the punzi significance for before/after applying N2DDT/N2
   there are two ways to use this code:
   1. scan mode:
      with options: isScanSignal = true, scanfileName = "xxx.txt"
   2. run once mode:
      with options: isScanSignal = false, filesdir = "dir_to_file", sig_root = filesdir + "filename.root"
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
// the top sample xs:
#define semi 687.1*0.438
#define LL 687.1*0.105
#define hadron 687.1*0.457
#define Lumi 41000 // 1/pb
double n2_cut;
TH2D* h_rho_pt;
using namespace std;

// setup the running option //
string n2ddtweight = "h_pt_rho_26";
string fN2weight = "TH3_output.root";
bool isScanSignal = true;  string scanfileName = "signal_list.txt"; // if true, needs to setup the list filename
string filesdir = "/afs/cern.ch/work/k/kuchen/public/v06_00_05/";// if false, needs to setup this line and next line 
string sig_root= filesdir+"crab_EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV_0000_1.root";
bool isN2 = false; int n2cut_id = 2; // true for N2 cut , false for N2DDT cut // cut_id 0 = 5%, 1 = 20%, 2=26%, 3 = 50%


void run_(); // the main program, see below //
void cal_n2b1_3(){  // controlls the scan mode or run once mode
	if (isScanSignal){
		ifstream infile(scanfileName.c_str());
		while (getline(infile,sig_root)){
			cout << "running " << sig_root << endl;
			run_();
		}
	} else {
			run_();
	}
}

void load_to_hist(string s, double& count, double& N, double xsbkg, bool isSignal){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
	TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
	TTreeReaderValue< Double_t > nj(myRead,"nJets");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	
	while (myRead.Next()){  // loop in one root file
		if (*dphi < 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		if (*ddb < 0.86 ) continue;
		if (*nj > 2) continue;
		N+=1;
		if (isN2) {
			if (*n2b1 < n2_cut) count+=1;
		} else {
			if (*rho < Minrho ||*rho > Maxrho) continue;
			int x = ceil((double)(*rho - Minrho) / d );
			int y;
			if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
			else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
			else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
			else if (*pt >= pt_r4) y = 4;
			else continue;
			double n_cut = h_rho_pt->GetBinContent(x,y);
			if (n_cut == 0) continue;
			if ( *n2b1 - n_cut < 0 ) count+=1;
		}
	}
	if (isSignal) {
		TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
		double N_origin = h_event->Integral();
		count = count / N_origin;
		N = N / N_origin;
	} else {
		TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
		double totalevent = h_event->Integral();
		count = (double)count* (double)Lumi* (double)xsbkg/ (double)totalevent;
		N = (double)N* (double)Lumi* (double)xsbkg/ (double)totalevent;
	}
}

void run_(){
	TFile* myfile = new TFile((TString)fN2weight,"READ");
	// load the cut value 
	TTreeReader cutRead("tree",myfile); // cut value for N2 (5%, 20%, 26%, 50%)
	TTreeReaderValue< vector<double> > icut(cutRead,"n2b1_cut"); 
	vector<double> vcut;
	while (cutRead.Next()) {
		for (auto x : *icut) vcut.push_back(x);
	}
	n2_cut = vcut[n2cut_id];
	h_rho_pt = (TH2D*) myfile->Get(n2ddtweight.c_str()); // cut value for N2DDT
	
	double N=0, count=0;
	double eff_s=0,eff_s_origin=0 ,count_bkg=0,N_bkg=0;

	// QCD //
		// there is no QCD because there is few event after preselection
	// signal //
	load_to_hist(sig_root,eff_s,eff_s_origin,0,true); // xs = 0, used to show it is signal in code
	// top //
	string line,name;
	double xs;
	stringstream ss;
	ifstream infile("top_xs_list.txt");
	while(getline(infile,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist(name,count=0,N=0,xs,false);
		count_bkg += count;
		N_bkg += N;
	}	
	// w+jet and z+jet //
	ifstream infile2("Wjet_Zjet_list.txt");
	while(getline(infile2,line)){
		ss << line;
		ss >> name >> xs;
		ss.clear();
		load_to_hist(name,count=0,N=0,xs,false);
		N_bkg += N;
		count_bkg += count;
	}
	// punzi significance //

	double puzi = eff_s / (double)(1+TMath::Sqrt(count_bkg) );
	double puzi_origin = eff_s_origin / (double)(1+TMath::Sqrt(N_bkg) ) ;
	ofstream ofile("punzi_result.txt",ios::app);
	ofile << "signal is: " << sig_root << endl;
	ofile << "puzi significance: " <<endl;
	if (isN2) {
		ofile << "befor N2 : after N2" << endl;
		ofile << puzi_origin << " : " <<  puzi << endl;
	} else {
		ofile << "befor N2DDT : after N2DDT " << endl;
		ofile << puzi_origin << " : " << puzi <<endl;
	}
	if (puzi_origin > puzi) ofile << "worse" << endl;
	else ofile << "better" << endl;
}		
