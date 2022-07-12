using namespace std;
void quickGetCutFlow(string input,string output){
  TFile* fin = new TFile(input.data(),"READONLY");
  TH1F* h = (TH1F*) fin->Get("h_cutFlow_withSR");
  string label[10] = {"init","preselection","metTrigger","METpT","tauVeto","photonVeto","EleVeto","LooseMuVeto","extraAk4","nFatJ"};
  for (int i=0; i< (int)(sizeof(label)/sizeof(label[0]) ); i++){
    h->GetXaxis()->SetBinLabel(i+1,label[i].data() );
  }
  h->GetXaxis()->SetRangeUser(1,10);
  TCanvas* c = new TCanvas("c","c");
  h->Draw("HIST text0");
  c->SaveAs(output.data() );

}
