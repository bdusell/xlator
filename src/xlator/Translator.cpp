#include "xlator/Translator.h"

namespace xlator {

void Translator::load_parser_from_file(std::istream &input)
	throw(load_parser_from_file_error)
{
	_parser.load_from_file(input);
}

void Translator::load_interpreter_from_file(std::istream &input)
	throw(load_interpreter_from_file_error)
{
	_interpreter.load_from_file(input);
}

void Translator::translate(const input_token_string &input, output_token_string_set &output) const
	throw(parsing_error, translation_error)
{

	// Parse the input token string into a set of parse trees
	Parser::parse_tree_set parse_trees;
	_parser.parse(input, parse_trees);

	// Translate each parse tree
	Interpreter::tree_set output_tree_set;
	ParseTree::value_list_type output_symbol_string;
	Translator::output_token_string output_token_string;

	for(Parser::parse_tree_set::const_iterator
		i = parse_trees.begin(), n = parse_trees.end(); i != n; ++i)
	{
		// Insert the leaves of each translated parse tree into the set
		// of output strings
		output_tree_set.clear();
		_interpreter.interpret(**i, output_tree_set);
		for(Interpreter::tree_set::const_iterator
			i = output_tree_set.begin(), n = output_tree_set.end(); i != n; ++i)
		{
			output_token_string.clear();
			(*i)->get_leaves(output_symbol_string);
			_interpreter.to_tokens(output_symbol_string, output_token_string);
			output.push_back(output_token_string);
		}
	}
}

} // namespace xlator

