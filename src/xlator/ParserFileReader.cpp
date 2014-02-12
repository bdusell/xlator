#include "xlator/ParserFileReader.h"

namespace xlator {

ParserFileReader::ParserFileReader(
	std::istream &fin,
	output_type &output,
	SymbolInfo &symbol_info,
	SymbolIndexer &symbol_indexer
)
	: FileReader(fin)
	, output(output)
	, symbol_info(symbol_info)
	, symbol_indexer(symbol_indexer)
{}

void ParserFileReader::read() {
	do read_token(); while(curr_token_type == NEWLINE);
	while(curr_token_type != END) {
		if(curr_token_type != NONTERMINAL) {
			raise_error(
				std::string("expected ")
				+ token_type_name(NONTERMINAL)
				+ " on left side of rule but read "
				+ curr_token_name());
		}
		curr_rule.left = symbol_indexer.index_symbol(
			curr_token_value, SymbolIndexer::symbol::NONTERMINAL);
		expect_token(ARROW);
		curr_rule.right.clear();
		curr_rule.right.push_back(rule::right_type::value_type());
		while(true) {
			read_token();
			if(curr_token_type == NEWLINE) break;
			rule::right_type::value_type &curr_string = curr_rule.right.back();
			switch(curr_token_type) {
			case NONTERMINAL:
			case TERMINAL:
				curr_string.push_back(
					symbol_indexer.index_symbol(
						curr_token_value,
						curr_token_type == NONTERMINAL ?
							SymbolIndexer::symbol::NONTERMINAL
							: SymbolIndexer::symbol::TERMINAL));
				break;
			case PIPE:
				curr_rule.right.push_back(rule::right_type::value_type());
				break;
			default:
				raise_error(
					std::string("expected ")
					+ token_type_name(NONTERMINAL)
					+ " or " + token_type_name(TERMINAL)
					+ " but read " + curr_token_name());
			}
		}
		output.push_back(curr_rule);
		do read_token(); while(curr_token_type == NEWLINE);
	}
	if(output.empty()) {
		raise_error("no rules specified");
	}
	symbol_indexer.create_mapping(symbol_info);
}

const char *ParserFileReader::token_type_name(token_type t) const {
	static const char *names[] = {
		"nonterminal",
		"terminal",
		"arrow",
		"newline",
		"pipe",
		"end of file"
	};
	return names[t];
}

void ParserFileReader::throw_exception(const std::string &s) const {
	throw error(s);
}

void ParserFileReader::read_token() {
	while(!at_eof() && isspace(next_char) && next_char != '\n') read_char();
	if(at_eof()) {
		curr_token_type = END;
		return;
	}
	switch(next_char) {
	case '<':
		curr_token_type = NONTERMINAL;
		read_to_delim('>');
		break;
	case '\"':
		curr_token_type = TERMINAL;
		read_to_delim('\"');
		break;
	case '-':
		curr_token_type = ARROW;
		read_char();
		if(next_char != '>') {
			error_stray('-');
		}
		break;
	case '\n':
		curr_token_type = NEWLINE;
		break;
	case '|':
		curr_token_type = PIPE;
		break;
	default:
		error_stray(next_char);
		break;
	}
	read_char();
/*
#	ifdef DEBUG
	std::cerr
		<< token_type_name(curr_token_type)
		<< " \"" << curr_token_value << "\""
		<< std::endl;
#	endif
*/
}

} // namespace xlator

