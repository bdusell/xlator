#include "LightParseTree.h"
using namespace std;

LightParseTree::LightParseTree(const GrammarSymbol &initValue, const vector<const LightParseTree *> &initChildren) :
	value(initValue), children(initChildren) {
}

const GrammarSymbol &LightParseTree::getSymbol() const {
	return value;
}

const LightParseTree &LightParseTree::getChild(int i) const {
	return *children[i];
}

int LightParseTree::numChildren() const {
	return children.size();
}

bool LightParseTree::leaf() const {
	return children.empty();
}

bool LightParseTree::operator<(const LightParseTree &other) const {
	if(value < other.value) return true;
	else if(value == other.value) {
		for(int i = 0; i < children.size() && i < other.children.size(); ++i) {
			if(*children[i] < *other.children[i]) return true;
		}
		return children.size() < other.children.size();
	}
	return false;
}

bool LightParseTree::operator==(const LightParseTree &other) const {
	if(value == other.value && children.size() == other.children.size()) {
		for(int i = 0; i < children.size(); ++i) {
			if(*children[i] != *other.children[i]) return false;
		}
	}
	return false;
}

bool LightParseTree::operator!=(const LightParseTree &other) const {
	return !(*this == other);
}

vector<string> LightParseTree::asWords() const {
	vector<string> result;
	asWordsRecurse(result);
	return result;
}

void LightParseTree::asWordsRecurse(vector<string> &result) const {
	if(leaf()) result.push_back(value.isTerminal() ? value.getName() : '<' + value.getName() + '>');
	else {
		for(int i = 0; i < children.size(); ++i) {
			children[i]->asWordsRecurse(result);
		}
	}
}

ostream &operator<<(ostream &output, const LightParseTree &val) {
	if (val.leaf()) output << val.value << ' ';
	else {
		output << val.value << '{' << ' ';
		for(int i = 0; i < val.children.size(); ++i) {
			output << *val.children[i];
		}
		output << '}' << ' ';
	}
	return output;
}
