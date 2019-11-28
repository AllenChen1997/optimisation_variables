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
#define minN2 0
#define maxN2 0.5
#define Nrho 20
#define minrho -5
#define maxrho 0
using namespace std;

class Data{
private:
	Double_t n2,pt,rho;
public:
	Data(){}
	Data(Double_t& a,Double_t& b,Double_t& c ) : n2(a),pt(b),rho(c){}
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
	TH3D* h1 = new TH3D("h1","N2-rho-jetPt",NN2,minN2,maxN2, Nrho,minrho,maxrho, 3,0, Maxpt); // n2-rho-pt
		TAxis* xaxis = h1->GetXaxis();
		TAxis* yaxis = h1->GetYaxis();
		TAxis* zaxis = h1->GetZaxis();
		xaxis->SetTitle("N^{1.0}_{2}");
		yaxis->SetTitle("#rho");
		zaxis->SetTitle("jetPt");
	TH3D* h2 = (TH3D*) h1->Clone("h2");
		xaxis = h2->GetXaxis();
		xaxis->SetTitle("N^{2.0}_{2}");
	TH3D* h3 = new TH3D("h3","N2ddt-rho-jetpt",2*NN2,-maxN2,maxN2,Nrho,minrho,maxrho,3,0,Maxpt);
		xaxis = h3->GetXaxis();
		xaxis->SetTitle("N2DDT(n2b1)");
		yaxis = h3->GetYaxis();
		yaxis->SetTitle("#rho");
		zaxis = h3->GetZaxis();
		zaxis->SetTitle("jetPt");
	TH3D* h4 = (TH3D*) h3->Clone("h4");
		xaxis = h4->GetXaxis();
		xaxis->SetTitle("N2DDT(n2b2)");
		
	vector<Data> v1[Nrho],v2[Nrho];
	ifstream infile("QCD_list.txt"); // used to input file. in each line: xxx.root name_for_plot
	string line,s1,s2;
	stringstream ss;
	int i=0,overN=0,lowerN=0;
	double d = (double) (maxrho - minrho) / (double)Nrho;
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
			int region_rho = floor((double)(*rho - minrho) / d );
//cout << region_rho << " ";
			if (region_rho < 0) continue;
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
		dt1->SetPoint(n,minrho+(i+0.5)*d,in2b1);
		dt2->SetPoint(n,minrho+(i+0.5)*d,in2b2);
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
		
		
		
		
