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

#define NN2 20
#define lumi 41000
//#define MinN2 -0.2 // this is for n2DDT
#define MinN2 0
#define MaxN2 0.5
#define NN 7 //the number of input files

using namespace std;
void plot(string inputf = "QCD_list_for_plot.txt"){
	// define the STL contianers
	TH1D* h_n2b1[NN+1];
	TH1D* h_n2b2[NN+1];
	TH1D* h_rho[NN+1];
	TH1D* h_jet1pt[NN+1];
	TH2D* h_rhoN2b1 = new TH2D("rho-N2b1","rho-N2b1",10,-5,0,10,0,0.5);
  	TH2D* h_rhoN2b2 = new TH2D("rho-N2b2","rho-N2b2",10,-5,0,10,0,0.5);
  	TH1D* n2b1_1 = new TH1D("n2b1_1","rho<-2",10,0,0.5); // collect the case where rho < -2
  	TH1D* n2b1_2 = new TH1D("n2b1_2","rho>-2",10,0,0.5); // collect the case where rho > -2

	ifstream infile(inputf.data() ); // used to input file. in each line: xxx.root name_for_plot
	string line,s1,s2;  // s1 is file name, s2 is legend name in plot
	float s3; // s3 is xs
	stringstream ss;
	
	vector<string> fname;
	vector<float> fxs;
	vector<float> ftotalevent;
	
	int i=0;
	while(getline(infile,line)){
		// deal with the input lines
		cout << line << endl;
		ss << line;
		ss >> s1 >> s2 >> s3 ;// s1=xxx.root, s2=name_for_plot, s3 = xs
		ss.clear();
		
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		TTreeReaderValue< Double_t > pt(myRead,"Jet1Pt");
		
		// add for make the pre-selections
		TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
		TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
		TTreeReaderValue< Double_t > nj(myRead,"nJets");
		TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
		
		int N = myRead.GetEntries(); //get the entires info.
		if (N == 0) {
			cout << s1 << " is empty!! " << endl;
			continue;
		}
		cout << "entries = " << N << endl;
		h_n2b1[i] = new TH1D(Form("FJetN2b1_%s",s2.c_str()),Form("FJetN2b1_%s",s2.c_str()),NN2,MinN2,MaxN2 ); // i start from 0
		h_n2b2[i] = new TH1D(Form("FJetN2b2_%s",s2.c_str()),Form("FJetN2b2_%s",s2.c_str()),NN2,MinN2,MaxN2 );
		h_rho[i] = new TH1D(Form("FJetrho_%s",s2.c_str()),Form("FJetrho_%s",s2.c_str()),10,-5,0 );
		h_jet1pt[i] = new TH1D(Form("Jet1Pt_%s",s2.c_str()),Form("Jet1Pt_%s",s2.c_str()),1000,0,800);
		
		TH1F* h_totalEvent = (TH1F*) myfile->Get("h_total_mcweight");
		ftotalevent.push_back(h_totalEvent->Integral() );
		
		while (myRead.Next()){
			// apply the lost 3 cuts //
			//if ( *mass < 100 || *mass > 150) continue;
			if ( *nj > 2 ) continue;
			if ( *dphi < 0.4 ) continue;
			
			h_n2b1[i]->Fill(*n2b1);
			h_n2b2[i]->Fill(*n2b2);
			h_rho[i]->Fill(*rho);
			if (*pt > -9999) h_jet1pt[i]->Fill(*pt);// there is some case pt will be -9999, like no pt
			h_rhoN2b1->Fill(*rho,*n2b1);
			h_rhoN2b2->Fill(*rho,*n2b2);
			if (*rho > -2) n2b1_1->Fill(*n2b1);
			else n2b1_2->Fill(*n2b1);
		}
		fname.push_back(s2);
		fxs.push_back(s3);
		i++; // count for the files with non zero entries in branch
	} // end of loop all lines in file list, and i is total files which is not empty.(now is constant)
	
	TFile* fout = new TFile("QCD_n2b1_plot.root","RECREATE");
	cout << "i = " << i << endl;
	TH1F* h_n2b1_total = (TH1F*) h_n2b1[1]->Clone("h_n2b1_total"); 
	h_n2b1_total->Reset("ICESM");
	for (int j=0; j<i; j++){
		h_n2b1[j]->Write();
		h_n2b1_total->Add(h_n2b1[j],lumi*fxs[j]/(float)ftotalevent[j]);
	}
	h_n2b1_total->Write();
	fout->Close();
	
	gStyle->SetOptStat("");
	//gPad->SetLogy(1);
	auto c0 = new TCanvas("c0","c0");
	auto legend = new TLegend(0.7,0.7,0.9,0.9);
	
	float maxY = 0;
	float imax;
	int m;

	for (int j=0;j<i;j++){
		h_jet1pt[j]->Scale(1.0/h_jet1pt[j]->Integral());
		h_jet1pt[j]->SetLineWidth(3);
		h_jet1pt[j]->Draw("SAME PLC");
		legend->AddEntry(h_jet1pt[j],Form("%s",fname[j].c_str()),"l");
	}
	legend->Draw();
	c0->SaveAs("./pt_plot.png");
	
	auto c1 = new TCanvas("c1","c1");
	auto legend1 = new TLegend(0.8,0.7,1.0,0.9);
	maxY = 0;
	for (int j=0;j<i;j++){
		h_n2b1[j]->Scale(1.0/h_n2b1[j]->Integral());
		imax = (float)h_n2b1[j]->GetBinContent(h_n2b1[j]->GetMaximumBin());
		if (maxY < imax ) {
			maxY=imax;
			m=j;
		}
	}
	TAxis* xaxis1 = h_n2b1[m]->GetXaxis();
	xaxis1->SetTitle("N_2(beta=1)");
	xaxis1->SetTitleSize(0.04);
	xaxis1->SetLabelSize(0.05);
	h_n2b1[m]->SetTitleOffset(1.1,"X");
	h_n2b1[m]->GetYaxis()->SetLabelSize(0.05);
	h_n2b1[m]->SetTitle("");
	h_n2b1[m]->Draw();
	
	for (int j=0;j<i;j++){
		h_n2b1[j]->SetLineWidth(3);
		h_n2b1[j]->Draw("SAME PLC");
		legend1->AddEntry(h_n2b1[j],Form("%s",fname[j].c_str()),"l");
	}
	legend1->Draw();
	c1->SaveAs("./n2b1_plot.png");
	
	auto c2 = new TCanvas("c2","c2");
	auto legend2 = new TLegend(0.7,0.7,0.9,0.9);
	maxY = 0;
	for (int j=0;j<i;j++){
		h_n2b2[j]->Scale((double)1.0/h_n2b2[j]->Integral());
		imax = h_n2b2[j]->GetBinContent(h_n2b2[j]->GetMaximumBin());
		if (maxY < imax ) {
			maxY=imax;
			m=j;
		}
	}
	TAxis* xaxis2 = h_n2b2[m]->GetXaxis();
	xaxis2->SetTitle("N_2(beta=2)");
	xaxis2->SetTitleSize(0.04);
	xaxis2->SetLabelSize(0.05);
	h_n2b2[m]->SetTitleOffset(1.1,"X");
	h_n2b2[m]->SetTitle("");
	h_n2b2[m]->GetYaxis()->SetLabelSize(0.05);
	h_n2b2[m]->Draw();
	for (int j=0;j<i;j++){
		h_n2b2[j]->Scale(1.0/h_n2b2[j]->Integral());
		h_n2b2[j]->SetLineWidth(3);
		h_n2b2[j]->Draw("SAME PLC");
		legend2->AddEntry(h_n2b2[j],Form("%s",fname[j].c_str()),"l");
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
	
	// output datas
	for (auto x : fname ) cout << x << " ";
	cout << endl;
	for (auto x : fxs ) cout << x << " ";
	cout << endl;
	for (auto x : ftotalevent){
		cout << x << " ";
	}
	cout << endl;

}
