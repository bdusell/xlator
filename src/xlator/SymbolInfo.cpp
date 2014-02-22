#include "xlator/SymbolInfo.h"

#include <cassert>

namespace xlator {

void SymbolInfo::resize(symbol_key n) {
	_symbols.resize(n);
}

SymbolInfo::symbol &SymbolInfo::operator[](symbol_key i) {
	assert(i < _symbols.size());
	return _symbols[i];
}

const SymbolInfo::symbol &SymbolInfo::operator[](symbol_key i) const {
	assert(i < _symbols.size());
	return _symbols[i];
}

void SymbolInfo::print_symbol(symbol_key i, std::ostream &out) const {
	assert(i < _symbols.size());
	const symbol &s = _symbols[i];
	print_symbol(s.name, s.type, out);
}

void SymbolInfo::print_symbol(
	const std::string &name,
	symbol::symbol_type type,
	std::ostream &out)
{
	char left, right;
	switch(type) {
	case symbol::TERMINAL:
		left = right = '\"';
		break;
	case symbol::NONTERMINAL:
	default:
		left = '<';
		right = '>';
		break;
	}
	out << left << name << right;
}

void SymbolInfo::print() const {
	std::cout << "SYMBOL INFO:" << std::endl;
	for(symbols_type::size_type
		i = 0, n = _symbols.size(); i < n; ++i)
	{
		std::cout << i << ". ";
		print_symbol(i, std::cout);
		std::cout << std::endl;
	}
}

} // namespace xlator

