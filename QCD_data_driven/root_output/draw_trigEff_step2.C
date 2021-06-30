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

void draw_trigEff_step2(string inputlist){
	string histlist[9] = {"PFHT250_divide","PFHT370_divide","PFHT430_divide","PFHT510_divide", "PFHT590_divide", "PFHT680_divide","PFHT780_divide","PFHT890_divide","PFHT1050_divide"};


	for (int i=0;i<9;i++){
		int colorN =0;
		string ifile;
		ifstream infile(inputlist.data() );
		gStyle->SetOptStat("");
		TLegend legend(0.7,0.7,0.9,0.9);
		TCanvas* c = new TCanvas("c","c");
		while (getline(infile, ifile)){
			colorN++;
			cout << "Reading " << ifile << endl;
			TFile* myFile;
			myFile = TFile::Open(ifile.data(),"READONLY");
			TH1F* htmp = (TH1F*) myFile->Get(histlist[i].data() );
			htmp->SetLineColor(colorN);
			setDrawOpt(htmp,"","MET","");
			c->cd();
			htmp->Draw("SAME");
			legend.AddEntry(htmp,histlist[i].data() );
		}
		legend.Draw("SAME");
		c->SaveAs(Form("%s.png",histlist[i].data() ) );
	}
}