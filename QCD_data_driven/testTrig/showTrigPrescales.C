/*
	this code is used to print out the list of triggers and prescales
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "TH1D.h"
#include "TFile.h"
//#include "untuplizer.h" // for some unknown reason, can not load input files
//#include <TDirectory.h>
#include "TTree.h"
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TString.h"
#include <map>


bool isTest = false;

using namespace std;

void showTrigPrescales(string inputFile = "testTrigOutputList.txt"){
	// data collection STLs
	map<int , set<int> >  PFHT_names;
	
	string line;
	ifstream flist(inputFile.data() );
	ofstream fout("prescaleList.txt");
	int nfile = -1;
	while (getline(flist, line) ){
		PFHT_names.clear();
		cout << "reading " << line << endl;
		nfile++;
		//get TTree from file ...
		TFile* myFile;
		myFile = TFile::Open(line.data(), "READONLY" );
		//TDirectory* td = (TDirectory*) myFile->Get("tree");
		TTreeReader data("tree",myFile);
		TTreeReaderValue< int > nPassTrig(data, "nPassTrig");
		TTreeReaderArray< int > Trig(data, "passTrigList");
		TTreeReaderArray< int > prescale(data, "prescaleList");
	
		Long64_t nPass[20]={0};
		int total_entry = data.GetEntries(true);
		int jEntry = 0;

		while (data.Next() ){			
			if(isTest) if (jEntry > 100) break;
			if (jEntry % 1000 == 0) cout << "running " << jEntry << " of " << total_entry << endl;
			for( int nT=0; nT < (int)*nPassTrig; nT++){
				int iTrigname = Trig[nT];
				int iprescale = prescale[nT];
				PFHT_names[iTrigname].insert(iprescale);
			}
			jEntry++;
		}// end of entries
		fout << "reading " << line << endl;
		for (auto x : PFHT_names){
			fout << x.first << " ";
			for (auto y : x.second) fout << y << " ";
			fout << endl;
		}
		PFHT_names.clear();
	}// end of loop files
	// output files
	fout.close();
}
