#include "xlator/Interpreter.h"

#include "xlator/TranslationFileReader.h"

namespace xlator {

Interpreter::Interpreter(
	const SymbolIndexer &input_symbol_indexer,
	SymbolInfo &symbol_info)
	: input_symbol_indexer(input_symbol_indexer)
	, symbol_info(symbol_info)
	, parse_tree_matcher(symbol_info)
{
}

void Interpreter::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	TranslationFileReader reader(
		input, rules,
		input_symbol_indexer, output_symbol_indexer,
		symbol_info);
	reader.read();
	ParseTreeMatcher::key_type key(1);
	for(rule_list_type::const_iterator
		i = rules.begin(), n = rules.end(); i != n; ++i)
	{
		key[0] = i->pattern;
		parse_tree_matcher.insert(key, &*i);
#		ifdef DEBUG
		parse_tree_matcher.print();
#		endif
	}
}

void Interpreter::interpret(const ParseTree &input, tree_set &output) const
	throw(translation_error)
{
	Helper helper(*this, output);
	helper.interpret();
	throw translation_error("translation is not implemented yet");
}

void Interpreter::to_tokens(const symbol_string &s, token_string &output) const {
	for(symbol_string::const_iterator
		i = s.begin(), n = s.end(); i != n; ++i)
	{
		output.push_back(symbol_info[*i].name);
	}
}

Interpreter::Helper::Helper(const Interpreter &interpreter, tree_set &output)
	: interpreter(interpreter)
	, output(output)
{
}

void Interpreter::Helper::interpret() {
	// TODO
}

} // namespace xlator

