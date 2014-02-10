#ifndef _XLATOR_PARSEFORESTNODE_H_
#define _XLATOR_PARSEFORESTNODE_H_

#include <vector>
#include "xlator/SymbolInfo.h"

namespace xlator {

class ParseForestNode {

public:

	typedef SymbolInfo::symbol_key symbol_type;
	typedef std::vector<const ParseForestNode *> child_list_type;

	explicit ParseForestNode(symbol_type value);

	ParseForestNode(symbol_type value, const child_list_type &children);

	ParseForestNode(
		symbol_type value,
		child_list_type::const_iterator children_begin,
		child_list_type::const_iterator children_end
	);

	const symbol_type value;
	const child_list_type children;

	/* Compare subtrees by value. */
	bool operator<(const ParseForestNode &that) const;
	bool operator==(const ParseForestNode &that) const;

};

} // namespace xlator

#endif

