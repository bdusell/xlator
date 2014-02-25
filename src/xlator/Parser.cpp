#include "xlator/Parser.h"

#include <cassert>
#include "xlator/ParserFileReader.h"

namespace xlator {

Parser::Parser(SymbolIndexer &symbol_indexer, SymbolInfo &symbol_info)
	: symbol_indexer(symbol_indexer)
	, symbol_info(symbol_info)
{
}

void Parser::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	ParserFileReader::output_type output;
	ParserFileReader reader(input, output, symbol_info, symbol_indexer);
	reader.read();
	for(ParserFileReader::output_type::const_iterator
		i = output.begin(), n = output.end(); i != n; ++i)
	{
		for(ParserFileReader::output_type::value_type::right_type::const_iterator
			j = i->right.begin(), m = i->right.end(); j != m; ++j)
		{
			rules[*j].push_back(i->left);
		}
	}
	// TODO validate grammar for acyclicity and no e-rules
#	ifdef DEBUG
	print_rules();
#	endif
}

void Parser::parse(const token_string &input, parse_tree_set &output) const
	throw(parsing_error)
{
	Helper::output_type helper_output;
	Helper helper(*this, helper_output);
	helper.parse(input);
	if(helper_output.empty()) {
		throw parsing_error(
			"the grammar does not recognize the input string");
	}
#	ifdef DEBUG
	print_helper_output(helper_output);
#	endif
	for(Helper::output_type::const_iterator
		i = helper_output.begin(), n = helper_output.end(); i != n; ++i)
	{
		output.push_back(parse_tree_pointer_type(new ParseTree(*i)));
	}
}

void Parser::print_rules() const {
	std::cout << "RULES:" << std::endl;
	for(rule_set_type::const_iterator
		i = rules.begin(), n = rules.end(); i != n; ++i)
	{
		symbol_info.print_string(i->first.begin(), i->first.end(), std::cout);
		std::cout << " => { ";
		symbol_info.print_string(i->second.begin(), i->second.end(), std::cout);
		std::cout << " }" << std::endl;
	}
	std::cout << std::endl;
}

void Parser::print_helper_output(const Helper::output_type &helper_output) const {
	std::cout << "HELPER:" << std::endl;
	for(Helper::output_type::const_iterator
		i = helper_output.begin(), n = helper_output.end(); i != n; ++i)
	{
		print_tree(*i);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Parser::print_tree(const ParseForestNode *node) const {
	symbol_info.print_symbol(node->value, std::cout);
	if(!node->children.empty()) {
		std::cout << " {";
		for(ParseForestNode::child_list_type::const_iterator
			i = node->children.begin(), n = node->children.end(); i != n; ++i)
		{
			std::cout << ' ';
			print_tree(*i);
		}
		std::cout << " }";
	}
}

Parser::Helper::Helper(
	const Parser &parser,
	output_type &output
)
	: parser(parser)
	, output(output)
{
}

Parser::Helper::~Helper() {
	for(forest_type::const_iterator
		i = subtree_cache.begin(), n = subtree_cache.end(); i != n; ++i)
	{
		delete *i;
	}
}

namespace {
bool value_less(const ParseForestNode *a, const ParseForestNode *b) {
	return a->value < b->value;
}
}

bool Parser::Helper::VisitedSetLess::operator()(
	const node_string &a, const node_string &b) const
{
	return std::lexicographical_compare(
		a.begin(), a.end(),
		b.begin(), b.end(),
		&value_less
	);
}

bool Parser::Helper::ParseForestNodeLess::operator()(
	const ParseForestNode *a, const ParseForestNode *b) const
{
	return (
		a->value < b->value ||
		(a->value == b->value && a->children < b->children));
}

void Parser::Helper::parse(const token_string &s) {
	/* Convert the string of tokens into a string of parse forest nodes.
	*/
	node_string scopy;
	symbol_index index;
	for(token_string::const_iterator
		i = s.begin(), n = s.end(); i != n; ++i)
	{
		if(!parser.symbol_indexer.get_index(*i, SymbolInfo::symbol::TERMINAL, index)) {
			throw parsing_error("input token \"" + *i + "\" not recognized");
		}
		scopy.push_back(get_branch_pointer(ParseForestNode(index)));
	}
	parse_recurse(scopy);
}

void Parser::Helper::parse_recurse(const node_string &s) {
	/* Iterate through all substrings of s looking for matching rules. */
#	ifdef DEBUG
	std::cout << "parse(";
	print_string(s);
	std::cout << ')' << std::endl;
	print_forest();
#	endif
	/* Skip visited strings. By skipping sequences of
	symbols which we have already explored, we actually
	avoid getting stuck in cyclic grammars. */
	std::pair<visited_set::const_iterator, bool> insert_result =
		visited_strings.insert(s);
	if(insert_result.second) {
		symbol_string substring_key;
		node_string substituted_string;
		for(node_string::const_iterator
			i = s.begin(), n = s.end(); i != n; ++i)
		{
			for(node_string::const_iterator
				j = i + 1, m = s.end(); j <= m; ++j)
			{
				/* Create the key used to look up the matching rules.
				*/
				substring_key.clear();
				for(node_string::const_iterator
					k = i; k != j; ++k)
				{
					substring_key.push_back((*k)->value);
				}
				/* Look up the matching rules. */
				rule_set_type::const_iterator pos = parser.rules.find(substring_key);
				if(pos != parser.rules.end()) {
					/* For each variable which may replace the
					substring, perform the substitution and recurse
					on the new input. Record any results which are
					one symbol long, as these represent complete
					parses. */
					for(rule_set_value_type::const_iterator
						vari = pos->second.begin(), varn = pos->second.end(); vari != varn; ++vari)
					{
						substituted_string.assign(s.begin(), i);
						/* Substitute the substring with a node
						with the substituted parts as children.
						*/
						substituted_string.push_back(
							get_branch_pointer(
								ParseForestNode(*vari, i, j)));
						substituted_string.insert(substituted_string.end(), j, s.end());
						parse_recurse(substituted_string);
						/* Adding results after recursing allows
						successful parses with the tallest trees to be
						recorded first. In other words, if tree height
						is measure of successfulness, then results are
						sorted roughly in decreasing order of
						successfulness. */
						if(substituted_string.size() == 1) {
							output.push_back(substituted_string.front());
						}
					}
				}
			}
		}
	}
}

const ParseForestNode *Parser::Helper::get_branch_pointer(const ParseForestNode &branch) {
	/* If the given branch is found in the cache, use the existing tree. If
	not, create a new copy of the branch. */

	/* Look for an existing copy of the branch. */
	forest_type::const_iterator result = subtree_cache.find(&branch);

	/* If the branch is not found, create a new copy of the branch on the
	heap and insert it. */
	const ParseForestNode *ptr;
	if(result == subtree_cache.end()) {
#		ifdef DEBUG
		std::pair<forest_type::const_iterator, bool> pos =
#		endif
		subtree_cache.insert(ptr = new ParseForestNode(branch));
#		ifdef DEBUG
		assert(pos.second);
#		endif
	}
	else {
		ptr = *result;
	}

	return ptr;
}

void Parser::Helper::print_forest() const {
	std::cout << "FOREST:" << std::endl;
	for(forest_type::const_iterator
		i = subtree_cache.begin(), n = subtree_cache.end(); i != n; ++i)
	{
		parser.print_tree(*i);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Parser::Helper::print_string(const node_string &s) const {
	bool first = true;
	for(node_string::const_iterator
		i = s.begin(), n = s.end(); i != n; ++i)
	{
		if(first) first = false;
		else std::cout << ' ';
		parser.symbol_info.print_symbol((*i)->value, std::cout);
	}
}

} // namespace xlator

