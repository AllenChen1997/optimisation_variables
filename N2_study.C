////////////////////////////////////////////////
/*
	this is used to study N2 value in jetpt/ rho
	in the end, output the root file with X(N% selection eff. of QCD) N = 20,26
	
	the flow of the work:
	1. load data to the vector: v1, v_pt1, v_pt2, v_pt3, and v_pt4 -> seperated with different rho region
	   v_total -> include all rho region
	2. run Draw_and_write() for getting n2b1 and n2b2 values in vector form
	3. run sort_Nper() for getting n2b1 values (without seperated by rho region)
	4. output TH3_output.root: this root file contains some violin plots and the "5, 20, 26, 50% QCD selection eff." n2b1 and n2b2 values 
	
*/
////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TH3D.h>
#include <TH2D.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLatex.h>
#include <TAxis.h>
#include <functional>

// the param. setting for TH3 histogram
#define Maxpt 2000
#define NN2 28
#define MinN2 -0.2
#define MaxN2 0.5
#define Nrho 20
#define Minrho -6
#define Maxrho -1
// pt bin region [200, 400, 600, 800, inf]
#define pt_r1 200
#define pt_r2 400
#define pt_r3 600
#define pt_r4 800
#define Npt 4
using namespace std;

double d = (double) (Maxrho - Minrho) / (double)Nrho;  // this is the width of the rho bin
// creat spacial data type for std::vector
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
	// less() is used for n2b1
	friend bool operator<(const Data& a, const Data& b){
		return a.Getn2b1() < b.Getn2b1();
	}
	// greater() is used for n2b2
	friend bool operator>(const Data& a, const Data& b){
		return a.Getn2b2() < b.Getn2b2();
	}
		
};

void sort_Nper( vector<Data>& v, vector<Double_t>& vout1){ // this is used to output N2b1 cuts
	double percent[4] = {0.05,0.2,0.26,0.5}; // 5, 20 ,26, 50 %
	sort(v.begin(),v.end(),less<Data>());
	for (int i=0;i<4;i++){
		double num = v[round(percent[i]*v.size())].Getn2b1();
		vout1.push_back(num);
	}
}
template<typename T>
void setDrawOpt(T& h,string title, string xTitle, string yTitle){
	h->SetTitle(title.c_str());
	h->SetTitleSize(0.07);
	h->GetXaxis()->SetLabelSize(0.05);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetLabelSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->SetXTitle(xTitle.c_str());
	h->SetYTitle(yTitle.c_str());
}
	

void Draw_and_write( vector<vector<Data>>& v, string name, vector<Double_t>& vn2b1, vector<Double_t>& vn2b2, double Npercent ){
// this function will write to the most current opened file
// find the 20% N2 value by sort the vector
	Double_t rho5per;
	TGraph* dt1 = new TGraph();
	TGraph* dt2 = new TGraph();
	TGraph* dt0 = new TGraph();
	
	TH3D* h1 = new TH3D(Form("h_%s_n2b1",name.c_str()),"N2-rho-jetPt",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt); // n2-rho-pt
	TH3D* h2 = (TH3D*) h1->Clone(Form("h_%s_n2b2",name.c_str()));
	TH3D* h3 = new TH3D(Form("h_%s_N2b1DDT",name.c_str()),"N2ddt-rho-jetpt",NN2,MinN2,MaxN2,Nrho,Minrho,Maxrho,3,0,Maxpt);
	TH3D* h4 = (TH3D*) h3->Clone(Form("h_%s_N2b2DDT",name.c_str()));
	int n=0;
	for (int i = 0;i<v.size();i++){
		if (v[i].empty() ) {
			vn2b1.push_back(-1);
			vn2b2.push_back(-1);
			continue;
		}
		rho5per = round(v[i].size()*Npercent);
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

	// plot 
	gStyle->SetOptStat("");	
	TLegend* legend = new TLegend(0.8,0.8,0.7,0.7);
	//legend->AddEntry(dt1,"X(20% QCD selection eff.)");
	
	// c1 = n2b1 violin plot //
	auto c1 = new TCanvas(Form("c_%s_n2b1",name.c_str()),Form("c_%s_n2b1",name.c_str()) );
	TH1* h11 = h1->Project3D("XY"); setDrawOpt(h11, name+"%_n2b1", "#rho", "N^{1.0}_{2}");
	h11->Draw("VIOLIN(03001000)");
	dt1->Draw("* SAME");
	c1->SaveAs(Form("c_%s_n2b1.png",name.c_str()) );
	
	// c2 = n2b2 violin plot //
	auto c2 = new TCanvas(Form("c_%s_n2b2",name.c_str()),Form("c_%s_n2b2",name.c_str()) );
	TH1* h22 = h2->Project3D("XY"); setDrawOpt(h22, name+"%_n2b2", "#rho", "N^{2.0}_{2}");
	h22->Draw("VIOLIN(03001000)");
	dt2->Draw("* SAME");
	c2->SaveAs(Form("c_%s_n2b2.png",name.c_str()) );
	
	// c3 = n2DDT(b=1) violin plot //		
	auto c3 = new TCanvas(Form("c_%s_n2b1DDT",name.c_str()),Form("c_%s_n2b1DDT",name.c_str()),800,500 );
	TH1* h33 = h3->Project3D("XY"); setDrawOpt(h33, name+"%_n2b1DDT", "#rho", "N2DDT(N^{1.0}_{2})");
	h33->Draw("VIOLIN(03001000)");
	dt0->Draw("* SAME");
	c3->SaveAs(Form("c_%s_n2b1DDT.png",name.c_str()) );
	
	// c4 = n2DDT(b=2) violin plot //
	auto c4 = new TCanvas(Form("c_%s_n2b2DDT",name.c_str()),Form("c_%s_n2b2DDT",name.c_str()),800,500 );
	TH1* h44 = h4->Project3D("XY"); setDrawOpt(h44, name+"%_n2b2DDT", "#rho", "N2DDT(N^{2.0}_{2})");
	h44->Draw("VIOLINX(03001000)");
	dt0->Draw("* SAME");
	c4->SaveAs(Form("c_%s_n2b2DDT.png",name.c_str()) );
}

TH2D* getRhoPt(int num, vector< vector< Data> > v_pt1, vector< vector< Data> > v_pt2, vector< vector< Data> > v_pt3, vector< vector< Data> > v_pt4){
	vector< Double_t > v_for_no_used,v_for_no_used_2;// just for call the function
	TH2D* h_pt_rho = new TH2D(Form("h_pt_rho_%i",num ),"",Nrho,Minrho,Maxrho,4,200,1000);
	string name = to_string(num);
	Draw_and_write(v_pt1,"pt200to400_"+name,v_for_no_used,v_for_no_used_2,num*0.01);
	for (int i=0;i<v_for_no_used.size();i++){
		if (v_for_no_used[i] == -1) continue;
		h_pt_rho->SetBinContent(i+1,1,v_for_no_used[i]);
	}
	v_for_no_used.clear(); v_for_no_used_2.clear();
	Draw_and_write(v_pt2,"pt400to600_"+name,v_for_no_used,v_for_no_used_2,num*0.01);
	for (int i=0;i<v_for_no_used.size();i++){
		if (v_for_no_used[i] == -1) continue;
		h_pt_rho->SetBinContent(i+1,2,v_for_no_used[i]);
	}
	v_for_no_used.clear(); v_for_no_used_2.clear();
	Draw_and_write(v_pt3,"pt600to800_"+name,v_for_no_used,v_for_no_used_2,num*0.01);
	for (int i=0;i<v_for_no_used.size();i++){
		if (v_for_no_used[i] == -1) continue;
		h_pt_rho->SetBinContent(i+1,3,v_for_no_used[i]);
	}
	v_for_no_used.clear(); v_for_no_used_2.clear();
	Draw_and_write(v_pt4,"ptOver800_"+name,v_for_no_used,v_for_no_used_2,num*0.01);
	for (int i=0;i<v_for_no_used.size();i++){
		if (v_for_no_used[i] == -1) continue;
		h_pt_rho->SetBinContent(i+1,4,v_for_no_used[i]);
	}
	gStyle->SetPaintTextFormat("5.3f");
	h_pt_rho->SetMinimum(0.2);
	h_pt_rho->SetMaximum(0.35);
	h_pt_rho->SetMarkerSize(1.6);
	TCanvas* cpt = new TCanvas("cpt","cpt",800,500);
	setDrawOpt(h_pt_rho, "", "#rho", "P_{T} (GeV)");
	h_pt_rho->SetTitleOffset(1.0,"Y");
	h_pt_rho->Draw("COLZ TEXT45");
	cpt->SaveAs(Form("n2b1_rho_pt_%s.png",name.c_str() ) );
	return h_pt_rho;
}

void N2_study(){
	vector< vector<Data>> v1(Nrho); // used to store & sort the data in different rho region
	vector< vector<Data>> v_pt1(Nrho),v_pt2(Nrho),v_pt3(Nrho),v_pt4(Nrho); // used to store & sort the data in different pt region
	vector< Double_t > n2b1_v20, n2b1_v26, n2b2_v20, n2b2_v26, n2b1_cut; // the output values will put into them
	vector< Data > v_total; // for generate N2 cut
	TH3D* h_n2b1 = new TH3D("h_n2b1","h_n2b1",NN2,MinN2,MaxN2, Nrho,Minrho,Maxrho, 3,0, Maxpt); // for short test
	TH3D* h_n2b2 = (TH3D*) h_n2b1->Clone("h_n2b2");
	// input file data
	ifstream infile("QCD_list.txt"); // used to load input file list. in each line: xxx.root name_for_plot
	string line;
	int i=0,overN=0,lowerN=0; // lowerN is the number of which pt lower than 0, overN is the number of which pt > Maxpt
	
	while(getline(infile,line)){ // loop in the root file list
	   cout << line << endl;
		TFile* myfile = new TFile(line.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);  
		TTreeReaderValue< Double_t > n2b1(myRead,"FJetN2b1");
		TTreeReaderValue< Double_t > n2b2(myRead,"FJetN2b2");
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		TTreeReaderValue< Double_t > pt(myRead,"FJetPt");
		TTreeReaderValue< Double_t > mass(myRead,"FJetMass");
		int N = myRead.GetEntries(); //get the number entires info.
		if (N == 0) continue;
		i++;
		while (myRead.Next()){  // loop in one root file
			if (*mass < 30) continue;
			v_total.push_back(Data(*n2b1,*n2b2,*rho,*pt));
			if (*rho > Maxrho || *rho < Minrho) continue;
			int region_rho = floor((double)(*rho - Minrho) / d ); // use this to decide the data in which rho region 
			v1[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			h_n2b1->Fill(*n2b1,*rho,*pt);
			h_n2b2->Fill(*n2b2,*rho,*pt);
			// cut in different pt region + different rho region
			if (*pt >= pt_r1 && *pt < pt_r2 ) v_pt1[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			else if (*pt >= pt_r2 && *pt < pt_r3) v_pt2[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			else if (*pt >= pt_r3 && *pt < pt_r4) v_pt3[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			else if (*pt >= pt_r4) v_pt4[region_rho].push_back(Data(*n2b1,*n2b2,*rho,*pt));
			if (*pt<0) lowerN++;
		}
	}

	// prepare to output
	TFile* fout = new TFile("TH3_output.root","NEW");
	TTree outTree("tree","out branches");
	outTree.Branch("n2b1_v20", &n2b1_v20);
	outTree.Branch("n2b2_v20", &n2b2_v20);
	outTree.Branch("n2b1_v26", &n2b1_v26);
	outTree.Branch("n2b2_v26", &n2b2_v26);
	outTree.Branch("n2b1_cut", &n2b1_cut); // no seperate with rho regions
	Draw_and_write(v1,"20",n2b1_v20,n2b2_v20,0.2);
	Draw_and_write(v1,"26",n2b1_v26,n2b2_v26,0.26);
	// for the diff-pt-regions //
		TH2D* h_rho_pt_5 = getRhoPt(5,v_pt1,v_pt2,v_pt3,v_pt4);
		TH2D* h_rho_pt_20 = getRhoPt(20,v_pt1,v_pt2,v_pt3,v_pt4);
		TH2D* h_rho_pt_26 = getRhoPt(26,v_pt1,v_pt2,v_pt3,v_pt4);
		TH2D* h_rho_pt_50 = getRhoPt(50,v_pt1,v_pt2,v_pt3,v_pt4);

	// get different N2b1 cut //
	sort_Nper(v_total,n2b1_cut);
	
	outTree.Fill();
	fout->Write();
	cout << "lower: " << lowerN << endl;
	fout->Close();
}
