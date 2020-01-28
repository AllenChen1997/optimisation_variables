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
//
using namespace std;
//string sig_root="/eos/cms/store/group/phys_exotica/monoHiggs/monoHbb/Analyser_Outputs/N2_N3_Study/EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1600_MH4_150_MH2_1600_MHC_1600_CP3Tune_13TeV_0000_0.root";

string sig_root="/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/EXO-ggToXdXdHToBB_sinp_0p35_tanb_1p0_mXd_10_MH3_1000_MH4_150_MH2_1000_MHC_1000_CP3Tune_13TeV_0000_0.root";
string s1 = "n2b1_v20";
string s2 = "n2b2_v20";

//string s1 = "n2b1_v26";
//string s2 = "n2b2_v26";

// the top sample xs:
#define semi 308.9
#define LL 78.5
#define hadron 303.9

void load_to_hist(string s1, TH3D* h, vector<double>& v, int& count, int& N){
	TFile* myfile = new TFile(s1.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TH1D* h_tmp = (TH1D*) h->ProjectionY("",0,-1,0,-1);
	double width = h_tmp->GetBinWidth(1);
	double init = h_tmp->GetBinLowEdge(1);
	while (myRead.Next()){  // loop in one root file
		if (*rho < Minrho ||*rho > Maxrho) continue;
		int i = floor((double)(*rho - init) / width );
		if(v[i] == -1) {
			h->Fill(*n2b1,*rho,*pt);
			count++;
		}else { 
			h->Fill(*n2b1-v[i],*rho,*pt);
			if ( *n2b1-v[i] < 0 ) count++;
		}
		N++;
	}
}
void plot_3(){
	TH3D* h_sig = new TH3D("signal_3D","signal",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt);
	//TH3D* h_sig_b2 = (TH3D*) h_sig_b1->Clone("signal_n2b2");
	TH3D* h_top[3];
	h_top[0] = new TH3D("top_1","top_1",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt);
	h_top[1] = (TH3D*) h_top[0]->Clone("top_2");
	h_top[2] = (TH3D*) h_top[0]->Clone("top_3");
	TH3D* h_QCD = new TH3D("QCD","QCD",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt);
	vector<double> v_n2b1,v_n2b2;
	
	TFile* myfile = new TFile("TH3_output.root","READ");
	TTreeReader myRead("tree",myfile);  
	TTreeReaderValue<vector<double>> n2b1(myRead,Form("%s",s1.c_str()));
	TTreeReaderValue<vector<double>> n2b2(myRead,Form("%s",s2.c_str()));
	while(myRead.Next()){		
		for (auto x : *n2b1) v_n2b1.push_back(x);
		for (auto x : *n2b2) v_n2b2.push_back(x);
	}

	ifstream infile("QCD_list.txt");
	string line;
	int N = 0, count = 0;
	while(getline(infile,line)){
		load_to_hist(line,h_QCD,v_n2b1,count,N);
	}
	cout << "the selection eff. of QCD " << (float) count / (float) N * 100 << " (%) " << endl;
	N=0;count=0;
	load_to_hist(sig_root,h_sig,v_n2b1,count,N);
	cout << "the selection eff. of signal " << (float) count / (float) N * 100 << " (%) " << endl;
	N=0;count=0;
	load_to_hist("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",h_top[0],v_n2b1,count=0,N=0);
	load_to_hist("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/TTTo2L2Nu_TuneCP5_PSweights_13TeV-powheg-pythia8.root",h_top[1],v_n2b1,count,N);
	load_to_hist("/afs/cern.ch/work/d/dekumar/public/monoH/Analyzer/CMSSW_10_3_0/src/ExoPieProducer/ExoPieAnalyzer/OutputForRaman/crab_TTToHadronic_TuneCP5_PSweights_13TeV-powheg-pythia8.root",h_top[2],v_n2b1,count,N);
	
	//cout << "the selection eff. of top " << (float) count / (float) N * 100 << " (%) " << endl;
	TH1D* h_top_0 = (TH1D*) h_top[0]->ProjectionX("top_semi",0,-1,0,-1);
	h_top_0->Scale(semi/h_top_0->Integral());
	TH1D* h_top_1 = (TH1D*) h_top[1]->ProjectionX("top_LL",0,-1,0,-1);
	h_top_1->Scale(LL/h_top_1->Integral());
	TH1D* h_top_2 = (TH1D*) h_top[2]->ProjectionX("top_had",0,-1,0,-1);
	h_top_2->Scale(hadron/h_top_2->Integral());

	gStyle->SetOptStat("");	
	/*auto c2 = new TCanvas("c2","c2");
	h_top_0->Draw();
	h_top_1->SetLineColor(kRed);
	h_top_1->Draw("SAME");
	h_top_2->SetLineColor(kBlack);
	h_top_2->Draw("SAME");
	TLegend* legend2 = new TLegend(0.7,0.7,0.9,0.9);
	legend2->AddEntry(h_top_0,"semi","l");
	legend2->AddEntry(h_top_1,"LL","l");
	legend2->AddEntry(h_top_2,"hadron","l");
	legend2->Draw();
	c2->SaveAs("TT_sample.png");*/
	
	h_top_0->Add(h_top_2);
	h_top_0->Add(h_top_1);
	auto c1 = new TCanvas("c1","c1");
	TH1D* h1 = (TH1D*)h_sig->ProjectionX("signal_1D",0,-1,0,-1);
	h1->Scale(1.0/h1->Integral());
	h1->SetXTitle("N_{2}^{DDT}(N_{2}^{1.0})");
	h1->SetLineColor(kRed);
	h1->Draw("HIST E");
	TH1D* h2 = (TH1D*)h_QCD->ProjectionX("QCD",0,-1,0,-1);
	h2->Scale(1.0/h2->Integral());
	h2->Draw("SAME HIST E");
	h_top_0->Scale(1.0/h_top_0->Integral());
	h_top_0->SetLineColor(kBlack);
	h_top_0->Draw("SAME HIST E");
	
	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
	legend->AddEntry(h1,"signal","l");
	legend->AddEntry(h2,"QCD bkg","l");
	legend->AddEntry(h_top_0,"top bkg","l");
	legend->Draw();
	
	c1->SaveAs("n2b1_new.png");
	
}
