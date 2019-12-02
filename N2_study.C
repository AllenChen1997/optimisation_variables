///////////////////////////
/*
	this is used to study N2 value in jetpt/ rho

*/
//////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH3D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>

#define Maxpt 2000
#define NN2 20
#define MinN2 0
#define MaxN2 0.5
#define Nrho 20
#define Minrho -5
#define Maxrho 0
using namespace std;

class Data{
private:
	Double_t n2,rho,pt;
public:
	Data(){}
	Data(Double_t& a,Double_t& b,Double_t& c ) : n2(a),rho(b),pt(c){}
	Double_t Getn2() const { return n2; }
	Double_t Getpt() const {return pt;}
	Double_t Getrho() const {return rho;}
	friend ostream& operator<<(ostream& out,const Data& foo){
		return out << foo.n2 << " " << foo.pt << " " << foo.rho << endl;
	}
	friend bool operator<(const Data& a, const Data& b){
		return a.Getn2() < b.Getn2();
	}
		
};

void N2_study(){
	//setup histograms for N2b1/N2b2 and N2DDT
	TH3D* h1 = new TH3D("h1","N2-rho-jetPt",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt); // n2-rho-pt
		TAxis* xaxis = h1->GetXaxis();
		TAxis* yaxis = h1->GetYaxis();
		TAxis* zaxis = h1->GetZaxis();
		xaxis->SetTitle("N^{1.0}_{2}");
		yaxis->SetTitle("#rho");
		zaxis->SetTitle("jetPt");
	TH3D* h2 = (TH3D*) h1->Clone("h2");
		xaxis = h2->GetXaxis();
		xaxis->SetTitle("N^{2.0}_{2}");
	TH3D* h3 = new TH3D("h3","N2ddt-rho-jetpt",NN2,MinN2,MaxN2,Nrho,Minrho,Maxrho,3,0,Maxpt);
		xaxis = h3->GetXaxis();
		xaxis->SetTitle("N2DDT(n2b1)");
		yaxis = h3->GetYaxis();
		yaxis->SetTitle("#rho");
		zaxis = h3->GetZaxis();
		zaxis->SetTitle("jetPt");
	TH3D* h4 = (TH3D*) h3->Clone("h4");
		xaxis = h4->GetXaxis();
		xaxis->SetTitle("N2DDT(n2b2)");
		
	vector<Data> v1[Nrho],v2[Nrho]; // used to store & sort the data 
	ifstream infile("QCD_list.txt"); // used to load input file list. in each line: xxx.root name_for_plot
	string line,s1,s2;
	stringstream ss;
	int i=0,overN=0,lowerN=0; // lowerN is the number of which pt lower than 0, overN is the number of which pt > Maxpt
	double d = (double) (Maxrho - Minrho) / (double)Nrho;  // this is the width of the rho bin
	double ixs;
	while(getline(infile,line)){
      cout << line << endl;
      ss << line;
      ss >> s1 >> s2 >> ixs;// s1=xxx.root, s2=name_for_plot
      ss.clear();
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
		int N = myRead.GetEntries(); //get the entires info.
		if (N == 0) continue;
		i++;
		while (myRead.Next()){
			if (*rho > Maxrho || *rho < Minrho) continue;
			int region_rho = floor((double)(*rho - Minrho) / d ); // use this to decide the data in which rho region 
			v1[region_rho].push_back(Data(*n2b1,*rho,*pt));
			v2[region_rho].push_back(Data(*n2b2,*rho,*pt));
			h1->Fill(*n2b1,*rho,*pt);
			h2->Fill(*n2b2,*rho,*pt);
			if (*pt>Maxpt ) overN++;
			else if (*pt<0) lowerN++;
		}
	}
	Double_t rho5per;
	TGraph* dt1 = new TGraph();
	TGraph* dt2 = new TGraph();
	int n=0;
	for (int i = 0;i<Nrho;i++){
		if (v1[i].size() == 0) continue;
		rho5per = ceil(v1[i].size()*0.05);
		sort(v1[i].begin(),v1[i].end(),less<Data>());
		sort(v2[i].begin(),v2[i].end(),less<Data>());
		Double_t in2b1 = v1[i][rho5per].Getn2();
		Double_t in2b2 = v2[i][rho5per].Getn2();
		cout << "i= " << i << " | " << in2b1 << " " << in2b2 << endl;
		dt1->SetPoint(n,Minrho+(i+0.5)*d,in2b1);
		dt2->SetPoint(n,Minrho+(i+0.5)*d,in2b2);
		n++;
		for (auto x : v1[i])	h3->Fill(x.Getn2()-in2b1, x.Getrho(), x.Getpt() );
		for (auto x : v2[i])	h4->Fill(x.Getn2()-in2b2, x.Getrho(), x.Getpt() );
	}
	
	auto c1 = new TCanvas("c1","c1");
	h1->Project3D("XY")->Draw("VIOLIN");
	dt1->Draw("* SAME");
	
	auto c2 = new TCanvas("c2","c2");
	h2->Project3D("XY")->Draw("VIOLIN");
	dt2->Draw("* SAME");
	
	TFile* fout = new TFile("TH3_output.root","NEW");
	h1->Write();
	h2->Write();
	h3->Write();
	h4->Write();
	c1->Write();
	c2->Write();
	cout << "lower: " << lowerN << "| over: " << overN << endl;
	fout->Close();
	
	/*for (int i = 0; i<Nrho;i++){
		cout << "i= " << i << endl;
		for (auto x:v[i]) cout << x;
	}*/
}
		
		
		
		
