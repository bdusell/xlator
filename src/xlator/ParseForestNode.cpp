#include "xlator/ParseForestNode.h"

#include <algorithm>

#include "functional/pointer_cmp.h"

namespace xlator {

ParseForestNode::ParseForestNode(symbol_type value)
	: value(value)
{
}

ParseForestNode::ParseForestNode(symbol_type value, const child_list_type &children)
	: value(value)
	, children(children)
{
}

ParseForestNode::ParseForestNode(
	symbol_type value,
	child_list_type::const_iterator children_begin,
	child_list_type::const_iterator children_end
	)
	: value(value)
	, children(children_begin, children_end)
{
}

bool ParseForestNode::operator<(const ParseForestNode &that) const {
	return (
		value < that.value ||
		std::lexicographical_compare(
			children.begin(), children.end(),
			that.children.begin(), that.children.end(),
			functional::pointer_less<const ParseForestNode *>()
		));
}

bool ParseForestNode::operator==(const ParseForestNode &that) const {
	return (
		value == that.value && 
		children.size() == that.children.size() &&
		std::equal(
			children.begin(), children.end(),
			that.children.begin(),
			functional::pointer_equal<const ParseForestNode *>()));
}

} // namespace xlator

