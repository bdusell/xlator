#ifndef _XLATOR_PARSETREE_H_
#define _XLATOR_PARSETREE_H_

#include <vector>
#include <iostream>

#include "xlator/SymbolInfo.h"
#include "resource/SharedPointer.h"

namespace xlator {

class ParseForestNode;
class SymbolInfo;

class ParseTree {

public:

	typedef SymbolInfo::symbol_key value_type;
	typedef resource::SharedPointer<const ParseTree> child_pointer_type;
	typedef std::vector<child_pointer_type> child_list_type;
	typedef std::vector<value_type> value_list_type;

	ParseTree(value_type value);
	ParseTree(value_type value, const child_list_type &children);
	explicit ParseTree(const ParseForestNode *node);

	bool is_leaf() const;
	void get_leaves(value_list_type &output) const;

	void print(const SymbolInfo &info, std::ostream &out) const;

	const value_type value;
	const child_list_type children;

};

} // namespace xlator

#endif

