#include "xlator/Parser.h"

#include <set>

#include "xlator/ParserFileReader.h"
#include "functional/pointer_cmp.h"

namespace xlator {

namespace {

struct ParseForest {

};

class ParserHelper {

public:

	void parse() {
		// TODO
	}

private:

	typedef std::set<const ParseForest *, functional::pointer_less<const ParseForest> >
		forest_set_type;

	forest_set_type forest_set;

};

} // anonymous namespace

void Parser::load_from_file(std::istream &input)
	throw(load_from_file_error)
{
	ParserFileReader::output_type output;
	ParserFileReader reader(input, output, symbol_info);
	reader.read();
	for(ParserFileReader::output_type::const_iterator
		i = output.begin(), n = output.end(); i != n; ++i)
	{
		for(ParserFileReader::output_type::value_type::right_type::const_iterator
			j = i->right.begin(), m = i->right.end(); j != m; ++j)
		{
			rules[*j].push_back(i->left);
		}
	}
#ifdef	DEBUG
	print_rules();
#endif
}

void Parser::parse(const token_string &input, parse_tree_set &output) const
	throw(parsing_error)
{
	// TODO
}

void Parser::print_rules() const {
	for(rule_set_type::const_iterator
		i = rules.begin(), n = rules.end(); i != n; ++i)
	{
		symbol_info.print_string(i->first.begin(), i->first.end(), std::cout);
		std::cout << " => { ";
		symbol_info.print_string(i->second.begin(), i->second.end(), std::cout);
		std::cout << " }" << std::endl;
	}
}

} // namespace xlator

