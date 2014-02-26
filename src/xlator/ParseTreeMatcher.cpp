#include "xlator/ParseTreeMatcher.h"

#include <cassert>
#include <set>
#include <algorithm>

namespace xlator {

ParseTreeMatcher::ParseTreeMatcher(const SymbolInfo &symbol_info)
	: matcher(symbol_info)
{
}

void ParseTreeMatcher::rule_type::print(const SymbolInfo &info, std::ostream &out) const {
	unsigned int counter = 0;
	out << index << ". ";
	print_parse_tree(pattern, info, counter, true, out);
	out << " -> ";
	print_translation_tree(translation, info, out);
}

void ParseTreeMatcher::rule_type::print_parse_tree(
	const ParseTree::child_pointer_type tree,
	const SymbolInfo &info,
	unsigned int &counter,
	bool at_top,
	std::ostream &out) const
{
	info.print_symbol(tree->value, out);
	if(tree->is_leaf()) {
		if(info[tree->value].type == SymbolInfo::symbol::NONTERMINAL) {
			out << " { " << counter++ << " }";
		}
	}
	else {
		out << " {";
		for(ParseTree::child_list_type::const_iterator
			i = tree->children.begin(), n = tree->children.end(); i != n; ++i)
		{
			out << ' ';
			print_parse_tree(*i, info, counter, false, out);
		}
		std::cout << " }";
	}
}

void ParseTreeMatcher::rule_type::print_translation_tree(
	const TranslationTree::child_pointer_type tree,
	const SymbolInfo &info,
	std::ostream &out) const
{
	info.print_symbol(tree->symbol, out);
	if(tree->is_leaf()) {
		if(tree->donor_index != TranslationTree::NO_DONOR) {
			out << " { " << tree->donor_index << " }";
		}
	}
	else {
		out << " {";
		for(TranslationTree::child_list_type::const_iterator
			i = tree->children.begin(), n = tree->children.end(); i != n; ++i)
		{
			out << ' ';
			print_translation_tree(*i, info, out);
		}
		out << " }";
	}
}

void ParseTreeMatcher::insert(const key_type &trees, const rule_type *rule) {
	matcher.insert(trees, rule);
}

void ParseTreeMatcher::find(const key_type &trees, match_list_type &output) const {
	matcher.find(trees, output);
}

void ParseTreeMatcher::print() const {
	std::cout << "MATCHER:" << std::endl;
	matcher.print("");
}

bool ParseTreeMatcher::RuleComparator::operator()(
	const rule_type *a, const rule_type *b) const
{
	return a->index < b->index;
}

ParseTreeMatcher::ParseTreeSequenceMatcher::ParseTreeSequenceMatcher(
	const SymbolInfo &symbol_info)
	: symbol_info(&symbol_info)
{
}

void ParseTreeMatcher::ParseTreeSequenceMatcher::insert(
	const ParseTree::child_list_type &key,
	const rule_type *rule)
{
	map_type::mapped_type &m = map[key];
	if(m.size() != key.size()) m.assign(key.size(), symbol_info_type(*symbol_info));
	for(ParseTree::child_list_type::size_type
		i = 0, n = key.size(); i < n; ++i)
	{
		if(key[i]->children.empty()) {
			m[i].matched.push_back(rule);
		}
		else {
			m[i].subtree.insert(key[i]->children, rule);
		}
	}
}

void ParseTreeMatcher::ParseTreeSequenceMatcher::find(
	const ParseTree::child_list_type &key,
	match_list_type &output) const
{
	/* Look up the given sequence of symbols in the map. If it is not
	found, give up and return no matches. */
	map_type::const_iterator pos = map.find(key);
	if(pos != map.end()) {

		/* If the sequence is found, recursively check the children
		of each of these nodes for matches. If there are any rules
		which are matched by all children, return those in the matches.
		Then also include any rules which are fully matched by this set
		of children (found in symbol_info_list[i].matched). */

		const std::vector<symbol_info_type> &symbol_info_list = pos->second;

		assert(!symbol_info_list.empty());

		match_list_type matches;

		/* In output we store the set of rules which are reported to
		have been matched in every subtree of the list provided as
		the key value. It is the set intersection of all the sets of
		rules fully matched in the sub-trees. */

		/* Loop through the children in the list provided as the key
		value and each child's corresponding symbol info
		simultaneously. */
		bool first = true;
		for(ParseTree::child_list_type::size_type
			i = 0, n = key.size(); i < n; ++i)
		{
			const symbol_info_type &info = symbol_info_list[i];

			/* Recursively search for matches in the subtree. Here
			we get rules which were matched deeper in the tree than
			at the current child node. */
			matches.clear();
			info.subtree.find(key[i]->children, matches);

			/* We get the rules matched at the current child node
			from info.matched. For each of these, we insert the
			current node as a leaf for this matched rule (unless
			it is a terminal). Note that this set of rules is
			disjoint from those returned by info.subtree.find(). */
			for(rule_list_type::const_iterator
				j = info.matched.begin(), m = info.matched.end(); j != m; ++j)
			{
				assert(matches.count(*j) == 0);
				ParseTree::child_list_type &v = matches[*j];
				if((*symbol_info)[key[i]->value].type == SymbolInfo::symbol::NONTERMINAL) {
					/* This is the base case of the recursion
					where we actually send leaf nodes back
					up to the caller. */
					v.push_back(key[i]);
				}
			}

			/* Incrementally compute the set intersection of
			the rules matched at each node. */
			if(first) {
				output.swap(matches);
				first = false;
			}
			else {
				for(match_list_type::iterator
					j = output.begin(), m = output.end(); j != m;)
				{
					match_list_type::iterator pos = matches.find(j->first);
					if(pos == matches.end()) {
						match_list_type::iterator jcopy = j;
						++j;
						output.erase(jcopy);
					}
					else {
						j->second.insert(
							j->second.end(),
							pos->second.begin(),
							pos->second.end());
						++j;
					}
				}
			}

			/* Stop early if there are no rules which can be
			matched in every subtree. */
			if(output.size() == 0) return;
		}
	}
}

void ParseTreeMatcher::ParseTreeSequenceMatcher::print(const std::string &indent) const {
	typedef std::vector<ParseTree::value_type> value_string_type;
	value_string_type value_string;
	std::cout << indent << '{' << std::endl;
	for(map_type::const_iterator
		i = map.begin(), n = map.end(); i != n; ++i)
	{
		std::cout << indent;
		value_string.clear();
		for(ParseTree::child_list_type::const_iterator
			j = i->first.begin(), m = i->first.end(); j != m; ++j)
		{
			value_string.push_back((*j)->value);
		}
		symbol_info->print_string(
			value_string.begin(), value_string.end(), std::cout);
		std::cout << " => [" << std::endl;
		for(map_type::mapped_type::const_iterator
			j = i->second.begin(), m = i->second.end(); j != m; ++j)
		{
			for(rule_list_type::const_iterator
				ii = j->matched.begin(), nn = j->matched.end(); ii != nn; ++ii)
			{
				std::cout << indent << '\t';
				(*ii)->print(*symbol_info, std::cout);
				std::cout << std::endl;
			}
			j->subtree.print(indent + '\t');
		}
		std::cout << indent << ']' << std::endl;
	}
	std::cout << indent << '}' << std::endl;
}

namespace {
struct ParseTreeComparator {
	typedef ParseTree::child_list_type::value_type value_type;
	bool operator()(const value_type &a, const value_type &b) const {
		return a->value < b->value;
	}
};
}

bool ParseTreeMatcher::ParseTreeSequenceMatcher::ChildListComparator::operator()(
	const ParseTree::child_list_type &a,
	const ParseTree::child_list_type &b) const
{
	return std::lexicographical_compare(
		a.begin(), a.end(),
		b.begin(), b.end(),
		ParseTreeComparator());
}

ParseTreeMatcher::symbol_info_type::symbol_info_type(const SymbolInfo &symbol_info)
	: subtree(symbol_info)
{
}

} // namespace xlator

