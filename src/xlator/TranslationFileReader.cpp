#include "xlator/TranslationFileReader.h"

namespace xlator {

TranslationFileReader::TranslationFileReader(
	std::istream &input, output_type &output,
	const SymbolIndexer &input_symbol_indexer,
	const SymbolInfo &input_symbol_info,
	SymbolIndexer &output_symbol_indexer,
	SymbolInfo &output_symbol_info)
	: FileReader(input)
	, output(output)
	, input_symbol_indexer(input_symbol_indexer)
	, input_symbol_info(input_symbol_info)
	, output_symbol_indexer(output_symbol_indexer)
	, output_symbol_info(output_symbol_info)
{
}

void TranslationFileReader::read() {
	do read_token(); while(curr_token_type == NEWLINE);
	while(curr_token_type != END) {
		read_tree(LEFT_SIDE);
		expect_token(ARROW);
		read_tree(RIGHT_SIDE);
		if(curr_token_type != NEWLINE && curr_token_type != END) {
			raise_error(
				std::string("expected ") + token_type_name(NEWLINE)
				+ " or " + token_type_name(END) + " but read "
				+ curr_token_name());
		}
	}
}

void TranslationFileReader::read_tree(short side) {
	// TODO
	switch(curr_token_type) {
	case NONTERMINAL:

		break;
	case TERMINAL:

		break;
	default:
		raise_error("");
	}
}

const char *TranslationFileReader::token_type_name(token_type t) const {
	static const char *names[] = {
		"nonterminal",
		"terminal",
		"variable",
		"arrow",
		"newline",
		"pipe",
		"left brace",
		"right brace",
		"end of file"
	};
	return names[t];
}

void TranslationFileReader::raise_error(const std::string &s) const {
	throw error(s);
}

void TranslationFileReader::read_token() {
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
	case '{':
		curr_token_type = LBRACE;
		break;
	case '}':
		curr_token_type = RBRACE;
		break;
	default:
		curr_token_type = VARIABLE;
		curr_token_value.clear();
		do {
			curr_token_value += next_char;
			read_char();
		} while(!isspace(next_char));
		return; // don't read an extra char
	}
	read_char();
}

} // namespace xlator

