////////////////////////////////////////////////
/*
	this code is used to get any variable in branch as a histo.
*/
////////////////////////////////////////////////

#include "N2_study.C"
using namespace std;

#define nbins 20

bool isApplyN2DDT = true;

TH1D* gethisto(string inputf, int& histomin, int& histomax);

TH1D* gethisto(string inputf){
	int histomax = 10;
	int histomin = 0;
	gethisto(inputf,histomin,histomax);
	cout << "min " << histomin << " max " << histomax << endl;
	TH1D* h_out = gethisto(inputf,histomin,histomax);
	return h_out;
}

TH1D* gethisto(string inputf, int& histomin, int& histomax){
	TH2D* h_rho_pt;
	if (isApplyN2DDT) {
		TFile* cutsource = new TFile("TH3_output.root","READ");
		h_rho_pt = (TH2D*)cutsource->Get("h_pt_rho_26");
	}
	TH1D* h_tmp = new TH1D("hout","",nbins,histomin,(int)histomax*1.2);
	// input file data
	//cout << inputf << endl;
	TFile* myfile = new TFile(inputf.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
	TTreeReaderValue< Double_t > dphi(myRead,"min_dPhi");
	TTreeReaderValue< Double_t > ddb(myRead,"FJetCSV");
	TTreeReaderValue< Double_t > nj(myRead,"nJets");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	int N = myRead.GetEntries(); //get the number entires info.
	cout << "without 4 cut " << N << " " ;
	if (N == 0) exit;
	N=0; // use to cal. pass pre-selection #
	int over300 = 0; int over350 = 0;
	while (myRead.Next()){  // loop in one root file
		// apply 4 cuts as preselection //
		if (*dphi <= 0.4 ) continue;
		if (*mass < 100 || *mass > 150) continue;
		if (*ddb <= 0.86 ) continue;
		if (*nj > 2) continue;
		// apply N2DDT //
		if (isApplyN2DDT){
			if (*rho < Minrho ||*rho > Maxrho) continue;
			int x = ceil((double)(*rho - Minrho) / d );
			int y;
			if (*pt >= pt_r1 && *pt < pt_r2 ) y = 1;
			else if (*pt >= pt_r2 && *pt < pt_r3) y = 2;
			else if (*pt >= pt_r3 && *pt < pt_r4) y = 3;
			else if (*pt >= pt_r4) y = 4;
			else continue;
			double n_cut = h_rho_pt->GetBinContent(x,y);
			if (n_cut == 0) continue;
			if ( *n2b1 - n_cut > 0 ) continue;
		}
		N++;
		// fill into histo //
		h_tmp->Fill(*pt);
		if (*pt > histomax) histomax = (int)*pt;
		if (*pt < histomin) histomin = (int)*pt;
		if (*pt > 300) {
			over300++;
			if (*pt > 350) over350++;
		}
	}
	//cout << inputf << endl;
	cout << "passing " << N << endl;
	cout << "over300 " << over300 << " over350 " << over350 << endl;
	return 	h_tmp;
}