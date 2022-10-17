/*
	this code is build for letting trigger efficiency -> 100%
	we use off-line HT, trigger paths in tree
*/	

using namespace std;

int paths[11] = {180, 250, 350, 370, 430, 510, 590, 680, 780, 890, 1050};
int myLowEdge[11] = {-1, -1, -1, 300, 150, -1, -1, -1, -1, -1, -1};


bool isTest = false;

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

void draw_trigEff_manual_adjust(string inputname, string outputname){
	TFile* fin = new TFile(inputname.data(),"READONLY");
	TTreeReader myRead("tree",fin);
	TTreeReaderValue<int> nPassTrig(myRead,"nPassTrig");
	TTreeReaderValue<float> HT_MET(myRead,"HT_MET");
	TTreeReaderValue<float> HT_(myRead,"HT_noMET");
	TTreeReaderArray< int > passTrigs(myRead,"passTrigList");
	TTreeReaderArray< int > prescales(myRead,"prescaleList");
	
	ofstream textout("prescaleRich.txt",std::ofstream::app);
	TString prefixterm = "PFHT";
	map<int, int> LowEdge;
	// get what path + prescale we need
	for (int i=0; i<11; i++){
		TH1F* tmph = (TH1F*) fin->Get(prefixterm+paths[i]);
		int nmaxBin = tmph->GetMaximumBin();
		if (myLowEdge[i] == -1){
			LowEdge[paths[i]] = tmph->GetBinLowEdge(nmaxBin);
		} else {
			LowEdge[paths[i]] = myLowEdge[i];
		}
	}
	
	if (isTest){
		for (auto x : LowEdge) cout << x.first << " : " << x.second << endl;
	}
	textout << inputname.data() << ":" << endl;
	for (auto x : LowEdge) {
		textout << x.first << " : " << x.second << endl;
	}
	textout.close();
	
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
			if (prescales[it] != LowEdge[passTrigs[it]] ) continue;
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
		for (int i=0; i<sizeof(hpass1)/sizeof(hpass1[0]) ;i++){
			hpass21[i]->Divide(hpass2[i],hpass1[i],1,1,"B");
			//hpass21[i]->Draw();
			//c->SaveAs(prefixterm+paths[i+1]+"_noMET.png");
			hpass21[i]->Write();
		}
		fout->Close();
	}
}
	