#include "algorithm/alternations.h"

#include <vector>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
	std::string tmp;
	std::vector< std::vector<char> > strings;
	for(char **argi = argv + 1, **argn = argv + argc; argi < argn; ++argi) {
		tmp = *argi;
		strings.push_back(std::vector<char>());
		strings.back().assign(tmp.begin(), tmp.end());
	}
	std::vector< std::vector<char> > output;

	algorithm::generate_alternations(
		strings.begin(), strings.end(), output);

	for(std::vector< std::vector<char> >::const_iterator
		i = output.begin(), n = output.end(); i != n; ++i)
	{
		for(std::vector<char>::const_iterator
			j = i->begin(), m = i->end(); j != m; ++j)
		{
			std::cout << *j;
		}
		std::cout << std::endl;
	}
}

