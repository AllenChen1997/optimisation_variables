using namespace std;

void keep_output_controler(string inputfile,string n){
	TFile* fin = new TFile(inputfile.data(),"READONLY" );
	for (int i=0; i<6; i++){
		TH1F* h = (TH1F*)fin->Get(Form("h_withSR_%i",i));
		TH1F* h2 = (TH1F*)fin->Get(Form("h_%i",i));
		TCanvas* c1 = new TCanvas("c1","c1");
		h->GetYaxis()->SetRangeUser(0,10.);
		h->Draw("HIST E");
		h2->SetLineColor(kRed);
		h2->Draw("SAME HIST E");
		c1->SaveAs(Form("zoomin_cut%s_%i.png",n.data(),i) );
	}
	
	
	
}
