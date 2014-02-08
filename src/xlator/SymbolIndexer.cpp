#include "xlator/SymbolIndexer.h"

#include <cassert>

namespace xlator {

SymbolIndexer::symbol_index SymbolIndexer::index_symbol(const std::string &name, symbol_type type) {
	assert(type < symbol::NUM_TYPES);
	return symbols[type].insert(map_type::value_type(name, size())).first->second;
}

void SymbolIndexer::create_mapping(SymbolInfo &output) const {
	output.resize(size());
	for(size_t i = 0; i < symbol::NUM_TYPES; ++i) {
		const map_type &map = symbols[i];
		for(map_type::const_iterator
			j = map.begin(), m = map.end(); j != m; ++j)
		{
			symbol &s = output[j->second];
			s.name = j->first;
			s.type = (symbol::symbol_type) i;
		}
	}
}

SymbolIndexer::symbol_index SymbolIndexer::size() const {
	symbol_index result = 0;
	for(size_t i = 0; i < symbol::NUM_TYPES; ++i) {
		result += symbols[i].size();
	}
	return result;
}

} // namespace xlator

