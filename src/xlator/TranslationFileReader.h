#ifndef _XLATOR_TRANSLATIONFILEREADER_H_
#define _XLATOR_TRANSLATIONFILEREADER_H_

#include <iostream>

#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "xlator/ParseTree.h"
#include "xlator/TranslationTree.h"

namespace xlator {

class TranslationFileReader {

public:

	typedef SymbolIndexer::symbol_index symbol_index;

	struct rule {
		ParseTree::child_pointer_type pattern;
		TranslationTree::child_pointer_type translation;
	};

	typedef std::vector<rule> output_type;

	TranslationFileReader(
		std::istream &input, output_type &output,
		const SymbolIndexer &input_symbol_indexer,
		const SymbolInfo &input_symbol_info,
		SymbolIndexer &output_symbol_indexer,
		SymbolInfo &output_symbol_info);

	void read();

private:

	std::istream &input;
	output_type &output;

	const SymbolIndexer &input_symbol_indexer;
	const SymbolInfo &input_symbol_info;

	SymbolIndexer &output_symbol_indexer;
	SymbolInfo &output_symbol_info;

};

}

#endif

