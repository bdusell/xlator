#ifndef _XLATOR_PARSERFILEREADER_H_
#define _XLATOR_PARSERFILEREADER_H_

#include "xlator/Parser.h"
#include "xlator/SymbolInfo.h"

namespace xlator {

class ParserFileReader {

public:

	typedef SymbolIndexer::symbol_index symbol_index;

	struct rule {
		symbol_index left;
		typedef std::vector< std::vector<symbol_index> > right_type;
		right_type right;
	};

	typedef std::vector<rule> output_type;

	typedef Parser::load_from_file_error error;

	ParserFileReader(
		std::istream &fin,
		output_type &output,
		SymbolInfo &symbol_info);

	void read();

private:

	enum token_type { NONTERMINAL, TERMINAL, ARROW, NEWLINE, PIPE, END };

	std::istream &fin;
	output_type &output;
	SymbolInfo &symbol_info;

	std::string curr_token_value;
	token_type curr_token_type;
	char next_char;

	SymbolIndexer symbol_indexer;
	rule curr_rule;

	static const char *token_type_name(token_type t);
	const char *curr_token_name() const;
	void expect_token(token_type type);
	void fail_token(token_type type);
	void read_token();
	static void error_stray(char c);
	void read_to_delim(char delim);
	void need_char();
	void fail_if_eof() const;
	void read_char();

};

} // namespace xlator

#endif

