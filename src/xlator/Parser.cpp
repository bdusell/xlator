#include "xlator/Parser.h"

namespace xlator {

namespace {

class ParserFileReader {

public:

	typedef SymbolIndexer::symbol_index symbol_index;

	struct rule {
		symbol_index left;
		typedef std::vector< std::vector<symbol_index> > right;
	};

	typedef std::vector<rule> output_type;

	typedef Parser::load_from_file_error error;

	ParserFileReader(
		std::istream &fin,
		output_type &output
	)
		: fin(fin)
		, output(output)
	{
		read_char();
	}

	void read() {
		do {
			read_token();
		} while(curr_token_type != END);
		if(output.empty()) {
			throw error("no rules specified");
		}
	}

private:

	enum token_type { NONTERMINAL, TERMINAL, ARROW, NEWLINE, PIPE, END };

	std::istream &fin;
	output_type &output;

	std::string curr_token_value;
	token_type curr_token_type;
	char next_char;

	static const char *token_type_name(token_type t) {
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

	void read_token() {
		while(isspace(next_char) && next_char != '\n') read_char();
		if(!fin.good()) {
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
#		ifdef DEBUG
		std::cerr
			<< token_type_name(curr_token_type)
			<< " \"" << curr_token_value << "\""
			<< std::endl;
#		endif
*/
	}

	static void error_stray(char c) {
		throw error(
			std::string("stray \"") + c + "\"");
	}

	void read_to_delim(char delim) {
		curr_token_value.clear();
		std::getline(fin, curr_token_value, delim);
		fail_if_eof();
	}

	void need_char() {
		read_char();
		fail_if_eof();
	}

	void fail_if_eof() const {
		if(!fin.good()) {
			throw error(
				std::string(token_type_name(END))
				+ " reached while reading "
				+ token_type_name(curr_token_type));
		}
	}

	void read_char() {
		fin.read(&next_char, 1);
	}

};

} // anonymous namespace

void Parser::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	ParserFileReader::output_type output;
	ParserFileReader reader(input, output);
	reader.read();
	// TODO reformat rules
}

void Parser::parse(const token_string &input, parse_tree_set &output) const
	throw(parsing_error)
{
	// TODO
}

} // namespace xlator

