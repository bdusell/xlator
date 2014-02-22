#include "xlator/ParseTreeMatcher.h"

#include <set>
#include <algorithm>

namespace xlator {

ParseTreeMatcher::ParseTreeMatcher(const SymbolInfo &symbol_info)
	: matcher(symbol_info)
{
}

void ParseTreeMatcher::insert(const key_type &trees, const rule_type *rule) {
	matcher.insert(trees, rule);
}

void ParseTreeMatcher::find(const key_type &trees, match_list_type &output) const {
	matcher.find(trees, output);
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
		of children (found in symbol_info_list[i].matched).
		*/

		const std::vector<symbol_info_type> &symbol_info_list = pos->second;

		if(!symbol_info_list.empty()) {

			match_list_type sub_matches;

			/* The set of rules which are reported to have been matched in
			every subtree of the list provided as the key value. It is the
			set intersection of all the sets of symbols fully matched in
			the sub-trees. */
			rule_set_type intersection;

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
				at the current child node. We will later get the rules
				matched at the current child node from info.matched. */
				info.subtree.find(key[i]->children, output);
				if(first) {
					intersection.insert(info.matched.begin(), info.matched.end());
					first = false;
				}
				else {
					intersection.erase(info.matched.begin(), info.matched.end());
				}
				/* Stop early if there are no rules which can be
				matched in every subtree. */
				if(intersection.size() == 0) return;
			}
			/* For each matched rule, insert it into the results. */
			match m;
			for(rule_set_type::const_iterator
				i = intersection.begin(), n = intersection.end(); i != n; ++i)
			{
				m.rule = *i;
				// TODO m.children.assign();
				output.push_back(m);
			}
			/* TODO get children, add results to output */
		}
	}
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

