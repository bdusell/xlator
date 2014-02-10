#ifndef _XLATOR_PARSETREE_H_
#define _XLATOR_PARSETREE_H_

#include <vector>

#include "xlator/SymbolInfo.h"

namespace xlator {

class ParseTree {

public:

	typedef SymbolInfo::symbol_key value_type;
	typedef std::vector<const ParseTree *> child_list_type;
	typedef std::vector<value_type> value_list_type;

	template <typename I>
	ParseTree(value_type value, I children_begin, I children_end);

	~ParseTree();

	void get_leaves(value_list_type &output) const;

	const value_type value;
	const child_list_type children;

};

template <typename I>
ParseTree::ParseTree(value_type value, I children_begin, I children_end)
	: value(value)
	, children(children_begin, children_end)
{
}

} // namespace xlator

#endif

