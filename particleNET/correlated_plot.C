/////////////////////////////////////////////////
/*
   this code is used to apply pre-selections, produce cut flow plot, and generate 2D correlated plots
*/
////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TMath.h>
#include <TFile.h>
#include <TH2D.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TLorentzVector.h>
#include <TAxis.h> 
#include <TLine.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <vector>
#define NN2 20
#define MinN2 0
#define MaxN2 1

#define Nddb 40
#define Minddb 0
#define Maxddb 1

using namespace std;

int region = 1; // 1 for SingleL_B, 2 for SBand_B . 2 not yet done

void load_to_hist_bkg(string s , TH2D* h, TH1F* h_cut,vector<float>& vD, vector<float>& vP){
	cout << "Reading " << s << endl;
	TFile* myfile;
	myfile=TFile::Open(s.data());
	TTreeReader myRead("Events",myfile);  
	// var. in tree to read //
	TTreeReaderArray< Float_t > DDB(myRead,"FatJet_btagDDBvL"); 
	TTreeReaderArray< Float_t > pNET(myRead,"FatJet_ParticleNetMD_probXbb");
	// genpart // used to filter out the TTToSemilep. events
	TTreeReaderValue< UInt_t > nPar(myRead,"nGenPart");
	TTreeReaderArray< Int_t > Par_status(myRead,"GenPart_status");
	TTreeReaderArray< Int_t > Par_pid(myRead,"GenPart_pdgId");
	TTreeReaderArray< Int_t > Par_motherid(myRead,"GenPart_genPartIdxMother");
	// fat jet
	TTreeReaderValue< UInt_t >  nfj(myRead,"nFatJet");
	TTreeReaderArray< Float_t > fj_eta(myRead,"FatJet_eta");
	TTreeReaderArray< Float_t > fj_phi(myRead,"FatJet_phi");
	TTreeReaderArray< Float_t > fj_pt(myRead, "FatJet_pt");
	TTreeReaderArray< Float_t > fj_mass(myRead, "FatJet_mass");
	TTreeReaderArray< Float_t > fj_ddb(myRead, "FatJet_btagDeepB");
	// MET
	TTreeReaderValue< Float_t > MET_pt(myRead, "MET_pt");
	TTreeReaderValue< Float_t > MET_phi(myRead, "MET_phi");
	// Ak4 jet
	TTreeReaderValue< UInt_t > ntj(myRead, "nJet");
	TTreeReaderArray< Float_t > Jet_phi(myRead, "Jet_phi");
	TTreeReaderArray< Float_t > Jet_eta(myRead, "Jet_eta");
	TTreeReaderArray< Float_t > Jet_pt(myRead, "Jet_pt");
	TTreeReaderArray< Int_t > Jet_id(myRead, "Jet_jetId"); // (1:loose, 3:loose+tight, 7:loose+tight+tightLepVeto)
	TTreeReaderArray< Float_t > Jet_ddb(myRead, "Jet_btagDeepB");
	// ele
	TTreeReaderValue< UInt_t > nEle(myRead, "nElectron");
	TTreeReaderArray< Float_t > Ele_pt(myRead, "Electron_pt");
	TTreeReaderArray< Float_t > Ele_mass(myRead, "Electron_mass");
	TTreeReaderArray< Float_t > Ele_eta(myRead, "Electron_eta");
	TTreeReaderArray< Float_t > Ele_phi(myRead, "Electron_phi");
	TTreeReaderArray< Int_t > Ele_id(myRead, "Electron_cutBased"); // (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
	// pho
	TTreeReaderValue< UInt_t > nPho(myRead, "nPhoton");
	TTreeReaderArray< Float_t > Pho_pt(myRead, "Photon_pt");
	TTreeReaderArray< Float_t > Pho_eta(myRead, "Photon_eta");
	TTreeReaderArray< Float_t > Pho_phi(myRead, "Photon_phi");
	TTreeReaderArray< Float_t > Pho_mass(myRead, "Photon_mass");
	TTreeReaderArray< Int_t > Pho_id(myRead, "Photon_cutBased"); // (0:fail, 1:loose, 2:medium, 3:tight)
	// muon
	TTreeReaderValue< UInt_t > nMu(myRead, "nMuon");
	TTreeReaderArray< Float_t > Mu_pt(myRead, "Muon_pt");
	TTreeReaderArray< Float_t > Mu_mass(myRead, "Muon_mass");
	TTreeReaderArray< Float_t > Mu_eta(myRead, "Muon_eta");
	TTreeReaderArray< Float_t > Mu_phi(myRead, "Muon_phi");
	TTreeReaderArray< Bool_t > Mu_looseid(myRead, "Muon_looseId");
	TTreeReaderArray< Bool_t > Mu_tightid(myRead, "Muon_tightId");
	TTreeReaderArray< UChar_t > Mu_isoid(myRead, "Muon_miniIsoId");// (1=MiniIsoLoose, 2=MiniIsoMedium, 3=MiniIsoTight, 4=MiniIsoVeryTight)
	// tau
	TTreeReaderValue< UInt_t > nTau(myRead, "nTau");
	TTreeReaderArray< Float_t > Tau_eta(myRead, "Tau_eta");
	TTreeReaderArray< Float_t > Tau_phi(myRead, "Tau_phi");
	TTreeReaderArray< Float_t > Tau_pt(myRead, "Tau_pt");
	TTreeReaderArray< UChar_t > Tau_idMVA(myRead, "Tau_idMVAoldDM2017v2"); // (bitmask 1 = VVLoose, 2 = VLoose, 4 = Loose, 8 = Medium, 16 = Tight, 32 = VTight, 64 = VVTight)
	TTreeReaderArray< Bool_t > Tau_dm(myRead, "Tau_idDecayMode");
	
	
	// triggers //
	// MET triggers 2017
	//TTreeReaderValue< Bool_t > HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60(myRead, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60");
	TTreeReaderValue< Bool_t > HLT_PFMETNoMu120_PFMHTNoMu120_IDTight(myRead, "HLT_PFMETNoMu120_PFMHTNoMu120_IDTight");
	//TTreeReaderValue< Bool_t > HLT_PFMETNoMu140_PFMHTNoMu140_IDTight(myRead, "HLT_PFMETNoMu140_PFMHTNoMu140_IDTight");
	// Photon trigger 2017
	//TTreeReaderValue< Bool_t > HLT_Photon200(myRead, "HLT_Photon200");
	// Eletron trigger 2017
	//TTreeReaderValue< Bool_t > HLT_Ele32_WPTight_Gsf_L1DoubleEG(myRead, "HLT_Ele32_WPTight_Gsf_L1DoubleEG");	
	TTreeReaderValue< Bool_t > HLT_Ele32_eta2p1_WPTight_Gsf(myRead, "HLT_Ele32_eta2p1_WPTight_Gsf"); // use this ?	
	// Muon triggers 2017
	TTreeReaderValue< Bool_t > HLT_IsoMu24(myRead, "HLT_IsoMu24");
	TTreeReaderValue< Bool_t > HLT_IsoMu27(myRead, "HLT_IsoMu27");
	//TTreeReaderValue< Bool_t > HLT_IsoTkMu27(myRead, "HLT_IsoTkMu27");
	//TTreeReaderValue< Bool_t > HLT_IsoTkMu24(myRead, "HLT_IsoTkMu24");
	
	int total_entry = myRead.GetEntries(true);
	cout << "total entries = " << total_entry << endl;
	
	TH2D* h_tmp = (TH2D*) h->Clone(""); // for collect events from this inputfile
	h_tmp->Reset();
	TH1F* h_cut_tmp = (TH1F*) h_cut->Clone("");
	h_cut_tmp->Reset();
	int n = 0; // ientry
	int nNotW = 0;
	while (myRead.Next()){  // loop all entries
		n++;
		if (n%5000 == 0) cout << "running " << n << " / " << total_entry << endl;
		h_cut_tmp->Fill(0); // 1st bin: total
		// filter out TTToSemilep. events
		int nlep = 0;
		for (int ip=0; ip<(int)*nPar; ip++){
			if (Par_status[ip] == 23){  // focus on the daugter
				int abspid = TMath::Abs(Par_pid[ip]);
				if ( abspid == 11 || abspid == 13 || abspid == 15 ){
					nlep++;
					if (TMath::Abs( Par_pid[Par_motherid[ip] ] ) != 24) {
						nNotW++;
					}
				}
			}
		}
		if (nlep != 1) continue;
		h_cut_tmp->Fill(1); // 2nd bin: TTToSemi
		
		// ele loose id //
		vector<int> passEleId;
		for (int ie=0; ie<(int)*nEle;ie++){ // loop ele
			if (Ele_pt[ie] < 10) continue;
			if (Ele_id[ie] < 2) continue;
			float abs_Ele_eta = TMath::Abs(Ele_eta[ie]);
			if (abs_Ele_eta > 2.5) continue;
			if (abs_Ele_eta < 1.566 && abs_Ele_eta > 1.4442) continue;
			passEleId.push_back(ie);
		} // end of loop ele
		
		// muon id //
		vector<int> passLooseMuId;
		vector<int> passTightMuId;
		for (int imu=0; imu<(int)*nMu; imu++){ // loop muon
			if (Mu_pt[imu] <= 10) continue;
			if (TMath::Abs(Mu_eta[imu]) >= 2.4 ) continue;
			if (! Mu_looseid[imu]) continue;
			if (Mu_isoid[imu] < 1) continue;
				passLooseMuId.push_back(imu);
			if (! Mu_tightid[imu]) continue;
			if (Mu_isoid[imu] < 3) continue;
				passTightMuId.push_back(imu);
		} // end of loop muon
		
		// identify tau //
		vector<int> passTauId;
		vector<int> passTauId_AgainstLep;
		for (int itau=0; itau<(int)*nTau; itau++ ){ // loop tau
			if (Tau_pt[itau] <= 18 ) continue;
			if (TMath::Abs(Tau_eta[itau]) >= 2.3 ) continue;
			if (! Tau_dm[itau]) continue;
			if (Tau_idMVA[itau] < 3) continue;
			passTauId.push_back(itau);
		}
			// against lep 
		for (auto x : passTauId){
			bool noEle = true;
			bool noMu = true;
			if (passEleId.size() > 0){ // ele against
				for (auto xe : passEleId){
					float dphi = Tau_phi[x] - Ele_phi[xe];
					float deta = Tau_eta[x] - Ele_phi[xe];
					float dr = TMath::Sqrt(dphi*dphi + deta*deta);
					if (dr < 0.4) {
						noEle = false;
						break;
					}
				}
			}
			if (passLooseMuId.size() > 0){ // mu against
				for (auto xmu : passLooseMuId){
					float dphi = Tau_phi[x] - Mu_phi[xmu];
					float deta = Tau_eta[x] - Mu_eta[xmu];
					float dr = TMath::Sqrt(dphi*dphi + deta*deta);
					if (dr < 0.4){
						noMu = false;
						break;
					}
				}
			}
			if (noEle && noMu) passTauId_AgainstLep.push_back(x);
		}
			
		// photon //
		vector<int> passPhoId;
		for (int iph=0; iph<(int)*nPho;iph++){ // loop photons
			if (Pho_pt[iph] < 15) continue;
			if (Pho_id[iph] < 1) continue; // looseId
			float abs_Pho_eta = TMath::Abs(Pho_eta[iph]);
			if (abs_Pho_eta > 2.5) continue;
			passPhoId.push_back(iph);
		}// end of loop photon
		
		// MET //
		bool METState = (*MET_pt > 180);
		
		// Z CR //
		bool ZRecoilState = false; // true if recoilpt of Zee or Zmumu > 180
		if (passEleId.size() == 2){ // for ee, we need "ee mass" and "ZeeRecoil"
			TLorentzVector ve[3]; // e1,e2, and ee
			for (int i=0; i<2; i++){ 
				int ie = passEleId[i];
				ve[i].SetPtEtaPhiM(Ele_pt[ie],Ele_eta[ie],Ele_phi[ie],Ele_mass[ie]);
			}
			ve[2] = ve[0] + ve[1];
			float ee_mass = ve[2].M(); // ee invariant mass
			float Recoilpx = -(*MET_pt * TMath::Cos(*MET_phi) ) + ve[0].Px() + ve[1].Px();
			float Recoilpy = -(*MET_pt * TMath::Sin(*MET_phi) ) + ve[0].Py() + ve[1].Py();
			float Recoilpt = TMath::Sqrt(Recoilpx*Recoilpx + Recoilpy*Recoilpy);
			if (ee_mass > 60.0 && ee_mass < 120.0 && Recoilpt > 180.0 ){
				ZRecoilState = true;
			}
		}
		if (passLooseMuId.size() == 2){
			TLorentzVector vmu[3];
			for (int i=0; i<2; i++){
				int imu = passLooseMuId[i];
				vmu[i].SetPtEtaPhiM(Mu_pt[imu],Mu_eta[imu],Mu_phi[imu],Mu_mass[imu]);
			}
			vmu[2] = vmu[0] + vmu[1];
			float mumu_mass = vmu[2].M(); // mumu invariant mass
			float Recoilpx = -(*MET_pt * TMath::Cos(*MET_phi) ) + vmu[0].Px() + vmu[1].Px();
			float Recoilpy = -(*MET_pt * TMath::Sin(*MET_phi) ) + vmu[0].Py() + vmu[1].Py();
			float Recoilpt = TMath::Sqrt(Recoilpx*Recoilpx + Recoilpy*Recoilpy);
			if (mumu_mass > 60.0 && mumu_mass < 120.0 && Recoilpt > 180.0){
				ZRecoilState = true;
			}
		}
		
		// w CR recoil //
		bool WRecoilState = false; // true when Recoilpt of We or Wmu > 180
		float WmuRecoil=0;
		float WeRecoil=0;
		if (passEleId.size() == 1){
			int iele = passEleId[0];
			TLorentzVector ve;
			ve.SetPtEtaPhiM(Ele_pt[iele], Ele_eta[iele], Ele_phi[iele], Ele_mass[iele]);
			float Recoilpx = -(*MET_pt * TMath::Cos(*MET_phi) ) + ve.Px();
			float Recoilpy = -(*MET_pt * TMath::Sin(*MET_phi) ) + ve.Py();
			WeRecoil = TMath::Sqrt(Recoilpx*Recoilpx + Recoilpy*Recoilpy);
			if (WeRecoil > 180) WRecoilState = true;
		}
		if (passLooseMuId.size() == 1){
			int imu = passLooseMuId[0];
			TLorentzVector vmu;
			vmu.SetPtEtaPhiM(Mu_pt[imu], Mu_eta[imu], Mu_phi[imu], Mu_mass[imu]);
			float Recoilpx = -(*MET_pt * TMath::Cos(*MET_phi) ) + vmu.Px();
			float Recoilpy = -(*MET_pt * TMath::Sin(*MET_phi) ) + vmu.Py();
			WmuRecoil = TMath::Sqrt(Recoilpx*Recoilpx + Recoilpy*Recoilpy);
			if (WmuRecoil > 180) WRecoilState = true;
		}
		
		// Gamma CR 
		bool GammaRecoilState = false; // true when recoil of Gamma(single photon) > 180
		if (passPhoId.size() == 1){
			int ipho = passPhoId[0];
			TLorentzVector vpho;
			vpho.SetPtEtaPhiM(Pho_pt[ipho], Pho_eta[ipho], Pho_phi[ipho], Pho_mass[ipho]);
			float Recoilpx = -(*MET_pt * TMath::Cos(*MET_phi) ) + vpho.Px();
			float Recoilpy = -(*MET_pt * TMath::Sin(*MET_phi) ) + vpho.Py();
			float Recoilpt = TMath::Sqrt(Recoilpx*Recoilpx + Recoilpy*Recoilpy);
			if (Recoilpt > 180) GammaRecoilState = true;
		}
		
		if (! METState && ! ZRecoilState && ! WRecoilState && ! GammaRecoilState) continue; // if all false, fail pre-selection
		h_cut_tmp->Fill(2); // 3rd bin: pass pre-selection
		
		// trigger states //
			//bool MET_triggerState = (*HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60 || *HLT_PFMETNoMu120_PFMHTNoMu120_IDTight || *HLT_PFMETNoMu140_PFMHTNoMu140_IDTight);
		bool MET_triggerState = (*HLT_PFMETNoMu120_PFMHTNoMu120_IDTight);
			//bool Pho_triggerState = *HLT_Photon200;
			//bool Ele_triggerState = *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
		bool Ele_triggerState = *HLT_Ele32_eta2p1_WPTight_Gsf;
		// no muon trigger now, we use MET trigger for muon CR
		if (! MET_triggerState) continue;  // for SBregion
		h_cut_tmp->Fill(3); // 4th bin: trigger
		
		switch (region){
			case 1:
				// lep
				if (passLooseMuId.size() != 1 || passTightMuId.size() != 1 ) continue; // for h_topmunu_CR
				h_cut_tmp->Fill(4); // 5th bin: lep. (muon)
		
				// lep veto  
				if (passTauId_AgainstLep.size() > 0 || passEleId.size() > 0 ) continue;
				h_cut_tmp->Fill(5); // 6th bin: lep. veto
			
				// recoil
				if (WmuRecoil <= 200) continue;
				h_cut_tmp->Fill(6); // 7th bin: recoil
				
				// MET
				if (*MET_pt <= 50) continue;
				h_cut_tmp->Fill(7); // 8th bin: MET
				break;
			case 2:
				// lepVeto
				if (passEleId.size() > 0 || passLooseMuId.size() > 0) continue;
				h_cut_tmp->Fill(4); // 5th bin(region2) 
				
				// tauVeto
				if (passTauId_AgainstLep.size() > 0) continue;
				h_cut_tmp->Fill(5); // 6th bin (region2)
				
				// nPho
				if (passPhoId.size() > 0) continue;
				h_cut_tmp->Fill(6); // 7th bin(region2)
				
				// MET
				if (*MET_pt <= 200) continue;
				h_cut_tmp->Fill(7); // 8th bin(region2)
				break;
		}
			
		// identify fatjet //
		vector<int> fjpassID;
		for (int i=0; i<(int)*nfj; i++){ // loop of fj
			if (TMath::Abs(fj_eta[i]) >= 2.5) continue;
			if (fj_pt[i] <= 200) continue;
			if (fj_mass[i] < 100 || fj_mass[i] > 150) continue;
			fjpassID.push_back(i);
		} // end loop of fj
		if (fjpassID.size() != 1) continue; // only remains one fj can be used
		int fjID = fjpassID[0];
		h_cut_tmp->Fill(8); // 9th bin: nFJet
		
		// identify thinjet //
		int nAk4 = 0; // this is number of additional ak4 jet
		int nBjets_iso = 0; // count for jets pass btagDeepB
		float mindphi=-1; // this is mim dphi b/w MET and ak4
		for (int i=0; i<(int)*ntj;i++){ // loop all ak4jet
			// basic identify ak4 jet //
			if (Jet_pt[i] <= 30) continue;
			if (TMath::Abs(Jet_eta[i]) >= 4.5) continue;
			if (Jet_id[i] < 1) continue;
			// jet clean 
			bool noEle = true;
			if (passEleId.size() > 0){ // ele against
				for (auto xe : passEleId){
					float dphi = Jet_phi[i] - Ele_phi[xe];
					float deta = Jet_eta[i] - Ele_phi[xe];
					float dr = TMath::Sqrt(dphi*dphi + deta*deta);
					if (dr < 0.4) {
						noEle = false;
						break;
					}
				}
			}
			if (! noEle) continue;
			bool noMu = true;
			if (passLooseMuId.size() > 0){ // mu against
				for (auto xmu : passLooseMuId){
					float dphi = Jet_phi[i] - Mu_phi[xmu];
					float deta = Jet_eta[i] - Mu_eta[xmu];
					float dr = TMath::Sqrt(dphi*dphi + deta*deta);
					if (dr < 0.4){
						noMu = false;
						break;
					}
				}
			}
			if (! noMu) continue;
			// additional ak4 jet test (<=2)
			float dphi = fj_phi[fjID]-Jet_phi[i];
			float deta = fj_eta[fjID]-Jet_eta[i];
			float dR = TMath::Sqrt(dphi*dphi + deta*deta); // dR b/w fj and ak4j
			if (dR >= 1.2) nAk4++;
			// min dphi b/w MET and ak4jet
			float dphi_met_ak4 = TMath::Abs(*MET_phi-Jet_phi[i]);
			if (mindphi == -1) mindphi = dphi_met_ak4;
			else {
				if (mindphi > dphi_met_ak4) mindphi = dphi_met_ak4;
			}
			// btagDeepB
			if (Jet_ddb[i] > 0.1522 && TMath::Abs(Jet_eta[i]) < 2.5 ) nBjets_iso++;
		}
		if (nBjets_iso != 1) continue;
		h_cut_tmp->Fill(9); // 10th bin: nBjets
		
		/* tmp no used cut
		if (mindphi != -1) { // if mindphi == -1, it means there is no ak4 jet pass basic cuts
			if (mindphi <= 0.4) continue;
		}
		if (nAk4 > 2 ) continue;		
		*/
		
		
		// passed all pre-selections, fill in //
		h_tmp->Fill(pNET[0],DDB[0]);
		vD.push_back(DDB[0]);
		vP.push_back(pNET[0]);
		
	} // end loop all entries in one file
	h->Add(h_tmp);
	h_cut->Add(h_cut_tmp);
	cout << "nNotW = " << nNotW << endl;
}

void correlated_plot(string inputfilelist, string outputfile = "histo.root"){
	TH2D* h_pNET = new TH2D("h_pNET","",NN2,0,MaxN2,Nddb,Minddb,Maxddb);
		h_pNET->SetYTitle("DDB"); h_pNET->SetXTitle("particleNET");
	TH1F* h_cut_flow = new TH1F("h_cut_flow","",10,0,10); // incl, fJ_sel, mindphi, Additional_Ak4, MET_pt, ele_veto, photon_veto
	string labels[10] = {"Total","TTToSemi","preselection","trigger","lep","lepVeto","Recoil","MET","nJets","nBjets"};
	for (int i=0; i<(int)(sizeof(labels)/sizeof(labels[0]) ); i++){
		h_cut_flow->GetXaxis()->SetBinLabel(i+1,labels[i].data());
	}
	string line;
	ifstream fin(inputfilelist.data());
	vector<string> lines;
	vector<float> DDB;
	vector<float> PNET;
	vector<vector<float> > vDDB;
	vector<vector<float> > vPNET;
	while(getline(fin,line)){ // read the text file
		load_to_hist_bkg(line,h_pNET,h_cut_flow,DDB,PNET);
		vDDB.push_back(DDB);
		vPNET.push_back(PNET);
		DDB.clear();
		PNET.clear();
		lines.push_back(line);
	}
	// set up the output //
	TFile* fout = new TFile(outputfile.data(),"recreate");
	TTree ot("tree","input names");
	ot.Branch("inputname",&line);
	ot.Branch("DDB",&DDB);
	ot.Branch("PNET",&PNET);
	for (int i=0; i<(int)lines.size();i++){
		line = lines[i];
		DDB=vDDB[i];
		PNET=vPNET[i];
		ot.Fill();
	}
	h_pNET->Write();
	h_cut_flow->Write();
	fout->Write();
	fout->Close();
}		
