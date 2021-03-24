/*
	this code is build for letting trigger efficiency -> 100%
	we use off-line HT, trigger paths in tree
*/	
bool isTest = true;

using namespace std;
void draw_trigEff(string inputname){
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
	// get what path + prescale we need
	for (int i=0; i<11; i++){
		TH1F* tmph = (TH1F*) fin->Get(prefixterm+paths[i]);
		int nmaxBin = tmph->GetMaximumBin();
		LowEdge[paths[i]] = tmph->GetBinLowEdge(nmaxBin);
		HighEdge[paths[i]] = tmph->GetBinLowEdge(nmaxBin+1);
	}
	
	TH1F* hpass1;
	hpass1 = new TH1F(prefixterm+"_180","",120,0,1200);
	TH1F* hpass2[10];
	TH1F* hpass21[10];
	for (int i=0; i<10;i++){
		hpass2[i] = new TH1F(prefixterm+"_180_"+paths[i+1],"",120,0,1200);
		hpass21[i] = (TH1F*) hpass2[i]->Clone(prefixterm+paths[i+1]+"_divide");
	}
	int total_entry = myRead.GetEntries(true);
	int jEntry = 0;
	while(myRead.Next() ){
		jEntry++;
		//cout << "\r";
		if (isTest) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		else {
			if (jEntry % 10000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		}
		if (isTest) if(jEntry>100)break;
		vector<int> selectedList;
		for (int it=0; it<*nPassTrig; it++){ // we need to keep the most event prescales
			if (prescales[it] < LowEdge[passTrigs[it]] || prescales[it] > HighEdge[passTrigs[it]] ) continue;
			selectedList.push_back(passTrigs[it]);
		}
		auto ifound = find(selectedList.begin(),selectedList.end(),180);
		if (ifound != selectedList.end() ) {
			hpass1->Fill(*HT_);
			for(int i=0;i<10;i++){
				ifound = find(selectedList.begin(),selectedList.end(), paths[i+1]);
				if (ifound != selectedList.end() ) hpass2[i]->Fill(*HT_);
			}
		}
	} // end of all entries
	// output plots
	TCanvas* c = new TCanvas("c","c");
	for (int i=0;i<10;i++){
		hpass21[i]->Divide(hpass2[i],hpass1,1,1,"B");
		hpass21[i]->Draw();
		c->SaveAs(prefixterm+paths[i+1]+"_noMET.png");
	}
}
	