#ifndef _XLATOR_SYMBOLINDEXER_H_
#define _XLATOR_SYMBOLINDEXER_H_

#include <map>
#include <string>
#include <vector>

#include "xlator/SymbolInfo.h"

namespace xlator {

class SymbolIndexer {

public:

	typedef SymbolInfo::symbol_key symbol_index;
	typedef SymbolInfo::symbol symbol;
	typedef SymbolInfo::symbol::symbol_type symbol_type;

	/*
	Insert a symbol into the indexer. Give it a new index if it does not
	exist, or reuse an existing one.
	*/
	symbol_index index_symbol(const std::string &name, symbol_type type);

	/*
	Look up a symbol and store its index in out. Return whether an index
	was found.
	*/
	bool get_index(const std::string &name, symbol_type type, symbol_index &out) const;

	void create_mapping(SymbolInfo &output) const;

private:

	typedef std::map<std::string, symbol_index> map_type;

	map_type symbols[symbol::NUM_TYPES];

	symbol_index size() const;

};

} // namespace xlator

#endif

