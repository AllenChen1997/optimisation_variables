/*
	this code is used to get r plots with different selections
	the cuts: tauveto, photonveto, electron veto, muon veto, nAk4, nFatJ
	
	in this version we have 8 MCs, 1 data
	
	there exist bugs for letting code work for both MC and data at the same time.
	lets focus on data first
*/

using namespace std;

bool isMC;
bool isHalve; // entry = 0 use for fitting; entry = 1 used for N2B1 study
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

void runCode(int CutI=0,int nCut=6,bool isTest = false){
	// outputs // 	
	vector< TH1F* > h_met_mindphi_l(6); // the histo for mindphi > 0.4, 6 is for 6 path trigger regions
	vector< TH1F* > h_met_mindphi_s(6); // the histo for mindphi < 0.4
	vector< TH1F* > h_met_mindphi_l_(6); // _ means there is no SR cuts
	vector< TH1F* > h_met_mindphi_s_(6);
	vector< TH1F* > h_cutFlow(8); // cutFlows, 8 is the number of inputs
	vector< TH1F* > tmp_l(6);
	vector< TH1F* > tmp_s(6); 
	vector< TH1F* > tmp_l_(6); 
	vector< TH1F* > tmp_s_(6);
	vector< int > tmp_weight; // total 2 element, 0: withSRcut , 1: no SRcut
	string subline = "_withSR";
	Double_t xbins[16] = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500, 600, 1000};
	string label[6] = {"tauVeto","photonVeto","EleVeto","LooseMuVeto","extraAk4","nFatJ"};
	TH2F* h_map_l = new TH2F("map_l","",15,xbins,6,0,6);
	TH2F* h_map_s = (TH2F*) h_map_l->Clone("map_s");
	TH2F* h_map_l_ = (TH2F*) h_map_l->Clone("map_l_");
	TH2F* h_map_s_ = (TH2F*) h_map_l->Clone("map_s_");
	
	TH2F* h_DDB_N2 = new TH2F("DDB_N2","",20,0,0.5,20,0,1);
	setDrawOpt(h_DDB_N2,"","N2B1","DDB");

	for (int i=0; i< h_met_mindphi_l.size(); i++){
		h_met_mindphi_l[i] = new TH1F(Form("h_met_mindphi_l_withSR_%i",i),"",15,xbins);
		h_met_mindphi_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_s_withSR_%i",i));
		h_met_mindphi_l_[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_l_%i",i));
		h_met_mindphi_s_[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_s_%i",i));
		tmp_l[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_l_withSR_%i",i));
		tmp_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_s_withSR_%i",i));
		tmp_l_[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_l_%i",i));
		tmp_s_[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_s_%i",i));
		
		// call sumw2
		h_met_mindphi_l[i]->Sumw2();
		h_met_mindphi_s[i]->Sumw2();
		h_met_mindphi_l_[i]->Sumw2();
		h_met_mindphi_s_[i]->Sumw2();
		tmp_l[i]->Sumw2();
		tmp_s[i]->Sumw2();
		tmp_l_[i]->Sumw2();
		tmp_s_[i]->Sumw2();
	}	
	for (int i=0; i< h_cutFlow.size(); i++){
		h_cutFlow[i] = new TH1F(Form("h_cutFlow_%i",i),"",6,0,6);
	}
	
	// load files 
	string inputMClist[8] = {"QCD_100to200.root","QCD_200to300.root","QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	string outputMCname[8] = {"100to200","200to300","300to500","500to700","700to1000","1000to1500","1500to2000","2000toIng"};
	float xs[8] = {	23700000.0, 1547000.0, 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};
	string inputDatalist = "JetHT-Run2017B-31Mar2018-v1.root";
	string outputDataname = "JetHT-Run2017B-31Mar2018-v1";
	
	int totalFiles = 0;
	if (isMC) totalFiles = 8;
	else totalFiles = 1;
	for (int i=0; i<totalFiles; i++){
		// reset all vectors 
		for (int x=0; x< tmp_l.size(); x++){
			tmp_l[x]->Reset("ICESM");
			tmp_s[x]->Reset("ICESM");
			tmp_l_[x]->Reset("ICESM");
			tmp_s_[x]->Reset("ICESM");
		}
		h_map_l->Reset("ICESM");
		h_map_s->Reset("ICESM");
		h_map_l_->Reset("ICESM");
		h_map_s_->Reset("ICESM");
		tmp_weight = {0,0};
		TFile* fin;
		if (isMC){
			cout << "reading " << inputMClist[i] << endl;
			fin = new TFile(inputMClist[i].data(), "READONLY");
		} else {
			cout << "reading " << inputDatalist << endl;
			fin = new TFile(inputDatalist.data(), "READONLY");
		}
		TTreeReader data("tree",fin);
		TTreeReaderArray<bool> cuts(data,"cuts");
		TTreeReaderValue<float> mindphi(data,"mindphi");
		TTreeReaderValue<float> metpT(data,"metpT");
		TTreeReaderArray<float> DDB(data,"DDB");
		TTreeReaderArray<float> N2B1(data,"N2B1");
		TTreeReaderValue<int> whichHT(data,"whichHT");
		// TTreeReaderValue<int> weight(data,"weight");
		
		int total_entry = data.GetEntries(true);
		int jEntry= 0;
		while(data.Next() ){
			jEntry++;
			if (isTest && jEntry == 100) break; 
			if (jEntry % 1000000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			if (isHalve && jEntry%2 == 1) continue;
			// check cuts //
			if (*whichHT < 0 ) continue;
			if (isMC){
				if (jEntry%2 == 0) {
					// tmp_weight[1] += *weight;
					if (*mindphi > 0.4) tmp_l_[*whichHT]->Fill(*metpT);
					else tmp_s_[*whichHT]->Fill(*metpT);
				} else{
					bool ispass = true;
					for (int icut = CutI; icut < nCut; icut++){
						if (! cuts[icut]){
							ispass = false;
							break;
						}
						h_cutFlow[i]->Fill(icut);
					}
					if (! ispass ) continue;
					// if (! cuts[n-1]) continue;
					// tmp_weight[0] += *weight;
					if (*mindphi > 0.4) tmp_l[*whichHT]->Fill(*metpT);
					else tmp_s[*whichHT]->Fill(*metpT);
				}
			} else {
				bool ispass = true;
				for (int icut = CutI; icut < nCut; icut++){
					if (! cuts[icut]){
						ispass = false;
						break;
					}
					h_cutFlow[i]->Fill(icut);
				}
				if (! ispass ) continue;
				if (*mindphi > 0.4) tmp_l[*whichHT]->Fill(*metpT);
				else {
					tmp_s[*whichHT]->Fill(*metpT);
					h_DDB_N2->Fill(N2B1[0],DDB[0]);
				}
			}
		} // end of entries
		for (int k=0; k<h_met_mindphi_l.size(); k++){
			float calWeight[2] = {1,1};
			if (isMC) {
				calWeight[0] = xs[i] / (float) tmp_weight[0];
				calWeight[1] = xs[i] / (float) tmp_weight[1];
			}
			h_met_mindphi_l[k]->Add(tmp_l[k], calWeight[0]);
			h_met_mindphi_s[k]->Add(tmp_s[k], calWeight[0]);
			h_met_mindphi_l_[k]->Add(tmp_l_[k], calWeight[1]);
			h_met_mindphi_s_[k]->Add(tmp_s_[k], calWeight[1]);
			for (int nbin = 1; nbin<=15; nbin++){
				h_map_l->SetBinContent(nbin,k+1,tmp_l[k]->GetBinContent(nbin) );
				h_map_s->SetBinContent(nbin,k+1,tmp_s[k]->GetBinContent(nbin) );
				h_map_l_->SetBinContent(nbin,k+1,tmp_l_[k]->GetBinContent(nbin) );
				h_map_s_->SetBinContent(nbin,k+1,tmp_s_[k]->GetBinContent(nbin) );
			}
		}
		gStyle->SetOptStat("");
		TCanvas* c3 = new TCanvas("c3","c3");
		string tmpOutputName;
		if (isMC) tmpOutputName = outputMCname[i];
		else tmpOutputName = outputDataname;
		h_map_l->Draw("COLZ");
		c3->SaveAs(Form("map_l_withSR_%s.png",tmpOutputName.data() ) );
		h_map_s->Draw("COLZ");
		c3->SaveAs(Form("map_s_withSR_%s.png",tmpOutputName.data() ) );
		if (isMC) {
			h_map_l_->Draw("COLZ");
			c3->SaveAs(Form("map_l_noCut_%s.png",tmpOutputName.data() ) );
			h_map_s_->Draw("COLZ");
			c3->SaveAs(Form("map_s_noCut_%s.png",tmpOutputName.data() ) );
		}
	} // end of all files
		

	//TFile* fout = new TFile(Form("keep_histo_cut%i.root",n),"RECREATE");
	TFile* fout = new TFile("keep_histo_cutFull.root","RECREATE");
	TCanvas* c = new TCanvas("c","c");
	if (isMC) c->Divide(1,2,0,0);
	TH1F* htmp = (TH1F*) h_met_mindphi_l[0]->Clone("htmp");
	TH1F* htmp2 = (TH1F*) h_met_mindphi_l_[0]->Clone("htmp2");
	TH1F* htmp21 = (TH1F*) h_met_mindphi_l_[0]->Clone("htmp21");
	string output_cutInfo = "";
	if (nCut - CutI > 1) output_cutInfo = "FullCut";
	else output_cutInfo = Form("cut%i",CutI);
	for (int i = 0; i< h_met_mindphi_l.size() ;i++){
		if (isMC){
			c->cd(1);
			htmp->Divide(h_met_mindphi_l[i],h_met_mindphi_s[i] );
			setDrawOpt(htmp,"","MET","");
			htmp2->Divide(h_met_mindphi_l_[i],h_met_mindphi_s_[i] );
			htmp->Draw("HIST E TEXT0");
			htmp2->SetLineColor(kRed);
			htmp2->Draw("HIST E SAME");
			c->cd(2);
			htmp21->Reset("ICESM");
			htmp21->Add(htmp);
			htmp21->Add(htmp2,-1);
			setDrawOpt(htmp21,"","MET","");
			htmp21->Draw("HIST E");
		} else {
			htmp->Divide(h_met_mindphi_l[i], h_met_mindphi_s[i] );
			setDrawOpt(htmp,"","MET","");
			htmp->Draw("HIST E TEXT0");
		}
		c->SaveAs(Form("rFunction_MC_%s_%i.png",output_cutInfo.data(),i) );
		
		// save into root file
		htmp->SetName(Form("h_withSR_%i",i) );
		htmp->Write();
		if (isMC) {
			htmp2->SetName(Form("h_%i",i) );
			htmp2->Write();
		}
		
	}
	fout->Close();
	/*TCanvas* c2 = new TCanvas("c2","c2");
	for (int i=0; i<h_cutFlow.size(); i++){
		for (int j=0; j< (int)(sizeof(label)/sizeof(label[0]) ); j++){
			h_cutFlow[i]->GetXaxis()->SetBinLabel(j+1,label[j].data() );
		}
		h_cutFlow[i]->Draw("HIST TEXT0");
		c2->SaveAs(Form("cutFlow_%i.png",i));
	}*/
	TCanvas* c3 = new TCanvas("c3","c3");
	h_DDB_N2->Draw();
	c3->SaveAs("DDB_N2.png");

	
}
void DrawSRtest(bool setisMC , bool setisHalve = false, int cuti = 0, int ncut=6){
	// for (int i=1; i<=6; i++){
		// runCode(i,false);
	// }
	isMC = setisMC;
	isHalve = setisHalve;
	runCode(cuti, ncut);

}
