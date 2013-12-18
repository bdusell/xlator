/*
File: Translation.cpp
Authors: Brian DuSell and Justin Hintz, Spring 2011
Description: Implementation of the Translation class.
*/

#include "Translation.h"
using namespace std;

Translation::Translation(const ParseTree &initMatch, const TranslationTree &initCousins) :
	match(initMatch), cousins(initCousins) {

}

const ParseTree &Translation::getMatch() const {
	return match;
}

ostream &operator<<(ostream &output, const Translation &val) {

	output << val.match << "-> " << val.cousins;

	return output;

}
