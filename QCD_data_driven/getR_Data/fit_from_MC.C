/* 
	this code is used to study N2 and DDB correlation by real data
	the fitting for now is only trying the last region
	there are three input files are needed, one from MC fit result, one from signal ratio histogram, 
	and the other is the tree from real data
*/

#define nDDB 20
#define minDDB 0
#define maxDDB 1

#define nN2 20
#define minN2 0
#define maxN2 0.5

int LookingRegion = 5; // 0 to 5 

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

Double_t fit_Exp_c(Double_t *x, Double_t* par){
	//return par[0]*TMath::Exp(par[1]*x[0]);
	return (TMath::Exp(par[0] + par[1]*x[0]) + par[2] ) * par[3];
}

vector<float> getResults(TF1* func){
	int Nparams = 4;
	cout << "Nparams: " << Nparams << endl;
	vector<float> params;
	for (int i=0; i < Nparams; i++){
		params.push_back(func->GetParameter(i) );
	}
	return params;
}

vector<float> getErrors(TF1* func){
	int Nparams = 4;
	vector<float> errors;
	for(int i=0; i< Nparams; i++){
		errors.push_back(func->GetParError(i) );
	}
	return errors;
}

float calculateWeight(vector<float> pars, float met){
	return (TMath::Exp(pars[0] + pars[1]*met) + pars[2] ) * pars[3];
}

void fit_from_MC(string inputfile, int ChoosedHTRegion = LookingRegion){
	cout << "looking HT range is " << ChoosedHTRegion << endl;
	// setup STLs
	vector< vector<float> > rFactor;
	//vector< TH1F* > h_met_mindphi_l(6); // the histo for mindphi > 0.4, 6 is for 6 path trigger regions
	//vector< TH1F* > h_met_mindphi_s(6); // the histo for mindphi < 0.4
	TH2F* h_DDB_N2_l = new TH2F("h_DDB_N2_l","",nN2,minN2,maxN2,nDDB,minDDB,maxDDB);
	setDrawOpt(h_DDB_N2_l,"","N2B1","DDB");
	TH2F* h_DDB_N2_s = (TH2F*) h_DDB_N2_l->Clone("h_DDB_N2_s");

	TH1F* h_DDB_l = new TH1F("h_DDB_l","",nDDB,minDDB,maxDDB);
	setDrawOpt(h_DDB_l,"","DDB","");
	TH1F* h_DDB_s = (TH1F*) h_DDB_l->Clone("h_DDB_s");
	TH1F* h_N2_l = new TH1F("h_N2_l","",nN2,minN2,maxN2);
	setDrawOpt(h_N2_l,"","N2B1","");
	TH1F* h_N2_s = (TH1F*) h_N2_l->Clone("h_N2_s");

	// counter for test //
	int nWeightGt1 = 0; // try to see the weight number
	int nWeightLe1 = 0;
	TH1F* h_weight = new TH1F("h_weight","",10,0,1);

	// load transform vector //	
	TFile* fin = new TFile("./fit_result.root","READONLY"); // this file name is default
	//TFile* fin = new TFile("../../fit_result.root","READONLY"); // this file name is default
	TTreeReader mytree("tree",fin);
	TTreeReaderArray<float> par(mytree,"par");
	
	while(mytree.Next() ){
		vector<float> tmp;
		for (auto x : par){
			tmp.push_back(x);
		}
		rFactor.push_back(tmp);
	}
	
	for (auto x : rFactor){
		for (auto y : x ) {
			cout << y << " ";
		}
		cout << endl;
	}
	
	// fit again to get ratio a as the diff. b/w sig. and MC
	TF1* func = new TF1("func",fit_Exp_c,0,200,4); // bound min and max, free params
	cout << "rFactor size: " << rFactor[ChoosedHTRegion].size() << endl;
	for (int i=0; i<rFactor[ChoosedHTRegion].size(); i++){
		func->FixParameter(i,rFactor[ChoosedHTRegion][i]);
	}
	
	fin->Close();
	fin = new TFile("./keep_histo_cutFull.root","READONLY");
	//fin = new TFile("../../keep_histo_cutFull.root","READONLY");
	TH1F* h = (TH1F*) fin->Get(Form("h_withSR_%i",ChoosedHTRegion) );
	h->Fit("func","R");
	Double_t chi2 = func->GetChisquare();
	vector<float>  params = getResults(func);
	vector<float>  errors = getErrors(func);	
	cout << "chisquare : " << chi2 << endl;
		gStyle->SetOptStat("");
	// draw the fit result
	TText* t = new TText(.1*h->GetXaxis()->GetXmax(),.9*h->GetMaximum(),Form("chi2 %f",chi2) );
	TCanvas* c0 = new TCanvas("c0","c0");
	h->Draw();
	t->Draw("SAME");
	c0->SaveAs("fit_again.png");
	
	// input main real data //
	fin->Close();
	fin = new TFile(inputfile.data(), "READONLY");
	TTreeReader data("tree",fin);
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
		
		//if (*metpT > 200) continue;
		if (*whichHT != ChoosedHTRegion) continue;
		if (*mindphi > 0.4) {
			h_DDB_N2_l->Fill(N2B1[0],DDB[0]);
			h_DDB_l->Fill(DDB[0]);
			h_N2_l->Fill(N2B1[0]);
		} else {
			float weight = calculateWeight(params,*metpT);
			//float weight = 1;
			if (weight > 1 ) nWeightGt1++;
			else nWeightLe1++;
			h_DDB_N2_s->Fill(N2B1[0],DDB[0],weight);
			h_DDB_s->Fill(DDB[0],weight);
			h_N2_s->Fill(N2B1[0],weight);
			h_weight->Fill(weight);
		}
	}

	TFile* fout = new TFile("QCD_n2b1_realData.root","RECREATE");
	h_N2_l->Write();
	h_N2_s->Write();
	h_DDB_l->Write();
	h_DDB_s->Write();
	fout->Close();
	
	TCanvas* c1 = new TCanvas("c1","c1");
	h_DDB_N2_l->Draw("CANDLE");
	c1->SaveAs("N2B1_DDB_l_withMC.png");
	h_DDB_N2_s->SetLineColor(kRed);
	h_DDB_N2_s->Draw("CANDLE SAME");
	c1->SaveAs("N2B1_DDB_withMC.png");
	h_DDB_N2_s->Draw("CANDLE");
	c1->SaveAs("N2B1_DDB_s_withMC.png");
	cout << "check h_DDB_N2_s " << endl;
	cout << "nEvent = " << h_DDB_N2_s->Integral()<< endl;
	cout << " h_DDB_s: " << h_DDB_s->Integral() << endl;
	cout << " h_N2_s: " << h_N2_s->Integral() << endl;
	
	TLegend legend(0.7,0.7,0.9,0.9);
	legend.AddEntry(h_DDB_l,"mindphi > 0.4");
	legend.AddEntry(h_DDB_s,"mindphi < 0.4");
	
	TCanvas* c2 = new TCanvas("c2","c2");
	h_DDB_s->Scale(1.0 / (float)h_DDB_s->Integral() );
	h_DDB_s->SetLineColor(kRed);
	h_DDB_s->Draw("HISTE");
	h_DDB_l->Scale(1.0 / (float)h_DDB_l->Integral() );
	h_DDB_l->Draw("HISTE SAME");
	legend.Draw("SAME");
	c2->SaveAs("DDB_both_withMC.png");

	h_N2_s->Scale(1.0 / (float) h_N2_s->Integral() );
	h_N2_s->SetLineColor(kRed);
	h_N2_s->Draw("HISTE");
	h_N2_l->Scale(1.0 / (float) h_N2_l->Integral() );
	h_N2_l->Draw("HISTE SAME");
	legend.Draw("SAME");
	c2->SaveAs("N2_both_withMC.png");
	
	// show the static result
	cout << "nWeightGt1 = " << nWeightGt1 << endl;
	cout << "nWeightLe1 = " << nWeightLe1 << endl;
	new TCanvas;
	h_weight->Draw();
}


