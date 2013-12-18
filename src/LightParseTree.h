#ifndef LIGHTPARSETREE_H
#define	LIGHTPARSETREE_H

#include "GrammarSymbol.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// A lightweight, pointer-based version of the parse tree class for internal use
// in the parser class.
class LightParseTree {

public:

	// Constructor which initializes the parse tree with a name for the root
	// node as well as pointers to child nodes. Pointers are not deallocated
	// by the parse tree after use.
	LightParseTree(const GrammarSymbol &initName,
		const vector<const LightParseTree *> &initChildren = vector<const LightParseTree *>());

	// Gets the name of the root node.
	const GrammarSymbol &getSymbol() const;

	// Gets the value of the ith child node.
	const LightParseTree &getChild(int i) const;

	// Gets the number of child nodes.
	int numChildren() const;

	// Tells if the tree is a leaf node.
	bool leaf() const;

	// Overloaded operators for recursive comparison.
	bool operator<(const LightParseTree &other) const;
	bool operator==(const LightParseTree &other) const;
	bool operator!=(const LightParseTree &other) const;

	// Returns the leaves of the tree in order as a sequence of strings.
	vector<string> asWords() const;

	// Stream output operator.
	friend ostream &operator<<(ostream &output, const LightParseTree &val);

private:

	// Symbol at the root node.
	GrammarSymbol value;

	// Pointers to immutable child trees.
	vector<const LightParseTree *> children;

	void asWordsRecurse(vector<string> &result) const;

};

#endif

