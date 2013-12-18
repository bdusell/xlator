/*
File: Translation.h
Authors: Brian DuSell and Justin Hintz, Spring 2011
*/

#ifndef TRANSLATION_H
#define	TRANSLATION_H

#include "ParseTree.h"
#include "TranslationTree.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/// A class which specifies how to transform a section of a ParseTree.
/** The Translation class represents translation rules to be used in
the translation process. A Translation object specifies a parse tree pattern to
look for when translating recursively and how to change the parse tree. */
class Translation {

	friend class Interpreter;
	
public:

	/// Constructor which initializes the ParseTree used for matching and TranslationTree used for transforming.
	Translation(const ParseTree &initMatch, const TranslationTree &initCousins);

	const ParseTree &getMatch() const;

	/// Overloaded output operator for printing a transformation rule.
	friend ostream &operator<<(ostream &output, const Translation &val);

private:

	/// ParseTree structure to be looked for when applying a translation rule.
	ParseTree match;

	/// TranslationTree structure which specifies how to transform the matching ParseTree.
	TranslationTree cousins;

};

#endif

