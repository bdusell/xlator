#include "Parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <set>
using namespace std;

Parser::Parser() :
	grammar() {
}

Parser::Parser(istream &input) {
	loadRules(input);
}

void Parser::loadRules(istream &input) {

	// Required so that nextToken() can remember the last byte it read
	char sentinel;
	input.read(&sentinel, 1);

	while(input) readRule(input, sentinel);
}

void Parser::readRule(istream &input, char &sentinel) {

	string token;

	// Read first token
	token = nextToken(input, sentinel);

	// Allow blank lines
	if(token == "\n") return;

	// Process left side
	if(!isVariableToken(token)) throw GrammarInputError("expecting variable on left side of rule (read \'" + token + "\'");
	string leftSide = token.substr(1, token.size() - 2);

	// Read arrow
	if(nextToken(input, sentinel) != "->") throw GrammarInputError("expecting \'->\' after left side of rule \'" + token + "\'");

	// Read right hand side
	while(input) {
		vector<GrammarSymbol> rightSide;
		while(input) {
			token = nextToken(input, sentinel);
			if(token == "\n" || token == "" || token == "|") break;
			if(isVariableToken(token)) {
				rightSide.push_back(GrammarSymbol(token.substr(1, token.size() - 2), false));
			}
			else rightSide.push_back(GrammarSymbol(token, true));
		}

		// Add the rule to the grammar
		addRule(leftSide, rightSide);

		if(token != "|") break;
	}
}

// Gets the next significant token from the input stream. This can either be a
// variable name enclosed in chevrons, a word with no whitespace, or a newline
// character. Returns empty string only when EOF is detected. Assumes that one
// byte has already been read from the stream, and reads an extra byte after
// the input returned.
string Parser::nextToken(istream &input, char &sentinel) {

	while(input && isspace(sentinel) && sentinel != '\n') {
		input.read(&sentinel, 1);
	}

	if(!input) return "";
	
	string token;
	
	if(sentinel == '<') {
		string variable;
		input.read(&sentinel, 1);
		while(input && sentinel != '>' && sentinel != '\n') {
			variable += sentinel;
			input.read(&sentinel, 1);
		}
		if(sentinel == '\n') {
			throw GrammarInputError("line break in the middle of variable token \'" + variable + "\'");
		}
		if(!input) throw GrammarInputError("EOF in the middle of variable token \'" + variable + "\'");
		input.read(&sentinel, 1);
		return '<' + variable + '>';
	}
	else if(sentinel == '-') {
		token += sentinel;
		input.read(&sentinel, 1);
		if(sentinel == '>') {
			input.read(&sentinel, 1);
			return "->";
		}
	}
	else if(sentinel == '\n') {
		input.read(&sentinel, 1);
		return "\n";
	}

	token += sentinel;
	while(input) {
		input.read(&sentinel, 1);
		if(isspace(sentinel) || sentinel == '<' || !input) break;
		token += sentinel;
	}
	return token;

}

bool Parser::isVariableToken(const string &token) {
	return token.size() >= 2 && token[0] == '<' && token[token.size() - 1] == '>';
}

void Parser::addRule(const string &left, const vector<GrammarSymbol> &right) {
	// Currently issues no error if the rule is repeated
	grammar[right].insert(left);
	if(!rulesAreAcyclic()) throw GrammarInputError("cyclic grammar rules detected");
}

set<ParseTree> Parser::parse(const vector<string> &input) const {

	// Throughout the parsing process a collection of dynamically allocated
	// parse tree branches is maintained such that no two pointers in the
	// container point to parse trees with the same value. The correctness
	// of the algorithm depends on the condition that there is a one-tp-one
	// correspondence between pointers in this set and possible parse tree
	// values.
	set<const LightParseTree *, ParseTreePtrCompare> branches;

	// The parsing algorithm uses dynamic programming to skip parse tree
	// forests it has already explored. Since the parse trees are stored at
	// unique memory addresses according to their values, there is a one-to-
	// one correspondence between the pointers in the parse tree vectors and
	// the values they represent. Therefore, the pointer sequences may be
	// compared by value; no dereferencing is necessary.
	set< vector<const LightParseTree *> > visited;

	// Keep a collection of all solutions where the entire sequence is
	// unified under one tree.
	set<const LightParseTree *> solutions;
	
	// Initialize the first parse tree forest with the input words.
	vector<const LightParseTree *> forest;
	for(int i = 0; i < input.size(); ++i) {
		forest.push_back(getBranchPointer(LightParseTree(GrammarSymbol(input[i], true)), branches));
	}

	// Initiate recursive parsing algorithm.
	parseRecurse(forest, branches, visited, solutions);

	// Produce deep copies of the solutions found.
	set<ParseTree> results;
	for(set<const LightParseTree *>::const_iterator i = solutions.begin(); i != solutions.end(); ++i) {
		results.insert(ParseTree(*(*i)));
	}

	// Deallocate parse trees.
	for(set<const LightParseTree *>::const_iterator i = branches.begin(); i != branches.end(); ++i) {
		delete *i;
	}

	return results;

}

set<ParseTree> Parser::parseVerbose(const vector<string> &input, ostream &output) const {

	set<const LightParseTree *, ParseTreePtrCompare> branches;
	set< vector<const LightParseTree *> > visited;
	set<const LightParseTree *> solutions;
	
	vector<const LightParseTree *> forest;
	for(int i = 0; i < input.size(); ++i) {
		forest.push_back(getBranchPointer(LightParseTree(GrammarSymbol(input[i], true)), branches));
	}

	parseRecurseVerbose(forest, branches, visited, solutions, output);

	set<ParseTree> results;
	for(set<const LightParseTree *>::const_iterator i = solutions.begin(); i != solutions.end(); ++i) {
		results.insert(ParseTree(*(*i)));
	}

	for(set<const LightParseTree *>::const_iterator i = branches.begin(); i != branches.end(); ++i) {
		delete *i;
	}

	return results;

}

void Parser::parseRecurse(const vector<const LightParseTree *> &forest,
	set<const LightParseTree *, ParseTreePtrCompare> &branches,
	set< vector<const LightParseTree *> > &visited,
	set<const LightParseTree *> &solutions) const {

	vector<string> matches;
	vector<const LightParseTree *> replacement;
	for(int i = 0; i < forest.size(); ++i) {
		for(int j = i + 1; j <= forest.size(); ++j) {
			matches = getMatches(forest, i, j);
			for(int k = 0; k < matches.size(); ++k) {
				replacement = makeReplacement(forest, i, j, matches[k], branches);
				if(!visited.count(replacement)) {
					visited.insert(replacement);
					parseRecurse(replacement, branches, visited, solutions);
					if(replacement.size() == 1) {
						solutions.insert(replacement[0]);
					}
				}
			}
		}
	}
	
}

void Parser::parseRecurseVerbose(const vector<const LightParseTree *> &forest,
	set<const LightParseTree *, ParseTreePtrCompare> &branches,
	set< vector<const LightParseTree *> > &visited,
	set<const LightParseTree *> &solutions,
	ostream &output) const {

	output << "Parsing < ";
	for(int i = 0; i < forest.size(); ++i) {
		output << *forest[i];
		if(i < forest.size() - 1) output << ", ";
	}
	output << " >" << endl;

	output << "branch set = {" << endl;
	for(set<const LightParseTree *, ParseTreePtrCompare>::const_iterator i = branches.begin();
		i != branches.end(); ++i) {
		output << "    " << *(*i) << endl;
	}
	output << "}" << endl;
	output << "visit set = {" << endl;
	for(set< vector<const LightParseTree *> >::const_iterator i = visited.begin();
		i != visited.end(); ++i) {
		output << "    ";
		for(int j = 0; j < i->size(); ++j) {
			output << *((*i)[j]);
		}
		output << endl;
	}
	output << "}" << endl;
	output << "solution set = {" << endl;
	for(set<const LightParseTree *>::const_iterator i = solutions.begin();
		i != solutions.end(); ++i) {
		output << "    " << *(*i) << endl;
	}
	output << "}" << endl;

	vector<string> matches;
	vector<const LightParseTree *> replacement;
	for(int i = 0; i < forest.size(); ++i) {
		for(int j = i + 1; j <= forest.size(); ++j) {
			output << i << ":" << j << endl;
			matches = getMatches(forest, i, j);
			for(int k = 0; k < matches.size(); ++k) {
				replacement = makeReplacement(forest, i, j, matches[k], branches);
				if(!visited.count(replacement)) {
					visited.insert(replacement);
					parseRecurseVerbose(replacement, branches, visited, solutions, output);
					if(replacement.size() == 1) {
						solutions.insert(replacement[0]);
					}
				}
			}
		}
	}
}

const LightParseTree *Parser::getBranchPointer(const LightParseTree &branch,
	set<const LightParseTree *, ParseTreePtrCompare> &branches) {
	const LightParseTree *ptr = new LightParseTree(branch);
	set<const LightParseTree *, ParseTreePtrCompare>::iterator loc = branches.find(ptr);
	if(loc == branches.end()) {
		branches.insert(ptr);
		return ptr;
	}
	delete ptr;
	return *loc;
}

vector<string> Parser::getMatches(const vector<const LightParseTree *> &forest, int beg, int end) const {
	vector<GrammarSymbol> key;
	for(int i = beg; i < end; ++i) {
		key.push_back(forest[i]->getSymbol());
	}
	map< vector<GrammarSymbol>, set<string> >::const_iterator loc = grammar.find(key);
	if(loc == grammar.end()) return vector<string>();
	return vector<string>(loc->second.begin(), loc->second.end());
}

vector<const LightParseTree *> Parser::makeReplacement(const vector<const LightParseTree *> &forest,
	int beg, int end, const string &parent,
	set<const LightParseTree *, ParseTreePtrCompare> &branches) {

	vector<const LightParseTree *> result(forest.begin(), forest.begin() + beg);
	result.push_back(
		getBranchPointer(
			LightParseTree(
				GrammarSymbol(parent, false),
				vector<const LightParseTree *>(forest.begin() + beg, forest.begin() + end)),
			branches)
		);

	for(int i = end; i < forest.size(); ++i) {
		result.push_back(forest[i]);
	}

	return result;
}

void Parser::printRules(ostream &output) const {
	for(map< vector<GrammarSymbol>, set<string> >::const_iterator i = grammar.begin();
		i != grammar.end(); ++i) {
		output << "< ";
		for(int j = 0; j < i->first.size(); ++j) {
			output << i->first[j] << ' ';
		}
		output << "> => { ";
		for(set<string>::const_iterator j = i->second.begin(); j != i->second.end(); ++j) {
			output << '<' << *j << "> ";
		}
		output << '}' << endl;
	}
}

bool Parser::rulesAreAcyclic() const {
	
	Graph<string, GrammarSymbol, bool> test;

	map< vector<GrammarSymbol>, set<string> >::const_iterator i;
	set<string>::const_iterator j;

	for(i = grammar.begin(); i != grammar.end(); ++i) {
		if(i->first.size() == 1) { // only check unary rules
			string rightKey = i->first.front().getName();
			test.setVertex(rightKey, i->first.front());
			for(j = i->second.begin(); j != i->second.end(); ++j) {
				string leftKey = *j;
				test.setVertex(leftKey, GrammarSymbol(leftKey, true));
				test.setEdge(leftKey, rightKey, true);
			}
		}
	}

	return !test.cyclic();

}

bool Parser::ParseTreePtrCompare::operator()(const LightParseTree *left, const LightParseTree *right) const {
	return *left < *right;
}

GrammarInputError::GrammarInputError(const string &msg) :
	runtime_error(msg) {
}

