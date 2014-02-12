#include "xlator/FileReader.h"

#include <algorithm>
#include <sstream>

namespace xlator {

FileReader::FileReader(std::istream &fin)
	: fin(fin)
	, line(1)
	, column(1)
{
	read_char();
}

const char *FileReader::curr_token_name() const {
	return token_type_name(curr_token_type);
}

void FileReader::read_to_delim(char delim) {
	curr_token_value.clear();
	std::getline(fin, curr_token_value, delim);
	fail_if_eof();
	column += curr_token_value.size();
	// Uncomment to make this work for newlines within tokens
	//line += std::count(curr_token_value.begin(), curr_token_value.end(), '\n') + (delim == '\n');
}

void FileReader::need_char() {
	read_char();
	fail_if_eof();
}

void FileReader::fail_if_eof() const {
	if(at_eof()) {
		raise_error(
			std::string("end of file reached while reading ")
			+ token_type_name(curr_token_type));
	}
}

void FileReader::read_char() {
	fin.read(&next_char, 1);
	if(!at_eof()) {
		if(next_char == '\n') {
			++line;
			column = 1;
		}
		else {
			++column;
		}
	}
}

bool FileReader::at_eof() const {
	return !fin;
}

void FileReader::error_stray(char c) {
	raise_error(
		std::string("stray \"") + c + "\"");
}

void FileReader::expect_token(token_type type) {
	read_token();
	fail_token(type);
}

void FileReader::fail_token(token_type type) {
	if(curr_token_type != type) {
		raise_error(
			std::string("expected ") + token_type_name(type)
			+ " but read " + curr_token_name());
	}
}

void FileReader::raise_error(const std::string &s) const {
	std::ostringstream out;
	out << line << ':' << column << ": " << s;
	throw_exception(out.str());
}

} // namespace xlator

