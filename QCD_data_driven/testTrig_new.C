#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <TH1D.h>
#include <TFile.h>
//#include "untuplizer.h" // for some unknown reason, can not load input files
//#include <TDirectory.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>
#include <TClonesArray.h>
#include <TLorentzVector.h>
#include <map>


using namespace std;
void testTrig_new(string inputFile, string outfile){
  string afileName;
  ifstream infile(inputFile.data() );
  map<string, TH1F*> PFHT_names;
  while(getline(infile,afileName)){
	  //get TTree from file ...
	  TFile* myFile;
	  myFile->Open(afileName.data() );
	  //TDirectory* td = (TDirectory*) myFile->Get("tree");
	  TTreeReader data("tree/treeMaker",myFile);
	  TTreeReaderValue< Int_t > nVtx(data, "nVtx");
	  TTreeReaderArray< string > trigName(data, "hlt_trigName");
	  TTreeReaderArray< bool > trigResult(data, "hlt_trigResult");
	  TTreeReaderArray< int > prescale(data, "hlt_trigPrescale");

	  Long64_t nPass[20]={0};
	  int total_entry = data.GetEntries(true);
	  int jEntry = 0;
	
	  while (data.Next() ){
		jEntry++;
		//    if (jEntry % 50000 == 0)
		fprintf(stderr, "Processing event %i of %i\n", jEntry, total_entry);
		if(jEntry>10)break;
		//0. has a good vertex
		if(*nVtx<1)continue;
		nPass[0]++;

		//1. trigger 
		bool passTrigger=false;
		int it = -1;
		for(auto iter=trigResult.begin(); iter< trigResult.end(); iter++)
		{
			it++;
			std::string thisTrig= trigName[it];
			bool results = trigResult[it];
			if(thisTrig.find("HLT_PFHT")== 
			   std::string::npos)continue;
			// if (results == 1){
				// cout << thisTrig << "\t" << prescale[it] << "\t" << results << endl;
			// }
			if (PFHT_names[thisTrig] == nullptr){
				TH1F* tmph = new TH1F(thisTrig.data(),thisTrig.data() ,110,0,1100);
				PFHT_names[thisTrig] = tmph;
			}
			PFHT_names[thisTrig]->Fill(prescale[it]);
		} // end of triggers	

	  }// end of entries
  }
  // output files
  TFile* fout = new TFile(outfile.data(), "RECREATE" );
  for (auto x : PFHT_names)  x.second->Write();
  fout->Close();
	  
}
