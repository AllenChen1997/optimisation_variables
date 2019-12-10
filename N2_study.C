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
#define Nrho 16
#define Minrho -5
#define Maxrho -1
// pt bin region [200, 350, 480,1000]
#define pt_r1 200
#define pt_r2 350
#define pt_r3 480
#define pt_r4 1000
#define Npt 3
#define if_diffpt 1
using namespace std;

double d = (double) (Maxrho - Minrho) / (double)Nrho;  // this is the width of the rho bin

class Data{
private:
	Double_t n2b1,n2b2,rho,pt;
public:
	Data(){}
	Data(Double_t& a,Double_t& b,Double_t& c,Double_t& d ) : n2b1(a),n2b2(b),rho(c),pt(d){}
	Double_t Getn2b1() const { return n2b1; }
	Double_t Getn2b2() const { return n2b2; }
	Double_t Getpt() const {return pt;}
	Double_t Getrho() const {return rho;}
	friend ostream& operator<<(ostream& out,const Data& foo){
		return out << foo.n2b1 << " " << foo.n2b2 << " " << foo.pt << " " << foo.rho << endl;
	}
	friend bool operator<(const Data& a, const Data& b){
		return a.Getn2b1() < b.Getn2b1();
	}
	friend bool operator>(const Data& a, const Data& b){
		return a.Getn2b2() < b.Getn2b2();
	}
		
};

void Draw_and_write( vector<vector<Data>>& v, string name, vector<Double_t>& vn2b1, vector<Double_t>& vn2b2 ){
// this function will write to the most current opened file
// find the 5% N2 value by sort the vector
	Double_t rho5per;
	TGraph* dt1 = new TGraph();
	TGraph* dt2 = new TGraph();
	TGraph* dt0 = new TGraph();
	
	TH3D* h1 = new TH3D(Form("h_%s_n2b1",name.c_str()),"N2-rho-jetPt",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt); // n2-rho-pt
		TAxis* xaxis = h1->GetXaxis();
		TAxis* yaxis = h1->GetYaxis();
		TAxis* zaxis = h1->GetZaxis();
		xaxis->SetTitle("N^{1.0}_{2}");
		yaxis->SetTitle("#rho");
		zaxis->SetTitle("jetPt");
	TH3D* h2 = (TH3D*) h1->Clone(Form("h_%s_n2b2",name.c_str()));
		xaxis = h2->GetXaxis();
		xaxis->SetTitle("N^{2.0}_{2}");
	TH3D* h3 = new TH3D(Form("h_%s_N2b1DDT",name.c_str()),"N2ddt-rho-jetpt",2*NN2,-MaxN2,MaxN2,Nrho,Minrho,Maxrho,3,0,Maxpt);
		xaxis = h3->GetXaxis();
		xaxis->SetTitle("N2DDT(N^{1.0}_{2})");
		yaxis = h3->GetYaxis();
		yaxis->SetTitle("#rho");
		zaxis = h3->GetZaxis();
		zaxis->SetTitle("jetPt");
	TH3D* h4 = (TH3D*) h3->Clone(Form("h_%s_N2b2DDT",name.c_str()));
		xaxis = h4->GetXaxis();
		xaxis->SetTitle("N2DDT(N^{2.0}_{2})");
	int n=0;
	for (int i = 0;i<v.size();i++){
		if (v[i].empty() ) {
			vn2b1.push_back(-1);
			vn2b2.push_back(-1);
			continue;
		}
		rho5per = ceil(v[i].size()*0.05);
		sort(v[i].begin(),v[i].end(),less<Data>()); // use the "defined" less function in class to sort
		Double_t in2b1 = v[i][rho5per].Getn2b1();
		sort(v[i].begin(),v[i].end(),greater<Data>());
		Double_t in2b2 = v[i][rho5per].Getn2b2();
		//cout << "i= " << i << " | " << in2b1 << " " << in2b2 << endl;
		dt1->SetPoint(n,Minrho+(i+0.5)*d,in2b1);
		dt2->SetPoint(n,Minrho+(i+0.5)*d,in2b2);
		dt0->SetPoint(n,Minrho+(i+0.5)*d,0);
		n++;
		for (auto x : v[i])	{
			h1->Fill(x.Getn2b1(), x.Getrho(), x.Getpt() );
			h2->Fill(x.Getn2b2(), x.Getrho(), x.Getpt() );
			h3->Fill(x.Getn2b1()-in2b1, x.Getrho(), x.Getpt() );
			h4->Fill(x.Getn2b2()-in2b2, x.Getrho(), x.Getpt() );
		}
		vn2b1.push_back(in2b1);
		vn2b2.push_back(in2b2);
	}

	// plot and output
	auto c1 = new TCanvas(Form("c_%s_n2b1",name.c_str()),Form("c_%s_n2b1",name.c_str()) );
	h1->Project3D("XY")->Draw("VIOLIN(03001000)");
	dt1->Draw("* SAME");
	c1->Write();
	c1->SaveAs(Form("c_%s_n2b1.png",name.c_str()) );
	auto c2 = new TCanvas(Form("c_%s_n2b2",name.c_str()),Form("c_%s_n2b2",name.c_str()) );
	h2->Project3D("XY")->Draw("VIOLIN(03001000)");
	dt2->Draw("* SAME");
	c2->Write();
	c2->SaveAs(Form("c_%s_n2b2.png",name.c_str()) );
	auto c3 = new TCanvas(Form("c_%s_n2b1DDT",name.c_str()),Form("c_%s_n2b1DDT",name.c_str()) );
	h3->Project3D("XY")->Draw("VIOLIN(03001000)");
	dt0->Draw("* SAME");
	c3->Write();
	c3->SaveAs(Form("c_%s_n2b1DDT.png",name.c_str()) );
	auto c4 = new TCanvas(Form("c_%s_n2b2DDT",name.c_str()),Form("c_%s_n2b2DDT",name.c_str()) );
	h4->Project3D("XY")->Draw("VIOLINX(03001000)");
	dt0->Draw("* SAME");
	c4->Write();
	c4->SaveAs(Form("c_%s_n2b2DDT.png",name.c_str()) );
	//h1->Write();
	//h2->Write();
}

void N2_study(){
	vector< vector<Data>> v1(Nrho); // used to store & sort the data in different rho region
	vector< vector<Data>> v_pt1(Nrho),v_pt2(Nrho),v_pt3(Nrho); // used to store & sort the data in different pt region
	vector< Double_t > v1_n2b1, v1_n2b2;
	// input file data
	ifstream infile("QCD_list.txt"); // used to load input file list. in each line: xxx.root name_for_plot
	string line,s1,s2;
	stringstream ss;
	int i=0,overN=0,lowerN=0; // lowerN is the number of which pt lower than 0, overN is the number of which pt > Maxpt
	
	double ixs;
	while(getline(infile,line)){ // loop in the root file list
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
		while (myRead.Next()){  // loop in one root file
			if (*rho > Maxrho || *rho < Minrho) continue;
			int region_rho = floor((double)(*rho - Minrho) / d ); // use this to decide the data in which rho region 
			v1[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			// cut in different pt region + different rho region
			if (*pt >= pt_r1 && *pt < pt_r2 ) v_pt1[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			else if (*pt >= pt_r2 && *pt < pt_r3) v_pt2[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			else if (*pt >= pt_r3 && *pt < pt_r4) v_pt3[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
		
			if (*pt>Maxpt ) overN++;
			else if (*pt<0) lowerN++;
		}
	}

	// prepare to output
	TFile* fout = new TFile("TH3_output.root","NEW");
	TTree outTree("tree","out branches");
	outTree.Branch("noPt_n2b1", &v1_n2b1);
	outTree.Branch("noPt_n2b2", &v1_n2b2);
	Draw_and_write(v1,"noPt",v1_n2b1,v1_n2b2);
	//Draw_and_write(v_pt1,"pt_200to350",outTree);
	//Draw_and_write(v_pt2,"pt_350to480",outTree);
	//Draw_and_write(v_pt3,"pt_480to1000",outTree);
	outTree.Fill();
	fout->Write();
	cout << "lower: " << lowerN << "| over: " << overN << endl;
	fout->Close();
}
