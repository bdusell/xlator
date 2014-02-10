#include "ParseTree.h"

namespace xlator {

ParseTree::~ParseTree() {
	for(child_list_type::const_iterator
		i = children.begin(), n = children.end(); i != n; ++i)
	{
		delete *i;
	}
}

void ParseTree::get_leaves(value_list_type &output) const {
	// TODO
}

} // namespace xlator

