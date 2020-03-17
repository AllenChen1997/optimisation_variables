/*

captp1 = N2DDT(26%)

*/


using namespace std;

void makemyDatacard(string fail="fail.root", string pass="pass.root") {

  TFile *fpass = TFile::Open((TString)pass,"READONLY");
  TFile *ffail = TFile::Open((TString)fail,"READONLY");
  TH1D  *h_obs_pass = (TH1D*)fpass->Get("h_data");
  TH1D  *h_obs_fail = (TH1D*)ffail->Get("h_data");
  TH1D  *h_catp1_pass = (TH1D*)fpass->Get("h_MC");
  TH1D  *h_catp1_fail = (TH1D*)ffail->Get("h_MC");
  ofstream myfile("SF.txt");
  
  
  myfile << "imax 2  number of channels\n";
  myfile << "jmax 1  number of backgrounds\n";
  myfile << "kmax *  number of nuisance parameters (sources of systematical uncertainties)\n";
  myfile << "------------\n";
  myfile << "shapes  *  pass   " << pass << "  $PROCESS $PROCESS_$SYSTEMATIC\n";
  myfile << "shapes  *  fail   " << fail << "  $PROCESS $PROCESS_$SYSTEMATIC\n";
  myfile << "------------\n";
  myfile << "# we have just one channel, in which we observe 0 events\n";
  myfile << "bin             pass    fail\n";
  myfile << "observation      " << h_obs_pass->Integral(1,h_obs_pass->GetNbinsX()) << "  " <<  h_obs_fail->Integral(1,h_obs_fail->GetNbinsX())  << "\n";
  myfile << "------------\n";
  myfile << "# now we list the expected events for signal and all backgrounds in that bin\n";
  myfile << "# the second 'process' line must have a positive number for backgrounds, and 0 for signal\n";
  myfile << "# then we list the independent sources of uncertainties, and give their effect (syst. error)\n";
  myfile << "# on each process and bin\n";
  myfile << "bin          pass       fail     pass         fail\n";
  myfile << "process      h_data    h_data    h_MC       h_MC\n";  
  myfile << "process        0         0        1            1\n";
  myfile << "rate            " 
		<<  h_obs_pass->Integral(1,h_obs_pass->GetNbinsX()) << " "
		<<  h_obs_fail->Integral(1,h_obs_fail->GetNbinsX()) << " "
	    <<  h_catp1_pass->Integral(1,h_catp1_pass->GetNbinsX()) << " "
	    <<  h_catp1_fail->Integral(1,h_catp1_fail->GetNbinsX()) << "\n";
  myfile << "------------\n";
  myfile << "lumi    lnN     -      -     1.023  1.023\n";
  myfile << "jecs    lnN     -      -     1.02   1.02 \n";
  myfile << "pu      lnN     -      -     1.05   1.05 \n";

  myfile << "*  autoMCStats  0\n";
  
}
