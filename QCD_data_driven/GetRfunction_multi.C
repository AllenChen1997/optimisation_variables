/* 
	this code is used to getting r = min(dphi(met,ak4j)) > 0.4 / min(dphi(met,ak4j)) < 0.4 
	as a function of met
	using ExoPieElement output dataTemplate
*/
using namespace std;
float pi = TMath::Pi();

float cal_dphi(float phi1, float phi2){
	float dphi = phi1 - phi2;
	while (dphi >= pi) dphi -= 2*pi;
	while (dphi < -pi) dphi += 2*pi;
	return TMath::Abs(dphi);
}

bool pt_greater(const TLorentzVector& a, const TLorentzVector& b){
	return (a.Pt() > b.Pt());
}

void GetRfunction_multi(string inputfile, string outfile, bool isTest = false){
	Double_t xbins[16] = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500, 600, 1000};
	int HTUseRange[7] = {200, 300, 500, 600, 750, 1200, 100000};
	int totalNRange = sizeof(HTUseRange) / sizeof(HTUseRange[0]);
	// things to output //
	TH1F* h_met_mindphi_l[6];
	TH1F* h_met_mindphi_s[6];
	TH1F* h_MET_[6];
	//TH1F* h_MET_HTcut = (TH1F*) h_MET_->Clone("h_HTcut_MET");
	TH1F* h_mc_weight = new TH1F("h_mc_weight","",2,0,2);
	TH1F* h_dphi[6];
	TH1F* h_HT = new TH1F("h_HT","",200,0,2000);
	for (int i=0; i< sizeof(h_met_mindphi_l)/sizeof(h_met_mindphi_l[0]); i++){
		h_met_mindphi_l[i] = new TH1F(Form("h_met_mindphi_l_%i",i),"",15,xbins);
		h_met_mindphi_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_s_%i",i));
		h_dphi[i] =  new TH1F(Form("h_dphi_%i",i),"",10,0,pi);
		h_MET_[i] = new TH1F(Form("h_no_HTcut_MET_%i",i),"",150,0,1500);
	}
	// load file list //
	string line;
	ifstream flist(inputfile.data() );
	while (getline(flist,line) ){
		cout << "loading file: " << line << endl;
		TFile* fin;
		fin = TFile::Open(line.data(), "READONLY");
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
		  // fat jet //
		TTreeReaderValue< Int_t > nfj(data, "FATnJet");
		TTreeReaderArray< Float_t > fatJPx(data, "FATjetPx");
		TTreeReaderArray< Float_t > fatJPy(data, "FATjetPy");
		TTreeReaderArray< Float_t > fatJPz(data, "FATjetPz");
		TTreeReaderArray< Float_t > N2B1(data, "FATN2_Beta1_"); // N2B1
		TTreeReaderArray< Float_t > DDB(data, "FATjet_probHbb"); // DDB
		  // gen particle //
		TTreeReaderValue< Int_t > nPar(data, "nGenPar");
		TTreeReaderArray< Int_t > parState(data, "genParSt");
		TTreeReaderArray< Int_t > parPID(data, "genParId");
		TTreeReaderArray< Int_t > parMotherID(data, "genMomParId");
		  // Electron //
		TTreeReaderValue< Int_t > nEle(data, "nEle");
		TTreeReaderArray< Float_t > elePx(data, "elePx");
		TTreeReaderArray< Float_t > elePy(data, "elePy");
		TTreeReaderArray< Float_t > elePz(data, "elePz");
		TTreeReaderArray< Float_t > eleE(data, "eleEnergy");
		TTreeReaderArray< bool > eleLooseID(data, "eleIsPassLoose");
		  // Muon //
		TTreeReaderValue< Int_t > nMu(data, "nMu");
		TTreeReaderArray< Float_t > muPx(data, "muPx");
		TTreeReaderArray< Float_t > muPy(data, "muPy");
		TTreeReaderArray< Float_t > muPz(data, "muPz");
		TTreeReaderArray< Float_t > muE(data, "muEnergy");
		TTreeReaderArray< bool > muLooseID(data, "isLooseMuon");
		TTreeReaderArray< bool > muTightID(data, "isTightMuon");
		TTreeReaderArray< bool > muIsoLoose(data, "PFIsoLoose");
		TTreeReaderArray< bool > muIsoTight(data, "PFIsoTight");
		  // Photon //
		TTreeReaderValue< Int_t > nPho(data, "nPho");
		TTreeReaderArray< Float_t > phoPx(data, "phoPx");
		TTreeReaderArray< Float_t > phoPy(data, "phoPy");
		TTreeReaderArray< Float_t > phoPz(data, "phoPz");
		TTreeReaderArray< Float_t > phoE(data, "phoEnergy");
		TTreeReaderArray< bool > phoLooseID(data, "phoIsPassLoose");
		  // tau //
		TTreeReaderValue< Int_t > nTau(data, "HPSTau_n");
		TTreeReaderArray< Float_t > tauPx(data, "HPSTau_Px");
		TTreeReaderArray< Float_t > tauPy(data, "HPSTau_Py");
		TTreeReaderArray< Float_t > tauPz(data, "HPSTau_Pz");
		TTreeReaderArray< Float_t > tauE(data, "HPSTau_Energy");
		TTreeReaderArray< bool > tauIsLoose(data, "disc_byLooseIsolationMVArun2017v2DBoldDMwLT2017");
		TTreeReaderArray< bool > tauDM(data, "disc_decayModeFinding");
		
		
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
				if (absjetEta >= 2.5) continue;
				if (jetPt <= 30) continue;
				// collect ak4 jet pass pt, eta cut. for getting delta phi 
				ak4_eta_3p0_pt_20.push_back(tmpTL);
			}
			sort(ak4_eta_3p0_pt_20.begin(), ak4_eta_3p0_pt_20.end(), pt_greater );
			float offline_HT = 0;
			for (auto x : ak4_eta_3p0_pt_20) {
				offline_HT += x.Pt();
			}
			// select the certian range of HT regions -> find using trigger path -> check trigger result
			int whichHT = -1;
			if (offline_HT != 0){
				for (int i = 0; i< totalNRange-1 ; i++){
					if (offline_HT > HTUseRange[i] && offline_HT < HTUseRange[i+1]){
						whichHT = i;
						break;
					}
				}
			}
			//2. eletron looseID
			vector<int> passEleId;
			for (int ie=0; ie<(int)*nEle; ie++){
				TLorentzVector tmpTLE;
				tmpTLE.SetPxPyPzE(elePx[ie], elePy[ie], elePz[ie], eleE[ie]);
				float elept = tmpTLE.Pt();
				float abs_eleEta = TMath::Abs(tmpTLE.Eta() );
				if (elept < 10) continue;
				if (! eleLooseID[ie]) continue;
				if (abs_eleEta > 2.5) continue;
				if (abs_eleEta > 1.4442 && abs_eleEta < 1.566) continue;
				passEleId.push_back(ie);
			} // end of loop ele
			
			//3. muon  
			vector<int> passLooseMuId;
			vector<int> passTightMuId;
			for (int imu=0; imu<*nMu; imu++){
				TLorentzVector tmpTLMu;
				tmpTLMu.SetPxPyPzE(muPx[imu], muPy[imu], muPz[imu], muE[imu]);
				float mupt = tmpTLMu.Pt();
				float abs_muEta = TMath::Abs(tmpTLMu.Eta() );
				if (abs_muEta >= 2.4) continue;
				if (mupt <= 10) continue;
				if (! muLooseID[imu]) continue;
				if (! muIsoLoose[imu]) continue;
				passLooseMuId.push_back(imu);
				if (mupt <= 30) continue;
				if (! muIsoTight[imu]) continue;
				if (! muTightID[imu]) continue;
				passTightMuId.push_back(imu);
			} // end of loop muon
			
			//4. apply mindphi
			float mindphi = 999;
			float nLeadingJ = 0;
			for (auto x : ak4_eta_3p0_pt_20){
				float tmpdPhi = cal_dphi(*METPhi,x.Phi() );
				h_dphi[whichHT]->Fill(tmpdPhi);
				if (mindphi > tmpdPhi) mindphi = tmpdPhi;
				nLeadingJ++;
				if (nLeadingJ == 4) break; // take only 4 leading jet to calculate mindphi
			}
			//cout << "mindphi" << mindphi << endl;
			if (mindphi == 999) continue; // 999 means there is no ak4j
			h_HT->Fill(offline_HT);
			h_MET_[whichHT]->Fill(*METPT);
			//if (offline_HT > 200) h_MET_HTcut->Fill(*METPT);
			if (mindphi > 0.4) h_met_mindphi_l[whichHT]->Fill(*METPT);
			else h_met_mindphi_s[whichHT]->Fill(*METPT);
		} // end of entries
	}// end of file list
	TFile* fout = new TFile(outfile.data(),"RECREATE");
	for (int i=0; i<sizeof(h_met_mindphi_s) / sizeof(h_met_mindphi_s[0]); i++ ){
		h_met_mindphi_s[i]->Write();
		h_met_mindphi_l[i]->Write();
		h_dphi[i]->Write();
		h_MET_[i]->Write();
	}
	h_mc_weight->Write();
	h_HT->Write();
	//h_MET_HTcut->Write();
	fout->Close();
}