#ifndef _XLATOR_INTERPRETER_H_
#define _XLATOR_INTERPRETER_H_

#include <iostream>
#include <string>
#include <vector>

#include "xlator/ParseTree.h"
#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "meta/exception.h"

namespace xlator {

class Interpreter {

public:

	typedef std::vector<ParseTree::child_pointer_type> tree_set;
	typedef std::string token;
	typedef std::vector<token> token_string;
	typedef ParseTree::value_type symbol_type;
	typedef ParseTree::value_list_type symbol_string;

	EXCEPTION_CLASS(load_from_file_error)
	EXCEPTION_CLASS(translation_error)

	Interpreter(
		const SymbolIndexer &input_symbol_indexer,
		const SymbolInfo &input_symbol_info);

	void load_from_file(std::istream &input)
		throw(load_from_file_error);

	void interpret(const ParseTree &input, tree_set &output) const
		throw(translation_error);

	void to_tokens(const symbol_string &s, token_string &output) const;

private:

	const SymbolIndexer &input_symbol_indexer;
	const SymbolInfo &input_symbol_info;

	SymbolIndexer output_symbol_indexer;
	SymbolInfo output_symbol_info;

	void print_rules() const;

};

} // namespace xlator

#endif

