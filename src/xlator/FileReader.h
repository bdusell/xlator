#ifndef _XLATOR_FILEREADER_H_
#define _XLATOR_FILEREADER_H_

#include <iostream>
#include <string>

namespace xlator {

/* The abstract base class from which the input file parsers inherit. */
class FileReader {

public:

	/* Initialize the file reader with the file stream from which it will
	read. */
	FileReader(std::istream &fin);

protected:

	typedef unsigned char token_type;

	/* Get a descriptive string for a token type to dispaly to the user
	upon certain errors. */
	virtual const char *token_type_name(token_type t) const = 0;

	/* Throw an exception with a certain error message. Enables the client
	code to throw a custom exception type upon errors which are detected
	in the FileReader class code. This should be defined but not directly
	called by the client code; use the wrapper raise_error() instead. */
	virtual void throw_exception(const std::string &s) const = 0;

	/* Read a token. It is advisable to use helpers such as read_char() and
	read_to_delim() to read characters. This function should set token_type
	accordingly to one of the client-specified token type codes. */
	virtual void read_token() = 0;

	/* The string value of the most recently read token where a string
	value is significant. Must be cleared and populated by the client
	code in read_token(). */
	std::string curr_token_value;

	/* The type of the most recently parsed token. This must be set by the
	client code within read_token(). */
	token_type curr_token_type;

	/* The next character in the input stream. It is advisable to use
	read_char() and other helpers to populate this variable. */
	char next_char;

	/* Get the name of curr_token_type as defined by token_type_name(). */
	const char *curr_token_name() const;

	/* Read every charcter in the input stream until delim and store it in
	curr_token_value. Throw an exception if EOF is encountered first. Note
	that this does not update next_char. */
	void read_to_delim(char delim);

	/* Call read_char() then fail_if_eof(). */
	void need_char();

	/* Read the next character in the input stream and store it in
	next_char.  */
	void read_char();

	/* Test whether EOF has been reached and throw_exception() if it
	has. */
	void fail_if_eof() const;

	/* Tell whether EOF has been reached. */
	bool at_eof() const;

	/* Throw an exception with an appropriate error message upon
	encountering an unexpected character. */
	void error_stray(char c);

	/* Read the next token and throw an exception if it is not of the
	specified type. */
	void expect_token(token_type type);

	/* Throw an exception with an appropriate error message upon
	encountering an unexcpected token of the specified type. */
	void fail_token(token_type type);

	/* Throw an exception with the given message and also with line and
	column information. This is what should be called by client code to
	signal errors. */
	void raise_error(const std::string &s) const;

	/* Read and discard all white space (besides newlines) and line content
	following the '#' character before the next token. */
	void skip_white_space_and_comments();

private:

	std::istream &fin;
	unsigned int line, column;

};

} // namespace xlator

#endif

