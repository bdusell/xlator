#ifndef _XLATOR_PARSETREEMATCHER_H_
#define _XLATOR_PARSETREEMATCHER_H_

#include <map>
#include <set>

#include "xlator/ParseTree.h"
#include "xlator/TranslationTree.h"
#include "xlator/SymbolInfo.h"
#include "resource/SharedPointer.h"

namespace xlator {

class ParseTreeMatcher {

public:

	ParseTreeMatcher(const SymbolInfo &symbol_info);

	typedef ParseTree::child_list_type key_type;

	struct rule_type {
		unsigned int index;
		ParseTree::child_pointer_type pattern;
		TranslationTree::child_pointer_type translation;

		void print(const SymbolInfo &info, std::ostream &out) const;

	private:

		void print_parse_tree(
			const ParseTree::child_pointer_type tree,
			const SymbolInfo &info,
			unsigned int &counter,
			bool at_top,
			std::ostream &out) const;

		void print_translation_tree(
			const TranslationTree::child_pointer_type tree,
			const SymbolInfo &info,
			std::ostream &out) const;
	};

	void insert(const key_type &trees, const rule_type *rule);

private:

	struct RuleComparator {
		bool operator()(const rule_type *a, const rule_type *b) const;
	};

public:

	typedef std::map<
		const rule_type *,
		ParseTree::child_list_type,
		RuleComparator
	> match_list_type;

	void find(const key_type &trees, match_list_type &output) const;

	void print() const;

private:

	class value_type;

	class symbol_info_type;

	class ParseTreeSequenceMatcher {

	public:

		ParseTreeSequenceMatcher(const SymbolInfo &symbol_info);

		void insert(const ParseTree::child_list_type &key, const rule_type *rule);

		void find(const ParseTree::child_list_type &key, match_list_type &output) const;

		void print(const std::string &indent) const;

	private:

		struct ChildListComparator {
			bool operator()(
				const ParseTree::child_list_type &a,
				const ParseTree::child_list_type &b) const;
		};

		typedef
		std::map<
			ParseTree::child_list_type,
			std::vector<symbol_info_type>,
			ChildListComparator
		>
		map_type;

		map_type map;

		/* Info is necessary to distinguish nonterminals from terminals. */
		const SymbolInfo *symbol_info;

	}; // class ParseTreeSequenceMatcher

	typedef std::vector<const rule_type *> rule_list_type;

	struct symbol_info_type {

		symbol_info_type(const SymbolInfo &symbol_info);

		rule_list_type matched;
		ParseTreeSequenceMatcher subtree;
	};

	ParseTreeSequenceMatcher matcher;

};

} // namespace xlator

#endif

