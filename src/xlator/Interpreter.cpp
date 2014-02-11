#include "xlator/Interpreter.h"

#include "xlator/TranslationFileReader.h"

namespace xlator {

Interpreter::Interpreter(
	const SymbolIndexer &input_symbol_indexer,
	const SymbolInfo &input_symbol_info)
	: input_symbol_indexer(input_symbol_indexer)
	, input_symbol_info(input_symbol_info)
{
}

void Interpreter::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	TranslationFileReader::output_type output;
	TranslationFileReader reader(
		input, output,
		input_symbol_indexer, input_symbol_info,
		output_symbol_indexer, output_symbol_info);
	reader.read();
	// TODO
	throw load_from_file_error("reading translation file is not implemented yet");
}

void Interpreter::interpret(const ParseTree &input, tree_set &output) const
	throw(translation_error)
{
	throw translation_error("translation is not implemented yet");
	// TODO
}

void Interpreter::to_tokens(const symbol_string &s, token_string &output) const {
	// TODO
}

} // namespace xlator

