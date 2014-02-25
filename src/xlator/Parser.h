#ifndef _XLATOR_PARSER_H_
#define _XLATOR_PARSER_H_

#include <iostream>
#include <vector>
#include <set>

#include "xlator/ParseTree.h"
#include "xlator/SymbolIndexer.h"
#include "xlator/SymbolInfo.h"
#include "xlator/ParseForestNode.h"
#include "meta/exception.h"
#include "resource/SharedPointer.h"

namespace xlator {

class Parser {

public:

	typedef std::string token;
	typedef std::vector<token> token_string;
	typedef ParseTree::child_pointer_type parse_tree_pointer_type;
	typedef std::vector<parse_tree_pointer_type> parse_tree_set;

	EXCEPTION_CLASS(load_from_file_error)
	EXCEPTION_CLASS(parsing_error)

	Parser(SymbolIndexer &symbol_indexer, SymbolInfo &symbol_info);

	void load_from_file(std::istream &input)
		throw(load_from_file_error);

	void parse(const token_string &input, parse_tree_set &output) const
		throw(parsing_error);

private:

	typedef SymbolIndexer::symbol_index symbol_index;
	typedef std::vector<symbol_index> symbol_string;
	typedef std::vector<symbol_index> rule_set_value_type;
	typedef std::map<symbol_string, rule_set_value_type> rule_set_type;

	rule_set_type rules;
	SymbolIndexer &symbol_indexer;
	SymbolInfo &symbol_info;

	class Helper {

	public:

		typedef std::vector<const ParseForestNode *> output_type;

		Helper(
			const Parser &parser,
			output_type &output);

		~Helper();

		void parse(const token_string &s);

	private:

		typedef std::vector<const ParseForestNode *> node_string;

		struct VisitedSetLess {
			bool operator()(const node_string &a, const node_string &b) const;
		};

		typedef
			std::set<
				node_string,
				VisitedSetLess
			>
		visited_set;

		/* This special comparator compares the child lists of parse
		forest nodes by pointer value rather than comparing them
		recursively. This is a nice shortcut which we can take because,
		as can be deomonstrated inductively, every kind of subtree is
		only stored once in the forest and therefore is represented
		uniquely by its address. */
		struct ParseForestNodeLess {
			bool operator()(const ParseForestNode *a, const ParseForestNode *b) const;
		};

		typedef
			std::set<
				const ParseForestNode *,
				ParseForestNodeLess
			>
			forest_type;

		const Parser &parser;
		output_type &output;
		forest_type subtree_cache;
		visited_set visited_strings;

		void parse_recurse(const node_string &s);

		const ParseForestNode *get_branch_pointer(const ParseForestNode &branch);

		void print_forest() const;
		void print_string(const node_string &s) const;

	};

	void print_rules() const;
	void print_helper_output(const Helper::output_type &helper_output) const;
	void print_tree(const ParseForestNode *node) const;

};

} // namespace xlator

#endif

