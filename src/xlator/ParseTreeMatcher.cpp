#include "xlator/ParseTreeMatcher.h"

#include <set>
#include <algorithm>

#include "algorithm/SetIntersector.h"

namespace xlator {

ParseTreeMatcher::ParseTreeMatcher(const SymbolInfo &symbol_info)
	: matcher(symbol_info)
{
}

void ParseTreeMatcher::rule_type::print(const SymbolInfo &info, std::ostream &out) const {
	unsigned int counter = 0;
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
	matcher.print();
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
			m[i].matched.insert(rule);
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

		if(!symbol_info_list.empty()) {

			match_list_type sub_matches;
			rule_set_type sub_rules;

			/* The set of rules which are reported to have been matched in
			every subtree of the list provided as the key value. It is the
			set intersection of all the sets of symbols fully matched in
			the sub-trees. */
			algorithm::SetIntersector<rule_set_type> intersection;

			/* Loop through the children in the list provided as the key
			value and each child's corresponding symbol info
			simultaneously. */
			for(ParseTree::child_list_type::size_type
				i = 0, n = key.size(); i < n; ++i)
			{
				const symbol_info_type &info = symbol_info_list[i];
				/* Recursively search for matches in the subtree. Here
				we get rules which were matched deeper in the tree than
				at the current child node. We will later get the rules
				matched at the current child node from info.matched. */
				info.subtree.find(key[i]->children, sub_matches);
				sub_rules.clear();
				match_list_to_rule_set(sub_matches, sub_rules);
				intersection.add(sub_rules);

				/* Stop early if there are no rules which can be
				matched in every subtree. */
				if(intersection.set.size() == 0) return;
			}
			/* For each matched rule, insert it into the results. */
			match m;
			for(rule_set_type::const_iterator
				i = intersection.set.begin(), n = intersection.set.end(); i != n; ++i)
			{
				m.rule = *i;
				// TODO m.children.assign();
				output.push_back(m);
			}
			/* TODO get children, add results to output */
		}
	}
}

void ParseTreeMatcher::ParseTreeSequenceMatcher::print() const {
	typedef std::vector<ParseTree::value_type> value_string_type;
	value_string_type value_string;
	std::cout << '{' << std::endl;
	for(map_type::const_iterator
		i = map.begin(), n = map.end(); i != n; ++i)
	{
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
			for(rule_set_type::const_iterator
				ii = j->matched.begin(), nn = j->matched.end(); ii != nn; ++ii)
			{
				(*ii)->print(*symbol_info, std::cout);
				std::cout << std::endl;
			}
			j->subtree.print();
		}
		std::cout << ']' << std::endl;
	}
	std::cout << '}' << std::endl;
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

void ParseTreeMatcher::ParseTreeSequenceMatcher::match_list_to_rule_set(
	const match_list_type &matcher, rule_set_type &output)
{
	for(match_list_type::const_iterator
		i = matcher.begin(), n = matcher.end(); i != n; ++i)
	{
		output.insert(i->rule);
	}
}

ParseTreeMatcher::symbol_info_type::symbol_info_type(const SymbolInfo &symbol_info)
	: subtree(symbol_info)
{
}

} // namespace xlator

