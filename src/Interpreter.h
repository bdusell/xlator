#ifndef INTERPRETER_H
#define	INTERPRETER_H

#include "ParseTree.h"
#include "TranslationTree.h"
#include <iostream>
#include <string>
#include <set>
#include <stdexcept>
using namespace std;

// Interpreter class which recursively performs parse tree transformations
// according to specified translation rules.
class Interpreter {

public:
	// Default constructor.
	Interpreter();

	// Constructor which initializes the interpreter with translation rules
	// from an input stream.
	Interpreter(istream &input);

	// Loads translation rules into the interpreter as read in from an input
	// stream.
	void loadRules(istream &input);

	// Takes a parse tree and returns its transformation according to the
	// loaded translation rules.
	set<ParseTree> interpret(const ParseTree &input) const;

	// Prints extra information during the transformation process.
	set<ParseTree> interpretVerbose(const ParseTree &input, ostream &output) const;

	// Prints the loaded translation rules to an ouput stream.
	void printRules(ostream &output) const;
	
private:

	// Translation rules.
	vector< pair<ParseTree, TranslationTree> > translations;

	// Processes a single rule from an input stream.
	void readRule(istream &input, char &sentinel);

	// Extracts a token from an input stream.
	static string nextToken(istream &input, char &sentinel);

	// Tells if a token is a variable encoding
	static bool isVariableToken(const string &token);

	// Builds a parse tree from its encoding in an input stream.
	static ParseTree readParseTree(istream &input, string &token, char &sentinel);

	// Builds a translation tree from its encoding in an input stream.
	static TranslationTree readTranslationTree(istream &input, string &token, char &sentinel);

	// Tags all translation rules such that no accidental overlap between
	// the native and new languages can occur during the translation 
	// process. This mitigates cases of infinite recursion.
	void tagRules();

	// Predicate to determine recursively whether a translation rule can specify a transformation of a start variable.
	static bool hasCousin(const GrammarSymbol &s, const TranslationTree &t);

	// Find the identifier which should replace the start variable to initialize the recursive translation process.
	static GrammarSymbol getCousinIdentifier(const GrammarSymbol &s, const TranslationTree &t);
	static void getCousinIdentifierRecurse(vector<GrammarSymbol> &results, const GrammarSymbol &s, const TranslationTree &t);
	static GrammarSymbol getCousinIdentifierVerbose(const GrammarSymbol &s, const TranslationTree &t, ostream &output);

	// Recursive method for finding translation solutions to an input ParseTree.
	vector<ParseTree> interpretRecurse(const ParseTree &val, const TranslationTree &rule) const;
	vector<ParseTree> interpretRecurseVerbose(const ParseTree &val, const TranslationTree &rule, ostream &output) const;

	// Predicate function to tell if a translation rule applies to a ParseTree
	static bool hasMatch(const ParseTree &val, const ParseTree &match);

	// Construct recursively the ParseTree resulting from a transformation rule
	static ParseTree makeChange(const ParseTree &pt, const ParseTree &match, const TranslationTree &tt);
	static void makeChangeRecurse(const ParseTree &original, const ParseTree &match, const TranslationTree &cousins, ParseTree &replacement);

	// Find recursively the children which should be adopted by a transformed ParseTree
	static vector<ParseTree> findChildren(const ParseTree &original, const ParseTree &match, const GrammarSymbol &cousin);
	static void findChildrenRecurse(vector< vector<ParseTree> > &results, const ParseTree &original, const ParseTree &match, const GrammarSymbol &cousin);
	static vector<ParseTree> findChildrenVerbose(const ParseTree &original, const ParseTree &match, const GrammarSymbol &cousin, ostream &output);

	/// Generate all combinations of solutions of a ParseTree's children. 
	static vector< vector<ParseTree> > combinateSolutions(const vector< vector<ParseTree> > &uncombinated);
	static void combinateRecurse(vector< vector<ParseTree> > &combinations, vector<ParseTree> &buffer, const vector< vector<ParseTree> > &uncombinated, int n);

};

class TranslationInputError : public runtime_error {
public:
	TranslationInputError(const string &msg);
};

#endif

