#ifndef _XLATOR_SYMBOLINFO_H_
#define _XLATOR_SYMBOLINFO_H_

#include <iostream>
#include <vector>

namespace xlator {

class SymbolInfo {

public:

	typedef unsigned int symbol_key;

	struct symbol {
		std::string name;
		enum symbol_type { TERMINAL, NONTERMINAL, NUM_TYPES } type;
	};

	void resize(symbol_key n);

	symbol &operator[](symbol_key i);
	const symbol &operator[](symbol_key i) const;

	void print_symbol(symbol_key i, std::ostream &out) const;

	template <typename I>
	void print_string(I begin, I end, std::ostream &out) const;

	static void print_symbol(const std::string &name, symbol::symbol_type type, std::ostream &out);

private:

	typedef std::vector<symbol> symbols_type;
	symbols_type _symbols;

};

} // namespace xlator

namespace xlator {
template <typename I>
void SymbolInfo::print_string(I begin, I end, std::ostream &out) const {
	bool first = true;
	for(I i = begin; i != end; ++i) {
		if(first) first = false;
		else out << ' ';
		print_symbol(*i, out);
	}
}
}

#endif

