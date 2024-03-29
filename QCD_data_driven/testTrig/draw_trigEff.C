/*
	this code is build for letting trigger efficiency -> 100%
	we use off-line HT, trigger paths in tree
*/	

using namespace std;

bool isTest = false;
bool useNoTrigResCut = true; // if applied, there is no fixed most event rich pre-scale

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

void draw_trigEff(string inputname, string outputname){
	TFile* fin = new TFile(inputname.data(),"READONLY");
	TTreeReader myRead("tree",fin);
	TTreeReaderValue<int> nPassTrig(myRead,"nPassTrig");
	TTreeReaderValue<float> HT_MET(myRead,"HT_MET");
	TTreeReaderValue<float> HT_(myRead,"HT_noMET");
	TTreeReaderArray< int > passTrigs(myRead,"passTrigList");
	TTreeReaderArray< int > prescales(myRead,"prescaleList");
	
	TString prefixterm = "PFHT";
	int paths[11] = {180, 250, 350, 370, 430, 510, 590, 680, 780, 890, 1050};
	map<int, int> LowEdge;
	map<int, int> HighEdge;
	map<int, int> LowEdge_;  // with underscope mean there is no trigger result cut (= 1)
	map<int, int> HighEdge_;
	// get what path + prescale we need
	for (int i=0; i<11; i++){
		if (! useNoTrigResCut || isTest){
			TH1F* tmph = (TH1F*) fin->Get(prefixterm+paths[i]);
			int nmaxBin = tmph->GetMaximumBin();
			LowEdge[paths[i]] = tmph->GetBinLowEdge(nmaxBin);
			HighEdge[paths[i]] = tmph->GetBinLowEdge(nmaxBin+1);
		}
		if (useNoTrigResCut || isTest){
			TH1F* tmph2 = (TH1F*) fin->Get(prefixterm+paths[i]+"_");
			int nmaxBin = tmph2->GetMaximumBin();
			LowEdge_[paths[i]] = tmph2->GetBinLowEdge(nmaxBin);
			HighEdge_[paths[i]] = tmph2->GetBinLowEdge(nmaxBin+1);
		}
	}
	
	if (isTest){
		for (auto x : LowEdge) cout << x.first << " : " << x.second << endl;
		cout << "without trigger result cut" << endl;
		for (auto x : LowEdge_) cout << x.first << " : " << x.second << endl;
	}
	
	TH1F* hpass1[10];
	TH1F* hpass2[10];
	TH1F* hpass21[10];
	for (int i=0; i<10;i++){
		hpass1[i] = new TH1F(prefixterm+paths[i+1]+"_180","",30,0,1500);
		hpass2[i] = new TH1F(prefixterm+"_"+paths[i+1],"",30,0,1500);
		hpass21[i] = (TH1F*) hpass2[i]->Clone(prefixterm+paths[i+1]+"_divide");
	}
	int total_entry = myRead.GetEntries(true);
	int jEntry = 0;
	while(myRead.Next() ){
		jEntry++;
		//show the status of run loop
		if (isTest) {
			if (jEntry>100) break;
			cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		}
		else {
			if (jEntry % 10000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		}
		
		vector<int> selectedList; // the list of prescales which in the 
		for (int it=0; it<*nPassTrig; it++){ // we need to keep the most event prescales
			if (! useNoTrigResCut){
				if (prescales[it] < LowEdge[passTrigs[it]] || prescales[it] > HighEdge[passTrigs[it]] ) continue;
			} else {
				if (prescales[it] < LowEdge_[passTrigs[it]] || prescales[it] > HighEdge_[passTrigs[it]] ) continue;
			}
			selectedList.push_back(passTrigs[it]);
		}
		for (int i=0; i<sizeof(paths)/sizeof(paths[0])-1 ; i++){
			auto ifound = find(selectedList.begin(),selectedList.end(),180); // start from the second one, the first one is denominator
			if (ifound != selectedList.end() ) {
				hpass1[i]->Fill(*HT_);
				ifound = find(selectedList.begin(),selectedList.end(), paths[i+1]);
				if (ifound != selectedList.end() ) hpass2[i]->Fill(*HT_);
			}
		}
	} // end of all entries
	// output plots
	if( ! isTest) {
		//TCanvas* c = new TCanvas("c","c");
		TFile* fout = new TFile(outputname.data(),"RECREATE");
		for (int i=0; i<sizeof(paths)/sizeof(paths[0]) ;i++){
			hpass21[i]->Divide(hpass2[i],hpass1[i],1,1,"B");
			//hpass21[i]->Draw();
			//c->SaveAs(prefixterm+paths[i+1]+"_noMET.png");
			hpass21[i]->Write();
		}
		fout->Close();
	}
}
	