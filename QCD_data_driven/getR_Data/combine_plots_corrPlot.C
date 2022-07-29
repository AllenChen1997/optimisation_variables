/*
	this code is used to combine tow plots into one.
	also get the ratio between both as lower canvas
	
	limits: two plots with the same binning, same bin width for whole range
	here h1 is data, h2 is MC
	target histogram is n2b1 distribution
*/

using namespace std;
#define lumi 41000

int lookingHTRegion = 4; // from 1 to 5, [300, 500, 600, 750, 1200, 100000]
string default_input1 = Form("QCD_n2b1_realData.root");
string default_input2 = "../getR_MC/keep_histo_cutFull.root";
string default_hist1 = "h_DDB_N2_s";
string default_hist2 = Form("DDB_N2_%i",lookingHTRegion);

void combine_plots_corrPlot(string inputf1 = default_input1, string inputh1 = default_hist1, string inputf2 = default_input2, string inputh2 = default_hist2){
	TFile* fin1 = new TFile(inputf1.data(), "READONLY");
	TH1F* h1 = (TH1F*) fin1->Get(inputh1.data() );
	TFile* fin2 = new TFile(inputf2.data(), "READONLY");
	TH1F* h2 = (TH1F*) fin2->Get(inputh2.data() );
	
	h1->SetLineColor(kRed);
	//h1->Scale(1.0/ h1->Integral());
	//h2->Scale(lumi);
	gStyle->SetOptStat("");
	TCanvas* c = new TCanvas("c","c");
	THStack* hs = new THStack("hs","");
	//h1->Draw("AXIS");
	hs->Add(h1);
	hs->Add(h2);
	hs->Draw("CANDLE SAME");
	hs->GetXaxis()->SetTitle("N2B1");
	hs->GetYaxis()->SetTitle("DDB");
	hs->GetXaxis()->SetBinLabel(1,"");
	
	TLegend legend(0.8,0.8,1.0,1.0);
	legend.SetFillColor(0);
	legend.SetFillStyle(0);
	legend.SetLineWidth(0);
	legend.AddEntry(h1,"Data Driven");
	legend.AddEntry(h2,"MC");
	legend.Draw("SAME");
	
	TGaxis* axis1 = new TGaxis(0,0,0.5,0,0.0,0.5,20,"");
	axis1->Draw("SAME");
	
	c->SaveAs("combine_MC_Data_DDB_N2B1_corr.png");
}

