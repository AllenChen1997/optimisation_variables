/* 
	this code is used to draw R function by divide the denominator and numerator histogram
*/

using namespace std;

void quickDrawRfunction(bool isTest = false){
	string inputlist[8] = {"QCD_100to200.root","QCD_200to300.root","QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	float xs[8] = {23700000.0, 1547000.0, 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};
	TH1F* h_l;
	TH1F* h_s;
	cout << "sample : weight : xs" << endl;
	TCanvas* c0 = new TCanvas("c0","c0");
	for (int i = 0; i< sizeof(xs)/sizeof(xs[0]); i++){
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
		cout << inputlist[i] << " : " << weight << " : " << xs[i] << endl;
		float h_weight = xs[i] / weight;
		h_l->Add(tmp_l,h_weight);
		h_s->Add(tmp_s,h_weight);
		if (isTest){
			tmp_l->Draw();
			c0->SaveAs(Form("%s.png",inputlist[i].data()));
		}
	}
	TH1F* htmp = (TH1F*) h_l->Clone("tmp");
	htmp->Clear();
	htmp->Divide(h_l,h_s,1,1,"B");
	htmp->GetXaxis()->SetRangeUser(0,550);
	TCanvas* c = new TCanvas("c","c");
	htmp->Draw();
	c->SaveAs("rFunction_MC.png");
	if (isTest){
		//if (h_l->GetMaximum() < h_s->GetMaximum() ) h_l->SetMaximum(h_s->GetMaximum()*1.1);
		h_l->Draw();
		c->SaveAs("dphi_large.png");
		h_s->SetLineColor(kRed);
		h_s->Draw();
		c->SaveAs("dphi_small.png");
	}
}
