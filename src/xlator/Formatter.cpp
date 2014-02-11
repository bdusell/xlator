#include "xlator/Formatter.h"

namespace xlator {

void Formatter::format(const token_string &input, std::string &output) const {
	bool first = true;
	for(token_string::const_iterator
		i = input.begin(), n = input.end(); i != n; ++i)
	{
		if(first) first = false;
		else output += ' ';
		output += *i;
	}
}

} // namespace xlator

