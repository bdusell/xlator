#ifndef _XLATOR_PARSETREEENUMERATOR_H_
#define _XLATOR_PARSETREEENUMERATOR_H_

#include "xlator/ParseTree.h"


namespace xlator {

class ParseForestNode;

class ParseTreeEnumerator {

public:

	typedef std::vector<ParseTree> parse_tree_set;

	ParseTreeEnumerator(parse_tree_set &output);

	// TODO
	//void enumerate(const ParseForestNode *node);

private:

	parse_tree_set output;

};

} // namespace xlator

#endif

