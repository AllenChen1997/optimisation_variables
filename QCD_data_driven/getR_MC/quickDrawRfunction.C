/* 
	this code is used to draw R function by divide the denominator and numerator histogram
*/

using namespace std;

void quickDrawRfunction_(int num,bool isTest = false){
	string inputlist[8] = {"QCD_100to200.root","QCD_200to300.root","QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	float xs[8] = {23700000.0,1547000.0, 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};
	TH1F* h_l[2];
	TH1F* h_s[2];
	cout << "sample : weight : xs" << endl;
	TCanvas* c0 = new TCanvas("c0","c0");
	for (int i = 0; i< sizeof(xs)/sizeof(xs[0]); i++){
		TFile* fin = new TFile(inputlist[i].data(),"READONLY");
		TH1F* tmp_l = (TH1F*) fin->Get(Form("h_met_mindphi_l_%i",num) );
		TH1F* tmp_l_withSR = (TH1F*) fin->Get(Form("h_met_mindphi_l_withSR_%i",num) );
		TH1F* tmp_s = (TH1F*) fin->Get(Form("h_met_mindphi_s_%i",num) );
		TH1F* tmp_s_withSR = (TH1F*) fin->Get(Form("h_met_mindphi_s_withSR_%i",num) );
		TH1F* tmp_weight = (TH1F*) fin->Get("h_mc_weight");
		TH1F* tmp_weight_withSR = (TH1F*) fin->Get("h_mc_weight_withSR");
		float weight = tmp_weight->Integral();
		float weight_withSR = tmp_weight_withSR->Integral();
		cout << inputlist[i] << " : " << weight << " : " << xs[i] << endl;
		float h_weight = xs[i] / weight;
		float h_weight_withSR = xs[i] / weight_withSR;
		if (i == 0){
			h_l[0] = (TH1F*) tmp_l->Clone(Form("h_l_%i",num) );
			h_l[0]->Scale(h_weight);
			h_s[0] = (TH1F*) tmp_s->Clone(Form("h_s_%i",num) );
			h_s[0]->Scale(h_weight);
			
			h_l[1] = (TH1F*) tmp_l_withSR->Clone(Form("h_l_withSR_%i",num) );
			h_l[1]->Scale(h_weight_withSR);
			h_s[1] = (TH1F*) tmp_s_withSR->Clone(Form("h_s_withSR_%i",num) );
			h_s[1]->Scale(h_weight_withSR);
		} else {
		h_l[0]->Add(tmp_l,h_weight);
		h_s[0]->Add(tmp_s,h_weight);
		h_l[1]->Add(tmp_l_withSR,h_weight_withSR);
		h_s[1]->Add(tmp_s_withSR,h_weight_withSR);

		}
		if (isTest){
			tmp_l->Draw();
			c0->SaveAs(Form("%s.png",inputlist[i].data()));
		}
	}
	TH1F* htmp = (TH1F*) h_l[0]->Clone(Form("h_divirde_%i",num) );
	TH1F* htmp2 = (TH1F*) h_l[1]->Clone(Form("h_divirde_withSR_%i",num) );
	//htmp->Clear();
	htmp->Divide(h_l[0],h_s[0],1,1,"B");
	htmp2->SetXTitle("METpT");
	htmp2->Divide(h_l[1],h_s[1],1,1,"B");
//	htmp->GetXaxis()->SetRangeUser(0,550);
	gStyle->SetOptStat("");
	TCanvas* c = new TCanvas("c","c");
	c->Divide(1,2,0,0);
	c->cd(1);
	htmp->Draw();
	htmp2->SetLineColor(kRed);
	htmp2->Draw("SAME");
	TLegend* legend = new TLegend(.7,.7,.9,.9);
	legend->AddEntry(htmp,"MC no SRcut","l");
	legend->AddEntry(htmp2,"MC with SRcut","l");
	legend->Draw();
	c->cd(2);
	TH1F* htmp3 = (TH1F*) htmp->Clone("tmp3");
	htmp3->Divide(htmp,htmp2,1,1,"");
	htmp3->Draw();
	c->SaveAs(Form("rFunction_MC_%i.png",num) );
	TFile* fout = new TFile("MCout.root","update");
	htmp->Write();
	htmp2->Write();
	fout->Close();
	if (isTest){
		//if (h_l->GetMaximum() < h_s->GetMaximum() ) h_l->SetMaximum(h_s->GetMaximum()*1.1);
		h_l[0]->Draw();
		c->SaveAs("dphi_large.png");
		h_s[0]->SetLineColor(kRed);
		h_s[0]->Draw();
		c->SaveAs("dphi_small.png");
	}
}

void quickDrawRfunction(){
 for(int i =0; i<6;i++){
	quickDrawRfunction_(i);
}
}
