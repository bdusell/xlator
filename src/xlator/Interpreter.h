#ifndef _XLATOR_INTERPRETER_H_
#define _XLATOR_INTERPRETER_H_

#include <iostream>

#include "xlator/ParseTree.h"
#include "meta/exception.h"

namespace xlator {

class Interpreter {

public:

	typedef std::vector<ParseTree> tree_set;

	EXCEPTION_CLASS(load_from_file_error)
	EXCEPTION_CLASS(translation_error)

	void load_from_file(std::istream &input)
		throw(load_from_file_error);

	void interpret(const ParseTree &input, tree_set &output) const
		throw(translation_error);

private:

};

} // namespace xlator

#endif

