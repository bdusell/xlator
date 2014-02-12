#ifndef _XLATOR_PARSERFILEREADER_H_
#define _XLATOR_PARSERFILEREADER_H_

#include "xlator/FileReader.h"
#include "xlator/Parser.h"
#include "xlator/SymbolInfo.h"

namespace xlator {

class ParserFileReader : public FileReader {

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
		SymbolInfo &symbol_info,
		SymbolIndexer &symbol_indexer);

	void read();

private:

	enum { NONTERMINAL, TERMINAL, ARROW, NEWLINE, PIPE, END };

	output_type &output;
	SymbolInfo &symbol_info;
	SymbolIndexer &symbol_indexer;

	rule curr_rule;

	virtual const char *token_type_name(token_type t) const;
	virtual void throw_exception(const std::string &s) const;
	virtual void read_token();

};

} // namespace xlator

#endif

