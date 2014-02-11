#ifndef _XLATOR_FILEREADER_H_
#define _XLATOR_FILEREADER_H_

#include <iostream>
#include <string>

namespace xlator {

class FileReader {

public:

	typedef unsigned char token_type;

	FileReader(std::istream &fin);

protected:

	virtual const char *token_type_name(token_type t) const = 0;
	virtual void raise_error(const std::string &s) const = 0;
	virtual void read_token() = 0;

	const char *curr_token_name() const;

	std::string curr_token_value;
	token_type curr_token_type;
	char next_char;

	void read_to_delim(char delim);
	void need_char();
	void read_char();
	void fail_if_eof() const;
	bool at_eof() const;
	void error_stray(char c);
	void expect_token(token_type type);
	void fail_token(token_type type);

private:

	std::istream &fin;

};

} // namespace xlator

#endif

