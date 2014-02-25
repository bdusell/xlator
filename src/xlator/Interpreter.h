#ifndef _XLATOR_INTERPRETER_H_
#define _XLATOR_INTERPRETER_H_

#include <iostream>
#include <string>
#include <vector>

#include "xlator/ParseTree.h"
#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "xlator/ParseTreeMatcher.h"
#include "meta/exception.h"

namespace xlator {

class Interpreter {

public:

	typedef ParseTree::child_list_type tree_set;
	typedef std::string token;
	typedef std::vector<token> token_string;
	typedef ParseTree::value_type symbol_type;
	typedef ParseTree::value_list_type symbol_string;

	EXCEPTION_CLASS(load_from_file_error)
	EXCEPTION_CLASS(translation_error)

	Interpreter(
		const SymbolIndexer &input_symbol_indexer,
		SymbolInfo &symbol_info);

	void load_from_file(std::istream &input)
		throw(load_from_file_error);

	void interpret(const ParseTree::child_pointer_type &input, tree_set &output) const
		throw(translation_error);

	void to_tokens(const symbol_string &s, token_string &output) const;

private:

	const SymbolIndexer &input_symbol_indexer;
	SymbolIndexer output_symbol_indexer;
	SymbolInfo &symbol_info;

	typedef ParseTreeMatcher::rule_type rule_type;
	typedef std::vector<rule_type> rule_list_type;

	rule_list_type rules;
	ParseTreeMatcher parse_tree_matcher;

	class Helper {

	public:

		Helper(const Interpreter &interpreter);

		void interpret(
			const ParseTree::child_pointer_type &node,
			tree_set &output);

	private:

		const Interpreter &interpreter;
		ParseTree::child_list_type helper_key;

	};

};

} // namespace xlator

#endif

