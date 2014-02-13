#ifndef _XLATOR_TRANSLATIONTREE_H_
#define _XLATOR_TRANSLATIONTREE_H_

#include <vector>

#include "xlator/ParseTree.h"
#include "resource/SharedPointer.h"

namespace xlator {

class TranslationTree {

public:

	typedef ParseTree::value_type symbol_type;
	typedef resource::SharedPointer<const TranslationTree> child_pointer_type;
	typedef std::vector<child_pointer_type> child_list_type;
	typedef int donor_index_type;

	static const donor_index_type NO_DONOR = -1;

	TranslationTree(symbol_type symbol);

	TranslationTree(
		symbol_type symbol,
		donor_index_type donor_index,
		const child_list_type &children);

	const symbol_type symbol;
	const donor_index_type donor_index;

	const child_list_type children;

	bool is_leaf() const;

private:

};

} // namespace xlator

#endif

