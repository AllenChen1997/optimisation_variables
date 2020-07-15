	///////////////////////////
/*
	this is used to study N2 value in jetpt/ rho
	in the end, output the root file with X(N% selection eff. of QCD)
*/
//////////////////////////
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

// protected settings //
#define Npt 4
#define Minpt 0
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
//  
using namespace std;
// there are two ways to use this macro
void N2_study_new(string inputfile, string outputfile, bool isusexs);
void N2_study_new(){
	string inputfile = "top_xs_list.txt";
	string outputfile = "TH3_output_top.root";
	bool isusexs = true;
	N2_study_new(inputfile,outputfile,isusexs);
}

template<typename T>
void setDrawOpt(T& h,string title, string xTitle, string yTitle){
	h->SetTitle(title.c_str());
	h->SetTitleSize(0.07);
	h->GetXaxis()->SetLabelSize(0.05);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetLabelSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->SetXTitle(xTitle.c_str());
	h->SetYTitle(yTitle.c_str());
}

void get_n2(TH3D* h, TH2D*& hout, string outputname, double pro){
		// do the short test
	Double_t *q = new Double_t[1]; // for n2b1
	Double_t *p = new Double_t[1];
	p[0] = pro; 
	for (int i = 0; i<h->GetNbinsY(); i++){
		for (int j=0; j<h->GetNbinsZ(); j++){
		TH1D* h_project = (TH1D*) h->ProjectionX("",i+1,i+1,j+1,j+1);
		if (h_project->GetEntries() == 0 ) continue;
		q[0] = 0.0; 
		h_project->GetQuantiles(1,q,p);
		hout->SetBinContent(i+1,j+1,q[0]);
		}
	}
	// plot out the 2D map 
	gStyle->SetPaintTextFormat("5.3f");
	gStyle->SetOptStat("");
	hout->SetMinimum(0.2);
	hout->SetMaximum(0.35);
	TCanvas* cpt = new TCanvas("cpt","cpt",800,500);
	setDrawOpt(hout, "", "#rho", "P_{T} (GeV)");
	hout->SetTitleOffset(1.0,"Y");
	hout->Draw("COLZ TEXT0");
	cpt->SaveAs((TString)outputname);
}

void getviolinplot(TH3D* h_total, string outputname, double pro){
	TH2D* h_violin = (TH2D*) h_total->Project3D("XY"); setDrawOpt(h_violin, "", "#rho", "P_{T} (GeV)" );
	double* q = new Double_t[1]; q[0] = 0.0; // q is result of GetQuantiles()
	double* p = new Double_t[1]; p[0] = pro; // p is precentage of histo.
	TGraph* dt1 = new TGraph(); int ndt =0;
	for (int i=0; i<h_total->GetNbinsY() ;i++ ){
		q[0] = 0.0; // reset the value
		TH1D* h_tmp = (TH1D*) h_total->ProjectionX("",i+1,i+1,0,-1); // treat rho one by one
		if (h_tmp->GetEntries() == 0) continue;
		h_tmp->GetQuantiles(1,q,p);
		ndt++;
		dt1->SetPoint(ndt,Minrho+(i+0.5)*d,q[0]);
	}
	auto cviolin = new TCanvas("cviolin","cviolin");
	h_violin->Draw("VIOLIN(03001000)");
	dt1->Draw("* SAME");
	cviolin->SaveAs((TString)outputname);
}

void N2_study_new(string inputfile, string outputfile, bool isusexs){
	// set hists //
	Double_t* ptbounds = new Double_t[5];
	ptbounds[0] = 200; ptbounds[1] = 400; ptbounds[2] = 600; ptbounds[3] = 800; ptbounds[4] = 2000;
	Double_t n2bounds[NN2+1];
	for (int i =0; i<= NN2;i++) n2bounds[i] = MinN2+(double)i*(MaxN2-MinN2)/(double)NN2;
	Double_t rhobounds[Nrho+1];
	for (int i=0; i<= Nrho;i++) rhobounds[i] = Minrho+(double)i*(Maxrho-Minrho)/(double)Nrho;
	// n2-rho-pt
	TH3D* h_total = new TH3D("h_total","",NN2,n2bounds,Nrho,rhobounds,Npt,ptbounds);
	TH3D* h_top = (TH3D*) h_total->Clone("");
	// input file //
	ifstream infile( (TString)inputfile); // used to load input file list. in each line: xxx.root name_for_plot
	string line,s1;
	stringstream ss;
	double ixs;
	int i=0;
	while(getline(infile,line)){ // loop in the root file list
		cout << line << endl;
		ss << line;
		ss >> s1 >> ixs;
		ss.clear();
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
		TTreeReaderValue< Double_t > mass(myRead,"FJetMass");	
		TH1F* h_event = (TH1F*) myfile->Get("h_total_mcweight");
		int N = myRead.GetEntries(); //get the entires info.
		if (N == 0) continue;
		h_top->Reset();
		while (myRead.Next()){  // loop in one root file
			if (*mass < 30) continue;
			h_top->Fill(*n2b1,*rho,*pt);
		}
		if (isusexs) h_top->Scale((double)ixs/(double)h_event->Integral());
		h_total->Add(h_top);
	}
	
	// prepare to output //
	vector<double> n2b1_cut;
	TFile* fout = new TFile( (TString)outputfile,"NEW");
	TTree outTree("tree","out branches");
	outTree.Branch("n2b1_cut", &n2b1_cut);
	TH2D* h_pt_rho_5 = new TH2D("h_pt_rho_5","",Nrho,rhobounds,Npt,ptbounds);
	TH2D* h_pt_rho_20 = (TH2D*) h_pt_rho_5->Clone("h_pt_rho_20");
	TH2D* h_pt_rho_26 = (TH2D*) h_pt_rho_5->Clone("h_pt_rho_26");
	TH2D* h_pt_rho_50 = (TH2D*) h_pt_rho_5->Clone("h_pt_rho_50");
	
	// fill the vector //
	get_n2(h_total,h_pt_rho_5,"2D_map_5.png",0.05);
	get_n2(h_total,h_pt_rho_20,"2D_map_20.png",0.20);
	get_n2(h_total,h_pt_rho_26,"2D_map_26.png",0.26);
	get_n2(h_total,h_pt_rho_50,"2D_map_50.png",0.50);

	TH1D* h_no_rho_regions = (TH1D*) h_total->ProjectionX("",0,-1,0,-1);
		Double_t *q = new Double_t[4]; // for n2b1
		Double_t *p = new Double_t[4];
		q[0] = 0.0; q[1] = 0.0; q[2] = 0.0; q[3] = 0.0;
		p[0] = 0.05; p[1] = 0.2; p[2] =0.26; p[3] =0.5;
		h_no_rho_regions->GetQuantiles(4,q,p);
		for (int j=0;j<4;j++)n2b1_cut.push_back(q[j]);
	
	outTree.Fill();
	h_total->Write();
	fout->Write();
	fout->Close();
	// plot out the violin plot //
	getviolinplot(h_total,"n2b1_5.png",0.05);
	getviolinplot(h_total,"n2b1_20.png",0.2);
	getviolinplot(h_total,"n2b1_26.png",0.26);
	getviolinplot(h_total,"n2b1_50.png",0.5);
}