

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "ozutility.h"



using namespace std;


int main(int argc, char * argv[]){


//	vector<int> c;
//	c.push_back(100);
//	cout << c.back()<< endl;

//	cout << "State: "<<S19 << endl;
//	cout << "Enum:" << OPREL << endl;
//	string infile = "test.cpp";
//	string infile = "test2.cpp";
	string infile = "test3.cpp";
	if (argc == 2) {
		infile = argv[1];
	}
	//cout <<argc << ":" << infile << endl;
//	return 0;
//	cout << infile.substr(0,1)<< endl;
//	cout << infile.length() << endl;
	scanner ozscanner;
//	ozscanner.readFile(infile);
	ozscanner.openFile(infile);
	ozscanner.parseStart();
	ozscanner.closeFile();

	//ozscanner.listToken();


	return 0;
} 


