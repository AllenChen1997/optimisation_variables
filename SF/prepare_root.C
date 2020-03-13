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
string cut_branch = "N2b1_v26";  //used to read cut root file branch

using namespace std;
void prepare_root(string inputname="/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",string outputname="MC.root", string cutname="../TH3_output.root"){
	TFile* fcut = new TFile((TString)cutname,"READ");
	TH2D* h_cut = (TH2D*) fcut->Get("h_pt_rho");

	TFile* fin = new TFile((TString)inputname,"READ");
	TTreeReader myRead("monoHbb_SR_boosted",fin);
	TTreeReaderValue< Double_t > N2(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	
	TFile* fout = new TFile((TString)outputname,"NEW");
	TH1D* h_pass = new TH1D("h_pass","h_pass",NN2,MinN2,MaxN2);
	TH1D* h_fail = (TH1D*) h_pass->Clone("h_fail"); h_fail->SetTitle("h_fail");
	int N = myRead.GetEntries();
	int x,y;
	int i=0;
	while(myRead.Next()){
		// for making fake data
		i++;
		if (i > N/2) continue;
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
		double N2cut = h_cut->GetBinContent(x,y);
		if (N2cut == 0) continue;
		double N2ddt = *N2 - N2cut;
		if(N2ddt<0) h_pass->Fill(N2ddt);
		else h_fail->Fill(N2ddt);
		
	}

	fout->Write();
	fout->Close();
	
}