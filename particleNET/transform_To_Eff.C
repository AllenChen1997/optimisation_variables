/* 
	this code is used to convert pass counts per cut into selection eff alone cut flow
*/
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>

using namespace std;

bool noBin1InH1 = true;

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

void transform_To_Eff(string input1, string histo1, string input2 = "", string histo2 = "", string outputname = "", bool printall = true){ // if only input1 and histo1 is ok
	bool notEmpty = false;
	if (input2 != "") notEmpty = true;
	TH1F* h1 = geth(input1, histo1); // original cut Flow histo in input1
	TH1F* h2;
	if (notEmpty) h2 = geth(input2, histo2); // original cut Flow histo in input2
	int Nbin = h1->GetNbinsX();
	if (noBin1InH1) {
		h1 = copyh(h1,2,Nbin);
		Nbin -= 1;
	}
	if (notEmpty){ 
		if (Nbin < h2->GetNbinsX() ) Nbin = h2->GetNbinsX(); // comparing which one has more bins
	}
	TH1F* h_eff1 = geth_eff(h1, Nbin);
	TH1F* h_eff2;
	if (notEmpty) h_eff2 = geth_eff(h2, Nbin);
	
	gStyle->SetOptStat("");
	TCanvas* c0 = new TCanvas("c0","c0");
	c0->SetLogy();
	if (notEmpty){
		h2->SetLineColor(kRed);
		if (h1->GetMaximum() > h2->GetMaximum() ){
			h1->Draw();
			h2->Draw("SAME");
		} else {
			h2->Draw();
			h1->Draw("SAME");
		}
	} else {
		h1->Draw();
	}
	c0->SaveAs(Form("%s_cutFlow.png",outputname.data() ) );
	
	TCanvas* c1 = new TCanvas("c1","c1");
	if (notEmpty) {
		h_eff2->SetLineColor(kRed);
		if (h_eff1->GetMaximum() > h_eff2->GetMaximum() ) {
			h_eff1->Draw();
			h_eff2->Draw("SAME");
		} else {
			h_eff2->Draw();
			h_eff1->Draw("SAME");
		}
	} else {
		h_eff1->Draw();
	}
	c1->SaveAs(Form("%s_eff.png",outputname.data() ) );
	
	// print out eff and eff error in screen
	if (printall){
		cout << "print out sel. eff. and errors" << endl;
		cout << "-----------------------------" << endl;
		for (int i=1; i<Nbin; i++) cout << "bin" << i << "  |  ";
		cout << endl;
		for (int i=1; i<Nbin; i++){
			cout << h_eff1->GetBinContent(i) << "     ";
		}
		cout << endl;
		for (int i=1; i<Nbin; i++){
			cout << h_eff1->GetBinError(i) << "      ";
		}
		cout << endl;
		if (notEmpty) {
			for (int i=1; i<Nbin; i++){
				cout << h_eff2->GetBinContent(i) << "     ";
			}
			cout << endl;
			for (int i=1; i<Nbin; i++){
				cout << h_eff2->GetBinError(i) << "      ";
			}
			cout << endl;
		}
	}
}	

	