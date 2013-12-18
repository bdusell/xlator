#ifndef PARSETREE_H
#define	PARSETREE_H

#include "GrammarSymbol.h"
#include "LightParseTree.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TranslationTree;
#include "TranslationTree.h"

// A recursive data structure which represents sentence structure as a nested
// hierarchy of phrases and other grammatical constructions. This tree may have
// an arbitrary number of children per node. The purpose of this tree is not to
// be well-balanced or searchable, but simply to encode sentence structure.
class ParseTree {

public:

	// Constructor which initializes the parse tree with a name for the root
	// and values for the child nodes.
	ParseTree(const GrammarSymbol &initValue, const vector<ParseTree> &initChildren = vector<ParseTree>());

	// Conversion constructor from the lightweight version of the parse tree
	// class which produces a deep copy.
	ParseTree(const LightParseTree &copy);

	// Coversion constructor from translation tree.
	ParseTree(const TranslationTree &copy);

	// Gets the name of the root node.
	const GrammarSymbol &getSymbol() const;

	// Sets the mark on the symbol on the root node.
	void markRoot(bool m);

	// Marks all nodes in the tree.
	void markAll(bool m);

	// Gets the value of the ith child node.
	const ParseTree &getChild(int i) const;
	ParseTree &getChild(int i);

	// Gets a reference to a vector of the tree's children.
	const vector<ParseTree> &getChildren() const;

	// Adds a child tree to the root node.
	void addChild(const ParseTree &c);

	// Gets the number of child nodes.
	int numChildren() const;

	// Tells if the tree is a leaf node.
	bool leaf() const;

	// Adopts the children of another tree.
	void adopt(const ParseTree &other);
	void adopt(const vector<ParseTree> &newChildren);

	// Overloaded operators for recursive comparison.
	bool operator<(const ParseTree &other) const;
	bool operator==(const ParseTree &other) const;
	bool operator!=(const ParseTree &other) const;

	// Returns the leaves of the tree in order as a sequence of strings.
	vector<string> asWords() const;

	// Stream output operator.
	friend ostream &operator<<(ostream &output, const ParseTree &val);

private:

	// Name of the root node.
	GrammarSymbol value;

	// Pointers to immutable child trees.
	vector<ParseTree> children;

	void asWordsRecurse(vector<string> &result) const;

};

#endif

