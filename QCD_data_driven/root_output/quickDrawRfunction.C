/* 
	this code is used to draw R function by divide the denominator and numerator histogram
*/

using namespace std;

void quickDrawRfunction(){
	string inputlist[5] = {"QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	float xs[5] = {32100, 6831, 1207, 119.9, 25.24};
	TH1F* h_l;
	TH1F* h_s;
	for (int i = 0; i< 5; i++){
		TFile* fin = new TFile(inputlist[i].data(),"READONLY");
		TH1F* tmp_l = (TH1F*) fin->Get("h_met_mindphi_l");
		TH1F* tmp_s = (TH1F*) fin->Get("h_met_mindphi_s");
		TH1F* tmp_weight = (TH1F*) fin->Get("h_mc_weight");
		if (i == 0){
			h_l = (TH1F*) tmp_l->Clone("h_l");
			h_l->Clear();
			h_s = (TH1F*) h_l->Clone("h_s");
		}
		float weight = tmp_weight->Integral();
		h_l->Add(tmp_l,xs[i]/weight);
		h_s->Add(tmp_s,xs[i]/weight);
	}
	TH1F* htmp = (TH1F*) h_l->Clone("tmp");
	htmp->Clear();
	htmp->Divide(h_l,h_s,1,1,"B");
	htmp->GetXaxis()->SetRangeUser(0,550);
	TCanvas* c = new TCanvas("c","c");
	htmp->Draw();
	c->SaveAs("rFunction_MC.png");
}