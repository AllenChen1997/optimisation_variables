///////////////////////////////
/*
	Date 2020/ 4/ 24
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
#include <TH2D.h>
#include <TTreeReader.h>
#include <iterator>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>

#define NN 7 //the number of input files

using namespace std;
void plot(){
	TH1D* h1[NN+1];
	TH1D* h2[NN+1];
	TH1D* h3[NN+1];
	TH1D* h4[NN+1];
	TH2D* h_rhoN2b1 = new TH2D("rho-N2b1","rho-N2b1",10,-5,0,10,0,0.5);
  	TH2D* h_rhoN2b2 = new TH2D("rho-N2b2","rho-N2b2",10,-5,0,10,0,0.5);
  	TH1D* n2b1_1 = new TH1D("n2b1_1","rho<-2",10,0,0.5);
  	TH1D* n2b1_2 = new TH1D("n2b1_2","rho>-2",10,0,0.5);

	ifstream infile("QCD_list_for_plot.txt"); // used to input file. in each line: xxx.root name_for_plot
	string line,s1,s2;
	stringstream ss;
	vector<string> fname;
	fname.push_back("n/a");
	int i=0;
	while(getline(infile,line)){
      cout << line << endl;
      ss << line;
      ss >> s1 >> s2 ;// s1=xxx.root, s2=name_for_plot
      ss.clear();
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		TTreeReaderValue< Double_t　> pt(myRead,"Jet1Pt");
		
		int N = myRead.GetEntries(); //get the entires info.
		if (N == 0) continue;
		cout << "entries = " << N << endl;
		i++; // count for the files with non zero entries in branch
		h1[i] = new TH1D(Form("FJetN2b1_%s",s2.c_str()),Form("FJetN2b1_%s",s2.c_str()),10,0,0.5 );
		h2[i] = new TH1D(Form("FJetN2b2_%s",s2.c_str()),Form("FJetN2b2_%s",s2.c_str()),10,0,0.5 );
		h3[i] = new TH1D(Form("FJetrho_%s",s2.c_str()),Form("FJetrho_%s",s2.c_str()),10,-5,0 );
		h4[i] = new TH1D(Form("Jet1Pt_%s",s2.c_str()),Form("Jet1Pt_%s",s2.c_str()),1000,0,800);
		
		while (myRead.Next()){
			h1[i]->Fill(*n2b1);
			h2[i]->Fill(*n2b2);
			h3[i]->Fill(*rho);
			if (*pt>-9999) h4[i]->Fill(*pt);
			h_rhoN2b1->Fill(*rho,*n2b1);
			h_rhoN2b2->Fill(*rho,*n2b2);
			if (*rho>-2) n2b1_1->Fill(*n2b1);
			else n2b1_2->Fill(*n2b1);
		}
		fname.push_back(s2);
	}
	gStyle->SetOptStat("");
	//gPad->SetLogy(1);
	auto c0 = new TCanvas("c0","c0");
	auto legend = new TLegend(0.7,0.7,0.9,0.9);
	
	float maxY = 0;
	float imax;
	int m;
/*
	for (int j=1;j<=i;j++){
		h4[j]->Scale((double)xs[j]/h4[j]->Integral());
		imax = h4[j]->GetBinContent(h4[j]->GetMaximumBin());
		if (maxY < imax ) {
			maxY=imax;
			m=j;
		}
	}
	TAxis* xaxis = h4[m]->GetXaxis();
	xaxis->SetTitle("Pt");
	h4[m]->Draw();
	*/
	for (int j=1;j<=i;j++){
		h4[j]->Scale(1.0/h4[j]->Integral());
		h4[j]->SetLineWidth(3);
		h4[j]->Draw("SAME PLC");
		legend->AddEntry(h4[j],Form("%s",fname[j].c_str()),"l");
	}
	legend->Draw();
	c0->SaveAs("./pt_plot.png");
	
	auto c1 = new TCanvas("c1","c1");
	auto legend1 = new TLegend(0.8,0.7,1.0,0.9);
	maxY = 0;
	for (int j=1;j<=i;j++){
		h1[j]->Scale(1.0/h1[j]->Integral());
		imax = (float)h1[j]->GetBinContent(h1[j]->GetMaximumBin());
		if (maxY < imax ) {
			maxY=imax;
			m=j;
		}
	}
	TAxis* xaxis1 = h1[m]->GetXaxis();
	xaxis1->SetTitle("N_2(beta=1)");
	xaxis1->SetTitleSize(0.04);
	xaxis1->SetLabelSize(0.05);
	h1[m]->SetTitleOffset(1.1,"X");
	h1[m]->GetYaxis()->SetLabelSize(0.05);
	h1[m]->SetTitle("");
	h1[m]->Draw();
	
	for (int j=1;j<=i;j++){
		h1[j]->SetLineWidth(3);
		h1[j]->Draw("SAME PLC");
		legend1->AddEntry(h1[j],Form("%s",fname[j].c_str()),"l");
	}
	legend1->Draw();
	c1->SaveAs("./n2b1_plot.png");
	
	auto c2 = new TCanvas("c2","c2");
	auto legend2 = new TLegend(0.7,0.7,0.9,0.9);
	maxY = 0;
	for (int j=1;j<=i;j++){
		h2[j]->Scale((double)1.0/h2[j]->Integral());
		imax = h2[j]->GetBinContent(h2[j]->GetMaximumBin());
		if (maxY < imax ) {
			maxY=imax;
			m=j;
		}
	}
	TAxis* xaxis2 = h2[m]->GetXaxis();
	xaxis2->SetTitle("N_2(beta=2)");
	xaxis2->SetTitleSize(0.04);
	xaxis2->SetLabelSize(0.05);
	h2[m]->SetTitleOffset(1.1,"X");
	h2[m]->SetTitle("");
	h2[m]->GetYaxis()->SetLabelSize(0.05);
	h2[m]->Draw();
	for (int j=1;j<=i;j++){
		h2[j]->Scale(1.0/h2[j]->Integral());
		h2[j]->SetLineWidth(3);
		h2[j]->Draw("SAME PLC");
		legend2->AddEntry(h2[j],Form("%s",fname[j].c_str()),"l");
	}
	legend2->Draw();
	c2->SaveAs("./n2b2_plot.png");	
	
	TAxis* xaxis = h_rhoN2b1->GetXaxis();
	TAxis* yaxis = h_rhoN2b1->GetYaxis();
	xaxis->SetTitle("#rho");
	yaxis->SetTitle("N^{1.0}_{2}");
	auto c3_tmp = new TCanvas("c3_tmp","c3_tmp");
	gPad->Divide(2,1);
	c3_tmp->cd(1);
	h_rhoN2b1->Draw("COLZ");
	c3_tmp->cd(2);
	xaxis = h_rhoN2b2->GetXaxis();
	yaxis = h_rhoN2b2->GetYaxis();
	xaxis->SetTitle("#rho");
	yaxis->SetTitle("N^{2.0}_{2}");	
	h_rhoN2b2->Draw("COLZ");
	c3_tmp->SaveAs("./rho_n2.png");

	auto c4_tmp = new TCanvas("c4_tmp","c4_tmp");
	gPad->Divide(2,1);
	c4_tmp->cd(1);
	xaxis = n2b1_1->GetXaxis();
	xaxis->SetTitle("N^{1.0}_{2}");
	n2b1_1->Draw();
	c4_tmp->cd(2);
	xaxis = n2b1_2->GetXaxis();
	xaxis->SetTitle("N^{1.0}_{2}");
	n2b1_2->Draw();
	c4_tmp->SaveAs("./rho_n2_2.png");
/*
	auto c3 = new TCanvas("c3","c3");
	auto legend3 = new TLegend(0.7,0.7,0.9,0.9);
	for (int j=1;j<=i;j++){
		h3[j]->Scale((double)xs[j]/h3[j]->Integral());
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
	*/
}
