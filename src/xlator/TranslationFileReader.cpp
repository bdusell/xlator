#include "xlator/TranslationFileReader.h"

#include <sstream>

namespace xlator {

TranslationFileReader::TranslationFileReader(
	std::istream &input, output_type &output,
	const SymbolIndexer &input_symbol_indexer,
	SymbolIndexer &output_symbol_indexer,
	SymbolInfo &symbol_info)
	: FileReader(input)
	, output(output)
	, input_symbol_indexer(input_symbol_indexer)
	, output_symbol_indexer(output_symbol_indexer)
	, symbol_info(symbol_info)
{
	/* Make sure the codes for input grammar and translation grammar
	symbols are always distinct. */
	output_symbol_indexer.set_base(input_symbol_indexer.next_index());
}

void TranslationFileReader::read() {
	do read_token(); while(curr_token_type == NEWLINE);
	while(curr_token_type != END) {

		curr_vars.clear();

		at_top_level = true;
		curr_rule.pattern = read_left_tree();

		expect_token(ARROW);
		read_token();

		curr_rule.translation = read_right_tree();

		output.push_back(curr_rule);

		do read_token(); while(curr_token_type == NEWLINE);
	}
	output_symbol_indexer.create_mapping(symbol_info);
#ifdef	DEBUG
	print_rules();
	symbol_info.print();
#endif
}

ParseTree::child_pointer_type TranslationFileReader::read_left_tree() {
	/*
	Precondition: The first token of the tree has already been read.
	Postcondition: The last token of the tree has been read but not the one after it.
	*/

	ParseTree::child_pointer_type result;
	switch(curr_token_type) {
	case NONTERMINAL:
	{
		ParseTree::value_type value = get_symbol_index();

		expect_token(LBRACE);

		/* This helps decide where to look up the symbol index (input
		or translation grammar). */
		/*
		TODO see about allowing single terminals on the left side,
		since the translation algorithm can easily accommodate it
		*/
		at_top_level = false;

		/* Recursively read sub-trees. */
		ParseTree::child_list_type result_children;
		ParseTree::child_pointer_type subtree;
		while(true) {
			read_token();
			if(curr_token_type == RBRACE) break;
			was_variable = false;
			subtree = read_left_tree();
			bool subtree_was_variable = was_variable;
			was_variable = false;
			if(subtree_was_variable) {
				// Quit early if the subtree was a variable
				expect_token(RBRACE);
				break;
			}
			result_children.push_back(subtree);
		}
		result = ParseTree::child_pointer_type(
			new ParseTree(value, result_children));
	}
		break;
	case TERMINAL:
		result = ParseTree::child_pointer_type(
			new ParseTree(get_symbol_index()));
		break;
	case VARIABLE:
		curr_vars.index_key(curr_token_value, curr_vars.size());
		was_variable = true;
		break;
	default:
		raise_error(
			std::string("expected a syntax tree but got ")
			+ curr_token_name());
	}

	return result;
}

TranslationTree::child_pointer_type TranslationFileReader::read_right_tree() {
	/*
	Precondition: The first token of the tree has already been read.
	Postcondition: The last token of the tree has been read but not the one after it.
	*/

	// TODO think about code reuse

	TranslationTree::child_pointer_type result;
	switch(curr_token_type) {
	case NONTERMINAL:
	{

		/* Symbols on the right side are always in the translation
		grammar. */
		TranslationTree::symbol_type value =
			output_symbol_indexer.index_symbol(
				curr_token_value,
				SymbolInfo::symbol::NONTERMINAL);
		TranslationTree::donor_index_type result_donor_index =
			TranslationTree::NO_DONOR;

		expect_token(LBRACE);

		/* Recursively read sub-trees. */
		TranslationTree::child_list_type result_children;
		TranslationTree::child_pointer_type subtree;
		while(true) {
			read_token();
			if(curr_token_type == RBRACE) break;
			was_variable = false;
			subtree = read_right_tree();
			bool subtree_was_variable = was_variable;
			was_variable = false;
			if(subtree_was_variable) {
				/* If the subtree is a variable, set the donor index
				to something interesting. */
				if(!curr_vars.get_index(curr_token_value, result_donor_index)) {
					raise_error(
						std::string("variable ")
						+ curr_token_value
						+ " not found on left side of rule");
				}
				// Quit early if the subtree was a variable
				expect_token(RBRACE);
				break;
			}
			result_children.push_back(subtree);
		}
		result = TranslationTree::child_pointer_type(
			new TranslationTree(value, result_donor_index, result_children));
	}
		break;
	case TERMINAL:
		result = TranslationTree::child_pointer_type(
			new TranslationTree(
				output_symbol_indexer.index_symbol(
					curr_token_value,
					SymbolInfo::symbol::TERMINAL)));
		break;
	case VARIABLE:
		/* Just signal that we got a variable. */
		was_variable = true;
		break;
	default:
		raise_error(
			std::string("expected a syntax tree but got ")
			+ curr_token_name());
	}

	return result;
}

std::string TranslationFileReader::curr_token_repr() const {
	std::ostringstream out;
	SymbolInfo::print_symbol(
		curr_token_value,
		token_type_to_symbol_type(curr_token_type),
		out);
	return out.str();
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

void TranslationFileReader::throw_exception(const std::string &s) const {
	throw error(s);
}

void TranslationFileReader::read_token() {
	while(!at_eof() && isspace(next_char) && next_char != '\n') read_char();
	if(at_eof()) {
		curr_token_type = END;
		return;
	}
	bool need_to_read_char = true;
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
		} while(isalnum(next_char));
		need_to_read_char = false;
	}
	if(need_to_read_char) read_char();
}

ParseTree::value_type TranslationFileReader::get_input_index(const std::string &name) const {
	SymbolIndexer::symbol_index result;
	if(!input_symbol_indexer.get_index(
		name,
		token_type_to_symbol_type(curr_token_type),
		result))
	{
		raise_error(
			std::string("symbol does not exist in input grammar: ")
			+ curr_token_repr());
	}
	return result;
}

SymbolInfo::symbol::symbol_type TranslationFileReader::token_type_to_symbol_type(short token_type) {
	return token_type == NONTERMINAL ?
		SymbolInfo::symbol::NONTERMINAL :
		SymbolInfo::symbol::TERMINAL;
}

ParseTree::value_type TranslationFileReader::get_symbol_index() {
	return !at_top_level ?
		get_input_index(curr_token_value) :
		output_symbol_indexer.index_symbol(
			curr_token_value,
			token_type_to_symbol_type(curr_token_type));
}

void TranslationFileReader::print_rules() const {
	for(output_type::const_iterator
		i = output.begin(), n = output.end(); i != n; ++i)
	{
		unsigned int counter = 0;
		print_parse_tree(i->pattern, counter, true);
		std::cout << " -> ";
		print_translation_tree(i->translation);
		std::cout << std::endl;
	}
}

void TranslationFileReader::print_parse_tree(
	const ParseTree::child_pointer_type &t,
	unsigned int &counter,
	bool at_top) const
{
	symbol_info.print_symbol(t->value, std::cout);
	if(t->is_leaf()) {
		if(symbol_info[t->value].type == SymbolInfo::symbol::NONTERMINAL) {
			std::cout << " { " << counter++ << " }";
		}
	}
	else {
		std::cout << " {";
		for(ParseTree::child_list_type::const_iterator
			i = t->children.begin(), n = t->children.end(); i != n; ++i)
		{
			std::cout << ' ';
			print_parse_tree(*i, counter, false);
		}
		std::cout << " }";
	}
}

void TranslationFileReader::print_translation_tree(
	const TranslationTree::child_pointer_type &t) const
{
	symbol_info.print_symbol(t->symbol, std::cout);
	if(t->is_leaf()) {
		if(t->donor_index != TranslationTree::NO_DONOR) {
			std::cout << " { " << t->donor_index << " }";
		}
	}
	else {
		std::cout << " {";
		for(TranslationTree::child_list_type::const_iterator
			i = t->children.begin(), n = t->children.end(); i != n; ++i)
		{
			std::cout << ' ';
			print_translation_tree(*i);
		}
		std::cout << " }";
	}
}

} // namespace xlator

