#include "xlator/FileReader.h"

namespace xlator {

FileReader::FileReader(std::istream &fin)
	: fin(fin)
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

} // namespace xlator

