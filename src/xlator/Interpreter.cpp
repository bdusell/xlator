#include "xlator/Interpreter.h"

#include <cassert>
#include <sstream>
#include <iterator>
#include "xlator/TranslationFileReader.h"
#include "algorithm/alternations.h"

namespace xlator {

Interpreter::Interpreter(
	const SymbolIndexer &input_symbol_indexer,
	SymbolInfo &symbol_info)
	: input_symbol_indexer(input_symbol_indexer)
	, symbol_info(symbol_info)
	, parse_tree_matcher(symbol_info)
{
}

void Interpreter::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	TranslationFileReader reader(
		input, rules,
		input_symbol_indexer, output_symbol_indexer,
		symbol_info);
	reader.read();
	ParseTreeMatcher::key_type key(1);
	for(rule_list_type::const_iterator
		i = rules.begin(), n = rules.end(); i != n; ++i)
	{
		key[0] = i->pattern;
		parse_tree_matcher.insert(key, &*i);
	}
#	ifdef DEBUG
	parse_tree_matcher.print();
#	endif
}

void Interpreter::interpret(const ParseTree::child_pointer_type &input, tree_set &output) const
	throw(translation_error)
{
	Helper helper(*this);
	helper.interpret(input, output);
}

void Interpreter::to_tokens(const symbol_string &s, token_string &output) const {
	for(symbol_string::const_iterator
		i = s.begin(), n = s.end(); i != n; ++i)
	{
		output.push_back(symbol_info[*i].name);
	}
}

Interpreter::Helper::Helper(const Interpreter &interpreter)
	: interpreter(interpreter)
	, helper_key(1)
{
}

namespace {

void get_leaves(
	const TranslationTree::child_pointer_type &root,
	TranslationTree::child_list_type &output)
{
	/* Leaves which are terminals will be marked NO_DONOR. */
	if(root->donor_index == TranslationTree::NO_DONOR) {
		for(TranslationTree::child_list_type::const_iterator
			i = root->children.begin(), n = root->children.end(); i != n; ++i)
		{
			get_leaves(*i, output);
		}
	}
	else {
		output.push_back(root);
	}
}

ParseTree::child_pointer_type attach_leaves(
	const TranslationTree::child_pointer_type &translation,
	ParseTree::child_list_type::const_iterator &subroot_iter)
{
	// TODO
	if(translation->is_leaf()) {
		/* This is interesting to think about. Since we already re-
		ordered any child sets when adopting the children of the
		input parse tree just before the recursive call to interpret(),
		we don't use the donor index here except to check that this
		leaf becomes the next tree root. The list of subtree roots
		given by subroot_iter is already in the new order. */
		if(translation->donor_index != TranslationTree::NO_DONOR) {
			/* Just return the root node of the next subtree,
			because it already has the new children and replaces
			this leaf node in the translation tree. Note that we
			do not check for the end of the iterator range because
			it should have exactly the number of leaf nodes of this
			translation tree. Also note that this subtree root node
			may end up being shared by multiple parent nodes. */
			return *subroot_iter++;
		}
		/* Otherwise, this is a terminal and we copy it as usual. */
	}
	ParseTree::child_list_type children;
	for(TranslationTree::child_list_type::const_iterator
		i = translation->children.begin(), n = translation->children.end(); i != n; ++i)
	{
		children.push_back(attach_leaves(*i, subroot_iter));
	}
	return ParseTree::child_pointer_type(
		new ParseTree(translation->symbol, children));
}

} // anonymous namespace

void Interpreter::Helper::interpret(
	const ParseTree::child_pointer_type &node,
	tree_set &output)
{
	ParseTreeMatcher::match_list_type matches;
	helper_key[0] = node;
	interpreter.parse_tree_matcher.find(helper_key, matches);

#	ifdef DEBUG
	std::cout << "MATCHED RULES:" << std::endl;
	for(ParseTreeMatcher::match_list_type::const_iterator
		i = matches.begin(), n = matches.end(); i != n; ++i)
	{
		i->first->print(interpreter.symbol_info, std::cout);
		std::cout << std::endl;
	}
#	endif

	typedef std::vector<tree_set> result_list_type;
	typedef std::vector<ParseTree::child_list_type> alternation_list_type;

	result_list_type sub_results;
	TranslationTree::child_list_type translation_leaves;
	alternation_list_type alternations;

	for(ParseTreeMatcher::match_list_type::const_iterator
		i = matches.begin(), n = matches.end(); i != n; ++i)
	{
		sub_results.clear();
		translation_leaves.clear();
		/* TODO A potential optimization is to precomute the list of
		leaf nodes in each translation tree. */
		get_leaves(i->first->translation, translation_leaves);
		for(TranslationTree::child_list_type::const_iterator
			j = translation_leaves.begin(), m = translation_leaves.end(); j != m; ++j)
		{
			assert((*j)->donor_index != TranslationTree::NO_DONOR);
			assert(0 <= (*j)->donor_index && (*j)->donor_index < (int) i->second.size());
			sub_results.push_back(tree_set());
			interpret(
				ParseTree::child_pointer_type(
					new ParseTree(
						(*j)->symbol,
						i->second[(*j)->donor_index]->children)),
				sub_results.back());
		}
		/* XXX Do note that since we are re-using references when
		generating alternations of the sub-results, the parse trees
		generated can share subtrees and therefore form a DAG. So, we
		can use this as a justification for using shared pointers as
		the pointer type for ParseTree. For TranslationTree, however,
		it is not important. */
		alternations.clear();
		algorithm::generate_alternations(
			sub_results.begin(), sub_results.end(), alternations);
		for(alternation_list_type::const_iterator
			j = alternations.begin(), m = alternations.end(); j != m; ++j)
		{
			ParseTree::child_list_type::const_iterator subroot_iter = j->begin();
			output.push_back(attach_leaves(i->first->translation, subroot_iter));
		}
	}

	/* If we ever can't come up with at least one result for a node in the
	parse tree in mid-translation, we're sunk. */
	if(output.empty()) {
		std::ostringstream tmp;
		node->print(interpreter.symbol_info, tmp);
		throw translation_error(
			"no rule to translate parse tree " + tmp.str());
	}

}

} // namespace xlator

