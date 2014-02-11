#include "xlator/ParseTreeEnumerator.h"

#include "xlator/ParseForestNode.h"

namespace xlator {

ParseTreeEnumerator::ParseTreeEnumerator(parse_tree_set &output)
	: output(output)
{
}

// TODO

/*
void ParseTreeEnumerator::enumerate(const ParseForestNode *node) {
	enumerate_recurse(node, output);
}

void ParseTreeEnumerator::enumerate_recurse(
	const ParseForestNode *node, parse_tree_set &result)
{
	if(node->children.empty()) {
		result.push_back(new ParseTree(node->value));
	}
	else {
		for(ParseForestNode::child_list_type::const_iterator
			i = node->children.begin(), n = node->children.end(); i != n; ++i)
		{
			result.push_back();
		}
	}
}
*/

} // namespace xlator

