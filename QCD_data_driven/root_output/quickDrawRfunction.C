/* 
	this code is used to draw R function by divide the denominator and numerator histogram
*/

using namespace std;

void quickDrawRfunction(bool isTest = false){
	string inputlist[8] = {"QCD_100to200.root","QCD_200to300.root","QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	string titles[8] = {"HT100to200", "HT200to300", "HT300to500", "HT500to700", "HT700to1000", "HT1000to1500", "HT1500to2000", "HT2000toInf"};
	float xs[8] = {23700000.0, 1547000.0, 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};
	TH1F* h_l;
	TH1F* h_s;
	TH1F* h_HT;
	TH1F* h_dphi;
	cout << "sample : weight : xs" << endl;
	TCanvas* c0 = new TCanvas("c0","c0");
	for (int i = 0; i< sizeof(xs)/sizeof(xs[0]); i++){
		TFile* fin = new TFile(inputlist[i].data(),"READONLY");
		TH1F* tmp_l = (TH1F*) fin->Get("h_met_mindphi_l");
		TH1F* tmp_s = (TH1F*) fin->Get("h_met_mindphi_s");
		TH1F* tmp_weight = (TH1F*) fin->Get("h_mc_weight");
		TH1F* tmp_HT = (TH1F*) fin->Get("h_HT");
		TH1F* tmp_dphi = (TH1F*) fin->Get("h_dphi");
		if (i == 0){
			h_l = (TH1F*) tmp_l->Clone("h_l");
			h_l->Clear();
			h_s = (TH1F*) h_l->Clone("h_s");
			h_HT = (TH1F*) tmp_HT->Clone("total_h_HT");
			h_HT->Clear();
			h_dphi = (TH1F*) tmp_dphi->Clone("total_h_dphi");
			h_dphi->Clear();
		}
		float weight = tmp_weight->Integral();
		cout << inputlist[i] << " : " << weight << " : " << xs[i] << endl;
		float h_weight = xs[i] / weight;
		h_l->Add(tmp_l,h_weight);
		h_s->Add(tmp_s,h_weight);
		h_HT->Add(tmp_HT,h_weight);
		h_dphi->Add(tmp_dphi,h_weight);
		if (isTest){
			//tmp_HT->GetXaxis()->SetRangeUser(0,600);
			tmp_HT->SetTitle(titles[i].data());
			tmp_HT->Draw();
			c0->SaveAs(Form("%s_HT.png",inputlist[i].data()));
		}
	}
	TH1F* htmp = (TH1F*) h_l->Clone("tmp");
	htmp->Clear();
	htmp->Divide(h_l,h_s,1,1,"B");
	//htmp->GetXaxis()->SetRangeUser(0,550);
	TCanvas* c = new TCanvas("c","c");
	htmp->Draw();
	c->SaveAs("rFunction_MC.png");
	
	TCanvas* c2 = new TCanvas("c2","c2");
	h_HT->GetXaxis()->SetRangeUser(0,1000);
	h_HT->Draw();
	c2->SaveAs("HT.png");
	
	c2->SetLogy();
	c2->SaveAs("HT_log.png");
	
	TCanvas* c3 = new TCanvas("c3","c3");
	h_dphi->Draw();
	c3->SaveAs("dphi.png");
	/*
	if (isTest){
		//if (h_l->GetMaximum() < h_s->GetMaximum() ) h_l->SetMaximum(h_s->GetMaximum()*1.1);
		h_l->Draw();
		c->SaveAs("dphi_large.png");
		h_s->SetLineColor(kRed);
		h_s->Draw();
		c->SaveAs("dphi_small.png");
	}*/
}
