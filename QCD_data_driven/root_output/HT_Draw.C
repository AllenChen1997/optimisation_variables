using namespace std;
void setdefault(TH1F* h, int scaleN, int colorN, float& maxN){
	h->GetXaxis()->SetRangeUser(0,1500);
	h->SetLineColor(colorN);
	h->Scale(scaleN);
	float nmax = h->GetMaximum();
	if (nmax > maxN) maxN = nmax;
}
void HT_Draw(){
	// choose backfix name for MET / HT(with/without) plots //
	TString backfix = "_MET";
	//TString backfix = "_HTnoMET";
	//TString backfix = "_HTwMET";
	int colors[7] = {1,632,416,800,880,600,432}; // kBlack,kRed,kGreen,kOrange,kViolet,kBlue,kCyan
	TFile* fin;
	fin = TFile::Open("trig_collect_result.root");
	
	TH1F* h180 = (TH1F*) fin->Get("PFHT180_pre1"+backfix);
	TH1F* h250 = (TH1F*) fin->Get("PFHT250_pre1000"+backfix);
	TH1F* h350 = (TH1F*) fin->Get("PFHT350_pre1000"+backfix);
	TH1F* h370 = (TH1F*) fin->Get("PFHT370_pre300"+backfix);
	TH1F* h430 = (TH1F*) fin->Get("PFHT430_pre150"+backfix);
	TH1F* h510 = (TH1F*) fin->Get("PFHT510_pre75"+backfix);
	TH1F* h590 = (TH1F*) fin->Get("PFHT590_pre30"+backfix);
	TH1F* h680 = (TH1F*) fin->Get("PFHT680_pre30"+backfix);
	TH1F* h780 = (TH1F*) fin->Get("PFHT780_pre30"+backfix);
	TH1F* h890 = (TH1F*) fin->Get("PFHT890_pre30"+backfix);
	TH1F* h1050 = (TH1F*) fin->Get("PFHT1050_pre1"+backfix);
	
	// set up hist
	float maxN = 0;
	setdefault(h180,1,1,maxN);
	setdefault(h250,1000,2,maxN);
	setdefault(h350,1000,3,maxN);
	setdefault(h370,300,4,maxN);
	setdefault(h430,150,5,maxN);
	setdefault(h510,75,6,maxN);
	setdefault(h590,30,7,maxN);
	setdefault(h680,30,8,maxN);
	setdefault(h780,30,9,maxN);
	setdefault(h890,30,11,maxN);
	setdefault(h1050,1,12,maxN);
	// draw
	TCanvas* c = new TCanvas("c","c");
	c->SetLogy();
	h180->SetMaximum(maxN*1.1);
	h180->Draw("HIST");
	h250->Draw("SAME HIST");
	h350->Draw("SAME HIST");
	h370->Draw("SAME HIST");
	h430->Draw("SAME HIST");
	h510->Draw("SAME HIST");
	h590->Draw("SAME HIST");
	h680->Draw("SAME HIST");
	h780->Draw("SAME HIST");
	h890->Draw("SAME HIST");
	h1050->Draw("SAME HIST");
	gStyle->SetOptStat("");
	TLegend* t = new TLegend(0.7,0.7,0.9,0.9);
	t->AddEntry(h180,"PFHT180_pre1");
	t->AddEntry(h250,"PFHT250_pre1000");
	t->AddEntry(h350,"PFHT350_pre1000");
	t->AddEntry(h370,"PFHT370_pre5");
	t->AddEntry(h430,"PFHT430_pre5");
	t->AddEntry(h510,"PFHT510_pre75");
	t->AddEntry(h590,"PFHT590_pre30");
	t->AddEntry(h680,"PFHT680_pre30");
	t->AddEntry(h780,"PFHT780_pre30");
	t->AddEntry(h890,"PFHT890_pre30");
	t->AddEntry(h1050,"PFHT1050_pre1");
	t->Draw();
	
	c->SaveAs(backfix+".png");
}