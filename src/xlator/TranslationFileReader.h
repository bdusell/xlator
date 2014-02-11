#ifndef _XLATOR_TRANSLATIONFILEREADER_H_
#define _XLATOR_TRANSLATIONFILEREADER_H_

#include <iostream>

#include "xlator/FileReader.h"
#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "xlator/ParseTree.h"
#include "xlator/TranslationTree.h"
#include "xlator/Interpreter.h"

namespace xlator {

class TranslationFileReader : public FileReader {

public:

	typedef SymbolIndexer::symbol_index symbol_index;

	struct rule {
		ParseTree::child_pointer_type pattern;
		TranslationTree::child_pointer_type translation;
	};

	typedef std::vector<rule> output_type;

	typedef Interpreter::load_from_file_error error;

	TranslationFileReader(
		std::istream &input, output_type &output,
		const SymbolIndexer &input_symbol_indexer,
		const SymbolInfo &input_symbol_info,
		SymbolIndexer &output_symbol_indexer,
		SymbolInfo &output_symbol_info);

	void read();

private:

	enum {
		NONTERMINAL, TERMINAL, VARIABLE, ARROW,
		NEWLINE, PIPE, LBRACE, RBRACE, END };

	output_type &output;

	const SymbolIndexer &input_symbol_indexer;
	const SymbolInfo &input_symbol_info;

	rule curr_rule;

	SymbolIndexer &output_symbol_indexer;
	SymbolInfo &output_symbol_info;

	virtual const char *token_type_name(token_type t) const;
	virtual void raise_error(const std::string &s) const;
	virtual void read_token();

	enum { LEFT_SIDE, RIGHT_SIDE };
	void read_tree(short side);

};

}

#endif

