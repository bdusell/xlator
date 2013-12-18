#ifndef PARSER_H
#define	PARSER_H

#include "GrammarSymbol.h"
#include "LightParseTree.h"
#include "ParseTree.h"
#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <stdexcept>
using namespace std;

// Comparator class that compares

// Parser class which produces parse trees from pre-tokenized sequences of
// words according to specified grammar rules.
class Parser {

public:

	// Default constructor
	Parser();

	// Constructor which initializes the parser with grammar rules as read
	// in from an input stream.
	Parser(istream &input);

	// Loads grammar rules into the parser as read in from an input stream.
	void loadRules(istream &input);

	// Adds a rule to the grammar set.
	void addRule(const string &left, const vector<GrammarSymbol> &right);

	// Parses a pre-tokenized sequence of words into its parse tree
	// solutions according to the loaded grammar rules.
	set<ParseTree> parse(const vector<string> &input) const;

	// Parses input and prints stepwise information to an output stream.
	set<ParseTree> parseVerbose(const vector<string> &input, ostream &output) const;

	// Prints the grammar rules loaded into the parser to an output stream.
	void printRules(ostream &output) const;

	// Checks to see if grammar rules are cyclic in order to mitigate
	// infinite recursion during the parsing algorithm.
	bool rulesAreAcyclic() const;

private:

	// Grammar rules, optimized for lookup according to symbol sequence.
	map< vector<GrammarSymbol>, set<string> > grammar;

	struct ParseTreePtrCompare {
		bool operator()(const LightParseTree *left, const LightParseTree *right) const;
	};

	void readRule(istream &input, char &sentinel);
	static string nextToken(istream &input, char &last);
	static bool isVariableToken(const string &token);

	static const LightParseTree *getBranchPointer(const LightParseTree &branch,
		set<const LightParseTree *, ParseTreePtrCompare> &branches);
	void parseRecurse(const vector<const LightParseTree *> &forest,
		set<const LightParseTree *, ParseTreePtrCompare> &branches,
		set< vector<const LightParseTree *> > &visited,
		set<const LightParseTree *> &solutions) const;
	void parseRecurseVerbose(const vector<const LightParseTree *> &forest,
		set<const LightParseTree *, ParseTreePtrCompare> &branches,
		set< vector<const LightParseTree *> > &visited,
		set<const LightParseTree *> &solutions,
		ostream &output) const;
	vector<string> getMatches(const vector<const LightParseTree *> &forest, int beg, int end) const;
	static vector<const LightParseTree *> makeReplacement(const vector<const LightParseTree *> &forest,
		int beg, int end, const string &parent,
		set<const LightParseTree *, ParseTreePtrCompare> &branches);

};

// Exception class for signaling grammar input errors
class GrammarInputError : public runtime_error {
public:
	GrammarInputError(const string &msg);
};

#endif

