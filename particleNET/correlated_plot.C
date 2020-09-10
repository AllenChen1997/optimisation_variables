/////////////////////////////////////////////////
/*
   this code is used to generate 2D correlated plots
*/
////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TMath.h>
#include <TFile.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h> 
#include <TLine.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <vector>
// make sure the variables are the same in the N2_study.C
#define Maxpt 2000
#define NN2 20
#define MinN2 0
#define MaxN2 1

#define Nddb 40
#define Minddb 0
#define Maxddb 1
//
using namespace std;
string testsample = "./nano_39.root";


void load_to_hist_bkg(string s , TH2D* h,vector<float>& vD, vector<float>& vP){
	cout << "Reading " << s << endl;
	TFile* myfile;
	myfile=TFile::Open(s.data());
	TTreeReader myRead("Events",myfile);  
	// obs to look
	TTreeReaderArray< Float_t > DDB(myRead,"FatJet_btagDDBvL"); 
	TTreeReaderArray< Float_t > pNET(myRead,"FatJet_ParticleNetMD_probXbb");
	// fat jet
	TTreeReaderValue< UInt_t >  nfj(myRead,"nFatJet");
	TTreeReaderArray< Float_t > fj_eta(myRead,"FatJet_eta");
	TTreeReaderArray< Float_t > fj_phi(myRead,"FatJet_phi");
	TTreeReaderArray< Float_t > fj_pt(myRead, "FatJet_pt");
	TTreeReaderArray< Float_t > fj_mass(myRead, "FatJet_mass");
	// MET
	TTreeReaderValue< Float_t > MET_pt(myRead, "MET_pt");
	TTreeReaderValue< Float_t > MET_phi(myRead, "MET_phi");
	// Ak4 jet
	TTreeReaderValue< UInt_t > ntj(myRead, "nJet");
	TTreeReaderArray< Float_t > Jet_phi(myRead, "Jet_phi");
	TTreeReaderArray< Float_t > Jet_eta(myRead, "Jet_eta");
	// ele
	TTreeReaderValue< UInt_t > nEle(myRead, "nElectron");
	TTreeReaderArray< Float_t > Ele_pt(myRead, "Electron_pt");
	TTreeReaderArray< Float_t > Ele_eta(myRead, "Electron_eta");
	TTreeReaderArray< Int_t > Ele_id(myRead, "Electron_cutBased"); // (0:fail, 1:veto, 2:loose, 3:medium, 4:tight)
	// pho
	TTreeReaderValue< UInt_t > nPho(myRead, "nPhoton");
	TTreeReaderArray< Float_t > Pho_pt(myRead, "Photon_pt");
	TTreeReaderArray< Float_t > Pho_eta(myRead, "Photon_eta");
	TTreeReaderArray< Int_t > Pho_id(myRead, "Electron_cutBased"); // (0:fail, 1:loose, 2:medium, 3:tight)
	
	TH2D* h_tmp = (TH2D*) h->Clone(""); // for collect events from this inputfile
	h_tmp->Reset();
	int n = 0;
	while (myRead.Next()){  // loop in one root file
		if (*nfj != 1) continue; // we must need only one fatjet
		
		// identify fatjet //
		if (fj_eta[0] > 2.5 || fj_eta[0] < -2.5) continue;
		if (fj_pt[0] < 200) continue;
		if (fj_mass[0] < 100 || fj_mass[0] > 150) continue;
		
		// additional ak4 jet test (<=2)
		int nAk4 = 0; // this is number of additional ak4 jet
		float mindphi=-1; // this is mim dphi of MET and ak4
		for (int i=0; i<(int)*ntj;i++){
			float dphi = fj_phi[0]-Jet_phi[i];
			float deta = fj_eta[0]-Jet_eta[i];
			float dR = TMath::Sqrt(dphi*dphi + deta*deta); // dR b/w fj and ak4j
			float dphi_met_ak4 = TMath::Abs(*MET_phi-Jet_phi[i]);
			if (dR >= 1.2) nAk4++;
			if (i==0) mindphi = dphi_met_ak4;
			else {
				if (mindphi > dphi_met_ak4) mindphi = dphi_met_ak4;
			}
		}
		if (mindphi < 0.4) continue;
		if (nAk4 > 2 ) continue;
		if (*MET_pt < 200) continue;
		
		// ele veto //
		bool isEle = false;
		for (int ie=0; ie<(int)*nEle;ie++){ // loop ele
			if (Ele_pt[ie] < 10) continue;
			if (Ele_id[ie] != 2) continue;
			float abs_Ele_eta = TMath::Abs(Ele_eta[ie]);
			if (abs_Ele_eta > 2.5) continue;
			if (abs_Ele_eta < 1.566 && abs_Ele_eta > 1.4442) continue;
			isEle = true;
			break;
		} // end of loop ele
		if (isEle) continue;
		
		// photon veto //
		bool isPho = false;
		for (int iph=0; iph<(int)*nPho;iph++){ // loop photons
			if (Pho_pt[iph] < 15) continue;
			if (Pho_id[iph] != 1) continue;
			float abs_Pho_eta = TMath::Abs(Pho_eta[iph]);
			if (abs_Pho_eta > 2.5) continue;
			isPho = true;
			break;
		}// end of loop photon
		if (isPho) continue;
		
		// passed all pre-selections, fill in //
		h_tmp->Fill(pNET[0],DDB[0]);
		vD.push_back(DDB[0]);
		//cout << pNET[0] << endl;
		vP.push_back(pNET[0]);
		
	}
	//cout << "num " << n << endl;
	h->Add(h_tmp);
}

void correlated_plot(string inputfilelist, string outputfile = "histo.root"){
	TH2D* h_pNET = new TH2D("h_pNET","",NN2,0,MaxN2,Nddb,Minddb,Maxddb);
		h_pNET->SetYTitle("DDB"); h_pNET->SetXTitle("particleNET");
	
	string line;
	ifstream fin(inputfilelist.data());
	vector<string> lines;
	vector<float> DDB;
	vector<float> PNET;
	vector<vector<float> > vDDB;
	vector<vector<float> > vPNET;
	while(getline(fin,line)){
		//cout << line << endl;
		load_to_hist_bkg(line,h_pNET,DDB,PNET);
		vDDB.push_back(DDB);
		vPNET.push_back(PNET);
		DDB.clear();
		PNET.clear();
		lines.push_back(line);
	}
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
	fout->Write();
	fout->Close();
}		