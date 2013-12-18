#include "LightParseTree.h"
#include "ParseTree.h"
#include "TranslationTree.h"
using namespace std;

ParseTree::ParseTree(const GrammarSymbol &initValue, const vector<ParseTree> &initChildren) :
	value(initValue), children(initChildren) {
}

ParseTree::ParseTree(const LightParseTree &copy) :
	value(copy.getSymbol()) {
	for(int i = 0; i < copy.numChildren(); ++i) {
		children.push_back(copy.getChild(i));
	}
}

ParseTree::ParseTree(const TranslationTree &copy) :
	value(copy.getSymbol()) {
	for(int i = 0; i < copy.numChidren(); ++i) {
		children.push_back(ParseTree(copy.getChild(i)));
	}
}

const GrammarSymbol &ParseTree::getSymbol() const {
	return value;
}

void ParseTree::markRoot(bool m) {
	value.setMark(m);
}

void ParseTree::markAll(bool m) {
	value.setMark(m);
	for(int i = 0; i < children.size(); ++i) {
		children[i].markAll(m);
	}
}

const ParseTree &ParseTree::getChild(int i) const {
	return children[i];
}

ParseTree &ParseTree::getChild(int i) {
	return children[i];
}

const vector<ParseTree> &ParseTree::getChildren() const {
	return children;
}

void ParseTree::addChild(const ParseTree &c) {
	children.push_back(c);
}

int ParseTree::numChildren() const {
	return children.size();
}

bool ParseTree::leaf() const {
	return children.empty();
}

void ParseTree::adopt(const ParseTree &other) {
	children = other.children;
}

void ParseTree::adopt(const vector<ParseTree>& newChildren) {
	children = newChildren;
}

bool ParseTree::operator<(const ParseTree &other) const {
	if(value < other.value) return true;
	else if(value == other.value) return children < other.children;
	return false;
}

bool ParseTree::operator==(const ParseTree &other) const {
	return value == other.value && children == other.children;
}

bool ParseTree::operator!=(const ParseTree &other) const {
	return !(*this == other);
}

vector<string> ParseTree::asWords() const {
	vector<string> result;
	asWordsRecurse(result);
	return result;
}

void ParseTree::asWordsRecurse(vector<string> &result) const {
	if(leaf()) result.push_back(value.isTerminal() ? value.getName() : '<' + value.getName() + '>');
	else {
		for(int i = 0; i < children.size(); ++i) {
			children[i].asWordsRecurse(result);
		}
	}
}

ostream &operator<<(ostream &output, const ParseTree &val) {
	output << val.value << ' ';
	if(!val.leaf()) {
		output << "{ ";
		for(int i = 0; i < val.children.size(); ++i) {
			output << val.children[i];
		}
		output << "} ";
	}
	return output;
}
