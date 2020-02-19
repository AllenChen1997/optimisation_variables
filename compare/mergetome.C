using namespace std;

void mergetome(){
	TFile* infile = new TFile("Output_smooth_2017MC.root");
	TH2F* h_input = (TH2F*)infile->Get("Rho2D");
	// setup the max Nbins //
	int Nx,Ny;
	Nx = h_input->GetNbinsX();
	Ny = h_input->GetNbinsY();
	
	/* 
	   for x axis, we merge 10 bins into 1 bin
	   for y axis, we merge bins by regions [450,600,800,1000]
	   in each one, they contains [1-50,50-115,116-Ny](bin idx)
	*/
	TH2F* h_output = new TH2F("Rho2D","Rho2D",18,-6,-1.5,3,400,1000);
	// for y bin = 1 //
	int merge_count = 0, output_count = 1;
	float tmp_merge = 0;
	for (int i=1;i<=Nx;i++){ // i for x axis
		for (int j=1;j<=50;j++){ // j for y axis
			tmp_merge+=(float)h_input->GetBinContent(i,j);
			merge_count++;
		}
		if (i%10==0) {
			h_output->SetBinContent(output_count,1,(float)tmp_merge/(float)merge_count);
			merge_count = 0;
			tmp_merge = 0;
			output_count++;
		}
	}
	
	// for y bin = 2 //
	merge_count = 0, output_count = 1;
	tmp_merge = 0;
	for (int i=1;i<=Nx;i++){ // i for x axis
		for (int j=50;j<=115;j++){ // j for y axis
			tmp_merge+=(float)h_input->GetBinContent(i,j);
			merge_count++;
		}
		if (i%10==0) {
			h_output->SetBinContent(output_count,2,(float)tmp_merge/(float)merge_count);
			merge_count = 0;
			tmp_merge = 0;
			//cout<< output_count << endl;
			output_count++;
		}
	}
	
	// for y bin = 3 //
	merge_count = 0, output_count = 1;
	tmp_merge = 0;
	for (int i=1;i<=Nx;i++){ // i for x axis
		for (int j=115;j<=Ny;j++){ // j for y axis
			tmp_merge+=(float)h_input->GetBinContent(i,j);
			merge_count++;
		}
		if (i%10==0) {
			h_output->SetBinContent(output_count,3,(float)tmp_merge/(float)merge_count);
			merge_count = 0;
			tmp_merge = 0;
			output_count++;
		}
	}
	// output plot //
	auto c1 = new TCanvas("c1","c1");
	gStyle->SetOptStat("");
	gStyle->SetPaintTextFormat("5.3f");
	h_output->SetMinimum(0.2);
	h_output->SetMaximum(0.35);
	h_output->Draw("COLZ TEXT0");
	c1->SaveAs("Rho2D.png");
}