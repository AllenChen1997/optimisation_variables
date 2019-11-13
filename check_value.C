///////////////////////////////
/*
	Date 2019/11/13
	owner Kung-Hsiang Chen
	
	check below -5 valus of rho entries 
*/
///////////////////////////////

#include <iostream>
#include <sstream>
#include <TTree.h>
#include <TFile.h>
#include <TROOT.h>
#include <TTreeReader.h>
#include <map>

using namespace std;
void check_value(){
	
	ifstream infile("list_2.txt");
	string line,s1,s2;
	stringstream ss;
	map<string,int> result; 
	while(getline(infile,line)){
		ss << line;
		ss >> s1 >> s2;
		cout << s1 << endl;
		ss.clear();
		TFile* myfile = new TFile(s1.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		int N = myRead.GetEntries();
		if ( N == 0 ) continue;
		cout << "entries = " << N << endl;
		int count=0;
		while (myRead.Next()){
			if (*rho < -5) count++;
		}
		result[s2]= count;
	}
	cout << "the amount of rho < -5" << endl;
	for ( auto x : result){
		cout << x.first << " " << x.second << endl;
	}
}

