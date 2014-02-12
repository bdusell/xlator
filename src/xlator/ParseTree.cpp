#include "ParseTree.h"

#include "xlator/ParseForestNode.h"

namespace xlator {

namespace {
ParseTree::child_list_type make_child_list(const ParseForestNode *node) {
	ParseTree::child_list_type result;
	for(ParseForestNode::child_list_type::const_iterator
		i = node->children.begin(), n = node->children.end(); i != n; ++i)
	{
		result.push_back(ParseTree::child_pointer_type(new ParseTree(*i)));
	}
	return result;
}
}

ParseTree::ParseTree(value_type value) 
	: value(value)
{
}

ParseTree::ParseTree(value_type value, const child_list_type &children)
	: value(value)
	, children(children)
{
}

ParseTree::ParseTree(const ParseForestNode *node)
	: value(node->value)
	, children(make_child_list(node))
{
}

bool ParseTree::is_leaf() const {
	return children.empty();
}

void ParseTree::get_leaves(value_list_type &output) const {
	if(is_leaf()) {
		output.push_back(value);
	}
	else {
		for(child_list_type::const_iterator
			i = children.begin(), n = children.end(); i != n; ++i)
		{
			(*i)->get_leaves(output);
		}
	}
}

} // namespace xlator

