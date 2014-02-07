#ifndef _XLATOR_PARSETREE_H_
#define _XLATOR_PARSETREE_H_

#include <string>
#include <vector>

namespace xlator {

class ParseTree {

public:

	typedef std::vector<std::string> token_string;

	void get_leaves(token_string &output) const;

private:

};

} // namespace xlator

#endif

