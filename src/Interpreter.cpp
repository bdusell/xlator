#include "Interpreter.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <istream>
#include <algorithm>

Interpreter::Interpreter() :
	translations() {
}

Interpreter::Interpreter(istream &input) {
	loadRules(input);
}

void Interpreter::loadRules(istream &input) {

	// Forwards the last byte read
	char sentinel;
	input.read(&sentinel, 1);

	while(input) readRule(input, sentinel);

	tagRules();

}

void Interpreter::readRule(istream &input, char &sentinel) {

	string token = nextToken(input, sentinel);

	if(token == "\n") return;

	ParseTree leftSide = readParseTree(input, token, sentinel);

	if(token != "->") {
		throw TranslationInputError("expecting \'->\' after left side of rule");
	}

	token = nextToken(input, sentinel);

	TranslationTree rightSide = readTranslationTree(input, token, sentinel);

	translations.push_back(make_pair(leftSide, rightSide));

	if(token != "\n" && token != "") {
		throw TranslationInputError("extra token \'" + token + "\' after translation tree");
	}

}

string Interpreter::nextToken(istream &input, char &sentinel) {
	
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
			throw TranslationInputError("line break in the middle of variable token \'" + variable + "\'");
		}
		if(!input) throw TranslationInputError("EOF in the middle of variable token \'" + variable + "\'");
		input.read(&sentinel, 1);
		return '<' + variable + '>';
	}
	else if(sentinel == '{' || sentinel == '}' || sentinel == '[' || sentinel == ']') {
		token += sentinel;
		input.read(&sentinel, 1);
		return token;
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
		if(isspace(sentinel) || sentinel == '<' || 
			sentinel == '{' || sentinel == '}' ||
			sentinel == '[' || sentinel == ']' ||
			!input) break;
		token += sentinel;
	}
	return token;

}

bool Interpreter::isVariableToken(const string &token) {
	return token.size() >= 2 && token[0] == '<' && token[token.size() - 1] == '>';
}

ParseTree Interpreter::readParseTree(istream &input, string &token, char &sentinel) {

	bool rootIsVariable = isVariableToken(token);
	string rootName = rootIsVariable ? token.substr(1, token.size() - 2) : token;
	ParseTree result(GrammarSymbol(rootName, !rootIsVariable));

	token = nextToken(input, sentinel);

	if(token != "{") {
		return result;
	}

	if(result.getSymbol().isTerminal()) {
		throw TranslationInputError("expecting variable at beginning of parse tree (read \'" + token + "\')");
	}

	token = nextToken(input, sentinel); // forward the first token
	while(input) {
		ParseTree child = readParseTree(input, token, sentinel);
		result.addChild(child);
		if(token == "}") break;
	}

	token = nextToken(input, sentinel);

	return result;

}

TranslationTree Interpreter::readTranslationTree(istream &input, string &token, char &sentinel) {

	bool rootIsVariable = isVariableToken(token);
	string rootString = (rootIsVariable ? token.substr(1, token.size() - 2) : token);
	TranslationTree result = TranslationTree(GrammarSymbol(rootString, !rootIsVariable));

	token = nextToken(input, sentinel);
	if(token == "[") {
		if(!rootIsVariable) {
			throw TranslationInputError("terminal symbol \'" + token + "\' cannot have a cousin");
		}
		token = nextToken(input, sentinel);
		if(!isVariableToken(token)) {
			throw TranslationInputError("cousin \'" + token + "\' must be a variable");
		}
		result.setCousin(token.substr(1, token.size() - 2));
		token = nextToken(input, sentinel);

		if(token != "]") {
			throw TranslationInputError("expecting \']\' after cousin token");
		}
		token = nextToken(input, sentinel);

		return result;
	}
	else if(token == "{") {
		if(!rootIsVariable) {
			throw TranslationInputError("terminal symbol \'" + token + "\' cannot have children");
		}
		token = nextToken(input, sentinel);
		while(input) {
			TranslationTree child = readTranslationTree(input, token, sentinel);
			result.addChild(child);
			if(token == "}") break;
		}
		token = nextToken(input, sentinel);
		return result;
	}

	return result;

}

void Interpreter::tagRules() {
	for(int i = 0; i < translations.size(); ++i) {
		// If a the root of the matching tree is not also a terminal, mark it.
		if(!translations[i].first.leaf()) {
			translations[i].first.markRoot(true);
		}
		translations[i].second.markAll(true);
	}
}

set<ParseTree> Interpreter::interpret(const ParseTree &input) const {
	
	// Initialize the recursive translation process by finding all cousins
	// which can adopt the root of the initial parse tree and recurse on all
	// such substitutions.
	set<ParseTree> solutions;
	set<GrammarSymbol> cousinIdentifiers;

	for(int i = 0; i < translations.size(); ++i) {
		if(hasCousin(input.getSymbol(), translations[i].second)) {
			cousinIdentifiers.insert(getCousinIdentifier(input.getSymbol(), translations[i].second));
		}
	}

	for(set<GrammarSymbol>::const_iterator i = cousinIdentifiers.begin(); i != cousinIdentifiers.end(); ++i) {
		ParseTree startTree(*i);
		startTree.adopt(input);
		vector<ParseTree> tempSolutions = interpretRecurse(startTree, TranslationTree(*i, vector<TranslationTree>()));
		for(int j = 0; j < tempSolutions.size(); ++j) {
			solutions.insert(tempSolutions[j]);
		}
	}
	
	return solutions;

}

set<ParseTree> Interpreter::interpretVerbose(const ParseTree &input, ostream &output) const {
	output << endl << "Input to interpreter:" << endl
		<< input << endl;
	set<ParseTree> solutions;
	set<GrammarSymbol> cousinIdentifiers;
	for(int i = 0; i < translations.size(); ++i) {
		if(hasCousin(input.getSymbol(), translations[i].second)) {
			output << "Found " << input.getSymbol() << " in " << translations[i].second << endl;
			GrammarSymbol tempSymbol = getCousinIdentifierVerbose(input.getSymbol(), translations[i].second, output);
			cousinIdentifiers.insert(tempSymbol);
		}
	}
	for(set<GrammarSymbol>::const_iterator i = cousinIdentifiers.begin(); i != cousinIdentifiers.end(); ++i) {
		ParseTree startTree(*i);
		startTree.adopt(input);
		vector<ParseTree> tempSolutions = interpretRecurseVerbose(startTree, TranslationTree(*i, vector<TranslationTree>()), output);
		for(int j = 0; j < tempSolutions.size(); ++j) {
			solutions.insert(tempSolutions[j]);
		}
	}

	return solutions;
}

bool Interpreter::hasCousin(const GrammarSymbol &s, const TranslationTree &t) {
	if(t.hasCousin()) {
		if(t.getCousin() == s.getName()) {
			return true;
		}
	}
	else {
		for(int i = 0; i < t.numChidren(); ++i) {
			if(hasCousin(s, t.getChild(i))) return true;
		}
	}
	return false;
}

GrammarSymbol Interpreter::getCousinIdentifier(const GrammarSymbol &s, const TranslationTree &t) {
	vector<GrammarSymbol> tempStrings;
	getCousinIdentifierRecurse(tempStrings, s, t);
	if(tempStrings.size() != 1) throw TranslationInputError("cousins are not consistent for sysmbol \'" + s.getName() + "\'");
	return tempStrings.front();
}

void Interpreter::getCousinIdentifierRecurse(vector<GrammarSymbol> &results, const GrammarSymbol &s, const TranslationTree &t) {
	if(t.hasCousin()) {
		if(t.getCousin() == s.getName()) {
			results.push_back(t.getSymbol());
		}
	}
	else {
		for(int i = 0; i < t.numChidren(); ++i) {
			getCousinIdentifierRecurse(results, s, t.getChild(i));
		}
	}
}

GrammarSymbol Interpreter::getCousinIdentifierVerbose(const GrammarSymbol &s, const TranslationTree &t, ostream &output) {
	vector<GrammarSymbol> tempStrings;
	getCousinIdentifierRecurse(tempStrings, s, t);
	if(tempStrings.size() != 1) output << "Warning: bad number of cousins found for symbol \'" << s.getName() << "\'" << endl;
	return tempStrings.front();
}

vector<ParseTree> Interpreter::interpretRecurse(const ParseTree &pt, const TranslationTree &tt) const {
	vector<ParseTree> newSolutions;
	if(tt.leaf()) {
		/**
		 If the given translation rule branch is a leaf, then look for possible
		 transformations of the corresponding parse tree branch, recursing to find all of its solutions.
		 */
		for(int i = 0; i < translations.size(); ++i) {
			if(hasMatch(pt, translations[i].first)) {
				ParseTree tempTree = makeChange(pt, translations[i].first, translations[i].second);
				vector<ParseTree> tempSolutions = interpretRecurse(tempTree, translations[i].second);
				for(int j = 0; j < tempSolutions.size(); ++j) {
					newSolutions.push_back(tempSolutions[j]);
				}
			}
		}

		/**
		 If no solutions are found when the rule branch is a terminal, check if the corresponing parse tree is also terminal.
		 */
		if(newSolutions.empty()) {
			/**
			 If it is, then it is a translated word and may be counted as a solution. This is the end result of the
			 translation process. Otherwise report that a solution is not availiable for a part of the tree.
			 */
			if(pt.leaf()) {
				newSolutions.push_back(pt);
			}
			else {
				throw TranslationInputError("No translation provided for \'" + pt.getSymbol().getName() + "\'");
			}
		}
	}
	else {
		/**
		 If the accompanying rule branch is not terminal, recurse on each of the parse tree/translation tree's children
		 and return solutions of the current parse tree's root with its children replaced with every combination of its
		 children's solutions.
		 */
		vector< vector<ParseTree> > childSolutions;
		for(int i = 0; i < pt.numChildren(); ++i) {
			childSolutions.push_back(interpretRecurse(pt.getChild(i), tt.getChild(i)));
		}
		
		vector< vector<ParseTree> > newChildSolutions;
		newChildSolutions = combinateSolutions(childSolutions);
		vector< vector<ParseTree> >::const_iterator iNewChildSolution, endNewChildSolution = newChildSolutions.end();
		for(iNewChildSolution = newChildSolutions.begin(); iNewChildSolution < endNewChildSolution; ++iNewChildSolution) {
			newSolutions.push_back(ParseTree(pt.getSymbol(), *iNewChildSolution));
		}
	}

	return newSolutions;

}

vector<ParseTree> Interpreter::interpretRecurseVerbose(const ParseTree &pt, const TranslationTree &tt, ostream &output) const {
	vector<ParseTree> newSolutions;
	if(tt.leaf()) {
		for(int i = 0; i < translations.size(); ++i) {
			if(hasMatch(pt, translations[i].first)) {
				output << pt << endl;
				ParseTree tempTree = makeChange(pt, translations[i].first, translations[i].second);
				output << tempTree << endl;
				vector<ParseTree> tempSolutions = interpretRecurseVerbose(tempTree, translations[i].second, output);
				for(int j = 0; j < tempSolutions.size(); ++j) {
					newSolutions.push_back(tempSolutions[j]);
				}
			}
		}
		if(newSolutions.empty()) {
			if(pt.leaf()) {
				newSolutions.push_back(pt);
			}
			else {
				output << "Warning: No translation provided for \'" << pt.getSymbol().getName() << "\'" << endl;
			}
		}
	}
	else {
		vector< vector<ParseTree> > childSolutions;
		for(int i = 0; i < pt.numChildren(); ++i) {
			childSolutions.push_back(interpretRecurseVerbose(pt.getChild(i), tt.getChild(i), output));
		}
		vector< vector<ParseTree> > newChildSolutions;
		newChildSolutions = combinateSolutions(childSolutions);
		vector< vector<ParseTree> >::const_iterator iNewChildSolution, endNewChildSolution = newChildSolutions.end();
		for(iNewChildSolution = newChildSolutions.begin(); iNewChildSolution < endNewChildSolution; ++iNewChildSolution) {
			newSolutions.push_back(ParseTree(pt.getSymbol(), *iNewChildSolution));
		}
	}
	return newSolutions;
}

bool Interpreter::hasMatch(const ParseTree &val, const ParseTree &match) {
	if(val.getSymbol() != match.getSymbol()) return false;
	for(int i = 0; i < match.numChildren(); ++i) {
		if(!(i < val.numChildren())) return false;
		if(!hasMatch(val.getChild(i), match.getChild(i))) return false;
	}
	return true;
}

ParseTree Interpreter::makeChange(const ParseTree &original, const ParseTree &match, const TranslationTree &tt) {
	ParseTree tempTree = ParseTree(tt);
	makeChangeRecurse(original, match, tt, tempTree);
	return tempTree;
}

void Interpreter::makeChangeRecurse(const ParseTree &original, const ParseTree &match,
	const TranslationTree &tt, ParseTree &replacement) {
	if(tt.leaf() && tt.hasCousin()) {
		replacement.adopt(findChildren(original, match, GrammarSymbol(tt.getCousin(), false)));
	}
	else {
		for(int i = 0; i < tt.numChidren(); ++i) {
			makeChangeRecurse(original, match, tt.getChild(i), replacement.getChild(i));
		}
	}
}

vector<ParseTree> Interpreter::findChildren(const ParseTree &original,
	const ParseTree &match, const GrammarSymbol &cousin) {

	vector< vector<ParseTree> > results;

	findChildrenRecurse(results, original, match, cousin);

	if(results.size() < 1) {
		throw TranslationInputError("cousin \'" + cousin.getName() + "\' not found when translating parse tree");
	}
	else if(results.size() > 1) {
		throw TranslationInputError("ambiguous cousin \'" + cousin.getName() + "\' found when translating parse tree");
	}
	
	return results.front();

}

void Interpreter::findChildrenRecurse(vector< vector<ParseTree> > &results, 
	const ParseTree &original, const ParseTree &match,
	const GrammarSymbol &cousin) {

	if(match.leaf()) {
		if(match.getSymbol() == cousin) {
			results.push_back(original.getChildren());
		}
	}
	else {
		for(int i = 0; i < match.numChildren(); ++i) {
			findChildrenRecurse(results, original.getChild(i), match.getChild(i), cousin);
		}
	}
}

vector<ParseTree> Interpreter::findChildrenVerbose(const ParseTree &original,
	const ParseTree &match, const GrammarSymbol &cousin, ostream &output) {
	vector< vector<ParseTree> > results;
	findChildrenRecurse(results, original, match, cousin);
	if(results.size() < 1) {
		output << "cousin \'" << cousin.getName() << "\' not found when translating parse tree" << endl;
	}
	else if(results.size() > 1) {
		output << "ambiguous cousin \'" << cousin.getName() << "\' found when translating parse tree" << endl;
	}
	return results.front();
}

vector< vector<ParseTree> > Interpreter::combinateSolutions(const vector< vector<ParseTree> > &uncombinated) {
	vector< vector<ParseTree> > combinations;
	vector<ParseTree> buffer;
	combinateRecurse(combinations, buffer, uncombinated, 0);
	return combinations;
}

void Interpreter::combinateRecurse(vector< vector<ParseTree> > &combinations,
	vector<ParseTree> &buffer, const vector< vector<ParseTree> > &uncombinated, int n) {
	int k = uncombinated[n].size();
	if(k == 0) {
		throw TranslationInputError("missing solution in the middle of translating");
	}
	if(n < uncombinated.size() - 1) {
		for(int i = 0; i < k; ++i) {
			buffer.push_back(uncombinated[n][i]);
			combinateRecurse(combinations, buffer, uncombinated, n + 1);
			buffer.pop_back();
		}
	}
	else {
		for(int i = 0; i < k; ++i) {
			buffer.push_back(uncombinated[n][i]);
			combinations.push_back(buffer);
			buffer.pop_back();
		}
	}
}

void Interpreter::printRules(ostream &output) const {
	for(int i = 0; i < translations.size(); ++i) {
		output << translations[i].first << "-> " << translations[i].second << endl;
	}
}

TranslationInputError::TranslationInputError(const string& msg) :
	runtime_error(msg) {
}
