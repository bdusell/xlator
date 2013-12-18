#include "Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

int main(int argc, char **argv) {

	if(argc <= 1) {
		cout << "Usage: parser_test <grammar file>" << endl;
		exit(0);
	}

	bool verbose = false;
	string filename;

	string arg;
	for(int i = 1; i < argc; ++i) {
		arg = argv[i];
		if((arg == "-v" || arg == "--verbose") && !verbose) {
			verbose = true;
		}
		else if(filename.empty()) {
			filename = arg;
		}
		else {
			cout << "Error: Unrecognized option \'" << arg << "\'" << endl;
			exit(0);
		}
	}

	ifstream input(filename.c_str());
	if(!input) {
		cout << "Error: Cannot open file \'" << filename << "\'" << endl;
		exit(0);
	}

	if(verbose) {
		system(string("cat " + filename).c_str());
		cout << endl << "-----" << endl;
	}

	Parser testParser;
	testParser.loadRules(input);

	input.close();

	if(verbose) {
		testParser.printRules(cout);
		cout << "-----" << endl;
	}

	string tempString;
	getline(cin, tempString);
	istringstream tempStream(tempString);
	vector<string> tokens;
	while(tempStream >> tempString) tokens.push_back(tempString);

	set<ParseTree> results = verbose ? testParser.parseVerbose(tokens, cout) : testParser.parse(tokens);

	if(verbose) {
		cout << "results = <" << endl;
		for(set<ParseTree>::const_iterator i = results.begin(); i != results.end(); ++i) {
			cout << "    " << (*i) << endl;
		}
		cout << ">" << endl;
	}
	else {
		for(set<ParseTree>::const_iterator i = results.begin(); i != results.end(); ++i) {
			cout << (*i) << endl;
		}
	}
	
	return 0;
}
