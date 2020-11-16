/* 
	this code is used to convert pass counts per cut into selection eff alone cut flow
*/
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <THStack.h>

#define showsinN 5 // this is used when printall is true
using namespace std;

bool noBin1InH1 = false;
bool printall = true;
TH1F* geth(string inputname, string histoname){
	TFile* fin;
	fin = TFile::Open(inputname.data() );
	TH1F* h = (TH1F*) fin->Get(histoname.data());
	return h;
}
TH1F* copyh(TH1F* h, int startbin, int endbin){
	int N = endbin-startbin+1;
	TH1F* htmp = new TH1F("","",N,0,N);
	for (int i=1; i<= N; i++){
		htmp->SetBinContent(i,h->GetBinContent(startbin-1+i) );
		htmp->SetBinError(i,h->GetBinError(startbin-1+i) );
		htmp->GetXaxis()->SetBinLabel(i,h->GetXaxis()->GetBinLabel(startbin-1+i) );
	}
	return htmp;
}

TH1F* geth_eff(TH1F*h, int Nbins){
	TH1F* h1 = copyh(h,1,Nbins-1); // total
	TH1F* h2 = copyh(h,2,Nbins); // pass
	for (int i=1; i<Nbins; i++){
		h1->GetXaxis()->SetBinLabel(i,h->GetXaxis()->GetBinLabel(i+1) );
	}
	TH1F* h_eff = (TH1F*) h1->Clone("h_eff");
	h_eff->Reset();
	h_eff->Divide(h2,h1,1,1,"B");
	return h_eff;
}

void transform_To_Eff(string input1, string histo1, string input2 = "", string histo2 = "", string input3 = "", string histo3 = "", string outputname = "", string legendname1 = "",string legendname2 = "", string legendname3 = "" ){ // if only input1 and histo1 is ok
	bool notEmpty = false;
	bool notEmpty2 = false;
	if (input2 != "") notEmpty = true;
	if (input3 != "") notEmpty2 = true;
	TH1F* h1 = geth(input1, histo1); // original cut Flow histo in input1
	TH1F* h2;
	TH1F* h3;
	if (notEmpty){
		h2 = geth(input2, histo2); // original cut Flow histo in input2
		if (notEmpty2) h3 = geth(input3, histo3);
	}
	int Nbin = h1->GetNbinsX();
	if (noBin1InH1) {
		h1 = copyh(h1,2,Nbin);
		Nbin -= 1;
	}
	if (notEmpty){ 
		if (Nbin < h2->GetNbinsX() ) Nbin = h2->GetNbinsX(); // comparing which one has more bins
		if (notEmpty2) {
			if (Nbin < h3->GetNbinsX() ) Nbin = h3->GetNbinsX();
		}
	}
	TH1F* h_eff1 = geth_eff(h1, Nbin);
	TH1F* h_eff2;
	TH1F* h_eff3;
	if (notEmpty) {
		h_eff2 = geth_eff(h2, Nbin);
		if (notEmpty2) h_eff3 = geth_eff(h3, Nbin);
	}
	
	gStyle->SetOptStat("");
	TCanvas* c0 = new TCanvas("c0","c0");
	TLegend* l0 = new TLegend(0.7,0.7,0.9,0.9);
	c0->SetLogy();
	THStack* hs0 = new THStack("hs0","");
	hs0->Add(h1);
	l0->AddEntry(h1,legendname1.data(),"l");
	if (notEmpty){
		h2->SetLineColor(kRed);
		l0->AddEntry(h2,legendname2.data(),"l");
		hs0->Add(h2);
		if (notEmpty2){
			h3->SetLineColor(kGreen);
			l0->AddEntry(h3,legendname3.data(),"l");
			hs0->Add(h3);
		}
	}
	hs0->Draw("nostack");
	if (legendname1 != "") l0->Draw();
	c0->SaveAs(Form("%s_cutFlow.png",outputname.data() ) );
	
	TCanvas* c1 = new TCanvas("c1","c1");
	THStack* hs1 = new THStack("hs1","");
	hs1->Add(h_eff1);
	if (notEmpty) {
		h_eff2->SetLineColor(kRed);
		hs1->Add(h_eff2);
		if (notEmpty2){
			h_eff3->SetLineColor(kGreen);
			hs1->Add(h_eff3);
		}
	}
	hs1->Draw("nostack");
	if (legendname1 != "") l0->Draw();
	c1->SaveAs(Form("%s_eff.png",outputname.data() ) );
	
	// print out eff and eff error in screen
	if (printall){
		cout << "print out sel. eff. and errors" << endl;
		cout << "-----------------------------" << endl;
		// check the label name length
		int s_size = 0;
		for (int i=1; i<Nbin; i++){
			string s_name = h1->GetXaxis()->GetBinLabel(i);
			if (s_name.size() > s_size) s_size = s_name.size();
		}
		// print them per 5 bins
		int k = 0;
		int N;
		while (k>=0){
			if (showsinN*(k+1) < Nbin) N = showsinN;
			else if (showsinN*(k+1) == Nbin) break;
			else N = Nbin - showsinN*k ;
			for (int i=1; i<=N; i++) cout << setw(s_size) << h1->GetXaxis()->GetBinLabel(i+showsinN*k) << " | ";
			cout << endl;
			
			for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff1->GetBinContent(i+showsinN*k) << "   ";
			cout << endl;
			
			for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff1->GetBinError(i+showsinN*k) << "   ";
			cout << endl;
			
			if (notEmpty) {
				for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff2->GetBinContent(i+showsinN*k) << "   ";
				cout << endl;
				
				for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff2->GetBinError(i+showsinN*k) << "   ";
				cout << endl;
			}
			if (notEmpty2) {
				for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff3->GetBinContent(i+showsinN*k) << "   ";
				cout << endl;
				
				for (int i=1; i<=N; i++)	cout << setw(s_size) << h_eff3->GetBinError(i+showsinN*k) << "   ";
				cout << endl;
			}
			if (N<5) break;
			else k++;
		}
	}
}	

	