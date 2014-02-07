#ifndef _XLATOR_TRANSLATOR_H_
#define	_XLATOR_TRANSLATOR_H_

#include <iostream>
#include <vector>

#include "xlator/Parser.h"
#include "xlator/Interpreter.h"
#include "meta/exception.h"

namespace xlator {

/*
The complete translator class which accepts tokenized natural language as input
and reproduces it in tokens corresponding to another language as output. It is
configured by two main parts: a context-free grammar for parsing the input
language, and a translation grammar for transforming the parsed input into the
output language.
*/
class Translator {

public:

	typedef Parser::token input_token;
	typedef Parser::token_string input_token_string;
	typedef input_token_string output_token_string;
	typedef std::vector<output_token_string> output_token_string_set;

	typedef Parser::load_from_file_error load_parser_from_file_error;
	typedef Interpreter::load_from_file_error load_interpreter_from_file_error;

	typedef Parser::parsing_error parsing_error;
	typedef Interpreter::translation_error translation_error;

	/*
	Load the parser rules from an input stream.
	*/
	void load_parser_from_file(std::istream &fin)
		throw(load_parser_from_file_error);

	/*
	Load the tranlation rules from an input stream.
	*/
	void load_interpreter_from_file(std::istream &fin)
		throw(load_interpreter_from_file_error);

	/*
	Parse and translate an input string and store the results in the output
	string.
	*/
	void translate(const input_token_string &input, output_token_string_set &output) const
		throw(parsing_error, translation_error);

private:

	Parser _parser;
	Interpreter _interpreter;

};

} // namespace xlator

#endif

