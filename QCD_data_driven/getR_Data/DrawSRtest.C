/*
	this code is used to get r plots after the cuts below
	the cuts in cutFlowLabel
	
	in this version we have 8 MCs, 1 data
	
	there exist bugs for letting code work for both MC and data at the same time.
	lets focus on data first
	the data name and number of HT regions  need to adjust by manually

*/

using namespace std;


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

void DrawSRtest(bool isTest = false){
	// load files 
	
	string inputDatalist = "JetHT-Run2017B-31Mar2018-v1.root";
	string outputDataname = "JetHT-Run2017B-31Mar2018-v1";
	
	int totalFiles = 1;
	
	// outputs // 	
		// decide the number of output regions
	int totalNRange = 0;
	TFile* fin_0 = new TFile(inputDatalist.data(), "READONLY");
	TH1F* h_HTRange = (TH1F*) fin_0->Get("h_HTRange");
	totalNRange = h_HTRange->GetNbinsX() + 1;
	
	vector< TH1F* > h_met_mindphi_l(totalNRange); // the histo for mindphi > 0.4, 6 is for 6 path trigger regions
	vector< TH1F* > h_met_mindphi_s(totalNRange); // the histo for mindphi < 0.4
	vector< TH1F* > tmp_l(totalNRange); // used in each file, after loop all entries. append to h_met_mindphi_l
	vector< TH1F* > tmp_s(totalNRange); 
	vector< TH1F* > h_n2b1(totalNRange);  // this is used to collect the N2b1 plot in different HT regions
	vector< TH1F* > tmp_n2b1(totalNRange); // this is used to get N2b1 plot in different HT regions in each file
	
	Double_t xbins[16] = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500, 600, 1000};
	string cutFlowLabel[11] = {"incl", "HTcut", "passTrig", "tauVeto", "photonVeto", "EleVeto", "LooseMuVeto", "extraAk4", "nFatJ", "hasN2B1", "atleast1AK4"};
	TH2F* h_map_l = new TH2F("map_l","",15,xbins,6,0,6);
	TH2F* h_map_s = (TH2F*) h_map_l->Clone("map_s");
	
	TH2F* h_DDB_N2 = new TH2F("DDB_N2","",20,0,0.5,20,0,1);
	setDrawOpt(h_DDB_N2,"","N2B1","DDB");
	TH2F* tmp_DDB_N2 = (TH2F*) h_DDB_N2->Clone("tmp_DDB_N2");

	for (int i=0; i< h_met_mindphi_l.size(); i++){
		h_met_mindphi_l[i] = new TH1F(Form("h_met_mindphi_l_withSR_%i",i),"",15,xbins);
		h_met_mindphi_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_s_withSR_%i",i));
		tmp_l[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_l_withSR_%i",i));
		tmp_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("tmp_s_withSR_%i",i));
		h_n2b1[i] = new TH1F(Form("h_n2b1_withHT_%i",i),"",20,0,0.5);
		tmp_n2b1[i] = (TH1F*) h_n2b1[i]->Clone(Form("tmp_n2b1_withHT_%i",i));
		// call sumw2
		h_met_mindphi_l[i]->Sumw2();
		h_met_mindphi_s[i]->Sumw2();
		tmp_l[i]->Sumw2();
		tmp_s[i]->Sumw2();
	}	
	
	
	for (int iFile=0; iFile<totalFiles; iFile++){
		// reset all vectors 
		for (int x=0; x< tmp_l.size(); x++){
			tmp_l[x]->Reset("ICESM");
			tmp_s[x]->Reset("ICESM");
			tmp_n2b1[x]->Reset("ICESM");
		}
		h_map_l->Reset("ICESM");
		h_map_s->Reset("ICESM");
		tmp_DDB_N2->Reset("ICESM");
		
		TFile* fin;
		cout << "reading " << inputDatalist << endl;
		fin = new TFile(inputDatalist.data(), "READONLY");
		
		TTreeReader data("tree",fin);
		TTreeReaderValue<float> mindphi(data,"mindphi");
		TTreeReaderValue<float> metpT(data,"metpT");
		TTreeReaderArray<float> DDB(data,"DDB");
		TTreeReaderArray<float> N2B1(data,"N2B1");
		TTreeReaderValue<int> whichHT(data,"whichHT");
		TH1F* h_cutFlow = (TH1F*) fin->Get("h_cutFlow");
		
		int total_entry = data.GetEntries(true);
		int jEntry= 0;
		int nN2B1gt1 = 0;
		while(data.Next() ){
			// deal with the entries in the tree
			jEntry++;
			if (isTest && jEntry == 100) break; // in test mode we will have only 100 events
			if (jEntry % 1000000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			
			if (N2B1.GetSize() > 1) nN2B1gt1++;
			

			tmp_DDB_N2->Fill(N2B1[0],DDB[0]); // this is only applied the first 6 cuts
			
			if (*mindphi > 0.4) {
				tmp_l[*whichHT]->Fill(*metpT);
				tmp_n2b1[*whichHT]->Fill(N2B1[0]);
			} else {
				tmp_s[*whichHT]->Fill(*metpT);
			}
			
			
		} // end of entries
		cout << "nN2B1gt1= " << nN2B1gt1 << endl;

		for (int k=0; k<h_met_mindphi_l.size(); k++){

			h_met_mindphi_l[k]->Add(tmp_l[k]);
			h_met_mindphi_s[k]->Add(tmp_s[k]);
			h_n2b1[k]->Add(tmp_n2b1[k]);
			
			for (int nbin = 1; nbin<=15; nbin++){
				h_map_l->SetBinContent(nbin,k+1,tmp_l[k]->GetBinContent(nbin) );
				h_map_s->SetBinContent(nbin,k+1,tmp_s[k]->GetBinContent(nbin) );
			}
		}
		h_DDB_N2->Add(tmp_DDB_N2);
		
		
		gStyle->SetOptStat("");
		string tmpOutputName;
		tmpOutputName = outputDataname;	
		// draw the cutflow plot
		TCanvas* c2 = new TCanvas("c2","c2");
		for (int ilabel=0; ilabel<sizeof(cutFlowLabel)/sizeof(cutFlowLabel[0]); ilabel++){
			h_cutFlow->GetXaxis()->SetBinLabel(ilabel+1,cutFlowLabel[ilabel].data() );	
		}
		h_cutFlow->Draw();
		c2->SaveAs(Form("cutFlow_%s.png",tmpOutputName.data() ) );
		
		// draw the map of mindphi larger than 0.4 or smaller 0.4
		TCanvas* c3 = new TCanvas("c3","c3");
		h_map_l->Draw("COLZ");
		c3->SaveAs(Form("map_l_withSR_%s.png",tmpOutputName.data() ) );
		h_map_s->Draw("COLZ");
		c3->SaveAs(Form("map_s_withSR_%s.png",tmpOutputName.data() ) );
	} // end of all files
		

	//TFile* fout = new TFile(Form("keep_histo_cut%i.root",n),"RECREATE");
	TFile* fout = new TFile("keep_histo_cutFull.root","RECREATE");
	TCanvas* c = new TCanvas("c","c");
	TH1F* htmp = (TH1F*) h_met_mindphi_l[0]->Clone("htmp"); // to get ratio plot of mindphi ? over mindphi <

	for (int i = 0; i< h_met_mindphi_l.size() ;i++){
		
		htmp->Divide(h_met_mindphi_l[i], h_met_mindphi_s[i] );
		setDrawOpt(htmp,"","MET","");
		htmp->Draw("HIST E TEXT0");

		c->SaveAs(Form("rFunction_MC_%i.png",i) );
		
		// save into root file
		htmp->SetName(Form("h_withSR_%i",i) );
		htmp->Write();

		h_n2b1[i]->Write();
	}
	fout->Close();
	

	
		// draw DDB_ N2 plot 
	TCanvas* c3 = new TCanvas("c3","c3");
	h_DDB_N2->Draw("CANDLE");
	c3->SaveAs("DDB_N2.png");

	
}
