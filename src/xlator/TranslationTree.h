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

	const symbol_type symbol;
	const int donor_index;

	static const int NO_DONOR = -1;

/*
	const union {
		symbol_type symbol;
		unsigned int donor_index;
	} value;
*/

	const child_list_type children;

private:

};

} // namespace xlator

#endif

