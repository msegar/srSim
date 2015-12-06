/*
 * main.cpp
 */
#define MAIN_CPP_

#include "defs.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[]) {

	// get command line arguments
	int cl = commandLine(argc, argv);

	if (cl != 0)
		cout << "Command line exited with: " << cl << endl;

	// initialize machine qv dataset
	initialize(argv);

	int sim = startSimulation();

	if (sim != 0)
		cout << "Exit failed with " << sim << endl;

	return 0;
}

int commandLine(int argc, char *argv[]){
	if (argc < 2) {
		cout << "Usage: [filename] [config file] [options]\n";
		cout << "-OconfOption=Value" << endl;
		exit(1);
	}
	
	confDB.setConfigFile(argv[1]);
	confDB.initializeDB(confDB.configFileName.c_str());

	if (argc == 3){
		string flag = argv[2];
		flag.erase(0,2);

		string::size_type pos = flag.find_first_of('=');
		if (pos == string::npos || pos + 1 == flag.length()){
			// do nothing
		}
		else {
			string name = flag.substr(0, pos);
			string val = flag.substr(pos+1);

			confDB.setKey(name, val);

			cout << "Added: " << name << ", " << val << endl;
		}
	}

	return 0;
}

void initialize(char *argv[]){
	ifstream input (confDB.getKey("qvdata").stringVal.c_str());
	string line;

	while (!input.eof()){
		for (int i = 0; i < 1000; ++i){
			for (int j = 0; j < 500; ++j){
				input >> dataset[i][j];
			}
		}
	}
	input.close();
}
