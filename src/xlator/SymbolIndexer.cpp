#include "xlator/SymbolIndexer.h"

#include <cassert>

namespace xlator {

SymbolIndexer::SymbolIndexer()
	: base(0)
{
}

SymbolIndexer::symbol_index SymbolIndexer::index_symbol(const std::string &name, symbol_type type) {
	assert(type < symbol::NUM_TYPES);
	return symbols[type].index_key(name, next_index());
}

bool SymbolIndexer::get_index(const std::string &name, symbol_type type, symbol_index &out) const {
	assert(type < symbol::NUM_TYPES);
	return symbols[type].get_index(name, out);
}

void SymbolIndexer::create_mapping(SymbolInfo &output) const {
	output.resize(next_index());
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

SymbolIndexer::symbol_index SymbolIndexer::next_index() const {
	return base + size();
}

void SymbolIndexer::set_base(symbol_index base) {
	this->base = base;
}

} // namespace xlator

