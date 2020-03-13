/*

captp1 = N2DDT(26%)

*/


using namespace std;

void makemyDatacard(string data="data.root", string MC="MC.root") {

  TFile *fdata = TFile::Open((TString)data,"READONLY");
  TFile *fMC = TFile::Open((TString)MC,"READONLY");
  TH1D  *h_obs_pass = (TH1D*)fdata->Get("h_pass");
  TH1D  *h_obs_fail = (TH1D*)fdata->Get("h_fail");
  TH1D  *h_catp1_pass = (TH1D*)fMC->Get("h_pass");
  TH1D  *h_catp1_fail = (TH1D*)fMC->Get("h_fail");
  ofstream myfile("SF.txt");
  
  
  myfile << "imax 2  number of channels\n";
  myfile << "jmax 1  number of backgrounds\n";
  myfile << "kmax *  number of nuisance parameters (sources of systematical uncertainties)\n";
  myfile << "------------\n";
  myfile << "shapes  *  pass   " << data << "  $PROCESS $PROCESS_$SYSTEMATIC\n";
  myfile << "shapes  *  fail   " << MC << "  $PROCESS $PROCESS_$SYSTEMATIC\n";
  myfile << "------------\n";
  myfile << "# we have just one channel, in which we observe 0 events\n";
  myfile << "bin             pass    fail\n";
  myfile << "observation      " << h_obs_pass->Integral(1,h_obs_pass->GetNbinsX()) << "  " <<  h_obs_fail->Integral(1,h_obs_fail->GetNbinsX())  << "\n";
  myfile << "------------\n";
  myfile << "# now we list the expected events for signal and all backgrounds in that bin\n";
  myfile << "# the second 'process' line must have a positive number for backgrounds, and 0 for signal\n";
  myfile << "# then we list the independent sources of uncertainties, and give their effect (syst. error)\n";
  myfile << "# on each process and bin\n";
  myfile << "bin               pass         fail\n";
  myfile << "process           catp1        catp1\n";  
  myfile << "process            1            1\n";
  myfile << "rate            " 
	    <<  h_catp1_pass->Integral(1,h_catp1_pass->GetNbinsX()) << " "
	    <<  h_catp1_fail->Integral(1,h_catp1_fail->GetNbinsX()) << "\n";
  myfile << "------------\n";
  myfile << "lumi    lnN    1.023  1.023  1.023  1.023\n";
  myfile << "jecs    lnN    1.02   1.02   1.02   1.02 \n";
  myfile << "pu      lnN    1.05   1.05   1.05   1.05 \n";

  myfile << "*  autoMCStats  0\n";
  
}
