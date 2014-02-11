#include "xlator/TranslationFileReader.h"

namespace xlator {

TranslationFileReader::TranslationFileReader(
	std::istream &input, output_type &output,
	const SymbolIndexer &input_symbol_indexer,
	const SymbolInfo &input_symbol_info,
	SymbolIndexer &output_symbol_indexer,
	SymbolInfo &output_symbol_info)
	: input(input)
	, output(output)
	, input_symbol_indexer(input_symbol_indexer)
	, input_symbol_info(input_symbol_info)
	, output_symbol_indexer(output_symbol_indexer)
	, output_symbol_info(output_symbol_info)
{
}

void TranslationFileReader::read() {
	// TODO
}

} // namespace xlator

