/*
	this code is used to combine tow plots into one.
	also get the ratio between both as lower canvas
	
	limits: two plots with the same binning, same bin width for whole range
	here h1 is data, h2 is MC
	target histogram is n2b1 distribution
	try to use TProfile to draw the plots
*/

using namespace std;
#define lumi 41000

int lookingHTRegion = 4;
 // from 1 to 5, [300, 500, 600, 750, 1200, 100000]
string default_input1 = Form("QCD_n2b1_realData.root");
string default_input2 = "../getR_MC/keep_histo_cutFull.root";
string default_hist1 = "h_DDB_N2_s";
string default_hist2 = Form("DDB_N2_%i",lookingHTRegion);

void combine_plots_corrPlot_profileVer(string inputf1 = default_input1, string inputh1 = default_hist1, string inputf2 = default_input2, string inputh2 = default_hist2){
	TFile* fin1 = new TFile(inputf1.data(), "READONLY");
	TH2F* h1 = (TH2F*) fin1->Get(inputh1.data() );
	TFile* fin2 = new TFile(inputf2.data(), "READONLY");
	TH2F* h2 = (TH2F*) fin2->Get(inputh2.data() );
	
	cout << "data integral " << h1->Integral() << endl;
	cout << "MC integral " << h2->Integral() << endl;
	
	TProfile* hpx1 = h1->ProfileX();
	TProfile* hpx2 = h2->ProfileX();
	
	gStyle->SetOptStat("");
	TCanvas* c = new TCanvas("c","c");
	hpx1->SetLineColor(kRed);
	hpx1->Draw();
	hpx2->Draw("SAME");
	TLegend legend(0.8,0.8,1.0,1.0);
	legend.SetFillColor(0);
	legend.SetFillStyle(0);
	legend.SetLineWidth(0);
	legend.AddEntry(hpx1,"Data Driven");
	legend.AddEntry(hpx2,"MC");
	legend.Draw("SAME");
	
	// TGaxis* axis1 = new TGaxis(0,0,0.5,0,0.0,0.5,20,"");
	// axis1->Draw("SAME");
	
	c->SaveAs("combine_MC_Data_DDB_N2B1_corr_profile.png");
}

