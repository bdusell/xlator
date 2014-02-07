#ifndef _XLATOR_TOKENIZER_H_
#define _XLATOR_TOKENIZER_H_

#include <string>

#include "xlator/Parser.h"

namespace xlator {

class Tokenizer {

public:

	typedef Parser::token token;
	typedef Parser::token_string token_string;

	void tokenize(const std::string &input, token_string &output) const;

private:

};

} // namespace xlator

#endif

