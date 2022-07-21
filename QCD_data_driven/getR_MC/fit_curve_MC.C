/*
	this code is used for fit the curve for getting the low MET region curve
*/

using namespace std;

vector<float> getResults(TF1* func){
	int Nparams = func->GetNumberFreeParameters();
	vector<float> params;
	for (int i=0; i < Nparams; i++){
		params.push_back(func->GetParameter(i) );
	}
	return params;
}

vector<float> getErrors(TF1* func){
	int Nparams = func->GetNumberFreeParameters();
	vector<float> errors;
	for(int i=0; i< Nparams; i++){
		errors.push_back(func->GetParError(i) );
	}
	return errors;
}

Double_t fit_Exp(Double_t *x, Double_t* par){
	//return par[0]*TMath::Exp(par[1]*x[0]);
	return TMath::Exp(par[0] + par[1]*x[0]);
}

Double_t fit_Exp_c(Double_t *x, Double_t* par){
	//return par[0]*TMath::Exp(par[1]*x[0]);
	return TMath::Exp(par[0] + par[1]*x[0]) + par[2];
}

Double_t fit_poly3(Double_t *x, Double_t* par){
	return par[0] + par[1] * x[0] + par[2] * x[0] * x[0] + par[3] * x[0] * x[0] * x[0];
}

vector<float> runfit(string input, string histname){
	TFile* fin = new TFile(input.data(), "READONLY");
	TH1F* h = (TH1F*)fin->Get(histname.data());
	h->Draw();
	//TF1* func = new TF1("func",fit_Exp,0,1000,2); // bound min and max, free params
	TF1* func = new TF1("func",fit_Exp_c,0,1000,3); // bound min and max, free params
	func->SetParLimits(2,0,5);
	//func->SetParLimits(0,0,1);
	//TF1* fPoly = new TF1("fPoly",fit_poly3,0,1000,4); 
	
	h->Fit("func");
	Double_t chi2 = func->GetChisquare();
	vector<float>  params = getResults(func);
	vector<float>  errors = getErrors(func);
	
	/*h->Fit("fPoly");
	Double_t chi2_poly = fPoly->GetChisquare();
	vector<float> params_poly = getResults(fPoly);
	vector<float> errors_poly = getErrors(fPoly);
	*/
	
	  // show the result //
	int count = 0 ;
	cout << " the results " << endl;
	for (auto x : params){
		cout << x << " : error : " << errors[count] << endl;
		count++;
	}
	// cout << " result for poly " << endl;
	// show the result for poly
	/*count = 0;
	for (auto x : params_poly){
		cout << x << " : error : " << errors_poly[count] << endl;
		count++;
	}
	h->GetYaxis()->SetRangeUser(0,5);*/
	gStyle->SetOptStat("");
	gStyle->SetOptFit(1111);
	TCanvas* c1 = new TCanvas("c1","c1");
	//if (h->GetMaximum() > 10 ) h->GetYaxis()->SetRangeUser(0,3);
	
	h->Draw();
	//func->SetLineColor(kBlue);
	//func->Draw("SAME");
	
	c1->SaveAs(Form("fit_%s.png",histname.data() ) );
	
	return params;
}

void fit_curve_MC(string input = "keep_histo_cutFull.root", string output = "fit_result.root", bool isTest = false){
	TFile* fout = new TFile(output.data(), "RECREATE");
	vector<float> par;
	TTree ot("tree","fit_results");
	ot.Branch("par",&par);
	if (isTest){
		par = runfit(input,"h_withSR_5");
		ot.Fill();
	} else {
		for (int i=0; i<6; i++){
			par.clear();
			par = runfit(input,Form("h_withSR_%i",i) );
			ot.Fill();
		}
	}
	fout->cd();
	ot.Write();
	fout->Close();
}
	
	