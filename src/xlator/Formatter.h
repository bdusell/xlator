#ifndef _XLATOR_FORMATTER_H_
#define _XLATOR_FORMATTER_H_

#include <string>

#include "xlator/Translator.h"

namespace xlator {

class Formatter {

public:

	typedef Translator::output_token_string token_string;

	void format(const token_string &input, std::string &output) const;

private:

};

} // namespace xlator

#endif

