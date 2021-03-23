/*
	this code is build for letting trigger efficiency -> 100%
	we use off-line HT, trigger paths in tree
*/	
bool isTest = false;

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
	
	TH1F* hpass1[10];
	TH1F* hpass12[10];
	for (int i=0; i<10;i++){
		hpass1[i] = new TH1F(prefixterm+"_"+paths[i],"",120,0,1200);
		hpass12[i] = new TH1F(prefixterm+"_"+paths[i]+"_"+paths[i+1],"",120,0,1200);
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
		for(int i=0;i<10;i++){
			auto ifound = find(selectedList.begin(),selectedList.end(),paths[i]);
			if (ifound != selectedList.end() ) {
				hpass1[i]->Fill(*HT_);
				ifound = find(selectedList.begin(),selectedList.end(), paths[i+1]);
				if (ifound != selectedList.end() ) hpass12[i]->Fill(*HT_);
			}
		}
	} // end of all entries
	// output plots
	TCanvas* c = new TCanvas("c","c");
	for (int i=0;i<10;i++){
		hpass12[i]->Divide(hpass1[i]);
		hpass12[i]->Draw();
		c->SaveAs(prefixterm+paths[i]+"_noMET.png");
	}
}
	