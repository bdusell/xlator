#include "Translator.h"
using namespace std;

Translator::Translator() :
	parser(), interpreter() {
}

Translator::Translator(istream &grammarRules, istream &translationRules) :
	parser(grammarRules), interpreter(translationRules) {
}

set< vector<string> > Translator::translate(const vector<string> &input) const {

	// Stores the set of resulting translations, in the form of tokenized
	// sequences of words.
	set< vector<string> > results;

	// Find all distinct parsing solutions to the input sentence.
	// For each parse tree, produce all distinct translations of it.
	// Compile all translations, not repeating identical ones.
	set<ParseTree> parsingSolutions = parser.parse(input);
	set<ParseTree> interpretationSolutions;
	for(set<ParseTree>::const_iterator i = parsingSolutions.begin();
		i != parsingSolutions.end(); ++i) {
		interpretationSolutions = interpreter.interpret(*i);
		for(set<ParseTree>::const_iterator j = interpretationSolutions.begin();
			j != interpretationSolutions.end(); ++j) {
			results.insert(j->asWords());
		}
	}
	
	return results;

}

set< vector<string> > Translator::translateVerbose(const vector<string> &input, ostream &output) const {
	set< vector<string> > results;
	//set<ParseTree> parsingSolutions = parser.parseVerbose(input, output);
	set<ParseTree> parsingSolutions = parser.parse(input);
	set<ParseTree> interpretationSolutions;
	for(set<ParseTree>::const_iterator i = parsingSolutions.begin();
		i != parsingSolutions.end(); ++i) {
		interpretationSolutions = interpreter.interpretVerbose(*i, output);
		for(set<ParseTree>::const_iterator j = interpretationSolutions.begin();
			j != interpretationSolutions.end(); ++j) {
			results.insert(j->asWords());
		}
	}
	return results;
}

void Translator::loadRules(istream &grammar, istream &translation) {
	loadGrammarRules(grammar);
	loadTranslationRules(translation);
}

void Translator::loadGrammarRules(istream &input) {
	parser.loadRules(input);
}

void Translator::loadTranslationRules(istream &input) {
	interpreter.loadRules(input);
}

void Translator::printRules(ostream &output) const {
	output << "Translator contains:" << endl
		<< endl;
	printGrammar(output);
	output << endl;
	printTranslation(output);
	output << endl;
}

void Translator::printGrammar(ostream &output) const {
	output << "Grammar:" << endl;
	parser.printRules(output);
}

void Translator::printTranslation(ostream &output) const {
	output << "Translation:" << endl;
	interpreter.printRules(output);
}
