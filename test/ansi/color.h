/* Emit control codes for changing colors on ANSI terminals. */

#ifndef ANSI_ANSICOLOR_H
#define ANSI_ANSICOLOR_H

#include <iostream>

namespace ansi {

namespace color {
enum color_code { black, red, green, yellow, blue, magenta, cyan, white };
enum clear_code { clear = white + 1 };
}

} // namespace ansi

inline std::ostream &operator<<(std::ostream &output, ansi::color::color_code c) {
	char str[6] = {033, '[', '3', 0, 'm', 0};
	str[3] = '0' + c;
	return output << str;
}

inline std::ostream &operator<<(std::ostream &output, ansi::color::clear_code c) {
	return output << "\033[0m";
}

#endif

