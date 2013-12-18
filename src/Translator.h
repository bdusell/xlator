#ifndef TRANSLATOR_H
#define	TRANSLATOR_H

#include "Parser.h"
#include "Interpreter.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
using namespace std;

// A translator class which translates natural language according to grammar
// and translation rules as encoded in a special input language. Natural
// languages are modeled as context-free languages in the scope of this class's
// design, and thus the translation process is fully recursive.
class Translator {

public:

	// Default constructor.
	Translator();

	// Initializes the translator with rules for the parser and
	// translator as read in from two input streams.
	Translator(istream &grammarRules, istream &translationRules);

	// Takes a pre-tokenized sequence of words and produces translations of
	// it according to the grammar and translation rules loaded into the
	// translator. Returns a set of tokenized translations in the target
	// language.
	set< vector<string> > translate(const vector<string> &input) const;

	// Prints additional output for each step of the translation.
	set< vector<string> > translateVerbose(const vector<string> &input, ostream &output) const;

	// Loads the grammar and translation rules from two input streams.
	void loadRules(istream &grammar, istream &translation);

	// Loads the grammar rules used for parsing sentences.
	void loadGrammarRules(istream &input);

	// Loads the translation rules used for mutating the parsed sentences.
	void loadTranslationRules(istream &input);

	// Prints the rules currently stored by the translator to an output 
	// stream.
	void printRules(ostream &output) const;
	
	// Prints the grammar rules stored in the translator's parser.
	void printGrammar(ostream &output) const;
	
	// Prints the translation rules stored in the translator's interpreter.
	void printTranslation(ostream &output) const;

private:

	// Parsing mechanism for the Translator class which feeds its output
	// into the interpreter.
	Parser parser;

	// Translation mechanism which relies on the parser's output for
	// interpretation of the initial input into the target language.
	Interpreter interpreter;

};

#endif

