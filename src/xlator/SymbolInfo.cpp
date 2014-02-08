#include "xlator/SymbolInfo.h"

#include <cassert>

namespace xlator {

void SymbolInfo::resize(symbol_key n) {
	_symbols.resize(n);
}

SymbolInfo::symbol &SymbolInfo::operator[](symbol_key i) {
	return _symbols[i];
}

const SymbolInfo::symbol &SymbolInfo::operator[](symbol_key i) const {
	return _symbols[i];
}

void SymbolInfo::print_symbol(symbol_key i, std::ostream &out) const {
	assert(i < _symbols.size());
	const symbol &s = (*this)[i];
	char left, right;
	switch(s.type) {
	case symbol::TERMINAL:
		left = right = '\"';
		break;
	case symbol::NONTERMINAL:
	default:
		left = '<';
		right = '>';
		break;
	}
	out << left << s.name << right;
}

} // namespace xlator

