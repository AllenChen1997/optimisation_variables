#include <TTree.h>
#include <TFile.h>

using namespace std;
// histogram 3D N2-rho-pt
void load_to_hist(string s1, TH3D* h){
	TFile* myfile = new TFile(s1.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	while (myRead.Next()){  // loop in one root file
		h->Fill(*n2b1,*rho,*pt);
		//h2->Fill(*n2b2,*rho,*pt);
	}
}

void load_to_hist(string s1, TH3D* h, vector<double>& v, int& count, int& N){
	TFile* myfile = new TFile(s1.c_str(),"READ");
	TTreeReader myRead("monoHbb_SR_boosted",myfile);  
	TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
	TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
	TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
	TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
	TH1D* h_tmp = (TH1D*) h->ProjectionY("",0,-1,0,-1);
	double width = h_tmp->GetBinWidth(1);
	double init = h_tmp->GetBinLowEdge(1);
	while (myRead.Next()){  // loop in one root file
		int i = floor((double)(*rho - init) / width );
		if(v[i] == -1) {
			h->Fill(*n2b1,*rho,*pt);
			count++;
		}else { 
			h->Fill(*n2b1-v[i],*rho,*pt);
			if ( *n2b1-v[i] < 0 ) count++;
		}
		N++;
	}
}