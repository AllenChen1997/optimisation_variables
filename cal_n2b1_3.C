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
//
using namespace std;

string sig_root="/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV_0000_0.root";

string s1 = "n2b1_v26";

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9
#define Lumi 41000 // 1/pb

void load_to_hist_bkg(string s, double& v, double& count, double& N, double xsbkg){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	while (myRead.Next()){  // loop in one root file
		N+=1;
		if (*rho < Minrho ||*rho > Maxrho) {
			count+=1;
			continue;
		}
		if ( *n2b1-v < 0 ) count+=1;
	}
	TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
	double totalevent = h_event->Integral();
	count = (double)count* (double)Lumi* (double)xsbkg/ (double)totalevent;
	N = (double)N* (double)Lumi* (double)xsbkg/ (double)totalevent;
}
void load_to_hist_signal(string s, double& v, double& count, double& N, double& N_origin){
	TFile* myfile = new TFile(s.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	while (myRead.Next()){  // loop in one root file
		N+=1;
		if (*rho < Minrho ||*rho > Maxrho) {
			count+=1;
			continue;
		}
		if ( *n2b1-v < 0 ) count+=1;
	}
	TH1F* h_event = (TH1F*) myfile->Get("h_total");
	N_origin = h_event->Integral();
}
void cal_n2b1_3(){
	vector<double> v_n2b1;
	TFile* myfile = new TFile("TH3_output.root","READ");
	TTreeReader myRead("tree",myfile);  
	TTreeReaderValue<vector<double>> n2b1(myRead,Form("%s",s1.c_str()));
	while(myRead.Next()){		
		for (auto x : *n2b1) v_n2b1.push_back(x);
	}

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
		load_to_hist_bkg(name,v_n2b1[2],count=0,N=0,xs);
		N_bkg += N;
		count_QCD += count;
	}

	// signal //
	load_to_hist_signal(sig_root,v_n2b1[2],count=0,N=0,N_origin); // xs = 0, used to show it is signal in code
	eff_s = (double) count / (double) N_origin;	
	eff_s_origin = (double) N / (double) N_origin;
	
	// top //
	double eff=0;
	load_to_hist_bkg("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",v_n2b1[2],count=0,N=0,semi);
	count_top += count;
	N_bkg += N;
	
	load_to_hist_bkg("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8.root",v_n2b1[2],count=0,N=0,LL);
	count_top += count;
	N_bkg += N;
	
	load_to_hist_bkg("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/crab_TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",v_n2b1[2],count=0,N=0,hadron);
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
