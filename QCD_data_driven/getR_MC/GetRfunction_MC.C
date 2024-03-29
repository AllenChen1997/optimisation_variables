/* 
	this code is used to getting r = min(dphi(met,ak4j)) > 0.4 / min(dphi(met,ak4j)) < 0.4 
	as a function of met
	using ExoPieElement output dataTemplate
	the output will be tree with cuts flag.
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

void run_code(string inputfile, string outfile, bool isTest = false, bool isApplySR = true, bool isSplit = false){
	Double_t xbins[16] = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500, 600, 1000};
	int HTUseRange[7] = {200, 300, 500, 600, 750, 1200, 100000};
	int totalNRange = sizeof(HTUseRange) / sizeof(HTUseRange[0]);
	// things to output //
	string subline = "";
	if (isApplySR) subline = "_withSR";
	
	TH1F* h_met_mindphi_l[6];
	TH1F* h_met_mindphi_s[6];
	TH1F* h_MET_[6];
	//TH1F* h_MET_HTcut = (TH1F*) h_MET_->Clone("h_HTcut_MET");
	TH1F* h_mc_weight = new TH1F(Form("h_mc_weight%s",subline.c_str() ),"",3,-1,2);
	TH1F* h_dphi[6];
	TH1F* h_HT = new TH1F(Form("h_HT%s",subline.c_str() ),"",200,0,2000);
	TH1F* h_cutFlow = new TH1F(Form("h_cutFlow%s",subline.c_str() ),"",10,0,10); // check the SR cuts
	
	// output tree //
	float mindphi;
	int whichHT;
	float metpT;
	int weight;
	vector<bool> cuts;
	vector<float> DDB;
	vector<float> N2B1; 
	TFile* fout = new TFile(outfile.data(),"UPDATE");
	TTree ot("tree","vars");
	ot.Branch("mindphi",&mindphi);
	ot.Branch("cuts",&cuts);
	ot.Branch("whichHT",&whichHT);
	ot.Branch("metpT",&metpT);
	ot.Branch("weight",&weight);
	ot.Branch("DDB",&DDB);
	ot.Branch("N2B1",&N2B1);
	

	for (int i=0; i< sizeof(h_met_mindphi_l)/sizeof(h_met_mindphi_l[0]); i++){
		h_met_mindphi_l[i] = new TH1F(Form("h_met_mindphi_l%s_%i",subline.c_str(),i),"",15,xbins);
		h_met_mindphi_s[i] = (TH1F*) h_met_mindphi_l[i]->Clone(Form("h_met_mindphi_s%s_%i",subline.c_str(),i));
		h_dphi[i] =  new TH1F(Form("h_dphi%s_%i",subline.c_str(),i),"",10,0,pi);
		h_MET_[i] = new TH1F(Form("h_no_HTcut_MET%s_%i",subline.c_str(),i),"",150,0,1500);
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
		TTreeReaderValue< Float_t > mcWeight(data, "mcWeight");
		TTreeReaderArray< string > trigName(data, "hlt_trigName");
		TTreeReaderArray< bool > trigResult(data, "hlt_trigResult");
		//TTreeReaderArray< int > prescale(data, "hlt_trigPrescale"); // this only for data
		  // MET //
		TTreeReaderValue< Float_t > METPT(data, "pfMetCorrPt");
		TTreeReaderValue< Float_t > METPhi(data, "pfMetCorrPhi");
		  // thinJet //
		TTreeReaderValue< int > THINnJet(data, "THINnJet");
		TTreeReaderArray< float > THINjetPx(data, "THINjetPx");
		TTreeReaderArray< float > THINjetPy(data, "THINjetPy");
		TTreeReaderArray< float > THINjetPz(data, "THINjetPz");
		TTreeReaderArray< float > THINjetEnergy(data, "THINjetEnergy");
		TTreeReaderArray< bool > THINjetPassIDTight(data, "THINjetPassIDTight");
		  // fat jet //
		TTreeReaderValue< Int_t > nfj(data, "FATnJet");
		TTreeReaderArray< Float_t > fatJPx(data, "FATjetPx");
		TTreeReaderArray< Float_t > fatJPy(data, "FATjetPy");
		TTreeReaderArray< Float_t > fatJPz(data, "FATjetPz");
		TTreeReaderArray< Float_t > fatJE(data, "FATjetEnergy");
		TTreeReaderArray< Float_t > N2B1_(data, "FATN2_Beta1_"); // N2B1
		TTreeReaderArray< Float_t > DDB_(data, "FATjet_probHbb"); // DDB
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
			if (isSplit) {
				if (jEntry%2 == 0 && isApplySR) continue;
				if (jEntry%2 == 1 && ! isApplySR) continue;
			}
			if (isTest) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else {
				if (jEntry % 1000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
				else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			}
			if (isTest) if(jEntry>100)break;
			
			weight = 0;
			if (*mcWeight > 0) weight = 1;
			else weight = -1;
			h_mc_weight->Fill(weight);
			//0. has a good vertex
			if(*nVtx<1)continue;
			
			//1.1 eletron looseID
			vector<TLorentzVector> passEle;
			for (int ie=0; ie<(int)*nEle; ie++){
				TLorentzVector tmpTLE;
				tmpTLE.SetPxPyPzE(elePx[ie], elePy[ie], elePz[ie], eleE[ie]);
				float elept = tmpTLE.Pt();
				float abs_eleEta = TMath::Abs(tmpTLE.Eta() );
				if (elept < 10) continue;
				if (! eleLooseID[ie]) continue;
				if (abs_eleEta > 2.5) continue;
				if (abs_eleEta > 1.4442 && abs_eleEta < 1.566) continue;
				passEle.push_back(tmpTLE);
			} // end of loop ele
			
			//1.2 muon  
			vector<TLorentzVector> passLooseMu;
			vector<TLorentzVector> passTightMu;
			for (int imu=0; imu<*nMu; imu++){
				TLorentzVector tmpTLMu;
				tmpTLMu.SetPxPyPzE(muPx[imu], muPy[imu], muPz[imu], muE[imu]);
				float mupt = tmpTLMu.Pt();
				float abs_muEta = TMath::Abs(tmpTLMu.Eta() );
				if (abs_muEta >= 2.4) continue;
				if (mupt <= 10) continue;
				if (! muLooseID[imu]) continue;
				if (! muIsoLoose[imu]) continue;
				passLooseMu.push_back(tmpTLMu);
				if (mupt <= 30) continue;
				if (! muIsoTight[imu]) continue;
				if (! muTightID[imu]) continue;
				passTightMu.push_back(tmpTLMu);
			} // end of loop muon
			
			//1.3 identify tau //
			vector<TLorentzVector> passTau;
			vector<TLorentzVector> passTau_againstLep;
			for (int itau=0; itau<(int)*nTau; itau++){
				TLorentzVector tmpTLTau;
				tmpTLTau.SetPxPyPzE(tauPx[itau],tauPy[itau],tauPz[itau],tauE[itau]);
				float tauPt = tmpTLTau.Pt();
				float abs_tauEta = TMath::Abs(tmpTLTau.Eta() );
				if(abs_tauEta >= 2.3) continue;
				if (tauPt <= 18 ) continue;
				if (! tauDM[itau] ) continue;
				if (! tauIsLoose[itau] ) continue;
				passTau.push_back(tmpTLTau);
			} 
				// against lep
			for (auto x : passTau){
				bool noEle = true;
				bool noMu = true;
				for (auto xe : passEle){
					float dr = x.DeltaR(xe);
					if (dr < 0.4) {
						noEle = false;
						break;
					}
				}
				for (auto xmu : passLooseMu){
					float dr = x.DeltaR(xmu);
					if (dr < 0.4) {
						noMu = false;
						break;
					}
				}
				if (noEle && noMu) passTau_againstLep.push_back(x);
			} // end of identify tau
			
			//2. identify photon //
			vector<TLorentzVector> passPho;
			for (int iph=0; iph<(int)*nPho ; iph++){
				TLorentzVector tmpTLPho;
				tmpTLPho.SetPxPyPzE(phoPx[iph],phoPy[iph], phoPz[iph], phoE[iph] );
				float phoPt = tmpTLPho.Pt();
				float abs_phoEta = TMath::Abs(tmpTLPho.Eta() );
				if (phoPt < 15) continue;
				if ( ! phoLooseID[iph] )continue;
				if ( abs_phoEta > 2.5) continue;
				passPho.push_back(tmpTLPho);
			} // end of loop photon
			
			//3. MET //
			bool METState = (*METPT > 180);
			
			
			//4.1 w CR recoil // 
			bool WRecoilState = false;
			if (passEle.size() == 1){
				float RecoilPx = -(*METPT * TMath::Cos(*METPhi) ) + passEle[0].Px();
				float RecoilPy = -(*METPT * TMath::Sin(*METPhi) ) + passEle[0].Py();
				float weRecoil = TMath::Sqrt(RecoilPx*RecoilPx + RecoilPy*RecoilPy);
				if (weRecoil > 180 ) WRecoilState = true;
			}
			if (passLooseMu.size() == 1 ){
				float RecoilPx = -(*METPT * TMath::Cos(*METPhi) ) + passLooseMu[0].Px();
				float RecoilPy = -(*METPT * TMath::Sin(*METPhi) ) + passLooseMu[0].Py();
				float wmuRecoil = TMath::Sqrt(RecoilPx*RecoilPx + RecoilPy*RecoilPy);
				if (wmuRecoil > 180 ) WRecoilState = true;
			}
			
			//4.2 Gamma CR //
			bool GammaRecoilState = false; // true when recoil > 180
			if (passPho.size() == 1){
				float RecoilPx = -(*METPT * TMath::Cos(*METPhi) ) + passPho[0].Px();
				float RecoilPy = -(*METPT * TMath::Sin(*METPhi) ) + passPho[0].Py();
				float RecoilPt = TMath::Sqrt(RecoilPx*RecoilPx + RecoilPy*RecoilPy);
				if (RecoilPt > 180) GammaRecoilState = true;
			}
			
			//4.3 Z CR recoil //
			bool ZRecoilState = false;
			if (passEle.size() == 2 ){
				TLorentzVector ee = passEle[0] + passEle[1];
				float ee_mass = ee.M();
				float RecoilPx = -(*METPT * TMath::Cos(*METPhi) ) + passEle[0].Px() + passEle[1].Px();
				float RecoilPy = -(*METPT * TMath::Sin(*METPhi) ) + passEle[0].Py() + passEle[1].Py();
				float RecoilPt = TMath::Sqrt(RecoilPx*RecoilPx + RecoilPy *RecoilPy);
				if (ee_mass > 60.0 && ee_mass < 120.0 && RecoilPt > 180.0) ZRecoilState = true;
			}
			if (passLooseMu.size() == 2 ){
				TLorentzVector mumu = passLooseMu[0] + passLooseMu[1];
				float mumu_mass = mumu.M();
				float RecoilPx = -(*METPT * TMath::Cos(*METPhi) ) + passLooseMu[0].Px() + passLooseMu[1].Px();
				float RecoilPy = -(*METPT * TMath::Sin(*METPhi) ) + passLooseMu[0].Py() + passLooseMu[1].Py();
				float RecoilPt = TMath::Sqrt(RecoilPx*RecoilPx + RecoilPy*RecoilPy);
				if (mumu_mass > 60.0 && mumu_mass < 120.0 && RecoilPt > 180.0) ZRecoilState = true;
			}
				
			//5. trigger
			int itrig = -1;
			bool HLT_PFMETNoMu120_PFMHTNoMu120_IDTight = false;
			bool HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = false;
			bool HLT_PFMETNoMu140_PFMHTNoMu140_IDTight = false;
			for (auto xTrig : trigName ){
				itrig++;
				
				string thisTrig= trigName[itrig];
				bool res = trigResult[itrig];

				if (thisTrig.find("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60")!= std::string::npos){
					if (res ) HLT_PFMETNoMu120_PFMHTNoMu120_IDTight = true;
				}		
				else if (thisTrig.find("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight")!= std::string::npos){
					if (res ) HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 = true;
				}		
				else if (thisTrig.find("HLT_PFMETNoMu140_PFMHTNoMu140_IDTight")!= std::string::npos){
					if (res ) HLT_PFMETNoMu140_PFMHTNoMu140_IDTight = true;
				}		
			}
			bool MET_triggerState = (HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 || HLT_PFMETNoMu120_PFMHTNoMu120_IDTight ||HLT_PFMETNoMu140_PFMHTNoMu140_IDTight);
			
			//6. fatJet identify //
			N2B1.clear();
			DDB.clear();
			vector<TLorentzVector> passFatJ;
			for (int ij=0; ij<(int)*nfj; ij++){
				TLorentzVector tmpTLFatJ;
				tmpTLFatJ.SetPxPyPzE(fatJPx[ij],fatJPy[ij], fatJPz[ij], fatJE[ij]);
				float fjPt = tmpTLFatJ.Pt();
				float abs_fjEta = TMath::Abs(tmpTLFatJ.Eta() );
				float fjMass = tmpTLFatJ.M();
				if (abs_fjEta >= 2.5) continue;
				if (fjPt <= 200 ) continue;
				if (fjMass < 100 || fjMass > 150 ) continue;
				passFatJ.push_back(tmpTLFatJ);
				
				// only keep N2B1 and DDB which fatJ pass basic cuts
				N2B1.push_back(N2B1_[ij]);
				DDB.push_back(DDB_[ij]);
			}// end loop of fj
			
			//7. ak4Jet
			vector<TLorentzVector> ak4_eta_3p0_pt_20; 
			for (int ijet = 0; ijet < *THINnJet; ijet++){
				TLorentzVector tmpTL;
				tmpTL.SetPxPyPzE(THINjetPx[ijet],THINjetPy[ijet],THINjetPz[ijet],THINjetEnergy[ijet]);
				float absjetEta = TMath::Abs(tmpTL.Eta() );
				float jetPt = tmpTL.Pt();
				// basic cuts
				if (absjetEta >= 2.5) continue;
				if (jetPt <= 30) continue;
				if (! THINjetPassIDTight[ijet] ) continue;
				// jet clean
				bool noEle = true;
				for (auto xe : passEle){
					float dr = tmpTL.DeltaR(xe);
					if (dr < 0.4 ){
						noEle = false;
						break;
					}
				}
				if (! noEle) continue;
				bool noMu = true;
				for (auto xmu : passLooseMu){
					float dr = tmpTL.DeltaR(xmu);
					if (dr < 0.4 ) {
						noMu = false;
						break;
					}
				}
				if (! noMu) continue;
				// collect ak4 jet pass pt, eta cut. for getting delta phi 
				ak4_eta_3p0_pt_20.push_back(tmpTL);
			}

			
			sort(ak4_eta_3p0_pt_20.begin(), ak4_eta_3p0_pt_20.end(), pt_greater );
			float offline_HT = 0;
			for (auto x : ak4_eta_3p0_pt_20) {
				offline_HT += x.Pt();
			}
			// select the certian range of HT regions -> find using trigger path -> check trigger result
			whichHT = -1;
			if (offline_HT != 0){
				for (int i = 0; i< totalNRange-1 ; i++){
					if (offline_HT > HTUseRange[i] && offline_HT < HTUseRange[i+1]){
						whichHT = i;
						break;
					}
				}
			}
			//7.2 apply mindphi && addtional ak4 jet test(<=2)
			int nExtraAk4 = 0;
			mindphi = 999;
			float nLeadingJ = 0;
			for (auto x : ak4_eta_3p0_pt_20){
				// additional ak4_jet 
				if (passFatJ.size() == 1){
					float dr = x.DeltaR(passFatJ[0]);
					if (dr >= 1.2 ) nExtraAk4++;
				}
				// min dphi
				float tmpdPhi = cal_dphi(*METPhi,x.Phi() );
				h_dphi[whichHT]->Fill(tmpdPhi);
				if (mindphi > tmpdPhi) mindphi = tmpdPhi;
				nLeadingJ++;
				//if (nLeadingJ == 4) break; // take only 4 leading jet to calculate mindphi
			}
			//8. do the cut
			/* skip in this iter. try to study these cuts
			if (isApplySR){
				h_cutFlow->Fill(0);
				if (! METState && ! ZRecoilState && ! WRecoilState && ! GammaRecoilState) continue;
				h_cutFlow->Fill(1);
				if (! MET_triggerState) continue;
				h_cutFlow->Fill(2);
				if (*METPT <= 200) continue;
				h_cutFlow->Fill(3);
				if (passTau_againstLep.size() > 0) continue;
				h_cutFlow->Fill(4);
				if (passPho.size() > 0) continue;
				h_cutFlow->Fill(5);
				if (passEle.size() > 0) continue;
				h_cutFlow->Fill(6);
				if (passLooseMu.size() > 0)continue;
				h_cutFlow->Fill(7);
				if (nExtraAk4 > 2 ) continue;
				h_cutFlow->Fill(8);
				if (passFatJ.size() != 1 ) continue;
				h_cutFlow->Fill(9);
			}
			*/
			// there are six cuts now.
			cuts.clear();
			if (passTau_againstLep.size() > 0) cuts.push_back(false);
			else cuts.push_back(true);
			if (passPho.size() > 0) cuts.push_back(false);
			else cuts.push_back(true);
			if (passEle.size() > 0) cuts.push_back(false);
			else cuts.push_back(true);
			if (passLooseMu.size() > 0) cuts.push_back(false);
			else cuts.push_back(true);
			if (nExtraAk4 > 2 ) cuts.push_back(false);
			else cuts.push_back(true);
			if (passFatJ.size() != 1 ) cuts.push_back(false);
			else cuts.push_back(true);
			
			//if (mindphi == 999) continue; // 999 means there is no ak4j
			h_HT->Fill(offline_HT);
			h_MET_[whichHT]->Fill(*METPT);
			//if (offline_HT > 200) h_MET_HTcut->Fill(*METPT);
			if (mindphi > 0.4) h_met_mindphi_l[whichHT]->Fill(*METPT);
			else h_met_mindphi_s[whichHT]->Fill(*METPT);
			
			metpT = *METPT;
			ot.Fill();
		} // end of entries
	}// end of file list
	//TFile* fout = new TFile(outfile.data(),"UPDATE");
	fout->cd();
	/*for (int i=0; i<sizeof(h_met_mindphi_s) / sizeof(h_met_mindphi_s[0]); i++ ){
		h_met_mindphi_s[i]->Write();
		h_met_mindphi_l[i]->Write();
		h_dphi[i]->Write();
		h_MET_[i]->Write();
	}
	h_mc_weight->Write();
	h_HT->Write();
	if (isApplySR) h_cutFlow->Write();
	//h_MET_HTcut->Write();
	*/
	
	ot.Write();
	fout->Close();
}

void GetRfunction_MC(string inputfile, string outfile, bool isTest = false){
	//run_code(inputfile,outfile,isTest,true,true); //for last two isApplySR , isSplit
	//run_code(inputfile,outfile,isTest,false,true); 
	run_code(inputfile,outfile,isTest,true,false); //run_code(string inputfile, string outfile, bool isTest = false, bool isApplySR = true, bool isSplit = false)
}
	