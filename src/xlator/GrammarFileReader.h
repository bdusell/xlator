#ifndef XLATOR_GRAMMARFILEREADER_H
#define XLATOR_GRAMMARFILEREADER_H

#include <iostream>

namespace xlator {

class ParserGrammar;
class TranslationGrammar;

class GrammarFileReader {

public:

	/*
	Set the parser and translation grammars into which the reader will read
	information.
	*/
	inline GrammarFileReader(
		ParserGrammar &parser_grammar,
		TranslationGrammar &translation_grammar
	)
		: _parser_grammar(parser_grammar)
		, _translation_grammar(translation_grammar)
	{}

	/*
	Read configuration information from a file, and store it in the
	grammars given in the constructor.
	*/
	void read(std::istream &fin);

private:

	ParserGrammar &_parser_grammar;
	TranslationGrammar &_translation_grammar;

};

} // namespace xlator

#endif

