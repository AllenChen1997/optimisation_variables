using namespace std;
bool isTest = false;
void setdefault(TH1F* h, int scaleN, int colorN, float& maxN){
	//h->GetXaxis()->SetRangeUser(0,1500); 
	h->SetLineColor(colorN);
	h->Scale(scaleN);
	float nmax = h->GetMaximum();
	if (nmax > maxN) maxN = nmax;
}
void HT_Draw2(){
	// setup draw color //
	int paths[11] = {180, 250, 350, 370, 430, 510, 590, 680, 780, 890, 1050};
	map<int, int> colormap;
	for (int i=0; i<sizeof(paths)/sizeof(paths[0]) ; i++){
		colormap[paths[i] ] = i;
	}
	float hmax = 0; // the maximum of histograms
	TFile* fin;
	fin = TFile::Open("trig_collect_result.root");
	TDirectory* current_sourcedir = gDirectory;
	TIter nextkey(current_sourcedir->GetListOfKeys() );
	TKey* key;
	vector<TH1F*> vh;
	while((key = (TKey*)nextkey() )){
		TObject* obj = key->ReadObj();
		if (obj->IsA()->InheritsFrom("TH1") ){
			//cout << "histogram " << obj->GetName() << endl;
			// lets focus on HT( no MET )
			string histname = obj->GetName();
			if (histname.find("HTnoMET") == string::npos) continue;
			stringstream ss(histname);
			string sline;
			int countN = 0;
			TString line;
			while(getline(ss,sline,'_')) {
				line[countN] = string tmpline(sline);
				countN++;
				if (countN == 2) break;
			}
			ss.clear();
			line[0].ReplaceAll("PFHT","");
			int ipath = line[0].Atoi();
			line[1].ReplaceAll("pre","");
			int ipre = line[1].Atoi();
			TH1F* h = (TH1F*) fin->FindObjectAny(obj->GetName());
			setdefault(h,ipre,colormap[ipath],hmax);
			vh.push_back(h);
		}// end of loop obj keys
	}
	cout << "we have " << vh.size() << " histograms "<< endl;
	for (auto x : vh) cout << x->GetName() << endl;
}