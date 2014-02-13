#ifndef _XLATOR_TRANSLATIONFILEREADER_H_
#define _XLATOR_TRANSLATIONFILEREADER_H_

#include <iostream>

#include "xlator/FileReader.h"
#include "xlator/Indexer.h"
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

	SymbolIndexer &output_symbol_indexer;
	SymbolInfo &output_symbol_info;

	rule curr_rule;
	Indexer<std::string, int> curr_vars;
	bool at_top_level, was_variable;
	enum { LEFT_SIDE, RIGHT_SIDE } curr_side;

	virtual const char *token_type_name(token_type t) const;
	virtual void throw_exception(const std::string &s) const;
	virtual void read_token();

	ParseTree::child_pointer_type read_left_tree();
	TranslationTree::child_pointer_type read_right_tree();
	std::string curr_token_repr() const;
	ParseTree::value_type get_input_index(const std::string &name) const;
	static SymbolInfo::symbol::symbol_type token_type_to_symbol_type(short token_type);
	ParseTree::value_type get_symbol_index();

	void print_rules() const;
	void print_parse_tree(
		const ParseTree::child_pointer_type &t,
		unsigned int &counter,
		bool at_top) const;
	void print_translation_tree(
		const TranslationTree::child_pointer_type &t) const;

};

}

#endif

