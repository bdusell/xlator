#include "xlator/ParseTreeEnumerator.h"

namespace xlator {

ParseTreeEnumerator::ParseTreeEnumerator(parse_tree_set &output)
	: output(output)
{
}

void ParseTreeEnumerator::enumerate(const ParseForestNode *node) {
	// TODO
}

} // namespace xlator

