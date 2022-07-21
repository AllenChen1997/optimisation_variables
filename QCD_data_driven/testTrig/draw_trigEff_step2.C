/* 
	this code is used to compare the outputs from draw_trigEff.C
	-> for comparing the difference in all period data
*/
using namespace std;

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

void draw_trigEff_step2(string inputlist="list_draw_step2.txt",bool withLegend = true){
	string histlist[10] = {"PFHT250_divide","PFHT350_divide","PFHT370_divide","PFHT430_divide","PFHT510_divide", "PFHT590_divide", "PFHT680_divide","PFHT780_divide","PFHT890_divide","PFHT1050_divide"};
	int total_hist = sizeof(histlist) / sizeof(histlist[0]);
	for (int i=0;i<total_hist;i++){
		int colorN =0;
		string line;
		stringstream ss;
		string ifile, ilegend;
		ifstream infile(inputlist.data() );
		gStyle->SetOptStat("");
		TLegend legend(0.1,0.7,0.3,0.9);
		TCanvas* c = new TCanvas("c","c");
		while (getline(infile, line)){
			ifile = "";
			ilegend = "";
			ss << line; // using ss to split one line into two strings
			ss >> ifile >> ilegend;
			ss.clear();
			if (ifile.empty() ) continue;
			if (ilegend.empty() ) withLegend = false;
			colorN++;
			if (colorN == 5) colorN++;
			cout << "Reading " << ifile << endl;
			TFile* myFile;
			myFile = TFile::Open(ifile.data(),"READONLY");
			TH1F* htmp = (TH1F*) myFile->Get(histlist[i].data() );
			htmp->SetLineColor(colorN);
			setDrawOpt(htmp,"","HT","");
			c->cd();
			htmp->Draw("SAME");
			legend.AddEntry(htmp,ilegend.data() );
		}
		if (withLegend) legend.Draw("SAME");
		c->SaveAs(Form("%s.png",histlist[i].data() ) );
	}
}