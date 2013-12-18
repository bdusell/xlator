#ifndef TRANSLATIONTREE_H
#define	TRANSLATIONTREE_H

class ParseTree;
#include "ParseTree.h"
#include "GrammarSymbol.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// A recursive data structure which represents translation rules as a tree.
// 
/** The TranslationTree class is much like the ParseTree class but
identifies equivalent syntactical structures between languages. A 
TranslationTree object either has other TranslationTree objects as children or
else has no children and has a cousin which specifies what children it should
adopt from another ParseTree, if it should at all. */
class TranslationTree {
	
public:

	/// Constructor for a terminal TranslationTree which initializes its name and its cousin (if any).
	TranslationTree(const GrammarSymbol &initName, const string &initCousin = string());

	/// Constructor for a non-terminal ParseTree which initializes its name and its children.
	TranslationTree(const GrammarSymbol &initName, const vector<TranslationTree> &initChildren);

	void adopt(const TranslationTree &other);

	const GrammarSymbol &getSymbol() const;

	const TranslationTree &getChild(int i) const;

	void addChild(const TranslationTree &cname);

	void setCousin(const string &name);

	const string &getCousin() const;

	int numChidren() const;

	/// Predicate function which tells if a ParseTree has no children and should be examined for a cousin.
	bool leaf() const;

	bool hasCousin() const;

	void markRoot(bool m);

	void markAll(bool m);

	/// Overloaded output operator for printing the nested tree structure recursively.
	friend ostream &operator<<(ostream &output, const TranslationTree &val);

private:

	/// Name of the symbol/variable which the TranslationTree represents.
	GrammarSymbol name;

	/// Subtrees under the TranslationTree object.
	vector<TranslationTree> children;

	/// Cousin from another ParseTree from which a TranslationTree should adopt children.
	string cousin;

};

#endif

