/* 
	this code is used to getting r = min(dphi(met,ak4j)) > 0.4 / min(dphi(met,ak4j)) < 0.4 
	as a function of met
	using ExoPieElement output dataTemplate
*/
using namespace std;

float cal_dphi(float phi1, float phi2){
	float dphi = phi1 - phi2;
	float pi = TMath::Pi();
	while (dphi >= pi) dphi -= 2*pi;
	while (dphi < -pi) dphi += 2*pi;
	return TMath::Abs(dphi);
}

void GetRfunction(string inputfile, string outfile, bool isTest = false){
	
	// things to output //
	TH1F* h_met_mindphi_l = new TH1F("h_met_mindphi_l","",60,0,600);
	TH1F* h_met_mindphi_s = (TH1F*) h_met_mindphi_l->Clone("h_met_mindphi_s");
	TH1F* h_mc_weight = new TH1F("h_mc_weight","",2,0,2);
	
	TFile* fin;
	fin = TFile::Open(inputfile.data(), "READONLY");
	TTreeReader data("tree/treeMaker",fin);
	TTreeReaderValue< Int_t > nVtx(data, "nVtx");
	//TTreeReaderArray< string > trigName(data, "hlt_trigName");
	//TTreeReaderArray< bool > trigResult(data, "hlt_trigResult");
	//TTreeReaderArray< int > prescale(data, "hlt_trigPrescale");
	  // MET //
	TTreeReaderValue< Float_t > METPT(data, "pfMetCorrPt");
	TTreeReaderValue< Float_t > METPhi(data, "pfMetCorrPhi");
	  // thinJet //
	TTreeReaderValue< int > THINnJet(data, "THINnJet");
	TTreeReaderArray< float > THINjetPx(data, "THINjetPx");
	TTreeReaderArray< float > THINjetPy(data, "THINjetPy");
	TTreeReaderArray< float > THINjetPz(data, "THINjetPz");
	TTreeReaderArray< float > THINjetEnergy(data, "THINjetEnergy");
	  // N2B1 //
	TTreeReaderArray< float > N2B1(data, "FATN2_Beta1_");
	
	int total_entry = data.GetEntries(true);
	int jEntry = 0;
	while (data.Next() ){
		// make event process counter //
		jEntry++;
		if (isTest) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		else {
			if (jEntry % 1000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
		}
		if (isTest) if(jEntry>100)break;
		h_mc_weight->Fill(1);
		//0. has a good vertex
		if(*nVtx<1)continue;

		//1. ak4Jet
		vector<TLorentzVector> ak4_eta_3p0_pt_20; 
		for (int ijet = 0; ijet < *THINnJet; ijet++){
			TLorentzVector tmpTL;
			tmpTL.SetPxPyPzE(THINjetPx[ijet],THINjetPy[ijet],THINjetPz[ijet],THINjetEnergy[ijet]);
			float absjetEta = TMath::Abs(tmpTL.Eta() );
			float jetPt = tmpTL.Pt();
			if (absjetEta >= 3.0) continue;
			if (jetPt <= 20) continue;
			// collect ak4 jet pass pt, eta cut. for getting delta phi 
			ak4_eta_3p0_pt_20.push_back(tmpTL);
		}
		//2. apply mindphi
		float mindphi = 999;
		for (auto x : ak4_eta_3p0_pt_20){
			float tmpdPhi = cal_dphi(*METPhi,x.Phi() );
			if (mindphi > tmpdPhi) mindphi = tmpdPhi;
		}
		//cout << "mindphi" << mindphi << endl;
		if (mindphi > 0.4) h_met_mindphi_l->Fill(*METPT);
		else h_met_mindphi_s->Fill(*METPT);
	}
	TFile* fout = new TFile(outfile.data(),"RECREATE");
	h_met_mindphi_s->Write();
	h_met_mindphi_l->Write();
	h_mc_weight->Write();
	fout->Close();
}