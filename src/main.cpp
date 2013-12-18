#include "Translator.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char **argv) {

	bool verbose = false;
	string grammarFileName, translationFileName;

	for(int i = 1; i < argc - 2; ++i) {
		if(string(argv[i]) == "-v" || "--verbose") {
			if(verbose) cerr << "Warning: Setting " << argv[i] << " is redundant" << endl;
			verbose = true;
		}
		else cerr << "Warning: Unknown option \'" << argv[i] << "\'" << endl;
	}

	if(argc > 2) {
		grammarFileName = argv[argc - 2];
		translationFileName = argv[argc - 1];
	}
	else {
		cerr << "Usage: " << argv[0] << " [-v] <grammar file> <translation file>" << endl;
		exit(0);
	}

	Translator myTranslator;

	ifstream grammarFile(grammarFileName.c_str()), translationFile(translationFileName.c_str());

	if(!grammarFile) {
		cerr << "Error: Unable to open file \'" << grammarFileName << "\'" << endl;
		exit(0);
	}

	if(!translationFile) {
		cerr << "Error: Unable to open file \'" << translationFileName << "\'" << endl;
		exit(0);
	}

	try {
		myTranslator.loadRules(grammarFile, translationFile);
	}
	catch(GrammarInputError &e) {
		cerr << "Error in " << grammarFileName << ": " << e.what() << endl;
		myTranslator.printGrammar(cerr);
		exit(0);
	}
	catch(TranslationInputError &e) {
		cerr << "Error in " << translationFileName << ": " << e.what() << endl;
		myTranslator.printTranslation(cerr);
		exit(0);
	}

	if(verbose) {
		myTranslator.printRules(cout);
		cout << endl;
	}

	grammarFile.close();
	translationFile.close();

	while(cin) {

		if(verbose) cout << "Type a sentence to translate (enter nothing to quit):" << endl;

		string tempString;
		getline(cin, tempString);
		istringstream tempStream(tempString);
		vector<string> tokens;
		while(tempStream >> tempString) tokens.push_back(tempString);
		if(tokens.empty()) break;

		set< vector<string> > results;
		if(verbose) results = myTranslator.translateVerbose(tokens, cout);
		else results = myTranslator.translate(tokens);
		int tnum = 1;
		for(set< vector<string> >::const_iterator i = results.begin(); i != results.end(); ++i) {
			if(verbose) cout << "Translation #" << tnum++ << ": ";
			for(int j = 0; j < i->size(); ++j) {
				cout << (*i)[j];
				if(j < i->size() - 1) cout << ' ';
			}
			cout << endl;
		}
		cout << endl;
	}

	if(verbose) cout << "Goodbye!" << endl;

	return 0;
}

