#ifndef _XLATOR_FILEREADER_H_
#define _XLATOR_FILEREADER_H_

#include <iostream>
#include <string>

namespace xlator {

class FileReader {

public:

	FileReader(std::istream &fin);

protected:

	typedef unsigned char token_type;

	virtual const char *token_type_name(token_type t) const = 0;
	virtual void throw_exception(const std::string &s) const = 0;
	virtual void read_token() = 0;

	std::string curr_token_value;
	token_type curr_token_type;
	char next_char;

	const char *curr_token_name() const;
	void read_to_delim(char delim);
	void need_char();
	void read_char();
	void fail_if_eof() const;
	bool at_eof() const;
	void error_stray(char c);
	void expect_token(token_type type);
	void fail_token(token_type type);
	void raise_error(const std::string &s) const;

private:

	std::istream &fin;
	unsigned int line, column;

};

} // namespace xlator

#endif

