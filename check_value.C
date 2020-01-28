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
	string line;
	map<string,int> result,result2; 
	while(getline(infile,line)){
		cout << line << endl;
		TFile* myfile = new TFile(line.c_str(),"READ");
		TTreeReader myRead("monoHbb_SR_boosted",myfile);
		TTreeReaderValue< Double_t > rho(myRead,"FJetrho");
		int N = myRead.GetEntries();
		if ( N == 0 ) continue;
		cout << "entries = " << N << endl;
		int count=0;
		int count2 = 0;
		while (myRead.Next()){
			if (*rho < -6) count++;
			if (*rho > -1) count2++;
		}
		result[line]= count;
		result2[line]= count2;
	}
	cout << "the amount of rho < -6" << endl;
	for ( auto x : result){
		cout << x.first << " " << x.second << endl;
	}
	cout << "the amount of rho > -1" << endl;
		for ( auto x : result2){
		cout << x.first << " " << x.second << endl;
	}
}

