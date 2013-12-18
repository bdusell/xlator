/*
File: TranslationTree.cpp
Authors: Brian DuSell and Justin Hintz, Spring 2011
Description: Implementation of the TranslationTree class.
*/

#include "TranslationTree.h"
using namespace std;

TranslationTree::TranslationTree(const GrammarSymbol &initName, const string &initCousin) :
	name(initName), cousin(initCousin), children() {
}

TranslationTree::TranslationTree(const GrammarSymbol &initName, const vector<TranslationTree> &initChildren) :
	name(initName), cousin(), children(initChildren) {
}

void TranslationTree::adopt(const TranslationTree &other) {
	children = other.children;
}

const GrammarSymbol &TranslationTree::getSymbol() const {
	return name;
}

const TranslationTree &TranslationTree::getChild(int i) const {
	return children[i];
}

void TranslationTree::addChild(const TranslationTree &c) {
	children.push_back(c);
}

void TranslationTree::setCousin(const string &cname) {
	cousin = cname;
}

const string &TranslationTree::getCousin() const {
	return cousin;
}

int TranslationTree::numChidren() const {
	return children.size();
}

bool TranslationTree::leaf() const {
	return children.empty();
}

bool TranslationTree::hasCousin() const {
	return !cousin.empty();
}

void TranslationTree::markRoot(bool m) {
	name.setMark(m);
}

void TranslationTree::markAll(bool m) {
	markRoot(m);
	for(int i = 0; i < children.size(); ++i) {
		if(children[i].leaf() && children[i].hasCousin()) children[i].markAll(m);
	}
}

ostream &operator<<(ostream &output, const TranslationTree &val) {

	if(val.leaf()) {
		output << val.name << ' ';
		if(!val.cousin.empty()) output << "[ <" << val.cousin << "> ] ";
	}
	else {
		vector<TranslationTree>::const_iterator iChild, endChild = val.children.end();
		cout << val.name << " { ";
		for (iChild = val.children.begin(); iChild < endChild; ++iChild) {
			output << *iChild;
		}
		cout << '}' << ' ';
	}

	return output;

}
