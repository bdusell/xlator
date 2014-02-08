#ifndef _XLATOR_PARSER_H_
#define _XLATOR_PARSER_H_

#include <iostream>
#include <vector>

#include "xlator/ParseTree.h"
#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "meta/exception.h"

namespace xlator {

class Parser {

public:

	typedef std::string token;
	typedef std::vector<token> token_string;
	typedef std::vector<ParseTree> parse_tree_set;

	EXCEPTION_CLASS(load_from_file_error)
	EXCEPTION_CLASS(parsing_error)

	void load_from_file(std::istream &input)
		throw(load_from_file_error);

	void parse(const token_string &input, parse_tree_set &output) const
		throw(parsing_error);

private:

	typedef SymbolIndexer::symbol_index symbol_index;
	typedef std::vector<symbol_index> symbol_string;
	typedef std::map<symbol_string, std::vector<symbol_index> > rule_set_type;

	rule_set_type rules;
	SymbolInfo symbol_info;

	void print_rules() const;

};

} // namespace xlator

#endif

