/*
	this code is used to get r plots after the cuts below
	the cuts in cutFlowLabel
	
	in this version we have 8 MCs, 1 data
	
	there exist bugs for letting code work for both MC and data at the same time.
	lets focus on data first
	the data name and number of HT regions  need to adjust by manually

*/
#define NN2 20
#define N2min 0
#define N2max 0.5
#define NDDB 20
#define DDBmin 0
#define DDBmax 1

using namespace std;

bool isTest = false; // open this for layout more information

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


void loadFitResult(vector<float>& pars){
	TFile* fin = new TFile("fit_result.root");
	TTreeReader mytree("tree",fin);
	TTreeReaderArray<float> par(mytree,"par");
	
	while(mytree.Next() ){
		for (auto x : par)	pars.push_back(x);
	}
	fin->Close();
}
		

void runCode(vector<float> pars){
	// load files 
	string inputMClist[8] = {"QCD_100to200.root","QCD_200to300.root","QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	// string inputMClist[6] = {"QCD_300to500.root","QCD_500to700.root","QCD_700to1000.root","QCD_1000to1500.root","QCD_1500to2000.root","QCD_2000toInf.root"};
	string outputMCname[8] = {"100to200","200to300","300to500","500to700","700to1000","1000to1500","1500to2000","2000toIng"};
	// string outputMCname[6] = {"300to500","500to700","700to1000","1000to1500","1500to2000","2000toIng"};
	float xs[8] = {	23700000.0, 1547000.0, 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};
	// float xs[6] = { 322600.0, 29980.0, 6334.0, 1088.0, 99.11, 20.23};

	int weight; // this one is used for initailization the "weight" variable for MC files(but there is no weight in data)
	
	int totalFiles = sizeof(inputMClist)/sizeof(inputMClist[0]);
	
	// outputs // 	
	// decide the number of output regions
	int totalNRange = 0;
	TFile* fin_0 = new TFile(inputMClist[0].data(), "READONLY");
	TH1F* h_HTRange = (TH1F*) fin_0->Get("h_HTRange");
	totalNRange = h_HTRange->GetNbinsX() + 1;
	
	vector< TH2F* > h_DDB_N2_SR(totalNRange); // notice that it is filled when mindphi > 0.4 (pass region)
	vector< TH2F* > h_DDB_N2_CR(totalNRange); // notice that it is filled when mindphi < 0.4 (fail region)
	vector< TH2F* > tmp_DDB_N2_SR(totalNRange);
	vector< TH2F* > tmp_DDB_N2_CR(totalNRange);
	
	Double_t xbins[16] = {0, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300, 350, 400, 500, 600, 1000};
	string cutFlowLabel[10] = {"incl","tauVeto","photonVeto","EleVeto","LooseMuVeto","extraAk4","nFatJ","hasN2B1","HTRegion","atleast1AK4"};

	for (int i=0; i< totalNRange; i++){
		
		h_DDB_N2_SR[i] = new TH2F(Form("DDB_N2_SR_%i",i) ,"",NN2,N2min,N2max,NDDB,DDBmin,DDBmax);
		setDrawOpt(h_DDB_N2_SR[i],"","N2B1","DDB");
		h_DDB_N2_CR[i] = (TH2F*) h_DDB_N2_SR[i]->Clone(Form("DDB_N2_CR_%i",i) );
		tmp_DDB_N2_SR[i] = (TH2F*) h_DDB_N2_SR[i]->Clone(Form("tmp_DDB_N2_SR_%i",i) );
		tmp_DDB_N2_CR[i] = (TH2F*) h_DDB_N2_SR[i]->Clone(Form("tmp_DDB_N2_CR_%i",i) );
		// call sumw2
		h_DDB_N2_SR[i]->Sumw2();
		h_DDB_N2_CR[i]->Sumw2();
		tmp_DDB_N2_SR[i]->Sumw2();
		tmp_DDB_N2_CR[i]->Sumw2();
	}	
	
	// load the fitting result 
	
	
	for (int iFile=0; iFile<totalFiles; iFile++){
		// reset all vectors 
		for (int x=0; x< totalNRange; x++){
			tmp_DDB_N2_SR[x]->Reset("ICESM");
			tmp_DDB_N2_CR[x]->Reset("ICESM");
		}
		
		TFile* fin;
		cout << "reading " << inputMClist[iFile] << endl;
		fin = new TFile(inputMClist[iFile].data(), "READONLY");
		
		TTreeReader data("tree",fin);
		TTreeReaderValue<float> mindphi(data,"mindphi");
		TTreeReaderValue<float> metpT(data,"metpT");
		TTreeReaderArray<float> DDB(data,"DDB");
		TTreeReaderArray<float> N2B1(data,"N2B1");
		TTreeReaderValue<int> whichHT(data,"whichHT");
		TTreeReaderValue<int> weight(data,"weight");
		TH1F* h_cutFlow = (TH1F*) fin->Get("h_cutFlow");
		
		int total_entry = data.GetEntries(true);
		int jEntry= 0;
		int nN2B1gt1 = 0;
		int nRweightLt0 = 0;
		while(data.Next() ){
			// deal with the entries in the tree
			jEntry++;
			if (isTest && jEntry == 100) break; // in test mode we will have only 100 events
			if (jEntry % 1000000 == 0) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			else if (jEntry == total_entry) cout <<  "Processing event " << jEntry << " of " << total_entry << endl;
			
			if (N2B1.GetSize() > 1) nN2B1gt1++;
			if (jEntry%2 == 1)  continue; // by using this to seperate files into two piece
			
			int p0 = *whichHT * 3;
			float met = *metpT;
			float rweight = TMath::Exp(pars[p0] + pars[p0+1]*met) + pars[p0+2];
			if (*mindphi > 0.4) {
				tmp_DDB_N2_SR[*whichHT]->Fill(N2B1[0],DDB[0],*weight);
			} else {
				tmp_DDB_N2_CR[*whichHT]->Fill(N2B1[0],DDB[0],*weight*rweight);
			}
			if (rweight < 0) nRweightLt0++;
		} // end of entries
		cout << "nN2B1gt1= " << nN2B1gt1 << endl;
		cout << "nRweightLt0 = " << nRweightLt0 << endl;
		float calWeight = xs[iFile] * 2.0 / (float) h_cutFlow->GetBinContent(1);;

		for (int k=0; k<totalNRange; k++){
			h_DDB_N2_SR[k]->Add(tmp_DDB_N2_SR[k], calWeight);
			h_DDB_N2_CR[k]->Add(tmp_DDB_N2_CR[k], calWeight);
		}
		
		
		
		gStyle->SetOptStat("");
		/*string tmpOutputName = outputMCname[iFile];	
		// draw the cutflow plot
		TCanvas* c2 = new TCanvas("c2","c2");
		for (int ilabel=0; ilabel<sizeof(cutFlowLabel)/sizeof(cutFlowLabel[0]); ilabel++){
			h_cutFlow->GetXaxis()->SetBinLabel(ilabel+1,cutFlowLabel[ilabel].data() );	
		}
		h_cutFlow->Draw(); */
		//c2->SaveAs(Form("cutFlow_%s.png",tmpOutputName.data() ) );
		
		// draw the map of mindphi larger than 0.4 or smaller 0.4
		fin->Close();
	} // end of all files

	// print out file
	TCanvas* c = new TCanvas("c","c");
	
	for (int i = 0; i< totalNRange ;i++){	
		// using projection to get 1D hist
		// draw n2b1
		TH1F* h_n2b1_SR = (TH1F*) h_DDB_N2_SR[i]->ProjectionX();
		TH1F* h_n2b1_CR = (TH1F*) h_DDB_N2_CR[i]->ProjectionX();
		setDrawOpt(h_n2b1_SR,"","N_2^1","");
		h_n2b1_SR->Draw();
		h_n2b1_CR->SetLineColor(kRed);
		h_n2b1_CR->Draw("SAME");
		c->SaveAs(Form("compare_n2b1_%i.png",i) );
		// draw ddb
		TH1F* h_DDB_SR = (TH1F*) h_DDB_N2_SR[i]->ProjectionY();
		TH1F* h_DDB_CR = (TH1F*) h_DDB_N2_CR[i]->ProjectionY();
		setDrawOpt(h_DDB_SR,"","DDB","");
		h_DDB_SR->Draw();
		h_DDB_CR->SetLineColor(kRed);
		h_DDB_CR->Draw("SAME");
		c->SaveAs(Form("compare_DDB_%i.png",i) );
	}
	
}
void closureTest(){
	vector<float> pars;
	loadFitResult(pars);
	runCode(pars);
	
}
