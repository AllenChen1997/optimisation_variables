/* 
	this code is used to study N2 and DDB correlation by real data
*/
template<typename T>
void setDrawOpt(T& h,string title, string xTitle, string yTitle){
	h->SetTitle(title.c_str());
	h->SetTitleSize(0.07);
	h->GetXaxis()->SetLabelSize(0.05);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetLabelSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->SetXTitle(xTitle.c_str());
	h->SetYTitle(yTitle.c_str());
}

float calculateWeight(vector<float> pars, float met){
	return TMath::Exp(pars[0] + pars[1]*met);
}

void getN2_DDB(string inputfile){
	// setup STLs
	vector< vector<float> > rFactor;
	//vector< TH1F* > h_met_mindphi_l(6); // the histo for mindphi > 0.4, 6 is for 6 path trigger regions
	//vector< TH1F* > h_met_mindphi_s(6); // the histo for mindphi < 0.4
	TH2F* h_DDB_N2_l = new TH2F("h_DDB_N2_l","",20,0,0.5,20,0,1);
	setDrawOpt(h_DDB_N2_l,"","N2B1","DDB");
	TH2F* h_DDB_N2_s = (TH2F*) h_DDB_N2_l->Clone("h_DDB_N2_s");


	// load transform vector //	
	TFile* fin = new TFile("fit_result.root","READONLY"); // this file name is default
	TTreeReader mytree("tree",fin);
	TTreeReaderArray<float> par(mytree,"par");
	
	while(mytree.Next() ){
		vector<float> tmp;
		for (auto x : par){
			tmp.push_back(x);
		}
		rFactor.push_back(tmp);
	}
	
	// input main real data //
	fin->Close();
	fin = new TFile(inputfile.data(), "READONLY");
	TTreeReader data("tree",fin);
	TTreeReaderArray<bool> cuts(data,"cuts");
	TTreeReaderValue<float> mindphi(data,"mindphi");
	TTreeReaderValue<float> metpT(data,"metpT");
	TTreeReaderArray<float> DDB(data,"DDB");
	TTreeReaderArray<float> N2B1(data,"N2B1");
	TTreeReaderValue<int> whichHT(data,"whichHT");
	int total_entry = data.GetEntries(true);
	int jEntry = 0;
	while(data.Next() ){
		jEntry++;
		if (jEntry%2 == 0) continue;
		
		// apply cuts
		bool ispass = true;
		for (int icut = 0; icut < 6; icut++){
			if (! cuts[icut]){
				ispass = false;
				break;
			}
		}
		if (! ispass ) continue;
		if (*metpT > 200) continue;
		if (*mindphi > 0.4) {
			h_DDB_N2_l->Fill(N2B1[0],DDB[0]);
		} else {
			float weight = calculateWeight(rFactor[(int) *whichHT],*metpT);
			h_DDB_N2_s->Fill(N2B1[0],DDB[0],weight);
		}
	}
	gStyle->SetOptStat("");
	TCanvas* c1 = new TCanvas("c1","c1");
	h_DDB_N2_l->Draw("CANDLE");
	c1->SaveAs("N2B1_DDB_l.png");
	h_DDB_N2_s->SetLineColor(kRed);
	h_DDB_N2_s->Draw("CANDLE SAME");
	c1->SaveAs("N2B1_DDB.png");
	h_DDB_N2_s->Draw("CANDLE");
	c1->SaveAs("N2B1_DDB_s.png");
	
}


