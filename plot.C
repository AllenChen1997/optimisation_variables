///////////////////////////////
/*
	Date 2019/11/13
	owner Kung-Hsiang Chen
	
	read signal and QCD samples and plot FJetN2b1 /FjetN2b2 /FJetrho figures
*/
///////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TH1D.h>
#include <TTreeReader.h>
#include <iterator>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>

#define NN 13 //the number of input files

using namespace std;
void plot(){
	TH1D* h1[NN+1];
	TH1D* h2[NN+1];
	TH1D* h3[NN+1];
	ifstream infile("list_2.txt"); // used to input file. in each line: xxx.root name_for_plot
	string line,s1,s2;
	stringstream ss;
	vector<string> fname;
	fname.push_back("n/a");
	int i=0;
	while(getline(infile,line)){
      cout << line << endl;
      ss << line;
      ss >> s1 >> s2;// s1=xxx.root, s2=name_for_plot
      ss.clear();
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		int N = myRead.GetEntries(); //get the entires info.
		if (N == 0) continue;
		cout << "entries = " << N << endl;
		i++; // count for the files with non zero entries in branch
		h1[i] = new TH1D(Form("FJetN2b1_%s",s2.c_str()),Form("FJetN2b1_%s",s2.c_str()),10,0,0.5 );
		h2[i] = new TH1D(Form("FJetN2b2_%s",s2.c_str()),Form("FJetN2b2_%s",s2.c_str()),10,0,0.5 );
		h3[i] = new TH1D(Form("FJetrho_%s",s2.c_str()),Form("FJetrho_%s",s2.c_str()),10,-5,0 );
		while (myRead.Next()){
			h1[i]->Fill(*n2b1);
			h2[i]->Fill(*n2b2);
			h3[i]->Fill(*rho);
		}
		fname.push_back(s2);
	}
	gStyle->SetOptStat("");
	auto c1 = new TCanvas("c1","c1");
	auto legend1 = new TLegend(0.7,0.7,0.9,0.9);
	TAxis* xaxis1 = h1[1]->GetXaxis();
	xaxis1->SetTitle("N^{1.0}_{2}");
	
	for (int j=1;j<=i;j++){
		h1[j]->Scale(1.0/h1[j]->Integral());
		h1[j]->SetLineWidth(3);
		h1[j]->Draw("SAME PLC");
		legend1->AddEntry(h1[j],Form("%s",fname[j].c_str()),"l");
	}
	legend1->Draw();
	c1->SaveAs("./n2b1_plot.png");
	
	auto c2 = new TCanvas("c2","c2");
	auto legend2 = new TLegend(0.7,0.7,0.9,0.9);
	TAxis* xaxis2 = h2[1]->GetXaxis();
	xaxis2->SetTitle("N^{2.0}_{2}");
	for (int j=1;j<=i;j++){
		h2[j]->Scale(1.0/h2[j]->Integral());
		h2[j]->SetLineWidth(3);
		h2[j]->Draw("SAME PLC");
		legend2->AddEntry(h2[j],Form("%s",fname[j].c_str()),"l");
	}
	legend2->Draw();
	c2->SaveAs("./n2b2_plot.png");	

	auto c3 = new TCanvas("c3","c3");
	auto legend3 = new TLegend(0.7,0.7,0.9,0.9);
	for (int j=1;j<=i;j++){
		h3[j]->Scale(1.0/h3[j]->Integral());
		h3[j]->SetLineWidth(3);
		h3[j]->Draw("SAME PLC");
		legend3->AddEntry(h3[j],Form("%s",fname[j].c_str()),"l");
	}
	legend3->Draw();
	c3->SaveAs("./rho_plot.png");	
	
	auto c1_1 = new TCanvas("c1_1","c1_1");
	gPad->Divide(2,2);
	for (int j=1;j<=4;j++){
		c1_1->cd(j);
		xaxis1 = h1[j]->GetXaxis();
		xaxis1->SetTitle("N^{1.0}_{2}");
		h1[j]->Sumw2();
		h1[j]->Scale(1.0/h1[j]->Integral());
		h1[j]->SetLineWidth(3);
		h1[j]->Draw("SAME PLC");
	}
	c1_1->SaveAs("./n2b1_plot_2.png");
	
	auto c2_1 = new TCanvas("c2_1","c2_1");
	gPad->Divide(2,2);
	for (int j=1;j<=4;j++){
		c2_1->cd(j);
		xaxis2 = h2[j]->GetXaxis();
		xaxis2->SetTitle("N^{2.0}_{2}");
		h2[j]->Sumw2();
		h2[j]->Scale(1.0/h2[j]->Integral());
		h2[j]->SetLineWidth(3);
		h2[j]->Draw("SAME PLC");
	}
	c2_1->SaveAs("./n2b2_plot_2.png");	
	
	auto c3_1 = new TCanvas("c3_1","c3_1");
	gPad->Divide(2,2);
	for (int j=1;j<=4;j++){
		c3_1->cd(j);
		h3[j]->Sumw2();
		h3[j]->Scale(1.0/h3[j]->Integral());
		h3[j]->SetLineWidth(3);
		h3[j]->Draw("SAME PLC");
	}
	c3_1->SaveAs("./rho_plot_2.png");

}
