/*
	this code is used to overlay pt plots for all of mass point of signals
*/
#include "gethisto.C"
// nbins in gethisto
using namespace std;

void pt_plots(string inputlist="signal_list_for_plot.txt"){
	ifstream infile(inputlist.c_str() );
	string line,fname,lname;
	stringstream ss;
	int Xmin=0; // histo min 
	int Xmax=10; // histo max
	int Nf=0; // count the # of files
	cout << "desiding the range of histo." << endl;
	while (getline(infile,line) ){
		ss << line;
		ss >> fname >> lname;
		ss.clear();
		gethisto(fname,Xmin,Xmax);
		Nf++;
	}
	infile.close();
	cout << "the range is [" << Xmin << "," << Xmax << "]" << endl;
	
	// //
	cout << "start to get histos" << endl;
	TH1D* h[Nf];
	TLegend* legend = new TLegend(0.7,0.7,0.9,0.9);
//	h[0] = new TH1D("hpt0","",nbins,Xmin,Xmax);
//	for (int i=1; i<Nf; i++) h[i] = h[0]->Clone(Form("hpt%i",i) );
	infile.open(inputlist.c_str() );
	Nf = 0;
	int hmax=0;
	while (getline(infile,line) ){
		ss << line;
		ss >> fname >> lname;
		ss.clear();
		cout << "input " << fname << endl;
		h[Nf] = gethisto(fname,Xmin,Xmax);
		h[Nf]->Scale(1.0/h[Nf]->Integral() );
		if (hmax < h[Nf]->GetMaximum() ) hmax = h[Nf]->GetMaximum();
		legend->AddEntry(h[Nf],(TString)lname,"l");
		Nf++;
	}
	// plot out //
	int colors[7] = {1,632,416,800,880,600,432}; // kBlack,kRed,kGreen,kOrange,kViolet,kBlue,kCyan
	gStyle->SetOptStat("");
	h[0]->SetMaximum(hmax*1.2);
	setDrawOpt(h[0],"","pT",""); // this function is in gethisto.C
	auto c1 = new TCanvas("c1","c1");
	for (int i=0; i<Nf; i++){
		if (i < 7) h[i]->SetLineColor(colors[i]);
		else h[i]->SetLineColor(i+1);
		h[i]->Draw("SAME");
	}
	legend->Draw();
	c1->SaveAs("pt_after_preselection.png");
}