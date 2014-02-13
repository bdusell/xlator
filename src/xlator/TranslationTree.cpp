#include "xlator/TranslationTree.h"

namespace xlator {

TranslationTree::TranslationTree(symbol_type symbol)
	: symbol(symbol)
	, donor_index(NO_DONOR)
{
}

TranslationTree::TranslationTree(
	symbol_type symbol,
	donor_index_type donor_index,
	const child_list_type &children)
	: symbol(symbol)
	, donor_index(donor_index)
	, children(children)
{
}

bool TranslationTree::is_leaf() const {
	return children.empty();
}

} // namespace xlator

