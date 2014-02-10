#include "xlator/Tokenizer.h"

#include <cctype>

namespace xlator {

void Tokenizer::tokenize(const std::string &input, token_string &output) const {
	std::string curr_token;
	for(std::string::const_iterator
		i = input.begin(), n = input.end(); i != n; ++i)
	{
		if(isspace(*i)) {
			if(!curr_token.empty()) {
				output.push_back(curr_token);
				curr_token.clear();
			}
		}
		else {
			curr_token += *i;
		}
	}
	if(!curr_token.empty()) {
		output.push_back(curr_token);
	}
}

} // namespace xlator

