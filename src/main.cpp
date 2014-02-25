#include <iostream>
#include <fstream>
#include <stdexcept>
#include "resource/ScopedFile.h"
#include "xlator/Translator.h"
#include "xlator/Tokenizer.h"
#include "xlator/Formatter.h"

void print_usage(std::ostream &output) {
	output << "Usage: xlator <parser file> <translator file>" << std::endl;
}

int main(int argc, char **argv) {

	// Parse the command line arguments
	char *parser_finname = NULL;
	char *translator_finname = NULL;
	for(char **argi = argv + 1, **argn = argv + argc; argi != argn; ++argi) {
		if(parser_finname == NULL) {
			parser_finname = *argi;
		}
		else if(translator_finname == NULL) {
			translator_finname = *argi;
		}
		else {
			print_usage(std::cerr);
			std::cerr << "error: unrecognized argument \"" << *argi << "\"" << std::endl;
			return 1;
		}
	}
	if(parser_finname == NULL || translator_finname == NULL) {
		print_usage(std::cerr);
		std::cerr << "error: input files are required" << std::endl;
		return 1;
	}

	// Load the translator data from the input files
	xlator::Translator translator;
	try {
		resource::ScopedFile<std::ifstream> parser_fin(parser_finname);
		translator.load_parser_from_file(parser_fin);
	}
	catch(xlator::Translator::load_parser_from_file_error &e) {
		std::cerr << parser_finname << ':' << e.what() << std::endl;
		return 1;
	}

	try {
		resource::ScopedFile<std::ifstream> translator_fin(translator_finname);
		translator.load_interpreter_from_file(translator_fin);
	}
	catch(xlator::Translator::load_interpreter_from_file_error &e) {
		std::cerr << translator_finname << ':' << e.what() << std::endl;
		return 1;
	}

	// Get the raw input line
	std::string input_line;
	std::getline(std::cin, input_line);

	// Tokenize the input text
	xlator::Tokenizer::token_string input_tokens;
	xlator::Tokenizer tokenizer;
	tokenizer.tokenize(input_line, input_tokens);

	// Translate the input tokens
	xlator::Translator::output_token_string_set output_token_set;
	try {
		translator.translate(input_tokens, output_token_set);
	}
	catch(xlator::Translator::parsing_error &e) {
		std::cerr << "ungrammatical input: " << e.what() << std::endl;
		return 1;
	}
	catch(xlator::Translator::translation_error &e) {
		std::cerr << "unable to translate: " << e.what() << std::endl;
		return 1;
	}

	// Print the results
	xlator::Formatter formatter;
	for(xlator::Translator::output_token_string_set::const_iterator
		i = output_token_set.begin(), n = output_token_set.end(); i != n; ++i)
	{
		// Convert the translated tokens back into text
		input_line.clear();
		formatter.format(*i, input_line);

		// Print the result
		std::cout << input_line << std::endl;
	}

	return 0;
}

