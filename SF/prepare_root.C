/*
	this is used to prepare the TH1 in root file for the datacard input
	we have 4 pt regions for the cut [200-400],[400-600],[600-800],[>800]
*/
#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>
// the def. in cut histo.
#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800

#define NN2 28
#define MinN2 -0.2
#define MaxN2 0.5

#define Nrho 20
#define Minrho -6
#define Maxrho -1
double d = (double)(Maxrho-Minrho)/(double)Nrho; // the width in rho histo.

using namespace std;
void runcut(string inputname, TH2D* hcut, TH1D* hout, bool ispass, bool isdata){  // isdata is used tmp
	TFile* fin = new TFile((TString)inputname,"READ");
	TTreeReader myRead("monoHbb_SR_boosted",fin);
	TTreeReaderValue< Double_t > N2(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");

	int N = myRead.GetEntries();
	int x,y;
	int i=0;
	while(myRead.Next()){
		// for making fake data
		i++;
		if (isdata) if (i > N/2) continue;  // tmp. used to divide the file
		if (! isdata) if(i < N/2) continue;
		// deside in which pt region
		if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
		else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
		else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
		else if (*pt >= pt_r4) y = 4;
		else continue;
		// deside in which rho region
		if (*rho < Minrho) continue;
		x = (ceil)(*rho - Minrho) / d;
		// deside pass or fail
		double N2cut = hcut->GetBinContent(x,y);
		if (N2cut == 0) continue;
		double N2ddt = *N2 - N2cut;
		if(ispass) {
			if(N2ddt<0) hout->Fill(N2ddt);
		}
		else {
			if(N2ddt>0) hout->Fill(N2ddt);
		}
	}

}


void prepare_root(string dataname="/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",
	string MCname="/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root", 
	string cutname="../TH3_output.root", 
	string outputname="pass.root", bool ispass=true){
	TFile* fcut = new TFile((TString)cutname,"READ");
	TH2D* h_cut = (TH2D*) fcut->Get("h_pt_rho");
	
	TFile* fout = new TFile((TString)outputname,"NEW");
	TH1D* h_data = new TH1D("h_data","h_data",NN2,MinN2,MaxN2);
	TH1D* h_MC = (TH1D*) h_data->Clone("h_MC"); h_MC->SetTitle("h_MC");
	TH1D* data_obs = (TH1D*) h_data->Clone("data_obs"); data_obs->SetTitle("data_obs");
	//TH1D* data_obs;
	runcut(dataname,h_cut,h_data,ispass,true); // the fifth op. is tmp
	runcut(MCname,h_cut,h_MC,ispass,false); // the fifth op. is tmp 
	
	
	runcut(dataname,h_cut,data_obs,ispass,true);
	
	fout->Write();
	fout->Close();
	
	if (outputname == "pass.root") prepare_root(dataname,MCname,cutname,"fail.root",false); //automatic generate fail.root
	
}