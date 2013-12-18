#ifndef GRAMMARSYMBOL_H
#define	GRAMMARSYMBOL_H

#include <string>
#include <iostream>
using namespace std;

// Grammar symbol class which is used to represent both variables and terminals
// in grammar rules.
class GrammarSymbol {

public:
	// Default constructor
	GrammarSymbol();

	// Initializes the symbol with an identifier and whether it is a
	// terminal or not.
	GrammarSymbol(const string &initName, bool initType, bool initMark = false);

	// Gets the name of the symbol.
	const string &getName() const;

	// Tells if the symbol is a variable.
	bool isVariable() const;

	// Tells if the symbol is a terminal.
	bool isTerminal() const;

	// Marks the symbol to distinguish it from something with the same name.
	void setMark(bool m);

	// Tells if symbol is marked.
	bool isMarked() const;

	// Overloaded comparison operators.
	bool operator<(const GrammarSymbol &other) const;
	bool operator==(const GrammarSymbol &other) const;
	bool operator!=(const GrammarSymbol &other) const;

private:
	string name;
	bool thisIsTerminal;
	bool mark;

};

ostream &operator<<(ostream &output, const GrammarSymbol &val);

#endif

