#ifndef _XLATOR_SYMBOLINDEXER_H_
#define _XLATOR_SYMBOLINDEXER_H_

#include <map>
#include <string>
#include <vector>

namespace xlator {

class SymbolIndexer {

public:

	typedef unsigned int symbol_index;

	struct symbol {
		std::string name;
		enum symbol_type { TERMINAL, NONTERMINAL, NUM_TYPES } type;
	};

	typedef std::vector<symbol> symbol_mapping;

	/*
	Insert a symbol into the indexer. Give it a new index if it does not
	exist, or reuse an existing one.
	*/
	symbol_index index_symbol(const std::string &name, symbol::symbol_type type);

	void create_mapping(symbol_mapping &output) const;

private:

	typedef std::map<std::string, symbol_index> map_type;

	map_type symbols[symbol::NUM_TYPES];

	symbol_index size() const;

};

} // namespace xlator

#endif

