#include "xlator/Interpreter.h"

namespace xlator {

void Interpreter::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	// TODO
}

void Interpreter::interpret(const ParseTree &input, tree_set &output) const
	throw(translation_error)
{
	throw translation_error("translation is not implemented yet");
	// TODO
}

} // namespace xlator
